#!/usr/bin/env python3
"""A minimal OpenAI-compatible endpoint for model-free GPT-seat fixtures.

Serves GET /v1/models and POST /v1/chat/completions so the AIPlayerGPT seat
runs its REAL prompt assembly, REAL HTTP round trip, REAL reply parsing and
REAL translog write - with a deterministic answer instead of an inference
call. Every request is echoed to a JSONL file so a fixture can assert on what
was actually ASKED (the arrival trace), not only on what the board did.

  --port N        listen port (default 8299)
  --answer TEXT   reply content (default "CHOICE: 1")
  --prefer SUBSTR repeatable. Scan the numbered option list in the last user
                  message; answer with the FIRST option whose text contains
                  SUBSTR (case-insensitive), earlier --prefer flags winning.
                  Falls back to --answer when nothing matches. This is what
                  makes a multi-step commitment (cast -> alternative-cost menu
                  -> X announcement) deterministic instead of "always 1".
  --log PATH      JSONL of {ts, headers, body} per chat request
"""
import argparse
import json
import re
import sys
import time
from http.server import BaseHTTPRequestHandler, HTTPServer

ARGS = None

#"12. Cast Foo {r}" -> (12, "Cast Foo {r}")
OPTION_RE = re.compile(r"^\s*(\d+)\.\s+(.*)$")


def pick_answer(body):
    """Choose a reply for this request. --prefer wins over --answer."""
    if not ARGS.prefer:
        return ARGS.answer
    try:
        msgs = body.get("messages") or []
        text = ""
        for m in msgs:
            if m.get("role") == "user":
                text = m.get("content") or ""
    except Exception:
        return ARGS.answer
    options = []
    for line in text.splitlines():
        m = OPTION_RE.match(line)
        if m:
            options.append((int(m.group(1)), m.group(2)))
    if not options:
        return ARGS.answer
    for want in ARGS.prefer:
        w = want.lower()
        for idx, otext in options:
            if w in otext.lower():
                return "CHOICE: %d" % idx
    return ARGS.answer


class Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"

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
        if ARGS.log:
            try:
                body = json.loads(raw.decode("utf-8", "replace"))
            except Exception:
                body = {"_unparsed": raw.decode("utf-8", "replace")}
            with open(ARGS.log, "a") as f:
                f.write(json.dumps({"ts": time.time(), "path": self.path,
                                    "body": body}) + "\n")
        try:
            parsed = json.loads(raw.decode("utf-8", "replace"))
        except Exception:
            parsed = {}
        self._send({
            "id": "stub", "object": "chat.completion", "model": "stub-model",
            "choices": [{"index": 0, "finish_reason": "stop",
                         "message": {"role": "assistant",
                                     "content": pick_answer(parsed)}}],
            "usage": {"prompt_tokens": 0, "completion_tokens": 0,
                      "total_tokens": 0},
        })

    def log_message(self, *a):
        pass


def main():
    global ARGS
    ap = argparse.ArgumentParser()
    ap.add_argument("--port", type=int, default=8299)
    ap.add_argument("--answer", default="CHOICE: 1")
    ap.add_argument("--prefer", action="append", default=[])
    ap.add_argument("--log", default="")
    ARGS = ap.parse_args()
    srv = HTTPServer(("127.0.0.1", ARGS.port), Handler)
    sys.stderr.write("gpt-stub-server on 127.0.0.1:%d\n" % ARGS.port)
    sys.stderr.flush()
    srv.serve_forever()


if __name__ == "__main__":
    main()
