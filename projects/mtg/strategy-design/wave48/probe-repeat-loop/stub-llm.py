#!/usr/bin/env python3
"""Deterministic stub LLM for the wave-48 lane N live probe.

It answers /v1/models so the engine's endpoint probe succeeds, and for every
chat completion it looks for the F1 repeat row in the prompt. If one is there it
answers with that row's index and a named N; otherwise it returns an empty
choice so the heuristic AI drives the rest of the game. The point is to exercise
the repeat-N dispatch end to end, not to play well.
"""
import json, re, sys
from http.server import BaseHTTPRequestHandler, HTTPServer

N = int(sys.argv[2]) if len(sys.argv) > 2 else 25
ROW = re.compile(r'^(\d+)\. (.*?), repeated N times, then stop', re.M)

class H(BaseHTTPRequestHandler):
    def log_message(self, *a): pass
    def _send(self, obj):
        b = json.dumps(obj).encode()
        self.send_response(200)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', str(len(b)))
        self.end_headers()
        self.wfile.write(b)
    def do_GET(self):
        self._send({"object": "list", "data": [{"id": "stub", "object": "model"}]})
    def do_POST(self):
        n = int(self.headers.get('Content-Length', 0))
        body = json.loads(self.rfile.read(n) or b'{}')
        prompt = "\n".join(m.get('content', '') for m in body.get('messages', []))
        m = ROW.search(prompt)
        if m:
            content = "CHOICE: %s (%s x%d)" % (m.group(1), m.group(2), N)
            print("REPEAT ROW TAKEN: %s" % content, flush=True)
        elif "Your legal actions" in prompt:
            # seed the loop: take option 1 until the repeat row appears
            m1 = re.search(r'^1\. (.*?)(?: [\[{]|$)', prompt, re.M)
            content = "CHOICE: 1 (%s)" % (m1.group(1) if m1 else "option")
        else:
            content = ""
        self._send({"id": "stub", "object": "chat.completion",
                    "choices": [{"index": 0, "message": {"role": "assistant", "content": content},
                                 "finish_reason": "stop"}]})

HTTPServer(('127.0.0.1', int(sys.argv[1])), H).serve_forever()
