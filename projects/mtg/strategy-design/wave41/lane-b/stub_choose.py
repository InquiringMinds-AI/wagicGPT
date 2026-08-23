#!/usr/bin/env python3
"""Deciding stub endpoint: answers the seat's own reply protocol with a legal
first choice, so the ACTING seat records "You used: ..." consumed-decision
lines alongside the observing seat's "Opponent used: ...". Every third call is
deliberately unparsable so no single option can be looped forever."""
import json
import re
import sys
from http.server import BaseHTTPRequestHandler, HTTPServer

PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8318
STATE = {"n": 0}


def answer(prompt):
    STATE["n"] += 1
    if STATE["n"] % int(__import__("os").environ.get("STUB_PASS_EVERY", "3")) == 0:
        return "PLAN: pass this window."
    if re.search(r"^ATTACK:", prompt, re.M) or "write ATTACK:" in prompt:
        return "ATTACK: A1\nPLAN: probe."
    if "BLOCK:" in prompt:
        return "BLOCK: none\nPLAN: probe."
    opts = re.findall(r"^\s*(\d+)\.\s", prompt, re.M)
    if opts:
        return "CHOICE: 1\nPLAN: probe."
    return "PLAN: probe."


class Handler(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def _json(self, obj):
        body = json.dumps(obj).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        if self.path.endswith("/v1/models"):
            self._json({"object": "list",
                        "data": [{"id": "stub-narration-probe", "object": "model"}]})
        else:
            self.send_error(404)

    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0))
        try:
            req = json.loads(self.rfile.read(n) or b"{}")
        except Exception:
            req = {}
        prompt = "\n".join(m.get("content", "") for m in req.get("messages", []))
        self._json({
            "id": "stub", "object": "chat.completion", "model": "stub-narration-probe",
            "choices": [{"index": 0, "finish_reason": "stop",
                         "message": {"role": "assistant", "content": answer(prompt)}}],
            "usage": {"prompt_tokens": 0, "completion_tokens": 0, "total_tokens": 0},
        })


if __name__ == "__main__":
    HTTPServer(("127.0.0.1", PORT), Handler).serve_forever()
