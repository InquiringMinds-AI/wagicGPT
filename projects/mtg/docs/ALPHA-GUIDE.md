# wagicGPT 0.1.0-alpha — setup guide

*for humans, and for the agents humans send to do this for them*

## What this is

wagicGPT is a fork of Wagic, a 15-year-old open-source card game engine with a
trick up its sleeve: the engine doesn't actually know it plays Magic. It knows
counters, zones, phases, triggers, costs. The ~26,000 cards are *data* — text
files describing what each card does in a compact scripting language. This fork
adds one more layer on top: an LLM opponent. Every time the AI has a real
decision to make — what to cast, what to attack with, how to block — it
assembles the game state into a prompt, asks a language model over plain HTTP,
and plays the answer. If the model is slow, wrong, or unreachable, the built-in
heuristic AI answers instead, per decision. The game never blocks on the
network.

The fork isn't only the opponent — playing against something smart made the
rough edges of playing *at all* obvious, so the GUI has picked up a stack of
quality-of-life changes over stock Wagic: casting auto-taps the right lands
(exact payment, abilities too, with a preview of what's about to tap), cards
you can actually afford are marked as such, priority auto-passes through the
phases where you have nothing to do instead of stopping to ask, prompts show
the buttons that are really bound to the actions, and the game paces combat
so a human defender isn't rushed by an AI attacker. If you knew old Wagic:
it's the same game with much less clicking-to-say-nothing.

The mental model for everything below:

```
game = binary (per platform)
     + Res/   (game data, ships in the box)
     + User/  (your saves and settings, created on first boot)
     + [optional] an LLM endpoint      -> the interesting opponent
     + [optional] card images          -> you fetch these yourself (see why below)
```

Install the binary, boot it once, point it at a model, fetch art. Four steps,
each independently skippable. Let's go.

## 1. Install

**Linux** — `wagicGPT-linux-alpha.tar.gz`. Untar anywhere, run the launcher:

```
tar xzf wagicGPT-linux-alpha.tar.gz
cd wagicGPT-0.1.0-alpha
./wagic.sh
```

Everything is bundled (34 shared libs; your GPU stack and glibc stay system).
The profile is created *next to the binary* — the folder is self-contained,
move it and your save moves with it.

**Windows** — `wagicGPT-windows-alpha.zip`. Unzip, run `wagic.exe`. That's it.
Also runs fine under Wine/Proton if you're on Linux but testing the Windows
build.

**Android** — `wagicGPT-android-alpha.apk`. Sideload it (enable "install
unknown apps" for your file manager if Android asks). Game data lives in
`/sdcard/Wagic/`; if the game can't see its files, give it **All files
access**: Settings → search "Wagic" → All files access → allow. First boot
unpacks the bundled resource pack automatically.

**PS Vita** — `wagicGPT-vita-alpha.vpk`. Needs a homebrew-enabled Vita.
Transfer the vpk to `ux0:/vpk/`, install it with VitaShell (X on the file,
accept the extended permissions prompt). Your data lives at `ux0:data/Wagic/`
and survives reinstalls of the bubble.

**PSP** — `wagicGPT-psp-alpha.zip`. Needs custom firmware (6.60 PRO-C or
similar). Extract the zip at the **root of the memory stick** — it lays down
`PSP/GAME/WAGIC/` with the EBOOT and game data. One deliberate difference: the
PSP build has **no LLM opponent** — you play the built-in AI. The hardware
predates the idea; the game is still the game.

## 2. First boot

Pick a language, land on the main menu, done. This creates the `User/` tree:

| platform | your files live at |
|---|---|
| Linux | `<install dir>/User/` (game data) + `~/.Wagic/` (LLM config + logs) |
| Windows | `<install dir>\User\` + `%USERPROFILE%\.Wagic\` |
| Android | `/sdcard/Wagic/User/` (LLM config under `.../User/ai/gpt/`) |
| Vita | `ux0:data/Wagic/` (LLM config under `.../ai/gpt/`) |
| PSP | `PSP/GAME/WAGIC/User/` (no LLM config — see above) |

Sanity check for agents: after one boot-and-quit, `User/settings/options.txt`
exists and contains a `Lang=` line. If it doesn't, the boot didn't complete.

## 3. The LLM opponent

Two ways to configure it. They edit the same file, so use whichever you like.

**The GUI way:** Options → the GPT tab. Flip "LLM opponent" On, pick a
provider preset (or Custom), enter the endpoint URL and API key, then hit
**Test connection**. It performs a real round trip and tells you the model it
found. If it says `unreachable / no usable reply`, the URL it probed is wrong
or down — fix that before anything else.

**The config-as-code way:** edit `ai/gpt/endpoints.txt` under the config dir
from the table above (e.g. `~/.Wagic/ai/gpt/endpoints.txt` on Linux). Format
is `key=value`, `#` comments:

```
enabled=1
url=http://192.168.1.50:8080      # repeatable; probed in order, first
url=http://127.0.0.1:8080         # one whose /v1/models answers is used
model=                            # empty = auto-detect from /v1/models
key=sk-...                        # bearer token, if your endpoint wants one
max_reply_tokens=4096             # decode-length guard, default is fine
```

Anything with an OpenAI-compatible `/v1/chat/completions` works. Your options,
as peers — pick by your own hardware, wallet, and taste:

- **A local server**: llama.cpp (`llama-server`), vLLM, LM Studio, Ollama —
  anything serving the OpenAI API on your machine or LAN.
- **OpenRouter / DeepSeek / any hosted API**: base URL + API key.
- **OpenAI (API key)**: standard API endpoint.
- **OpenAI (subscription)**: a preset that signs in with your ChatGPT
  account — no API key needed. Pick it in the GUI: the game shows a QR code
  (or URL) plus a short generated passkey; open the link on any device,
  **type that passkey in by hand** at the site, and the game finishes the
  sign-in on its own.

Environment variables override the file when you're experimenting:
`WAGIC_GPT_URL`, `WAGIC_GPT_MODEL`, `WAGIC_GPT_KEY`, and `WAGIC_AI=gpt` forces
the LLM opponent on.

**How to tell it's working:** start a duel. The opponent shows "opponent is
thinking…" while a request is in flight. If the endpoint dies mid-game you'll
see an "LLM: N fallbacks" marker — the heuristic AI is covering, per decision,
and the game carries on.

**Verification for agents**, in order, cheapest first:

```
curl <url>/v1/models                       # 1. endpoint answers?
# 2. boot game, GPT tab -> Test connection reports the model id
# 3. play one duel; then inspect <config dir>/ai/gpt/gpt-log.txt for errors
```

## 4. Card images

The release ships **no card art**. Not an oversight: card scans are
WotC-derived content we don't have the right to redistribute. What everyone
does — and what Scryfall exists for under the WotC Fan Content Policy — is
fetch images for personal use, one user at a time. The game renders text-only
frames without art and is fully playable; it's just nicer with pictures.

The toolkit lives in the repo at `projects/mtg/tools/cardart/`:

```
fetch-art.py       # downloads full-size art from Scryfall into a local pool
pack-psp.sh        # resizes + zips the pool for a platform:
pack-vita.sh       #   psp     180x250 art + 64x93 thumbs
pack-android.sh    #   vita    360x514 + 128x186
pack-windows.sh    #   android 488x680 + 114x166
pack-linux.sh      #   desktop full-size + 114x166 thumbs
```

Requirements: `python3` (stdlib only) for the fetcher; `bash` + ImageMagick
(`magick`) + `zip` for the packers. The fetcher is throttled to ≥120ms/request
— be a good citizen, don't defeat it. Full art for all ~330 sets is roughly
**12 GB** and takes a while; start with the sets your decks actually use.

**Desktop (simplest possible flow)** — fetch straight into the game's own
User tree, no packing:

```
cd projects/mtg/tools/cardart
./fetch-art.py --res-sets <install>/Res/sets --pool <install>/User/sets 10E M10 M11
```

Restart the game: those sets now render art. (`10E M10 M11` is an example —
no set codes means *all* of them, see the 12 GB note above.)

**Handhelds** — fetch into a pool once, pack per platform, copy the zips over:

```
./fetch-art.py --pool ~/wagic-art-pool --res-sets <install>/Res/sets 10E M10
POOL_BASE=~/wagic-art-pool ./pack-psp.sh
# -> ./wagic-card-packs/psp/sets/<SET>/<SET>.zip
```

Copy each `sets/<SET>/<SET>.zip` into the platform's user tree, keeping the
layout: `User/sets/<SET>/<SET>.zip` (PSP: under `PSP/GAME/WAGIC/User/`;
Vita: `ux0:data/Wagic/sets/`; Android: `/sdcard/Wagic/User/sets/`). The
engine reads art from inside the zips directly.

Useful knobs (env vars for packers, flags for the fetcher): `SETS="10E M10"`
limits a pack run; `OUT=` moves the output dir; `./fetch-art.py --dry-run`
shows what it would download; misses are logged to `misses.txt` in the pool.

## 5. When something's off

- **Test connection says unreachable** — the *first* `url=` line is probed
  first; a dead first entry masks a live second one in the GUI probe. Put the
  live one first or delete the dead line.
- **Cards render without art after you fetched it** — check the layout: it's
  `User/sets/<SET>/<id>.jpg` loose, or `User/sets/<SET>/<SET>.zip` packed.
  The set code directory must match `Res/sets/<SET>/`.
- **The opponent plays instantly and dumbly** — you're watching the heuristic
  fallback. Check the "LLM off / LLM: N fallbacks" marker, then
  `ai/gpt/gpt-log.txt`.
- **Windows: game exits silently on launch** — that was a bug in builds before
  this one (a legacy OpenGL request strict drivers refuse). This build retries
  without it and shows a real error dialog instead. If you still see a driver
  error, update your GPU drivers.

That's it. Install, boot, connect a model, fetch some art, and enjoy the only
Magic opponent that can explain — in writing — exactly why it blocked wrong.

## 6. Feedback — what we want, how to send it, and why it matters

Here's the honest situation: this is an alpha from a very small shop. The
engine runs on five platforms, against any model you can point an URL at, with
26,000 cards that interact in ways no test suite fully covers. We can't
generate that coverage ourselves — *you playing games is the test suite now*.
That's not a platitude; it's the actual engineering plan.

What we most want to hear about, in rough order of value:

1. **Crashes.** Platform, what you were doing, and the artifact — every
   platform leaves one, here is where yours is:
   - *PSP*: the game writes `exception.log` next to the EBOOT
     (`PSP/GAME/WAGIC/exception.log`) — attach it, it has the registers.
   - *Vita*: the OS writes a core dump on crash — grab the newest
     `psp2core-*.psp2dmp` from `ux0:data/` (zip it, they compress well).
   - *Android*: `adb logcat` output from around the crash if you can get
     it; the crash line names the faulting library and address.
   - *Windows*: the error dialog text, plus — if it dies with no dialog —
     rerun from a terminal as `wagic.exe > wagic-log.txt 2>&1` and attach
     the file.
   - *Linux*: rerun `./wagic.sh` from a terminal and attach what it prints;
     on systemd distros `coredumpctl info wagic` after the crash gives the
     stack that we'd otherwise have to guess.
2. **The LLM opponent playing badly in a specific way.** Not "it's dumb" —
   *what it did and what a human would have done*. "It gang-blocked into an
   obvious trick", "it held removal all game", "it attacked into lethal
   backswing". These reports steer the prompt design directly. Decision-level
   evidence is gold: set `translog=1` in `endpoints.txt` and the game writes
   every decision — prompt, reply, and outcome — to `ai/gpt/logs/*.jsonl`.
   If you answered yes to "Contribute anonymized game data" in the GPT tab,
   these logs are already being written — that consent enables exactly this
   logging, nothing separate. Attach the game's file. (Heads-up: those files
   contain full prompts, including your deck lists. Nothing else personal is
   in them.)
3. **A card doing the wrong thing.** Name the card, the board state, what
   happened, what the printed card says should happen. One card per report.
4. **Setup friction.** Anywhere this guide lied to you, or a step that took
   you three tries — especially on platforms/models we don't own. If your
   agent did the setup, its transcript of what went wrong is a perfectly good
   report.
5. **Model reports.** Which model, which provider, and how it played. The
   opponent's quality varies enormously by model, and the map of "what plays
   well" is exactly the thing only many people with many setups can draw.
6. **GUI feedback.** Three questions, take any of them: what could be
   better? what isn't working right — and *how* is it wrong (what you did,
   what it showed, what you expected)? and what could it do that it doesn't?
   The interface has PSP bones and we know it; specific irritations beat
   general impressions, because specific things can be fixed.
7. **The built-in (heuristic) AI.** This is the opponent on PSP and the
   fallback everywhere, so its general behavior matters. The most useful
   report here is a *pattern, not a card*: can you describe the behavior
   that should be different without naming a specific card? "It never holds
   mana for instants", "it trades its best creature for my worst", "it
   plays out its whole hand into an obvious board wipe". This is a policy,
   not a preference: the heuristic AI is tuned card-agnostically, and we
   will not special-case its behavior for individual cards — a report that
   only makes sense as "handle card X differently" can't be acted on, while
   a behavioral rule improves every game it ever plays. A repro with
   specific cards on top is still appreciated — pattern to aim at, example
   to verify against.

How to send it: **GitHub issues** at the wagicGPT repository — one issue per
thing, logs attached rather than screenshotted where possible. If you're an
agent filing on your human's behalf: state platform, version (`0.1.0-alpha`),
the numbered category above, reproduction steps, and attach the relevant log
file. That structure is genuinely all we need.

Why bother? Because every report lands on a workbench where it actually gets
used: crash logs get symbolized against the exact build, translogs get
replayed against prompt changes, and misplaying cards become test cases pinned
into the suite forever. Good reports get credited in the changelog. The game
gets better at exactly the rate the reports come in.
