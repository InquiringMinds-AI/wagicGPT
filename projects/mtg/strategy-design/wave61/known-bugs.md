# Wave-61 step-one input: KNOWN BUGS from the wave-60 evaluation (orchestrator list; details in the cited files)

Wave-60 corpus: 21/21 natural, 0 crash/hang, 2,330 decisions, fallbacks 12, empty_reply/bad_reply/reveal_stall_forced 0,
GAME LOG 62.9% of bytes (was 76%). Guides: 146/125/126 edited, others kept; no proposals.

HIGH (engine / interface):
1. `ATTACK TOTAL` (B11) states a resulting opponent life that ignores (a) blocking-trigger lifegain listed two
   lines above (Perimeter Captain ×2: floors 43/57/71 vs actual 66/80/94, +23 error, 3/3 swings — decided
   deck152's only loss), (b) attack-triggered punishers (Lightmine Field: "2 damage, at 31" → both attackers
   died, 0 damage). The aggregate must fold every visible trigger the per-row tags already price, or say
   "before triggers". `wave60/deck152/review.md` HIGH, `wave60/deck126/review.md` HIGH-2.
2. Blockers ask: `INCOMING THIS COMBAT ... (your creatures may legally block every attacker in that total)`
   printed over ONE blocker vs 47 attackers, and the `best case with every blocker assigned` clause dropped;
   the blockers ask needs the blocker-count-respecting floor B11 gave the attackers ask. `engine-seat.md`
   HIGH-1 (162 s18), `deck126` MED-2 (3×), `deck162` LOW.
3. `CRACK-BACK NEXT TURN` "for up to N" exceeded 6/26 turns (regressed from 3/34): activated direct damage
   (Siege-Gang sac), creature-lands excluded from the census (Hive of the Eye Tyrant), pump triggers before
   the attack (Luminarch/Ranger Class) — all pre-computable from the printed board; and it never nets out
   evasion vs an all-ground board (51/51). `deck125` HIGH-1, `deck126` MED-1.
4. B2's `ON THE STACK` total misses `target(anytarget)` activated abilities (Staff of Nin ×4 renders, 0
   lines; primitive `{T}:damage:1 target(anytarget)`). `engine-seat.md` HIGH-2.
5. Loop narration: `LOOP HALF PENDING` fires, then when both halves are on the seat's own battlefield the
   render names only the converter and says nothing about the closed loop (5/5 closed-pair renders silent);
   and LOOP HALF PENDING urges closing a pair whose other half is unaffordable. `engine-seat.md` HIGH-3,
   `deck126` MED-3.
6. Two `DRAW PRICE` clauses on one row fold both off the same base life ("at 22" AND "at 16" for a cast that
   cost 8) — the model took that row. `deck126` HIGH-1 (162 s16).
7. Hand listing has NO affordability marker; cast rows omit unaffordable cards with no reason (fourth wave;
   5/6 of deck123's fallbacks, both of deck162's; a lethal plan built on an uncastable Ob Nixilis).
   `deck123` H1, `deck162` HIGH-1. And the land-drop row never says a land enters tapped (Arcane Sanctum
   `auto=tap(noevent)`; Isolated Chapel conditional). `deck123` H2.
8. Choose-ONE reveal (Pelakka Predation) renders the multi-select `PUT: 1, 3` protocol; the model picked 2,
   the engine took 1, narration asserted a false two-card take carried in every later prompt. `deck146`
   HIGH-1 (125 s18/19).
9. Dungeon-selection room counts are wrong as decision inputs (prompt prints 5/9/7; explore ladders are
   4/7/4; Lost Mine completes at explore 4) and Acererak asks still pick Lost Mine 14/14 despite B12's
   source clause (B12-outcome FAIL). `deck146` HIGH-2, `engine-seat.md`.
10. The sweeper CAST row carries no ranking marker while the X menu's `[<- best X]` rung is obeyed 6/6 —
    Starstorm offered 51×, cast 0×, incl. rows naming ≥2 of THEIRS (decided two deck130 losses). Give the
    cast row the same marker. `deck130` HIGH-2. Related: the best-X badge ignores `NET -2 life` on its own
    row (`deck125` HIGH-2, second wave); damage wipes price a count, never survivors — extend B7's roster to
    the damage clause (`deck125` HIGH-3).
11. `{spare: you control N lands already}` prints on every land row unconditioned by cost/turn (three lands
    "spare" at 2 lands with a 5-drop in hand — the seat discarded two). `deck130` HIGH-1
    (AIPlayerGPT.cpp:34018/34212).
12. Peer into the Abyss's row omits the converter count every other draw row prints (cast at 3 life for 20
    damage with 0 punishers out). `deck162` HIGH-2.
13. Transport: 2/2 `curl=28` records at the full 900 s deadline still classed `wall_miss_unrecorded`,
    `transport_error` 0/2 (curl 28 here is the OVERALL timeout, not connect); `deadline_pct` no longer
    emitted — the instrument is gone. `engine-seat.md` B4 FAIL.
14. Prompt economy: 22.7% of deck146's decisions pure declines (10 consecutive 20 KB asks on an unchanged
    all-dead menu — "Hold priority" survives only to the turn boundary); HOLD useless in a life-drain loop
    (11 identical pass windows); 81/119 deck162 replies overran the protocol (46 KB). `deck146` HIGH-3,
    `deck152` MED, `deck162` MED.
MED: `{dead right now:}` answers the CASTING question at a cleanup ask (pitched the lone sweeper; `deck125`
MED-1); token repeat row prints M but not L/C (`deck123` H3); loop caution's "in a row" clause drops after
one decline (`deck123` H4); opponent creatures render no rules text while their enchantments do (`deck152`
MED); one ability's card-text blob repeated 10× across target rows (6.4 KB; `deck146`); MDFC front face
"Cast Card Normally"; malformed dungeon worked example; token named "Spirit Cai" (`deck146` LOW); legend-rule
annotation process-phrased (`deck152` LOW); `@lifelostfoeof` loop-binding question from wave-60 lane N
(does the pilot's own punisher damage feed the OPPONENT's Sanguine Bond? corpus says yes, Oracle says no)
— verify with a fixture; concurrent suite runs sharing ~/.Wagic are unsafe (harness item).
UNTESTED this corpus (no window): B1 fold, B5-2/3, B6-1, B8b, B14a/b, R1, R3, R4 infect, R6, R9, F2 decline.
