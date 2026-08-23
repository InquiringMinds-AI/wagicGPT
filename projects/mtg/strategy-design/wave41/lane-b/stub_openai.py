#!/usr/bin/env python3
"""Minimal fake OpenAI endpoint for wagicGPT narration probes.

Real prompt assembly, real translog, NO inference: every reply leads with
"PLAN:" and carries no answer label, so the seat's parser returns nothing
parsable and the heuristic answers the window (the documented fallback path).
The point of the run is the PROMPT the seat assembles, not the play.
"""
import json
import sys
from http.server import BaseHTTPRequestHandler, HTTPServer

PORT = int(sys.argv[1]) if len(sys.argv) > 1 else 8299

REPLY = "PLAN: narration probe - the heuristic owns this window."


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
        self.rfile.read(n)
        self._json({
            "id": "stub", "object": "chat.completion", "model": "stub-narration-probe",
            "choices": [{"index": 0, "finish_reason": "stop",
                         "message": {"role": "assistant", "content": REPLY}}],
            "usage": {"prompt_tokens": 0, "completion_tokens": 0, "total_tokens": 0},
        })


if __name__ == "__main__":
    HTTPServer(("127.0.0.1", PORT), Handler).serve_forever()
