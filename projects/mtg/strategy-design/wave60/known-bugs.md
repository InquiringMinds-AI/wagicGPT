# Wave-60 step-one input: KNOWN BUGS from the wave-59 evaluation (orchestrator list; details in the cited files)

Wave-59 corpus: 21/21 natural, 0 crash/hang, 3,005 decisions, fallbacks 12 (was 108), `empty_reply` 0.

HIGH (engine / interface):
1. `THIS WINS THE GAME` badged while the seat is already dead on the stack / to the cast-time life cost
   (Hammer of Bogardan row printed `this KILLS you` AND `WINS THE GAME`; target menu printed only the win).
   The win fold must subtract the cast's own life cost and any pending stack damage. `wave59/engine-seat.md`
   HIGH-1, `wave59/deck130/review.md` HIGH-1 (130v146 s43/44). Site unlocated by the seat.
2. Pending damage ON THE STACK is never priced (13/77 stack renders carry damage at the seat, 3 at life 1);
   TARGET CHOICE windows render the stack only 11/89; Ob Nixilis rows render as bare "triggered ability".
   `wave59/deck146/review.md` HIGH-1 (146v162 s26/s35/s37).
3. Cleanup `PUT:` with a repeated index rejects the whole reply, no re-ask, heuristic discards; the ask never
   says indices must be distinct. `engine-seat.md` HIGH-2, `deck125` HIGH-3 (125v126 s246, s354).
   Neighbour of wave-59 K9 (which PASSED).
4. Lane H moved the connect cost to the deadline: `curl=28` burns the whole 900 s, and one retry finished at
   deadline_pct 108.6 (977,594 ms). `transport_error` fired 0 times. The retry budget must fit inside the
   deadline and the connect timeout be a bounded fraction of it. `engine-seat.md` HIGH-3, `deck130` HIGH-2,
   `deck123` (vs146 s7).
5. No DISCARD-punisher lens: X-row `NET 0 life` with two Liliana's Caress on board, forced 11-card cleanup
   discard cost 44 life (13 -> -31); `converterScanZone` (AIPlayerGPT.cpp:8566) merges draw and discard
   punishers into one `{feeds:}` count (PARSETEST 43631 bakes the wrong pairing in). `deck125` HIGH-1
   (125v162 s111), `deck162` HIGH-2 (18 rows).
6. Draw-punisher forecast ignores the OPPONENT's Sanguine Bond + Exquisite Blood loop (own punishers chain
   onto own life; seat cast Fate Unraveler 16 -> 0); converter warning fires one decision AFTER the loop
   closes (25 silent frames with one half in play + other half seen). `deck162` HIGH-1 (126-game s20),
   `deck152` #4 (s9-33).
7. Sweeper rows price a count, never the names (180 rows `destroys 1 of their creature`, 0 named; Path rows
   do) — 5 sweepers spent 1-for-1. `deck125` HIGH-2.
8. Teferi +1 renders with its verbs truncated (60 corpus-wide) and its sub-menus never say which branch
   taps; seat tapped permanents that untap before the crack-back and lost. `deck152` #1 (146 s38-42).
9. A creature exiled "until this leaves" is invisible at the block that kills the exiler (Brutal Cathar
   chump returned a 4/4 that then counted in a 14-damage lethal). `deck152` #2 (146 s35/s43).
10. Tribute to Hunger's forced sacrifice renders as a generic TARGET CHOICE headed as the lifegain
    sub-ability, rows unpriced (no "you sacrifice this / they gain N"). `deck152` #3, `deck130` MED.
11. The ATTACKERS ask has no aggregate lethality line (total power, unblocked damage, resulting life,
    converter backlash) — the blockers ask has exactly that. `deck123` I1 (vs126 s71).
12. Venture ask never names the source (11/11; second wave) — Acererak's Oracle conditions on Tomb of
    Annihilation by name, seat picked Lost Mine every time. `deck146` HIGH-2.
13. Prompt economy: carried `YOUR PLAN` echo unbounded and un-vetted (148/410 > 400 chars; a 1,236-char
    stale stream produced `CHOICE: 4` on a 3-row menu); GAME LOG = 76% of prompt bytes with no compaction of
    identical events (50 identical "drew a card"); K10's declines arm FAILED (23/18/18/15 per turn) though
    161 re-serves fired; the decline counter counts re-serves the model never saw. `deck126` HIGH-1/2,
    `engine-seat.md` K10 + MED.
14. F2 residuals (wave59/known-bugs.md 12-13): option two never arms after a decline; the double-consumer
    at revealasyncticks 2. F2's decline path had NO window this corpus (UNTESTED).
MED: INCOMING "may legally block every attacker" printed with 1 blocker vs 5 (`deck126` MED-1); Idyllic
Tutor renders all 43 library rows with rules text (`deck126` MED-2); `wall_miss` records carry a wrong
phase and `options: 0` (`engine-seat.md`, `deck130` HIGH-2); `askExemplar` 48-byte truncation mid-quote
(`deck162` MED); CRACK-BACK "for up to N" exceeded 3/34 (Ranger Class trigger, daybound; `deck125` MED-2);
`Cast nothing right now` dominated by HOLD 282/282 (`deck125` MED-1); unaffordable hand cards omitted with
no reason (`deck123` I2, third wave); kill-preview enumerates 29 identical tokens (`deck123` I3); 21-row
valor menu at mana 0 (`deck152` MED); counter log line contradicts the frame's printed P/T (`deck152` MED);
D42 grouping "not visually intuitive" — OWNER-HELD.
METHOD NOTE (engine seat): the translog `phase` field is not the rendered phase — count phase-gated
predictions from the prompt text.
