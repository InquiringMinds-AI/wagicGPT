#!/usr/bin/env python3
"""Mint ChatGPT-subscription OAuth tokens via the Codex device-code flow.

Recreated 2026-08-09 (the original tmp/ copy was ephemeral and lost); every
endpoint below was verified live 2026-08-09 against openai/codex source. This
is the reference implementation for the future IN-CLIENT flow (C++): the game
shows the user code, the user enters it at auth.openai.com/codex/device on any
modern browser (their phone - the Vita browser cannot render that SPA), and
the client polls until tokens arrive.

Path facts (the research digest got these wrong once - trust these):
  usercode: POST {AUTH}/api/accounts/deviceauth/usercode   JSON {client_id}
  poll:     POST {AUTH}/api/accounts/deviceauth/token      JSON {device_auth_id, user_code}
            -> 403/404 while pending
            -> success: {authorization_code, code_challenge, code_verifier}
               (PKCE pair is SERVER-generated in this flow)
  exchange: POST {AUTH}/oauth/token  form-encoded
            grant_type=authorization_code&code&redirect_uri&client_id&code_verifier
            redirect_uri = {AUTH}/deviceauth/callback   (NOT localhost:1455)

Writes ~/.config/wagic-oai/auth.json (0600) + account.json - the same files
the game's "OpenAI subscription" preset reads.
"""
import base64, json, os, sys, time, urllib.error, urllib.parse, urllib.request

AUTH = "https://auth.openai.com"
CLIENT_ID = "app_EMoamEEZ73f0CkXaXp7hrann"
OUTDIR = os.path.expanduser("~/.config/wagic-oai")


def post(url, data, form=False):
    body = urllib.parse.urlencode(data).encode() if form else json.dumps(data).encode()
    ctype = "application/x-www-form-urlencoded" if form else "application/json"
    req = urllib.request.Request(url, data=body, headers={"Content-Type": ctype}, method="POST")
    with urllib.request.urlopen(req, timeout=30) as r:
        return r.status, json.loads(r.read().decode())


def jwt_claims(tok):
    p = tok.split(".")[1]
    p += "=" * (-len(p) % 4)
    return json.loads(base64.urlsafe_b64decode(p))


def main():
    _, d = post(f"{AUTH}/api/accounts/deviceauth/usercode", {"client_id": CLIENT_ID})
    device_auth_id = d["device_auth_id"]
    user_code = d["user_code"]
    interval = int(d.get("interval", 5))
    print(f"Enter this code at {AUTH}/codex/device :\n\n    {user_code}\n")
    print("(Device auth must be enabled in ChatGPT Settings -> Security. 15-min expiry.)")

    while True:
        time.sleep(interval)
        try:
            _, t = post(f"{AUTH}/api/accounts/deviceauth/token",
                        {"device_auth_id": device_auth_id, "user_code": user_code})
        except urllib.error.HTTPError as e:
            if e.code in (403, 404):
                print(".", end="", flush=True)
                continue
            raise
        break

    print("\ncode accepted, exchanging...")
    _, tokens = post(f"{AUTH}/oauth/token", {
        "grant_type": "authorization_code",
        "code": t["authorization_code"],
        "redirect_uri": f"{AUTH}/deviceauth/callback",
        "client_id": CLIENT_ID,
        "code_verifier": t["code_verifier"],
    }, form=True)

    claims = jwt_claims(tokens["id_token"])
    auth_claims = claims.get("https://api.openai.com/auth", {})
    account_id = auth_claims.get("chatgpt_account_id", "")
    plan = auth_claims.get("chatgpt_plan_type", "")

    os.makedirs(OUTDIR, exist_ok=True)
    authpath = os.path.join(OUTDIR, "auth.json")
    with open(authpath, "w") as f:
        json.dump({"tokens": tokens, "minted_at": int(time.time()),
                   "chatgpt_account_id": account_id}, f, indent=1)
    os.chmod(authpath, 0o600)
    accpath = os.path.join(OUTDIR, "account.json")
    with open(accpath, "w") as f:
        json.dump({"chatgpt_account_id": account_id, "plan": plan}, f)
    os.chmod(accpath, 0o600)
    print(f"tokens -> {authpath}\naccount {account_id} plan={plan}")


if __name__ == "__main__":
    sys.exit(main())
