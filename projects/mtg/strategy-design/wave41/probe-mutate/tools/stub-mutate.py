#!/usr/bin/env python3
"""Probe-1 stub endpoint: same contract as tools/gpt-stub-server.py, but the
answer is chosen from the prompt's own numbered option list so mutate casts
actually happen (a fixed "CHOICE: 1" never picks the mutate alternative)."""
import argparse, json, re, time
from http.server import BaseHTTPRequestHandler, HTTPServer

ARGS = None
OPT = re.compile(r'^\s*(\d+)\.\s+(.*)$')

PREFER = [
    re.compile(r'with its mutate cost', re.I),
    re.compile(r'^mutate \[cast for the MUTATE cost', re.I),
    re.compile(r'^Mutate (Over|Under)', re.I),
    re.compile(r"mutate pile - combined abilities"),
    re.compile(r"\[opponent's battlefield\]"),
]


def pick(prompt):
    opts = []
    for ln in prompt.splitlines():
        m = OPT.match(ln)
        if m:
            opts.append((int(m.group(1)), m.group(2).strip()))
    if not opts:
        return "CHOICE: 1"
    # Probe scheduling (model-free): the burn seat HOLDS its removal until a
    # mutate pile exists, so the target ask it raises actually enumerates one.
    if 'mutated pile' not in prompt:
        live = [(n, t) for n, t in opts if not t.lower().startswith('cast nothing')]
        decline = [(n, t) for n, t in opts if t.lower().startswith('cast nothing')]
        if decline and live and all(
                t.startswith('Cast Shock') or t.startswith('Cast Lightning Bolt')
                for n, t in live):
            return "CHOICE: %d (Cast nothing right now)" % decline[0][0]
    # keep only the trailing contiguous run (the live option list)
    for pat in PREFER:
        for n, t in opts:
            if pat.search(t):
                short = t.split('{')[0].split('[')[0].strip()[:40]
                return "CHOICE: %d (%s)" % (n, short)
    n, t = opts[0]
    short = t.split('{')[0].split('[')[0].strip()[:40]
    return "CHOICE: %d (%s)" % (n, short)


class Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

    def log_message(self, *a):
        pass

    def _send(self, obj, code=200):
        payload = json.dumps(obj).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(payload)))
        self.end_headers()
        self.wfile.write(payload)

    def do_GET(self):
        if self.path.rstrip("/").endswith("/v1/models"):
            self._send({"object": "list",
                        "data": [{"id": "stub-model", "object": "model"}]})
        else:
            self._send({"error": "not found"}, 404)

    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0) or 0)
        raw = self.rfile.read(n) if n else b""
        try:
            body = json.loads(raw.decode("utf-8", "replace"))
        except Exception:
            body = {}
        prompt = ""
        for m in body.get("messages", []):
            if m.get("role") == "user":
                prompt = m.get("content", "")
        answer = pick(prompt)
        if ARGS.log:
            with open(ARGS.log, "a") as f:
                f.write(json.dumps({"ts": time.time(), "answer": answer}) + "\n")
        self._send({
            "id": "stub", "object": "chat.completion", "model": "stub-model",
            "choices": [{"index": 0, "finish_reason": "stop",
                         "message": {"role": "assistant", "content": answer}}],
            "usage": {"prompt_tokens": 0, "completion_tokens": 0, "total_tokens": 0},
        })


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=8299)
    ap.add_argument("--log", default="")
    ARGS = ap.parse_args()
    HTTPServer(("127.0.0.1", ARGS.port), Handler).serve_forever()
