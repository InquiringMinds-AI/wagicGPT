# General / core-prompt suggestions (from deck109 wave-8)

Layer-routed per the skill. Single-seat evidence cannot cut or add a core line — these are OBSERVATIONS
for the synthesis agent to weigh against cross-deck convergence.

## 1. [ENGINE / REPRESENTATION — the wave's headline for synthesis] Blocker-seam math (#5) fired but did NOT stop reflexive/punisher blocking.
Engine change #5 surfaced the correct arithmetic in ALL 3 blocker prompts ("Unblocked... you would be at
M - NOT lethal: taking damage while ahead is often correct"). The pilot disobeyed it in BOTH loss games:
- vs110 seq13: gang-blocked Master of Etherium (6/6) at 19 life ("at 12 - NOT lethal").
- vs133 seq22: blocked Phyrexian Obliterator (5/5) at 10 life ("at 3 - NOT lethal") INTO its sacrifice
  punisher, going manaless.
The representation is PRESENT-AND-LOSING at this seat. Per doctrine this is NOT a call for a louder core
blocking sentence (rejected 3x; reflexive blocking is per-deck/representation). But it IS evidence the
synthesis agent should weigh: the shipped remedy for the reflexive-blocking convergence did not close it
here. Candidate ENGINE follow-ups (for the ledger, not the prompt body): make the "NOT lethal" rider a
harder directive, and — see #2 — actually surface the PUNISHER rider on Obliterator.

## 2. [ENGINE gap — route to notes/ledger] Punisher text rider (#5's second half) did NOT fire for Phyrexian Obliterator.
In the vs133 blocker prompt, A2 read only "Phyrexian Obliterator (5/5) [trample]" with NO sacrifice-
punisher rider, while A1 Bloodghast got a full "{text: ...}" rider. Obliterator IS the punisher whose text
should ride the blocker option (auto=@damaged(this)... sacrifice). Had the rider surfaced, the pilot might
not have blocked into it. Repro: file `1784027795-...deck109...jsonl` seq22. Deck-agnostic engine gap;
does not touch the prompt body. Handled per-deck this wave (named "never block Obliterator" line).

## 3. [FIXED — no core action] Reply-parser desync, cast no-op, phantom-missed-cast.
0 desyncs, 0 no-ops, 0 phantom framing across 6 games. Core `hand -> stack` anchor removal condition stays
met from this seat (0 no-ops even post-fallback-deletion); the wave-8 A/B for it is already adopted per the
reviewer brief. No change.

## 4. [WATCH only — no core action] Decline-a-listed-body.
No 2nd instance this wave (see skill.md #3). Core already forbids declining a listed action; keep at 1 seat.

No new core doctrine proposed from this seat.
