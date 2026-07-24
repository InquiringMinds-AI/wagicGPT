# deck137 notes.md — engine/harness/ledger items (wave 23)

## LEDGER (adventure-surface verdict — the brief predicted this is ledger material) [REPRESENTATION]

**R-ADV-FACE: the alternative/adventure-cost cast option prints the WRONG face's (P/T) and no
effect text.** For an adventure card the engine offers two cast options; the adventure-SPELL option
renders as `Cast NAME // ADV with its adventure cost {adv cost} (P/T)` where (P/T) is the CREATURE
face's stats, and it carries NO description of the adventure spell's effect. The executor reads (P/T)
as "the body I get by paying the adventure cost" and reads no effect at all.
- CONFIRMED WRONG-OUTCOME repro: deck131-vs... s15. Options `1. Cast Shepherd of the Flock //
  Usher to Safety with its adventure cost {w} (3/1)` / `2. Cast nothing`. Reply: "Shepherd of the
  Flock is a 3/1 creature ... develops my board, adding another body" -> chose 1. Game log:
  `Shepherd ... hand -> stack -> graveyard -> exile`; s16 forced a bounce TARGET CHOICE and the pilot
  returned its OWN Faerie Guidemother (deployed 1/1 flyer) to hand. A self-bounce it never intended.
- TAX repro: deck27-vs... s19 (9,315 chars) talking itself past the label before playing correctly.
- Contrast with the reveal/search menu (Once Upon a Time), which DOES include `{text: ...}` per card.
  The cast-option surface should do the same for the adventure face.
- FIX (engine/representation, ledger): on the "with its adventure cost" option, either (a) drop the
  creature (P/T) and print the adventure spell's short effect text (mirror the reveal menu's {text:}),
  or (b) label it as the effect, e.g. `Cast Usher to Safety (adventure of Shepherd of the Flock) {w}
  - return a permanent you control to hand; Shepherd castable later from exile`. The creature (P/T)
  belongs ONLY on the creature-face option. Guide teaches the capability table + "(P/T) is the
  creature you get LATER" as the INTERIM stopgap (strategy.txt); remove that block once the option
  annotation ships. Removal condition: adventure-cost options carry effect text / drop the wrong P/T.

**R-ADV-DISTINCT-FROM-DFC (clarification, not a defect): adventures do NOT use the R-DFC-FLIP /
DISPLAY TOGGLE annotation family.** Those annotations fired on 0 of deck137's 197 records. Adventures
are surfaced as two parallel cast options ("base cost" + "with its adventure cost") rather than a
flip-toggle. So the wave-23 delta (b) R-DFC-FLIP work does not touch adventure cards — the two modal
shapes (modal-DFC vs adventure) have SEPARATE surfacing paths, and adventure's path is the one with
the mislabel above. Verdict for the stress item: adventure surfacing is CLEANER than Tergrid's DFC
flip thrash (no dead-ends, no thrash, correct dual options); its single weakness is the (P/T)+no-text
annotation, not the dual-option structure.

## HARNESS (by-design, note only) — convoke tap sub-decisions deferred to heuristic

deck137's 3 fallbacks are all `kind=defer` / `fallback=deferred_to_heuristic` with empty
prompt+reply and latency_ms=-1 (no LLM call): the convoke creature-tap payment for Venerated Loxodon
(deck131 s21, s23) and March of the Multitudes (deck102 s19). This is the harness routing the
"which creatures to tap for convoke" sub-choice to baka by design. Mechanically harmless (engine taps
eligible creatures to satisfy the cost). Not a defect; flagged only so synthesis knows deck137 is the
sole source of the corpus's 3 deferred_to_heuristic fallbacks and none are the NEW unparsed_reply shapes.

## CARD-SCRIPT / ORACLE-VERIFICATION items (route per wagicgpt-verify-oracle-text; GUIDE uses engine)

None rise to a bug, but flag for the owner's Oracle pass (engine text vs real Oracle):
- Lovestruck Beast: engine enforces `cantattack` via TWO gates — needs a creature with power=1 AND a
  creature with toughness=1 on your battlefield. Real Oracle: "can't attack unless you control a 1/1
  creature." A single 1/1 satisfies both engine gates, so behavior matches for a true 1/1; but a 1/2
  (power=1, toughness=2) does NOT unlock it under the engine, whereas real Oracle would also require a
  literal 1/1 (a 1/2 is not a 1/1), so the engine and Oracle AGREE here. No fix needed; documented so
  the guide's "make a 1/1 to unlock" rule is engine-correct.
- Emmara: engine `@tapped(this):create soldier(1/1 white lifelink)` — triggers on ANY tap incl.
  convoke and attacking. Matches Oracle ("becomes tapped"). Confirms the convoke-Emmara-makes-a-token
  synergy the guide notes.
- Venerated Loxodon: engine ETB `all(creature[tapped;-attacking]) counter(1/1)` puts a +1/+1 on each
  tapped non-attacking creature — i.e. the ones that convoked it. Matches Oracle intent.
- Once Upon a Time: engine `otherrestriction` gates the free cast on empty graveyard/exile/stack
  (first-spell). Matches Oracle. Guide's "free as first spell, cast turn 1" is correct.

## ROTATION VERDICT

**deck137 does NOT rotate — newcomers never rotate on their first pass.** This is its Step-0
guideless baseline; the initial guide (strategy.txt) is a HYPOTHESIS that must be validated next
corpus. Keep it in the pool. (No prior guide to freeze, no convergence signal possible on pass 1.)
