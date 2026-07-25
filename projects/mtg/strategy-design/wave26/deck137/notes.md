# deck137 notes.md -- engine/harness/ledger items (wave 26)

## LEDGER (new, possible) -- R-DUPLICATE-NAME-INSTANCE: same-named creatures in different tap states have NO instance disambiguator; model resolves the collision against itself and declines an engine-offered play. [REPRESENTATION candidate, flag-not-acted, single-seat]
vs18 s36 t11 (deck137 game, log 1785003927-ai_baka_deck137-0x564c4a8f1190-vs-ai_baka_deck18.jsonl).
Board (from s35 render): "Your battlefield (creatures: 3): ... Faerie Guidemother (1/1)
[tapped - untaps and can attack next turn]; ... Lovestruck Beast (5/5) [adventure] [tapped -
untaps and can attack next turn]; ... Lovestruck Beast (5/5) [adventure]". The player controls
TWO Lovestruck Beasts -- one tapped (attacked t10), one untapped -- with identical printed
names and NO instance id (#1/#2, or a per-instance suffix).
At the blockers seam (s36), the engine correctly offered only the UNTAPPED copy:
`B1. Lovestruck Beast (5/5) - may block A1 (you kill it, your blocker lives), A2 (...)`. But the
B-line name gives no way to bind it to the untapped instance, so the model spiraled 10,817 chars
("the log says Lovestruck attacked... it is tapped... it went to graveyard... cannot block"),
overrode the engine's offer, and answered `BLOCKS: none` -- declining a free kill-and-survive
block, taking 4 (life 12->8). deck137 WON the game anyway (adj 8/6), so single-seat, non-fatal.
- The RENDER is not wrong per se (it does show one copy tapped, one untapped, and the B-list is
  correct) -- the gap is the ABSENCE of an instance handle that lets the model map the offered
  B-line to the specific untapped copy when the name collides. This is a MODEL/representation
  interaction, not a legality bug.
- PRIMARY MITIGATION shipped this wave = the DECK GUIDE: a BLOCKING clause that the offered B-list
  is authoritative -- a creature on a B-line is untapped and can block this turn even if the log
  says it attacked or a same-named creature is tapped; take the "you kill it, your blocker lives"
  block; do not re-derive tapped state from the log.
- REPRESENTATION candidate (flag only, single-seat -- do NOT act on one seat): IF the same
  duplicate-name confusion recurs at another deck that runs multiples of a creature (or with
  tokens sharing a name), disambiguate same-named permanents in the battlefield list AND in the
  A/B option lines with a stable per-instance handle (e.g. "Lovestruck Beast #2 (untapped)" or an
  instance suffix) so an offered A/B line binds unambiguously to one body. This is corpus-general
  by construction (any deck with 2+ same-named creatures in mixed states), but only ONE witness so
  far. Repro: seq 36 (and its s35 antecedent) in the log above.

## LEDGER -- R-SHOCKLAND-ETB-UNLABELED (wave-24 b4 / wave-25 cosmetic residual): CLOSED by c2. [REPRESENTATION -- resolve]
Wave-25 downgraded this to cosmetic: the shock consequence rode the card `{text:}` but the
standalone pay/tap MENU OPTIONS carried no per-option consequence tag (bare `1. pay 2 life /
2. tap`). The wave-26 c2 fix ("ETB pay-or-tap annotations NOW ACTUALLY FIRE") CLOSES this: all 7
Temple Garden menus this corpus render self-describing options --
`pay 2 life [this permanent then enters the battlefield UNTAPPED - usable (tap for mana / attack)
this turn]` / `tap [... enters the battlefield TAPPED - unusable until your next untap step]`
(vs131 s3/s52, vs18 s3/s18, vs22 s3, vs27 s13, vs133 s16). All chose=1 (pay 2 life), all parsed,
no stale_echo, no double-annotation. The wave-25 remedy (label the options with pay->untapped /
skip->tapped) is exactly what shipped. ONLY residual = the empty menu subject line
`Choose an option for :` in the standalone case -- pure cosmetic now that the options self-describe;
no observed harm. Recommend CLOSE (options-tagged) with the empty-subject as a trivial leftover.

## HARNESS -- no timeout this corpus; -T 3000 floor held under deck137's densest game. [note]
Zero timeouts/draws corpus-wide. deck137's densest game was vs131 (63 LLM calls, sum 2,250s,
med 16.7s, max 209.9s) and it WON (adj 8/1 t12) with no cap starvation -- the -T 3000 floor
absorbed the density that capped the wave-24/25 grinds. Per-game call/latency:
vs102 36/836s, vs131 63/2250s, vs18 42/1717s, vs22 43/1914s, vs27 32/703s, vs133 29/1188s.
- SIDE OBSERVATION (harness/server, recurring): every one of my 6 games again has exactly ONE
  ~200-215s latency outlier against a ~17s median (110.3/209.9/203.8/214.7/104.5/153.0 -- the
  four longer games each spike ~200s). Same per-game single-outlier signature as wave-25; likely
  a server-saturation queue event under -j3, not a play issue. Not actionable at my layer; noted
  for whoever tunes concurrency.

## CONVOKE DEFERS -- 15, by-design, HARMLESS (4th consecutive corpus). [note, no item]
All 15 fallbacks corpus-wide are deck137's convoke tap-payment defers (kind=defer,
deferred_to_heuristic, latency=-1, empty prompt+reply). Distribution deck22 x9, deck27 x4,
deck131 x2 (matches the brief's 9/4/2). No stranded attacker: vs22 attacked s28 t6 / s41 t8 /
s49 t10 despite defers those very turns. deck137 remains the sole defer seat. No ledger item.

## CARD-SCRIPT / ORACLE-VERIFICATION -- no new items.
No card-behavior discrepancies. Adventure/convoke/wither/lifelink all reasoned soundly against
their rendered text. The s36 issue is a duplicate-NAME instance ambiguity plus a model belief
overriding a CORRECT engine offer -- not a card-script or oracle discrepancy.

## c1-c5 WATCH -- c2 validated (above); c5 UNEXERCISED at this seat (no may-triggers); c1/c3 no exercising seat. Clean.

## ROTATION VERDICT
**deck137 does NOT rotate.** Fails BOTH rotation criteria:
- Guide WAS modified: the BLOCKING section gained a "the offered B-list is authoritative --
  trust the offered block, do not re-derive tapped state from the log" clause (from the s36
  breach); the blockers-side analogue of the wave-25 attack-list inverse.
- Fresh new-work signal EXISTS: (a) the s36 blockers value-decline + the new
  R-DUPLICATE-NAME-INSTANCE representation candidate; (b) the CLOSING-SPEED deck-layer play gap
  (2 of 3 wins were adjudicated-ahead, losses still late-stabilization one turn too late) --
  named this wave as the unconverged frontier.
HONEST endgame note: deck137's COMPREHENSION surface HAS converged (0 true-fallbacks,
adventure-body fixed and first-corpus-validated, c2/c4 clean, blockers-lethal floored). Its PLAY
quality has NOT -- the deck wins by being ahead at the cap and loses the same tempo races. That
is deck-layer play work (curve/sequencing/commit-timing), the new-work signal that keeps it in.
The new guide clause is an unvalidated hypothesis that must run again (this is only guided round 4).
