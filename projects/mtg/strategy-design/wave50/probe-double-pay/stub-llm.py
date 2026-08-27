#!/usr/bin/env python3
"""Stub LLM for the wave-50 lane V double-tap probe.

Casting decision -> option 1 (the first cast; for the Cancel seat in a response
window that is "Cast Cancel"). Land drop -> option 1 (play). Everything else ->
empty reply (heuristic decides).
"""
import json, re, sys, time
from http.server import BaseHTTPRequestHandler, HTTPServer

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
        user = body.get('messages', [{}])[-1].get('content', '')
        content = ""
        if ("Casting decision" in user and "ON THE STACK" in user
                and "Cast Cancel" not in user):
            last = re.findall(r'^(\d+)\. (.*?)(?: [\[{]|$)', user, re.M)
            content = "CHOICE: %s (%s)" % last[-1] if last else ""
        elif "Casting decision" in user:
            rows = re.findall(r'^(\d+)\. (.*?)(?: [\[{]|$)', user, re.M)
            pick = [r for r in rows if r[1].startswith("Cast Grizzly Bears") or r[1].startswith("Cast Cancel")]
            if pick:
                content = "CHOICE: %s (%s)" % pick[0]
            elif rows:
                content = "CHOICE: %s (%s)" % rows[-1]
        elif "Land drop:" in user:
            m1 = re.search(r'^1\. (.*?)(?: [\[{]|$)', user, re.M)
            content = "CHOICE: 1 (%s)" % (m1.group(1) if m1 else "option")
        elif "Your legal actions" in user:
            content = "CHOICE: 0 (pass)"
        if "ON THE STACK" in user:
            time.sleep(float(sys.argv[2]) if len(sys.argv) > 2 else 0)
        print("Q:", user[user.rfind('\n', 0, user.rfind('\n1. ')) + 1:].split('\n')[0][:80], "->", content, flush=True)
        self._send({"id": "stub", "object": "chat.completion",
                    "choices": [{"index": 0, "message": {"role": "assistant", "content": content},
                                 "finish_reason": "stop"}]})

HTTPServer(('127.0.0.1', int(sys.argv[1])), H).serve_forever()
