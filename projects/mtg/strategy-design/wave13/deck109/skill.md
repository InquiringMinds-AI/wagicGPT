# Skill contribution — deck109, wave 13 (VERIFICATION wave; one method refinement + one hybrid-close reconfirm)

**Mode: FREEZE + VERIFY.** Current skill = `wave11/strategy-writing-skill.md`. deck109's guide is
byte-identical to the live guide. This wave contributes ONE method refinement (how to SCORE attacker
declarations so a silent partial-drop is not mistaken for full declaration) and reconfirms the wave-11
hybrid-close. No new authoring headline — the HIGH adopt-a-revision bar holds on a guide at terminus.

## Method refinement — score attacker declarations by INTENDED-vs-RECORDED count parsed from the `ATTACK:` line, NEVER by window count or `chosen_text`; the MIXED index+name form is a SILENT non-fallback drop

Sharpens the confirmed-fired terminus's "measure in the reasoning, not the recorded choice" (wave-10
headline-2) and the brief's `ATTACK: <name>` ledger candidate, applied to the ATTACKERS seam.

The attacker reply has THREE forms and they fail at DIFFERENT layers; a reviewer MUST distinguish them
because two of the three are invisible to the obvious metrics:

- **FULL-INDEX (`A1, A2, A3`)** — parses; clean.
- **FULL-NAME (`Hellrider` / `Rakdos Cackler, Legion Loyalist`)** — UNPARSED (`fallback=unparsed_reply`,
  `choice=-1`, `chosen_text='None'`) but the baka heuristic then attacks with ALL eligible creatures,
  which for a pure-aggro deck COINCIDES with intent -> OUTCOME-NEUTRAL. This is the brief's
  note-don't-diagnose item. Verify neutrality in the NEXT record's `You declared attackers:` log, not
  by the fallback flag alone (the fallback flag makes it LOOK like a miss; the combat log shows it wasn't).
- **MIXED (`A1, Rakdos Cackler`)** — the dangerous one. The parser consumes the leading A-indices,
  IGNORES the trailing name token(s), and records a NORMAL `choice=N (count)` with NO `fallback`
  field. It looks like a clean declaration in the ledger, but it under-declares: deck109 dropped 6
  attacker-declarations across 5 windows / 3 games this corpus (vs140 s7/s40, vs110 s10/s14, vs135 s8),
  confirmed in combat at vs140 s7 (plan: "attack with both to deal 3"; log: `declared attackers:
  Stromkirk Noble` — one).

**Adopted method rule:** to score the "declare EVERY creature that can attack" guardrail, parse the
reply's `ATTACK:` line into A-index tokens vs NAME tokens and compare the INTENDED count to the
recorded `choice` count. A window is a full declaration ONLY when they match. A raw window count or a
`chosen_text` read (which shows creature names for whatever DID parse) SILENTLY passes the mixed-form
drop — this is why wave-11 scored "26/26 full declaration" and missed the class. The full-name form is
scored by the combat log (neutral if the heuristic attacked with everyone); the mixed form is scored by
the intended-vs-recorded delta (a real drop). Both are HARNESS/attackers-parser items -> notes ledger;
NEVER re-word the guide (the model already NAMES every attacker it wants — the guide is obeyed, the
parse seam drops it; papering a parser bug in prose is the sanctioned-against escalation).

This is single-seat THIS wave at deck109 but it is the 2nd-witness escalation of the brief's own
corpus-wide `ATTACK: <name>` ledger candidate, sharpened from "name form is unparsable" to "name form
splits into a benign full-name variant and a SILENT partial-drop mixed variant." Route to synthesis as
a promotion of the attackers name->index reconcile arm to cover BOTH forms (see general-suggestions).

## Reconfirm (no method change) — the hybrid card-note stopgap VALIDATES a THIRD wave and now the ENGINE-EXECUTION is confirmed

The wave-11 refinement ("a validating card-note stopgap stays at full prominence while its
representation backstop is unshipped") holds. This wave adds the engine-execution leg: not only did the
class stay at zero (0 declines, 0 spirals) and casts get TAKEN, the engine EXECUTED all 7 hybrid casts
(`hand -> stack -> battlefield`): Rakdos Cackler `{b/r}` 5/5 and — new this corpus — Boros Reckoner
`{r/w}{r/w}{r/w}` (triple hybrid off three Mountains) 2/2, confirming the guide's "three Mountains cast
it" at the engine level. The option-line `- hybrid:` annotation (Ledger Item B) remains UNSHIPPED (0
`- hybrid:` strings), so the card note is still the SOLE carrier -> KEEP at full prominence, frozen.

## Everything else held (freeze-checked, no method change)

- Attacker INTENT full declaration at every window (the drops are the harness class above, not intent).
- BLOCKS: none -> ONE window (vs131 s25), correct all-decline via the PARSEABLE `B1:none` form,
  NOT-lethal branch obeyed. LETHAL branch + Obliterator absolute UNTESTED (4th zero-window wave) —
  KEEP at full prominence (pause-aware demotion doctrine; untested guardrail vs a known game-loser).
- Name-echo 100% compliant at this seat, 0 echo-vs-index conflicts (no reconcile arm invoked).
- Burn face-default 0-defect; Young Pyromancer token-maker exception fired (vs131 s8).
- The 3 ask-unparse fallbacks are the benign long-ramble discard class, all in WON games; the wave-11
  vs135 s12 unlisted-card fixation did NOT recur.

Core status (deck109 vote): **PASS.** No 2+-seat core candidate at this seat. The one refinement is a
verification-METHOD item (score attacker declarations by parsed intent), and the one engine item is the
attackers name->index reconcile arm — neither touches the core prose.
