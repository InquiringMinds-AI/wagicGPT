# deck102 dev notes - wave 22 (engine/harness/representation ledger + rotation verdict)

## CLOSED / VALIDATED this corpus

### R-PAINLAND (wave-21 R-PAINLAND -> item e): CLOSED / VALIDATED at deck102.
Both durable surfaces render and the wave-21 self-death class stopped.
- CAUTION line (mana line): "CAUTION - some usable mana sources DAMAGE YOU when tapped for mana (auto-tap
  when you cast may spend them - count this life loss first): Ancient Tomb: 2 damage".
- Battlefield tag: "Ancient Tomb [tapping for mana deals 2 damage to its controller]".
- Self-death class did NOT recur: vs135 the model PASSED at 4 life (3 consecutive priority windows) instead
  of self-tapping; vs44 took 4 Ancient Tomb taps across 15 turns and SURVIVED to adjudication.
Repros: vs135 (drew Ancient Tomb, 3 self-taps), vs44 (4 self-taps, survived). Recommend closing R-PAINLAND
in the master ledger. Guide crutch DEMOTED (strategy edit 1) per the crutch-lifecycle rung; a now-FALSE
clause in the old crutch ("the Mana available line does NOT warn you") was removed.

### HARNESS-N9 residual: BOTH retracted_choice fires at this seat are CORRECT FIRES (no residual FP).
The N9 reference-scoping brought this seat's retraction class down to 2 fires; both are genuine disavowals:
- s21 vs62: first-line "CHOICE: 4 (Smallpox)", body then explicitly abandons option 4 dozens of times
  ("I will choose option 1, Thought Vessel" @6752; "the opponent as the target" @5388), truncates, no PLAN:.
- s29 vs140: first-line "CHOICE: 1 (Cast Arcane Signet)", body repeatedly commits to Mox Jet (option 2)
  ("I will cast Mox Jet" @9494, "So I will cast Mox Jet" @10499), never re-commits to Signet, truncates.
Both are real self-disavowals; heuristic fallback appropriate. N9 fix validated clean at deck102. Routed
to synthesis for the master N9 verdict.

## NEW representation / engine items

### R-DFC-FLIP (HIGH, cross-deck for any deck running MDFC/DFC cards) - the "Flip Side" face-toggle.
Tergrid, God of Fright is the Kaldheim DFC "Tergrid, God of Fright // Tergrid's Lantern". The engine surfaces
the card ONLY as a repeatable priority pseudo-action "Flip Side with Tergrid, God of Fright" /
"...Tergrid's Lantern" - never a clean "Cast (front)" / "Cast (back)". Consequences observed vs109:
- The model selected "Flip Side" 11 times (seq8,15-24) - each a no-op face toggle that produced zero game
  action; events show 12 "You: Flip Side with ..." lines and the card stayed in hand all game.
- The toggle is offered at PRIORITY / opponent-turn windows where a sorcery-speed DFC creature/artifact
  cannot be cast anyway - implying a legal play exists when none does.
- At s42 the model mistook the uncast, in-hand Lantern for an on-battlefield ACTIVATABLE permanent and
  emitted a phantom "CHOICE: 1 (Activate Tergrid's Lantern)" (option text was "Flip Side..."), which tripped
  the stale_echo fallback (see below). The name-mismatch (invented "Activate" vs offered "Flip Side") is the
  fallback trigger.
Proposed durable fix (representation): present a DFC as a single castable option per legal face
("Cast Tergrid, God of Fright ({3}{B}{B})" / "Cast Tergrid's Lantern ({3}{B})") gated to the phases/mana
where each face is legal, NOT a repeatable phase-agnostic "Flip Side" toggle. Repros: vs109 seq8 (first
flip), seq9-44 (the toggle offered every priority window), seq42 (the phantom-Activate stale_echo). Routed
to whoever owns option-generation / DFC rendering.

### R-POX-DISCARD-TARGET (MEDIUM) - self-discard rendered as a confusing "TARGET CHOICE".
vs62 s21: after Pox resolved, the player's own forced discard was presented as
"TARGET CHOICE for discard cards (this spell/ability is already on the stack and needs a target...) Pick
target 1 of exactly 2 from the list below, and answer with the chosen TARGET's name (not 'discard cards')"
with Hymn to Tourach's text embedded and the 4 hand cards listed as "targets". The model could not tell
whether it was choosing a player to target or a card to discard, and spiralled 16,752 chars (the s21
retracted_choice). The "target N of exactly 2" wording with 4 listed options and an unrelated spell's rules
text embedded is the confusing part. Proposed fix: render a player's own forced discard/sacrifice selection
as a plain "choose which of YOUR cards to discard" pick, not as a "TARGET CHOICE". Repro: vs62 s21.

### s42 vs109 stale_echo classification (owned): NEW shape, (c) fixes do NOT cover it.
Not a source-prefix ("<spell> targeting <target>") shape and not a combat-math "Attack:" label-hijack.
Shape: model emits leading "CHOICE: 0 (pass)", then a second "CHOICE: 1 (Activate Tergrid's Lantern)" whose
name does not match the offered option ("Flip Side with Tergrid's Lantern"). Root = R-DFC-FLIP. Routed to
deck140 (stale_echo cluster owner) to fold into the family verdict: (c) did not close this shape.

## Corpus-construction / deck-content observations (not engine bugs)

- Tergrid steal engine STILL unexercised: 0/18 games across THREE corpora. vs109 drew it turn 4 but the deck
  was mana-screwed at 3 mana all game (Lantern face needs 4, God needs 5) - neither face castable, compounded
  by R-DFC-FLIP. A Tergrid + Hymn/Thoughtseize/Smallpox/edicts probe deck vs an opponent with cheap
  permanents remains the only reliable way to validate the steal seam directly (per the skill's probe-deck
  pattern). Routed to whoever owns engine-seam validation.
- Mana-base fragility recurs: vs109 stuck at 3 mana (3 Swamps, no productive land 4) cost the whole game;
  vs62's fast loss again correlates with a slow non-Swamp opener. The guide already tells the pilot to keep
  2-4 Swamps at mulligan; this is a deck-construction observation (routed to the roster owner), not a guide
  or engine bug.

## Model-behavior observation (context for synthesis, not an engine item)
- The behind/confused giant-reply TAX persists and now manifests as verbatim-repeated reasoning loops that
  truncate: s29 repeats an identical mana-arithmetic block 6x before truncation; s21 repeats an identical
  flip-flop dozens of times. Answer-first still lands a complete CHOICE: line first (0 unparsed this corpus),
  so this only converts to a fallback when the looping body coherently disavows the committed choice (the 2
  retracted_choice fires). Not guide-fixable; a harness-side "verbatim-repeat -> cut" guard is a candidate
  but out of scope here.

## ROTATION VERDICT
**NOT a rotation candidate.** deck102 generated real targeted guide work this wave (two edits: DEMOTE the
painland crutch, and ADD Tergrid DFC/Lantern awareness the guide was missing) = a new-work signal, and its
keystone engine payoff (Tergrid steal) is STILL un-live-tested (0/18) and is now blocked by a newly found
representation defect (R-DFC-FLIP) that must be fixed before the steal can be validated. Re-validate next
wave (Step 0-quinquies): does the DFC guide edit help the pilot commit to a face once R-DFC-FLIP is fixed;
does the painland demotion hold; and (probe-deck permitting) does the steal finally fire.
