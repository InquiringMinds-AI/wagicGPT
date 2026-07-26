# deck137 notes.md -- engine/harness/ledger items (wave 27)

## LEDGER (NEW, HIGH -- step-1 candidate) -- CONVOKE CASTS DO NOT RESOLVE: the "Cast X with its convoke cost" variant, when chosen, aborts the cast; the card stays in hand. deck137's two payoff spells are functionally dead. [ENGINE/HARNESS, corpus-wide at the convoke seat]
Corpus matchups-20260725-193608. deck137 is the sole convoke deck; this is only observable here.

RESOLUTION AUDIT (convoke picks -> actual resolutions, all 6 deck137 games):
- Venerated Loxodon: 23 convoke picks -> 2 battlefield entries.
- March of the Multitudes: 17 convoke picks -> 0 resolutions, 0 Soldier tokens ever created.

REPRO A (affordable convoke fails) -- vs18 s16 t4
(log 1785026874-ai_baka_deck137-0x562af65a9270-vs-ai_baka_deck18.jsonl):
Mana `{g}{w}{w}` (3) + 2 untapped creatures (Flaxen Intruder 1/2, Giant Killer 1/2). Loxodon = {4}{W}
= 5 = exactly coverable (3 mana + 2 convoke). Model chose `Cast Venerated Loxodon with its convoke
cost`; defer fired (s17). NEXT record (s18): only ONE land tapped, both creatures STILL untapped,
Loxodon STILL in hand -- the convoke payment did NOT tap the creatures and the cast aborted. Model
re-picked Loxodon at s19/s24/s28/s39/s54; Loxodon entered the battlefield exactly ONCE (t11, per
mirror log 1785026874-ai_baka_deck18-...-vs-deck137). ~8 failed attempts; board stuck at 2 creatures;
deck137 raced 20 -> dead (loss).

REPRO B (plain cast offered alongside, convoke chosen, only convoke fails) -- vs134 s34/s36/s55
(log 1785029176-ai_baka_deck137-0x55ce87304360-vs-ai_baka_deck134.jsonl):
Engine offered BOTH `Cast Venerated Loxodon {4}{W}` (plain) AND `Cast Venerated Loxodon with its
convoke cost`, and `Cast March of the Multitudes {G}{W}{W}{X}` (plain) AND the convoke variant. At
s34 (5 mana pooled) and s55 (6 untapped mana) the PLAIN cast was fully affordable. Model chose the
CONVOKE variant; it FAILED. March sat in hand from s10 (t2) to s61 (t12), picked ~10 times, never
left the hand. The only movement events for March are `library -> hand` (draw) and
`Choose an option for March of the Multitudes: -> convoke` (the pick) -- never `hand -> stack`, never
a Soldier `created -> battlefield`.

MECHANISM (suspect, for the fixer): the convoke tap-payment sub-step is deferred to the AIPlayerBaka
heuristic (kind=defer, deferred_to_heuristic, latency -1, empty prompt/reply). The heuristic appears
to NOT tap creatures to satisfy the convoke portion of the cost (REPRO A taps only a land), so any
convoke cast that needs creature-taps to be affordable aborts and reverts the card to hand. For the
X-spell March it is worse -- 0/17 resolved even when mana alone would cover a small X (REPRO B s55),
so the convoke + X-selection + tap chain does not complete at all. Likely in the convoke-payment path
reached from AIPlayer's deferred-decision handling (convoke cost step) and/or the X-spell interaction.
PRIMARY layer = ENGINE/HARNESS; do NOT paper over with a guide clamp -- the guide edit this wave only
routes the model to the working PLAIN cast when one exists (recovers late casts); early tempo convoke
stays dead until this is fixed.

REFUTES the 4-corpus "convoke defers are harmless by-design" verdict: prior reviewers checked that
deck137 still attacked on defer turns (true) but never that the deferred CAST resolved. It mostly
does not. This is the mechanistic root of deck137's 3/6 -> 2/6 drop and its closing-speed gap.

## LEDGER (minor) -- DEFER CLASS IS NOT CONVOKE-EXCLUSIVE. [note]
The corpus's 23rd defer is a deck134 defer (log 1785032563-ai_baka_deck134-0x557a6a5e48e0-vs-deck131,
seq39 t19, Main phase 1, empty prompt, options=0). deck134 (mono-U tron) runs no convoke card, so the
`deferred_to_heuristic` fallback fires on any optionless deferred step, not just convoke. The brief's
"all 23 defers = deck137 convoke" attribution is off by one (22 deck137 convoke + 1 deck134 other).
Not actionable; corrects the tally for synthesis.

## LEDGER -- R-DUPLICATE-NAME-INSTANCE: CLOSED at the deck137 seat by the d1 fix. [REPRESENTATION -- resolve at this seat]
The wave-26 candidate (same-named permanents in mixed tap states had no instance handle; the model
resolved the collision against itself and declined an engine-offered block) is RESOLVED. The `#N`
handles render on board/A/B/target lines and the model binds them correctly. Direct successor bind:
vs131 s27 t12 -- two Faerie Guidemothers, one tapped one untapped, B-line `#2`, model bound `#2` to
the untapped copy and took the block ("My other Faerie Guidemother is tapped ... cannot block").
Further correct binds: vs22 s29 (distinct Soldiers onto Blind-Spot Giant #1 and #3), vs93 attacks
(Faerie #1 + #2). No duplicate-name bind failure this corpus. Recommend CLOSE at deck137; the core
reviewers still watch for the corpus-general promotion (2nd deck) at deck93's 36-Rat stressor.

## LEDGER (WATCH) -- illegal one-blocker-onto-many shape persists under lethal pressure; c4 parser recovers it. [note]
vs18 s58 t11 (life 2/13, board unwinnable): `BLOCKS: B1:A2, B1:A3, B1:A4, B2:none` -- Loxodon piled
onto three attackers, NOT self-corrected. Parser recovered to the first legal pair (Loxodon blocks
Field Marshal). vs22 s39: same illegal open (`B3:A1..A4`) but the model DID self-correct in prose.
So the illegal-gang reflex still fires when panicking under lethal; the parser's first-legal-pair
recovery is the floor. Non-fatal both times (vs18 was already dead; vs22 recovered to a good block).
No guide change (the BLOCKING clause already states the one-attacker rule); WATCH for a case where the
recovery discards a BETTER legal block than the first-listed pair.

## HARNESS -- latency: same single ~200s outlier per longer game as prior corpora. [note]
Per-game LLM-call latency again shows one ~200s spike against a ~17s median in the denser games; not
a play issue, consistent with server-saturation queueing under -j3. Not actionable at my layer.

## c1-c5 / d1-d5 WATCH -- d1 validated (above); d2 changeling annotation clean at my seat (Universal
Automaton `[changeling: counts as Giant]`); c2 shockland self-describing (all pay-2, all parsed);
c1/c3/c5 no exercising seat; d3/d4/d5 steady (no anomaly, no Kaldheim DFC, clean deck load). c4 gate
PASS (no reversal drop; two illegal-shape blockers recovered).

## ROTATION VERDICT
**deck137 does NOT rotate.** Fresh HIGH new-work signal: the CONVOKE resolution defect (step-1
candidate) -- deck137's payoff spells are dead, the mechanistic root of its 2/6 and closing-speed
gap, observable only at this convoke seat. Guide also modified (convoke-preference edit). The
comprehension surface has converged and d1 is validated, but this corpus produced the seat's most
important finding in several waves. KEEP in pool; re-audit convoke resolution once the engine fix
ships, and reassess closing speed (currently engine-bound) after that.
