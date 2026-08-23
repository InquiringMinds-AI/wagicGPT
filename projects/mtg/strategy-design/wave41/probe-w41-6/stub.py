#!/usr/bin/env python3
"""W41-G arrival-trace stub inference server.

Answers Wagic's OpenAI-compatible chat-completions calls without a model, so the
repeat-activation annotation can be traced across CONSECUTIVE priority windows
deterministically.

Policy (this is the point of the probe):
  * if a numbered option line offers the {T} token maker, take it -- UNLESS the
    line's printed [repeat: activated this turn N times already ...] annotation
    says N >= STOP, in which case pass.  That is a pilot keying its stopping
    rule to the printed COUNTABLE, which is exactly what #W41-6 ships.
  * everything else: pass / first option.
Every prompt is appended to prompts.log for the trace.
"""
import json, re, sys
from http.server import BaseHTTPRequestHandler, HTTPServer

STOP = 8
LOG = "/tmp/w41g/prompts.log"
OPT = re.compile(r"^\s*(\d+)\.\s+(.*)$")
REPEAT = re.compile(r"\[repeat: activated this turn (\d+) times already([^\]]*)\]")


def decide(msg):
    for line in msg.splitlines():
        m = OPT.match(line)
        if not m:
            continue
        idx, text = m.group(1), m.group(2)
        if "Thraben Doomsayer" in text and "[cost: Tap]" in text and text.startswith("Create"):
            r = REPEAT.search(text)
            if r and int(r.group(1)) >= STOP:
                return "CHOICE: 0 (pass)", "stop-on-countable N=%s" % r.group(1)
            short = text.split(" [")[0].split(" {")[0]
            return "CHOICE: %s (%s)" % (idx, short), "take"
    return "CHOICE: 0 (pass)", "pass"


class H(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def do_GET(self):
        out = json.dumps({"object": "list",
                          "data": [{"id": "stub", "object": "model"}]}).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(out)))
        self.end_headers()
        self.wfile.write(out)

    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0))
        raw = self.rfile.read(n)
        try:
            body = json.loads(raw)
        except Exception:
            body = {}
        msg = ""
        for m in body.get("messages", []):
            if m.get("role") == "user":
                msg = m.get("content", "")
        answer, why = decide(msg)
        with open(LOG, "a") as f:
            f.write("\n===== REQUEST (%s -> %s) =====\n" % (why, answer))
            f.write(msg)
            f.write("\n")
        out = json.dumps({
            "id": "stub", "object": "chat.completion", "model": "stub",
            "choices": [{"index": 0, "finish_reason": "stop",
                         "message": {"role": "assistant", "content": answer}}],
        }).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(out)))
        self.end_headers()
        self.wfile.write(out)


if __name__ == "__main__":
    HTTPServer(("127.0.0.1", int(sys.argv[1])), H).serve_forever()
