#!/usr/bin/env python3
"""Deterministic stub OpenAI-compatible endpoint for wagicGPT seam validation.

Always answers "CHOICE: 1" (the first option) unless the ask is the wave-41
batched may question, which is logged verbatim and answered per $STUB_BATCH
(1 = accept all, 2 = decline all, 3 = decide individually).
"""
import json, os, sys, re
from http.server import BaseHTTPRequestHandler, HTTPServer

BATCH = os.environ.get("STUB_BATCH", "1")
LOG = open(os.environ.get("STUB_LOG", "/tmp/stubmodel.log"), "w")


class H(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def do_POST(self):
        n = int(self.headers.get("Content-Length", 0))
        body = self.rfile.read(n).decode("utf-8", "replace")
        try:
            msgs = json.loads(body).get("messages", [])
            prompt = msgs[-1].get("content", "") if msgs else ""
        except Exception:
            prompt = body
        answer = "CHOICE: 1"
        if "triggered" in prompt and "accept ALL" in prompt:
            LOG.write("=== BATCHED MAY ASK ===\n")
            tail = prompt[prompt.rfind("triggered") - 200:]
            LOG.write(tail + "\n")
            LOG.flush()
            answer = "CHOICE: %s" % BATCH
        elif "Your available blockers" in prompt:
            # gang-block A1 with every offered blocker: the deterministic way to
            # produce a multi-trigger may window.
            bs = sorted(set(int(m) for m in re.findall(r"^B(\d+)\.", prompt, re.M)))
            if bs:
                answer = "BLOCKS: " + ", ".join("B%d:A1" % b for b in bs)
        elif "Gain 2 life with" in prompt:
            LOG.write("=== SINGLE MAY ASK ===\n")
            for line in prompt.splitlines():
                if "Gain 2 life with" in line or "Decline" in line:
                    LOG.write(line + "\n")
            LOG.flush()
        out = json.dumps({
            "id": "stub", "object": "chat.completion", "model": "stub",
            "choices": [{"index": 0, "finish_reason": "stop",
                         "message": {"role": "assistant", "content": answer}}],
            "usage": {"prompt_tokens": 1, "completion_tokens": 1, "total_tokens": 2},
        }).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(out)))
        self.end_headers()
        self.wfile.write(out)

    def do_GET(self):
        out = json.dumps({"data": [{"id": "stub"}]}).encode()
        self.send_response(200)
        self.send_header("Content-Type", "application/json")
        self.send_header("Content-Length", str(len(out)))
        self.end_headers()
        self.wfile.write(out)


HTTPServer(("127.0.0.1", int(sys.argv[1])), H).serve_forever()
