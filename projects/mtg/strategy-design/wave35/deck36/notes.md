# deck36 — wave-35 engine/harness notes (seq repros; feeds the wave-36 step-1 docket)

All arm-C repros from `matchups-20260822-111102`, file
`1787415089-ai_baka_deck36-0x56407b426cb0-vs-ai_baka_deck116.jsonl` (F2) and
`1787415096-ai_baka_deck36-0x55bcbbf24880-vs-ai_baka_deck152.jsonl` (F4) unless noted.

1. **Docket #1 (carried, now with a MATERIAL specimen + a parser-direction finding).** The
   "Choose card 3 of exactly 6" sacrifice phrasing still induces multi-answer replies at the
   standing FP8 pilot (3 events; the 122B produced zero):
   - **F2 s29 T13**: reply `CHOICE: Pest #1, Pest #2, Pest #3, Pest #4` (names, no number, one
     line) → engine fell through to **Vault of Whispers**, an off-ladder pick the model never
     chose. `fallback` field: **None** — the fall-through is invisible to the fallback
     instrument (HL11-shaped gap: chosen_text matches nothing in the reply, yet no fallback flag).
   - **F2 s39 T15**: three `CHOICE:` lines (Pest #3 / Island / Seat of the Synod) → engine took
     the **LAST** line (Seat, ladder rung 3 by luck). The model's first-committed pick was Pest #3
     (ladder rung 1). Parser preference for the FIRST parseable CHOICE line would have honored
     intent in both s39 and s40 (s40: comma list, intended Pest parsed).
   Asks: (a) reword per docket-#1 ("sacrifice #3 — of 6 total, asked one at a time"); (b) make
   multi-CHOICE replies resolve to the FIRST parseable line; (c) any no-parse fall-through must
   set a `fallback` field. Guide already carries its one-per-ask paragraph; intent was
   ladder-correct in all 3 events, so this is not guide-patchable — core/harness lane.

2. **Generic blockers-header hint conflicts with computed outcome lines (deck36 face of the
   brief's deck152/105 item).** F4 s19 T8 prompt header: "block only where the trade favors you;
   taking damage while ahead is often correct" — directly above
   `B1. Yotian Soldier (1/4) [vigilance] - may block A1 (you kill it, your blocker lives)`.
   The pilot declined the free kill (the seat's one real arm-C combat failure; game-deciding —
   the surviving Aspirant grew the Wolf to 10/10). The hint plausibly licensed the decline.
   Suggest scoping the header ("...but a block whose line says you kill it and your blocker
   lives is always favorable") or dropping the "often correct" clause. Render lane.

3. **Nuisance Engine offered at Upkeep windows** (carried from wave-35 tier review, divergence
   table #13): both prior arms and arm C routinely take the guide-divergent upkeep activation the
   menu invites (e.g. arm B B3 seq16; arm A A2 seq17/21). Net effect usually identical;
   divergent-neutral. Either suppress the upkeep offer for sorcery-speed-intent activations or
   accept and let guides stop teaching MP1 — currently a standing paper cut.

4. **Academy turn-discipline unexercised at the standing pilot** (HL10.1 witness note): Tolarian
   Academy appeared in **0** arm-C prompts (fp8-review class-2 row). The revised guide's Academy
   block therefore ships UNVALIDATED on the FP8 pilot; arm C is disqualified as its witness.
   Wave-36 reviewer: grep the corpus for "with Tolarian Academy" first; zero again means the
   verdict stays UNEXERCISED (second consecutive; retire-at-three per HL12.2 applies at wave-38).

5. **T1 cast asks labeled "Main phase 2"** (carried; arm B B5 seq4/5, tier-review docket #3):
   phase label contradicts the deploy-in-MP1 teach on the Glaze Fiend-relevant turn. Not observed
   biting at arm C this corpus, but the label is still wrong.
