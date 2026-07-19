# Deck-110 — Wave-19 general (cross-deck / non-deck-specific) suggestions

Layer-routed items that are NOT deck110 guide text. Each carries a repro from this seat; all are
cross-deck (any seat can exercise them). Ordered by leverage.

## 1. BLOCK-ECHO NAME→LABEL RECONCILE — VALIDATED FIXED (close the wave-18 cross-deck item)
Wave-18's highest-leverage new representation fix (the `blockers` matcher accepting only `B#:A#`, dropping
name-format block declarations to the heuristic — biting at deck21 s18) is **confirmed shipped and working**
by the wave-19 combat name-tolerance batch (`4becc83be`, parseBlockAssignments name→label second pass).
Direct proof at this seat: **deck27 s23** reply `BLOCKS: Ornithopter:Zombie Master` parsed to `choice=1`
(`chosen="Ornithopter blocks Zombie Master"`, no fallback), while label-format `BLOCKS: B2:A1` still parses
(deck49 s12). Block fallbacks at this seat collapsed to 0. **ACTION: mark the wave-18 cross-deck block-echo
item RESOLVED.** It now mirrors the working `CHOICE:` name-echo, as proposed. Keep as a regression watch
(re-verify by absence / name-format parse next corpus).

## 2. TRADE OUTCOME annotations — VALIDATED, keep; they measurably improve block quality
The wave-19 per-pairing block annotations ("(you kill it, your blocker lives)", "(your blocker dies,
attacker lives)", "[deals 0 - this block kills nothing]") are **correct and quality-positive**. At
**deck49 s12** the annotation drove the strictly-optimal block (Master of Etherium kills Dragonmaster
Outcast and survives, over chumping with a Vault Skirge). No mislead observed at this seat (deck27 s23
annotation was also correct, in a moot/lost position). **ACTION: none needed** — this is a positive
validation of the new annotation; retain it. Cross-deck reviewers should confirm the "(both die)" and
gang-block-composite annotations are equally accurate at combat-heavier seats.

## 3. salvageLoopedChoice — prose-commitment gap (standing cross-deck representation item, dormant)
The salvageLoopedChoice batch re-parses the *last well-formed `CHOICE:` line* from a rambling reply. But
the 2 unparsed at this seat (deck27 s18, deck49 s7, both 13–14k-char overthinks) never emitted the protocol
token — they ended on **freeform prose commitments** ("I will play the Mountain", "I will choose Cast
Cranial Plating"). Salvage had nothing well-formed to recover, so both fell to the heuristic (choice=-1).
LEVER: extend salvage (or the parser) to recognize a terminal prose-commitment line — "I will
{choose/play/cast} <option-name>" resolved against the option roster via the same name-echo the `CHOICE:`
matcher already uses — as a last-ditch recovery when no `CHOICE:` token exists. Owner: AIPlayerGPT reply
parsing. Cross-deck (every seat can overthink into prose); priority LOW/dormant (non-biting at this seat —
both fell in a land-drop and an early cast; but it is the residual giant-reply-tax failure mode now that
block-echo is fixed). Do NOT solve in guide prose (reply-protocol layer).

## 4. E2 — equip no-op marker keyed on OUTCOME DELTA (standing, dormant, unfixed)
The equip "(ALREADY attached — change NOTHING)" marker is TARGET-IDENTITY-keyed and blind to net-zero
MOVES (re-equipping a Plating from one carrier to another where resulting power does not rise). Validated
harmless a 5th consecutive wave (6/6 equips value-positive, 0/24 ALREADY-takes) — the blind spot got no
exercise because every real equip raised power. LEVER (unchanged): compute the no-op marker from OUTCOME
DELTA — suppress "Equip … targeting X" when the move would not raise X's resulting power above its current
carrier; this also subsumes the sole-option ALREADY re-offer menus. Owner: AIPlayerGPT equip-option
serialization. Priority MEDIUM/dormant. Cross-deck for any equipment deck.
