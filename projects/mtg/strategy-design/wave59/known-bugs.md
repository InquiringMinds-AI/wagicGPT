# Wave-59 step-one input: KNOWN BUGS from the wave-58 evaluation (orchestrator list; details in the cited files)

HIGH (engine / interface):
1. F3 — 2.5 s curl CONNECT timeout -> `empty_reply` -> heuristic, 89/2270 decisions, runs up to 10; the
   precondition of the F2 hang. `wave58/engine-findings.md` §F3 (site GptConfig.cpp:822). Fix: generous
   connect timeout, log curl code + HTTP status, retry inside the deadline.
2. F2 — engine hang after the fallback answered -1 on a may-menu (Sigarda). `engine-findings.md` §F2 +
   addenda; lane G (Codex, wave 58) — merge its fix if landed, else this is the lane.
3. F1 — SIGABRT: menu id indexes past ActionLayer::mObjects (DecisionContract.cpp:367). §F1; lane F
   (Codex, wave 58) — same.
4. Discard channel still badges `{dead right now:}` on live multi-scope verdicts (Devour Flesh, Tragic
   Slip): 11 of 139 — the rebadge site must call `verdictReadsZero`. `wave58/engine-seat.md` HIGH-1.
5. Lane B's D7 floor line prints a damage floor as a survivable LIFE floor ("at least 15 ... you would be
   at 3 or better" where a legal block reached 0) and the B-row cites an A-line number that is not
   there. `wave58/deck162/review.md` HIGH-1/2 (AIPlayerGPT.cpp ~15122).
6. Intrepid Adversary's repeated `{1}{W}` payment stops early with payable sources untapped, then
   "Mana available: 0". `wave58/deck152/review.md` HIGH-1 (rerun seq 71-72, 59-60). Card/engine.
7. MDFC land menu: back face fully annotated, front face renders bare `Play Land` (32 renders, 13 blind
   takes). `wave58/deck146/review.md` HIGH-2.
8. Own-turn crack-back number absent at every life total (D9 shipped for the opponent's turn only);
   count creatures that WILL untap. `wave58/deck123/review.md` HIGH; general R319.
9. Cleanup `PUT:` parser refuses the parenthetical name gloss every `CHOICE:` ask requires
   (`PUT: 9, 1 (Supreme Verdict, Fall of the Gavel)` -> unparsed). `wave58/deck125/review.md` HIGH-2.
10. 32 byte-identical asks in one turn while a decided drain loop resolves (`identical_option_asks_resolved`
    = 0). `wave58/deck126/review.md` HIGH-2.
11. D42 grouping: owner's "not visually intuitive" — OWNER-HELD, no lane. `wave58/owner-decisions.md`.
MED (candidates, take only with a HIGH lane's spare capacity): venture ask never names the source
(deck146 MED-1); `wall_miss` records stamp `options: 0` (deck146 MED-2); `moveto(hand)` rows unpriced
(deck130 E3); Teferi +1 = 7 round trips, text truncated before its verbs (deck152 HIGH-3/MED); search rows
unpriced (deck126 MED); Lightmine rows print damage but no kill count (deck125 MED); `[<- largest
affordable X]` ignores NET life on its row (deck125 MED); menace gang-block result unprinted (deck126 MED).

Added after lane G (F2) landed, from its report (`wave58/lane-G.md`):
12. After a DECLINE on an interactive reveal's option one, option two never arms: option one's nested
    target ability stays `currentWaitingAction` (removeFromGame returns early once it is out of
    mObjects) and option two is allocated at the freed address. Lane G fixed the EXIT (the stall guard
    now force-closes and writes `reveal_stall_forced`), not the cause; cards return to the library
    instead of its bottom. Needs the identity of the nested waiting element.
13. At `revealasyncticks 2` the seat's generic action pass consumes option one's chooser and takes
    "Get a human" AFTER a decline (library 6, hand 1, deterministic 3/3) — the W50-W (D2)
    double-consumer, wrong outcome not a hang.
