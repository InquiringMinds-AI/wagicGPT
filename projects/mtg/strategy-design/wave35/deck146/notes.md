# deck146 — wave-35 engine/harness notes (arm C = matchups-20260822-111102 unless stated)

1. **Silverquill Command fused-mode target ask RECURS at the standing pilot, now with a
   harmful pick** (escalates wave-35 tier-review docket item 1). Repro: arm C
   `1787415071-...deck146...vs...deck105.jsonl` s31-s32 (t12): modes "creature gains 3/3 and
   sacrifice creature" chosen, then ONE target ask listing the union — own TA #1/TA #2/Goblin
   AND the opponent's Plague Stinger — with no mode attribution. The pilot flagged the
   ambiguity in-trace ("It implies one target for the whole spell") and picked Plague
   Stinger. Engine ground truth (`borderline.txt`): the battlefield-creature target feeds the
   PUMP half (`target(creature) transforms(...) ueot`); the sacrifice half is
   `notaTarget(creature|mybattlefield) sacrifice ... opponent` — opponent-chosen, never asked
   of the caster. So the pick +3/+3'd the OPPONENT'S creature. Arm B's B3 seq35 was the same
   shape. Fix direction: scope the target list to the mode being paid (own side only for the
   pump), or label the ask with the mode it feeds. The revised guide routes around it
   rules-truthfully (always pick your own creature) but the surface still invites the error.
2. **Agadeem's Awakening no-op cast** (tier-review docket item 3, unchanged): not exercised
   at arm C (guide do-not held at ~10 offers), so no new evidence; the B3 seq74-75 repro
   stands. The revised guide's hardened do-not REDUCES future corpus exercise of this path —
   per HL10.1, deck146 is disqualified as the witness seat for any engine fix here; validate
   via probe or another Agadeem deck.
3. **NOT A BUG — recorded to prevent re-diagnosis**: `1787415106-...deck146...vs...deck158`
   shows zero decision records turns 3-7 (s5 t0 -> s6 t8) while losing 20->9. Verified
   legitimate: the 5-card keep bottomed both Hive lands (s4 "PUT: 3, 7"), leaving hand =
   Soul Shatter / Verse / Kaya / Nadaar with one Swamp in play — no land to play, nothing
   castable, no legal decisions existed. Distinct from the B3 no-attackers stall (docket
   item 2), where untapped legal attackers existed.
4. **Sequential Annihilator sacrifice asks vs multi-pick reply**: vs116 s21 (t9) the ask says
   "Choose card 1 of exactly 4"; the pilot answered all four in one line
   ("CHOICE: 1 (...), 2 (...), 4 (...), 7 (...)"). The engine took the first and re-asked
   (s22-s24) — outcome fine, and the pilot's per-ask picks then diverged from its own list
   (kept Plains, gave the Goblin token — an improvement). Harness question for the re-ask
   docket: is a multi-pick first-token parse the intended handling, or should the ask state
   "answer with ONE card; you will be asked again for the rest"? Low priority; no misplay.
5. Carried from tier review, no arm-C recurrence: blockers-surface "block enough to survive"
   in lethal-either-way states (B2 seq29); A2 truncated log (no gameend record).
