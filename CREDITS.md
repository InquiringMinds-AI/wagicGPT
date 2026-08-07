# Credits

Community contributions and information sources adopted into this fork.
Wagic itself is the work of the WagicProject team and its long line of
contributors — see the upstream repository for full history.

## Adopted fixes

- **flounderbounder** (Wagic Discord, *"Buttons on Retroid Pocket 5"*) —
  Android physical-gamepad mapping. The bundled SDL's Android keyboard layer
  mapped every `AKEYCODE_BUTTON_*` to `SDL_SCANCODE_UNKNOWN`, so controller
  buttons on Android handhelds were silently dropped; their PSP-style mapping
  is applied in `JGE/Dependencies/SDL/src/video/android/SDL_androidkeyboard.c`.
- **flounderbounder** (Wagic Discord, *"Android deck editor screen gestures
  optimisation"*) — deck editor touch tuning for high-DPI screens: vertical
  flick threshold raised (near-horizontal swipes were changing filters),
  vertical scroll capped at one filter step per swipe, horizontal collection
  browsing sped up (`GameStateDeckViewer.cpp`, `SDLActivity.java`).

## Field reports that drove fixes

- **solo_mag** (Wagic Discord) — the set-swap experiment ("0.25 engine with
  0.23-era sets works") plus a crash address from real hardware. Together
  these pointed the PSP memory campaign at data volume and texture pressure,
  which led to the card-data sidecars, the texture-eviction fix, and the
  texture cache cap.
- **flounderbounder** (Wagic Discord) — deck editor vertical-scroll position
  reset report; APK-upgrade data-wipe pitfall (keystore mismatch on reinstall).

## Idea sources

- **bobcyril** (Wagic Discord) — the case for keeping the Demo mode in the
  main menu as an AI deck-testing tool.
