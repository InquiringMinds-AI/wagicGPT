#!/usr/bin/env python3
"""Probe the ChatGPT-subscription Codex Responses backend.

Recreated 2026-08-09 (the original tmp/ copy was ephemeral and lost).
Facts baked in here were verified live 2026-08-09 against openai/codex source:
  - endpoint: POST https://chatgpt.com/backend-api/codex/responses
  - minimal header set works, NO attestation headers needed
  - response is SSE Responses events when stream:true
This run: stream:false — does the backend return a single JSON blob the
Vita's non-streaming parser could consume, or reject/stream anyway?

Usage: python3 oai-completion-probe.py [--stream]
"""
import json, sys, urllib.request

AUTH = json.load(open("/home/magi/.config/wagic-oai/auth.json"))["tokens"]
ACCT = json.load(open("/home/magi/.config/wagic-oai/account.json"))["chatgpt_account_id"]

stream = "--stream" in sys.argv

body = {
    "model": "gpt-5.6-luna",
    "instructions": "You are a helpful assistant. Answer in one short sentence.",
    "input": [
        {
            "type": "message",
            "role": "user",
            "content": [{"type": "input_text", "text": "Say the word 'transport' and nothing else."}],
        }
    ],
    "reasoning": {"effort": "low"},
    "store": False,
    "stream": stream,
}

req = urllib.request.Request(
    "https://chatgpt.com/backend-api/codex/responses",
    data=json.dumps(body).encode(),
    headers={
        "Authorization": f"Bearer {AUTH['access_token']}",
        "chatgpt-account-id": ACCT,
        "Content-Type": "application/json",
        "originator": "codex_cli_rs",
        "OpenAI-Beta": "responses=experimental",
        "User-Agent": "codex_cli_rs/0.45.0 (Ubuntu 24.04.2 LTS; x86_64) WindowsTerminal",
        "Accept": "text/event-stream" if stream else "application/json",
    },
    method="POST",
)

try:
    with urllib.request.urlopen(req, timeout=120) as r:
        print("HTTP", r.status)
        ct = r.headers.get("Content-Type", "")
        print("Content-Type:", ct)
        for h in r.headers:
            if h.lower().startswith("x-codex"):
                print(f"{h}: {r.headers[h]}")
        raw = r.read().decode("utf-8", "replace")
        print("--- body head (2000 chars) ---")
        print(raw[:2000])
        print("--- body bytes:", len(raw))
except urllib.error.HTTPError as e:
    print("HTTP", e.code)
    print("Content-Type:", e.headers.get("Content-Type", ""))
    print(e.read().decode("utf-8", "replace")[:2000])
