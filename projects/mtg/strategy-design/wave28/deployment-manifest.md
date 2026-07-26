# Wave-28 deployment manifest

Deploy target: `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (gitignored -> `git add -f`).
Source of truth for each proposed guide: `strategy-design/wave28/deckN/strategy.txt`.
The ORCHESTRATOR performs the deploy/commit/push -- this manifest documents WHAT to deploy and the
cmp/md5 evidence. Verified against the live tree this session.

## Actions

| Deck | Action | Deployed md5 | Proposed md5 | Bytes (dep -> prop) | Evidence |
|------|--------|--------------|--------------|---------------------|----------|
| 146 | **NEW initial deploy** | (none) | 547561f2a09bc673458084ee4bd4f543 | -- -> 7182 | no deployed file; Step-0 debut guide |
| 137 | **REVISION** | 8d5a54b2bf17e919f62609ee05fb5385 | 72893c81d34119472c4d2f5afd9fbf35 | 9084 -> 9074 | convoke softening + anti-re-pick |
| 136 | **REVISION** | 1cc0d4e0bb6f5bfe86e050a6ea904f11 | 296773bdbe70bec31050f86a4e285f2d | 7533 -> 9583 | MV3 fabrication fix + 4 more |
| 18 | BYTE-FROZEN | 8efec49899015c72aba59193cab42a58 | 8efec49899015c72aba59193cab42a58 | 5124 (identical) | cmp+md5 identical; no redeploy |
| 22 | BYTE-FROZEN | 5315a04536b766bf8243df1d55c19ee3 | 5315a04536b766bf8243df1d55c19ee3 | identical | cmp+md5 identical; no redeploy |
| 93 | BYTE-FROZEN | 1a27e8c4cc93c2884c52ee3dbc60265f | 1a27e8c4cc93c2884c52ee3dbc60265f | 5124 (identical) | cmp+md5 identical; no redeploy |
| 134 | BYTE-FROZEN | 8b5fc256e7ec75df16a2194f75725e3e | 8b5fc256e7ec75df16a2194f75725e3e | identical | cmp+md5 identical; guide correct, seat terminal |

Cross-wave check: deck18/22/93/134 proposed == wave-27 proposed == deployed (all md5-identical). deck136/137
proposed != wave-27 proposed (genuine revisions).

## deck146 (NEW -- Orzhov Dungeons WB, Step-0 initial guide)

First guide for this seat. Content anchored to engine card facts (verified per the wave-28 verify-oracle
hard rail): Vanishing Verse hits any MONO-colored permanent (corrected vs a wrong prior); Lost Mine of
Phandelver completion payoff (Nadaar anthem / Barrowin reanimate); Triumphant Adventurer as a deathtouch
blocker; activate a planeswalker EVERY turn (do not pass with an idle walker); named-option-against-option-1
for the dungeon pick ("pick Lost Mine, usually option 2"). Deploy as a new file, `git add -f`.

## deck137 (REVISION -- verified diff)

Two changes, both routed to guide-legitimate behavior (no engine bug named in prose):
1. **Convoke steering SOFTENED** (maintenance corollary, skill headline 2): the deployed guide's blanket
   "PREFER THE PLAIN CAST" is relaxed to "either cast works reliably; prefer plain only to keep creatures
   untapped" -- because Loxodon fixed-cost convoke now resolves live (e1 half closed). This also keeps deck137
   exercising the convoke path so a regression surfaces at the sole convoke seat.
2. **Anti-re-pick clause STRENGTHENED** and pulled into its own IMPORTANT block: "one attempt per card per
   turn, then act" -- the variant-agnostic recovery for the still-open March X-convoke defect (routes to a
   working alternative without naming the bug).
Byte delta -9 (9084 -> 9074). Diff confirmed this session.

## deck136 (REVISION -- verified diff)

The wave's largest guide change; five edits (byte delta +2050, 7533 -> 9583):
1. **MV3 FABRICATION FIX (the headline).** Removes the fabricated "Relentless Rats MV4" and its two inverted
   removal teaches. Now: Ritual of Soot (MV<=3) IS the #1 answer to the Rats (their +1/+1 pump does not raise
   mana value); Legion's End (MV<=2) CANNOT hit them -- "do not plan to Legion's End the Rats." Grounded in
   engine `mana={1}{B}{B}` (MV3) and borderline.txt Ritual/Legion's End scripts.
2. Long-game supremacy SCOPED: "you win the long game" -> scoped to "decks with no engine of their own" +
   a WARNING for value/attrition mirrors (Orzhov Dungeons) not to coast into a grind it can lose on cards.
3. Proactivity line: do not answer "Cast nothing" for several turns running with a castable play.
4. Ugin nudge: cast it on a 6-mana board stall (it was under-used / never cast).
5. Go-wide matchup split rewritten (Relentless Rats vs Kithkin/tokens) around the corrected MV facts.
Diff confirmed this session.

## Post-deploy verification (orchestrator)

- [ ] deck146 new file present; `git add -f` staged; cmp vs wave28/deck146/strategy.txt == identical.
- [ ] deck137 + deck136 redeployed; cmp vs wave28/deckN/strategy.txt == identical; md5 matches proposed above.
- [ ] deck18/22/93/134 UNTOUCHED (md5 still matches the frozen values above; no redeploy, no restage).
- [ ] `git diff | /usr/bin/grep -c U+FFFD == 0` on any edited Res guide.
- [ ] No AI attribution in the commit (verify-oracle: all deck-fact numbers grounded before deploy).
