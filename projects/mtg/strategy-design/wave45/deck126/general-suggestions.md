# Core-prompt / render proposals — deck126 seat (wave 45)

## G-D (render, HIGH — see the seat file's engine list for the ranked entry)

The `Mana available:` line needs a per-colour SOURCE COUNT. Full argument, emitter reference and
repro are in `../seats/seat-125-126.md` (engine item #1) and in `../deck125/general-suggestions.md`
G-B; not duplicated here.

## G-E (render, LOW) — `(blocking trigger: ...)` is rendered from the ATTACKER's seat but the DEFENDER's own attack prompt has no converse

#W45-3's tag is confirmed live on both legs this corpus (see the seat file). Note the asymmetry it
creates: an attacker is told what the defender gains
(`Wall of Omens (0/4) (neither dies (blocking trigger: they may gain 2))`), and the deck126 pilot,
whose whole plan is "make them attack into my walls", is never shown the same number on the side
that matters to it — its own B-lines carry `(blocking trigger: you gain 3)` only for the blocker
that itself triggers, not for the Perimeter Captain's per-defender 2. On the deck146 seq-9 window
the board had a Pride Guardian and two Walls of Omens; B1 read `(blocking trigger: you gain 3)`
and B2/B3 read nothing, which is correct card-by-card but under-reports the combat: with a
Perimeter Captain out, EVERY defender that blocks adds 2. Nothing was misplayed on it this corpus
(all 8 offered blockers were assigned in all 5 windows), so this is an observation, not a defect
claim — flagged because deck126's whole life-gain arithmetic lives in that gap.

## G-F (core prompt, LOW) — the mana-only priority window

39 of this seat's 188 decisions (20.7%), and 39 of its 46 priority windows (85%), offered nothing
but `Add N green mana with Overgrown Battlement`. See the seat file's #W44-9 recount — the wave-44
conclusion that 21.6% was "game-composition, not a stable rate" does not survive a second corpus.
The auto-pass proposal is a core-prompt/engine call, not a guide one.
