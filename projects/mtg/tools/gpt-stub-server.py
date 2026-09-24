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
  --hang-ms MS    #W53-Q (D10): accept the chat request and NEVER answer it -
                  sleep MS milliseconds before replying (default 0 = off). This
                  is the deterministic TIMEOUT fixture: the request is accepted
                  and the CLOCK runs out, which is the one no-answer shape a
                  reachable-but-slow endpoint produces and an unreachable one
                  does not. Set it above WAGIC_GPT_TIMEOUT and every decision
                  reaches the wall, with no model and no inference call.
  --hang-every N  hang only every Nth chat request (1 = every one, the default
                  when --hang-ms is set); use 2 to prove the ONE retry fires and
                  the second attempt is answered normally.
  --reasoning-only-first
                  #W82-EA (H6): the BUDGET-HIT fixture. The FIRST request for each
                  distinct (system, user) prompt is answered as a phase-1 budget
                  hit - content empty, `reasoning_content` = --reasoning-text,
                  finish_reason "length" - and every later request for the same
                  prompt (the seat's retry) is answered normally. A retry that
                  sends a prefill (continue_final_message) is answered normally
                  too, so the legacy close still resolves.
  --reasoning-text TEXT   the trace returned by the budget-hit reply.
"""
import argparse
import json
import re
import sys
import time
from http.server import BaseHTTPRequestHandler, HTTPServer, ThreadingHTTPServer

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


SEEN_PROMPTS = set()


def budget_hit_first(body):
    """#W82-EA (H6): True for the first request of a distinct prompt (a phase-1
    budget hit), False for a repeat (the retry) or a prefilled close."""
    msgs = body.get("messages") or []
    if any(m.get("role") == "assistant" for m in msgs):
        return False  #a prefill close: answer it
    key = json.dumps([(m.get("role"), m.get("content")) for m in msgs], sort_keys=True)
    if key in SEEN_PROMPTS:
        return False
    SEEN_PROMPTS.add(key)
    return True


class Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"
    seen = 0

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
        if ARGS.hang_ms > 0:
            Handler.seen += 1
            if ARGS.hang_every <= 1 or (Handler.seen % ARGS.hang_every) == 1:
                time.sleep(ARGS.hang_ms / 1000.0)
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
        if ARGS.reasoning_only_first and budget_hit_first(parsed):
            self._send({
                "id": "stub", "object": "chat.completion", "model": "stub-model",
                "choices": [{"index": 0, "finish_reason": "length",
                             "message": {"role": "assistant", "content": "",
                                         "reasoning_content": ARGS.reasoning_text}}],
                "usage": {"prompt_tokens": 0, "completion_tokens": 0,
                          "total_tokens": 0},
            })
            return
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
    ap.add_argument("--hang-ms", type=int, default=0)
    ap.add_argument("--hang-every", type=int, default=1)
    ap.add_argument("--reasoning-only-first", action="store_true")
    ap.add_argument("--reasoning-text", default="Let me think about the board. " * 40)
    ARGS = ap.parse_args()
    #Threading, because the hang fixture holds a connection open for the whole
    #deadline and BOTH selfplay seats have a request in flight at once - a
    #single-threaded server would serialise them and the second seat's wall
    #clock would start when the first one's ended.
    srv = ThreadingHTTPServer(("127.0.0.1", ARGS.port), Handler)
    sys.stderr.write("gpt-stub-server on 127.0.0.1:%d\n" % ARGS.port)
    sys.stderr.flush()
    srv.serve_forever()


if __name__ == "__main__":
    main()
