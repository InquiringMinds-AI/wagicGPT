# deck137 notes.md — engine/harness/ledger items (wave 25)

## LEDGER — R-STALE-ECHO-QUALIFIER (b3): CONTAINMENT HOLDING; exact tap-collision UNTESTED this corpus. [PARSER — keep open, low priority]
The wave-24 fix shipped (b3, echo-qualifier containment trust). Corpus-wide stale_echo = 0, including
my seat. The narrow s4 shape that motivated the item — `CHOICE: 2 (Tap Temple Garden)` colliding with
a stale "Play Temple Garden" sibling — did NOT recur because the model chose "pay 2 life" on EVERY
shock this corpus (never "tap"), so no "Tap <card>" qualifier was emitted to test the collision path
directly. Containment IS exercised and holding on the qualifier-decorated shapes that did occur:
deck59 s10 (Flower//Flourish + hybrid gloss), deck27 s22 (Create human with Castle Ardenvale),
deck133 s26 (Venerated Loxodon), deck133 s25 attackers with a name gloss — all parsed to the correct
index, fb=None. VERDICT: item effectively resolved for the general case; the specific tap-collision
remains unproven-not-refuted (behavior avoided it). Leave open at low priority; it re-tests only when
a deck chooses "tap" on a shockland (an aggro deck that wants the untapped land will keep paying 2).

## LEDGER — R-SHOCKLAND-ETB-UNLABELED (b4): DOWNGRADED to cosmetic. Card-text annotation shipped; standalone menu still unlabeled. [REPRESENTATION — low priority]
b4 shipped the shock consequence into the card's `{text:}` — `Temple Garden (land) {text: ({T}: Add
{G} or {W}.) -- As Temple Garden enters, you may pay 2 life. If you don't, Temple Garden enters
tapped.}` — visible in every reveal/hand list. This is what made the wave-24 comprehension confusion
go away: all shock decisions this corpus were answered correctly (pay 2 life, choice=1, no fallback,
no stale_echo). RESIDUAL: the standalone ETB DECISION MENU still renders `Choose an option for :`
(EMPTY subject) with bare `1. pay 2 life` / `2. tap` and no per-option consequence tag (deck109 s4,
deck59 s3). The annotation reached the card TEXT, not the MENU OPTIONS. Because decisions are now
correct, this is downgraded from a harm item to a cosmetic representation nit: the FULLER fix would
label the menu subject and tag the options, e.g. `As Temple Garden enters: pay 2 life (enters
untapped) / do not pay (enters tapped)`, mirroring how other ETB/cost menus carry subject+consequence.
Low priority (no observed harm); corpus-general (every dual/shockland in every deck hits the bare menu).

## LEDGER (possible) — R-ADV-BODY-TAG-MISREAD: model reads a resolved battlefield [adventure] creature as "not a real 1/1". [REPRESENTATION candidate, flag-not-acted — single-seat]
deck27 s21 (deck137 game). Board: I control Lovestruck Beast (5/5) + `Faerie Guidemother // Gift of
the Fae (1/1) [flying, adventure]`; opp has only `Oona's Gatewarden (2/1) [flying, defender, wither]`;
life 18/20, my turn. Engine correctly offered Lovestruck Beast as attacker A1 (its "control a 1/1"
condition is met — Faerie Guidemother is a live 1/1). Model chose `ATTACK: none`, reasoning
"Faerie Guidemother ... is an adventurer face, not a 1/1 body" — misreading the `[adventure]` tag on a
RESOLVED battlefield creature as meaning it is not a real 1/1. Passed a kills-or-trades-up 5/5 swing
while ahead (tempo leak).
- Render is CORRECT (the (1/1) IS shown), so this is a MODEL BELIEF, not a render bug — primary fix is
  the DECK GUIDE (shipped this wave: a paragraph that a battlefield [adventure] creature is its printed
  (P/T) body for all purposes; + the ATTACK exclusion's inverse "if Lovestruck IS listed, a 1/1 is
  already confirmed — swing").
- REPRESENTATION candidate (flag only, single-seat): IF the same "tag read as not-a-real-creature"
  misread appears at another seat ([adventure]/[from exile]/token), consider dropping the `[adventure]`
  tag once the card is a resolved battlefield body (the tag's job — disambiguating the two cast faces —
  is done once it is on the field), or annotating it "(a real 1/1 creature)". Do NOT act on one seat.
  Repro: log 1784973912-ai_baka_deck137-0x5567619bef80-vs-ai_baka_deck27.jsonl seq 21.

## HARNESS — the 1 timeout-draw (133v137) is a LATENCY-BOUND grind; -T 3000 bump WORKED for deck137. [note]
The corpus's single timeout is 133v137 (mine), turn 14 at 10/10 (cap-adjudicated draw). Decomposition:
- deck137 seat: 31 LLM calls, sum 700s, med 16.3s, max 140.8s — LEAN, full deliberation, no truncation.
- deck133 seat: 51 LLM calls, sum 2,146s, med 18.9s, max 202.5s — the density culprit this game.
- Combined 2,846s, just under the 3,000s cap.
Contrast wave-24's 27v137 timeout: capped at TURN 10, ~2,352s ~ the old 2,400s cap, deck137 THEN the
52-call density culprit. The +600s bump (-T 2400 -> 3000) bought +4 turns of depth (t10 -> t14) and let
deck137's decisions complete un-starved. So the bump did what the wave-24 note asked. deck137 was NOT
decision-dense this game (its go-wide never developed under pressure). Standing recommendation
unchanged: deck137 grinds can still cap when the OPPONENT is also dense; -T 3000 is the right floor,
and cap-adjudicated even-life draws are correct outcomes, not play failures.
- SIDE OBSERVATION (harness/server): every one of my 6 games has exactly ONE ~200-217s latency spike
  (209.8/140.8/206.5/204.9/209.3/217.0) against a ~17s median — a consistent per-game outlier, likely
  a server-saturation queue event under -j3, not a play issue. Noted for whoever tunes concurrency;
  not actionable at my layer.

## CONVOKE DEFERS — 15, by-design, HARMLESS (3rd consecutive corpus). [note, no item]
All 15 fallbacks corpus-wide are deck137's convoke tap-payment defers (kind=defer,
deferred_to_heuristic, latency=-1). Distribution deck131 x6, deck102 x4, deck27 x3, deck59 x2. No
stranded attacker/blocker: deck131 attacked t7 (Flaxen Intruder) and t11 (Flaxen + Innkeeper) despite
convoke defers those turns. deck137 remains the sole defer seat (convoke is a single-deck mechanic).
No ledger item; flagged so synthesis knows the 15/15 fallback attribution.

## CARD-SCRIPT / ORACLE-VERIFICATION — no new items.
No new card-behavior discrepancies surfaced. Adventure/convoke/wither/persist all reasoned soundly
against their rendered text. The deck27 s21 issue is a model belief about a CORRECT render, not a
card-script or oracle discrepancy.

## RULES-FIX BATCH (batch A) WATCH — nothing anomalous at this seat.
Turn-1 upkeep, no-priority untap/cleanup, combat-role clearing, planeswalker redirect removal, and
creature-Battle restrictions: deck137 has no cards that exercise these edge cases, and I observed no
turn-1 narration confusion, no expectation of an old redirect menu, and no combat-role leakage. Clean.

## ROTATION VERDICT
**deck137 does NOT rotate.** It fails BOTH rotation criteria:
- Guide WAS modified this wave: a new "resolved adventure creature is a real body" paragraph + the
  ATTACK exclusion's inverse (both from the deck27 s21 misplay) + a ROLE-CHECK strengthening (hold
  small bodies as blockers when stabilizing, from the deck109 kill).
- Fresh new-work signal EXISTS: the deck27 s21 adventure-body-misread misplay (new), the confirmed
  spiral-relocation-to-generic-seams (skill PROPOSAL 1 promotion), and the b4 standalone-menu residual.
The new guide paragraphs are unvalidated hypotheses that must run again (this is only guided round 3).
Keep in pool. HONEST endgame note per the brief: deck137's PARSER surface has converged (0
true-fallbacks, blockers floored, b2/b3 validated) — but its PLAY quality has NOT (1/6, chronic
tempo/late-stabilization losses, a live comprehension misplay this corpus). So it is not a converged
seat yet; it has open deck-layer play work, which is exactly the new-work signal that keeps it in.
