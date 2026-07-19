# deck27 (Zombies / "Undead Infiltrator") — wave-20 findings

**Seat:** FIRST GUIDED corpus (Step 0-ter validation). wave-19 wrote the initial guide; this is
its first at-scale test. Deployed guide `bin/Res/ai/baka/deck27_strategy.txt` is byte-identical
to `wave19/deck27/strategy.txt`.

**Binary:** /tmp/wagic-95e89c204. **Corpus:** matchups-20260719-151849, 6 games round-robin.

## Record — 3/6 (up from 2/6 guideless)

| vs | result | life (me/opp) | turn | shape |
|----|--------|---------------|------|-------|
| 135 | **WIN** | 3 / -7 | 11 | evasive beatdown (Vodalian islandwalk every turn) |
| 35 | **WIN** | 11 / 0 | 9 | Inkfathom unblockable every turn from t5 |
| 49 | **WIN** | 10 / -17 | 11 | Zombie Outlander (pro) + Lord of Atlantis beatdown |
| 102 | LOSS (adj) | 3 / 10 | 10 | raced by Tourach 7/6 (pro B/G/W) — deck can't interact |
| 62 | LOSS | -6 / 11 | 10 | ground-swarm + trample, behind-spiral fallbacks |
| 14 | LOSS (adj) | 3 / 6 | **23 (turn cap)** | **durdle-to-cap**: evasion drought + under-attack |

Beat 135 (frozen, 2/6->4/6 this wave = a genuinely strong opponent), 35 (flat 3/6), and 49
(**dropped 5/6->4/6** this wave). Two of three losses were adjudications; **vs14 ran to the turn
cap (t23)** — the deck's signature durdle failure, live.

## Step 0-ter: taught-class ledger (died / persisted / mutated) — read BEFORE the record

Tax gauge FIRST (Step 0-ter rule 1, the primary "did the guide work" signal):
**138 decisions, 3 fallbacks (2.2%), 6 giants >8k.** Wave-19 guideless: 141 / 3 (2.1%) / 6.
**The reasoning tax did NOT collapse — it is flat.** The guide moved *which* classes fire and the
win-path execution, not the aggregate tax. Per-class:

1. **Mana-flexibility / trust-the-cast-list (rule #1) — DIED (clean).** Zero mana fallbacks, zero
   "wrong color" spirals across a hybrid {U/B} base. Confirms wave-19's read that false-belief
   risk scales with color *commitment*, not two-color complexity. FIRED, no misfire.
2. **"[X/Y] already includes lord buffs" — DIED (clean).** No P/T re-derivation fallback.
3. **Evasion is the win condition / lead with unblockable — FIRED STRONGLY; correlated with all 3
   wins.** vs35 Inkfathom attacked s7/s10/s12 every turn; vs102 Inkfathom s10/s17/s20/s23; vs135
   Vodalian islandwalk (pro green) s8/s12/s17/s21; vs49 Zombie Outlander (pro) + Lord of Atlantis.
   **This is the guide's headline win-path and it executed.** Primary decision-level lift evidence.
4. **Regenerate/recur your BEST, not your worst — DIED (the wave-19 misplay did NOT recur).**
   Recursion was offered only in vs14 (the one long-grind game). s10: returned **Zombie Outlander**
   (correct — a real body, not a 1/1). s26/s29/s33/s37: offered "return Walking Dead (1/1) OR
   Zombie Outlander" — the model **PASSED every time; it never once returned the 1/1 Walking Dead.**
   The wave-19 vs49 "babysit/return a 1/1 Walking Dead" class is dead. Clean guide win.
5. **Lords on curve, not last — FIRED (improved).** vs14 deployed Lord of the Undead t5/t7, Glen
   t11; no t13-lord-dump like wave-19 vs62. Curve held in all games.
6. **Take the free block — PERSISTED and DISOBEYED (the wave-19 headline, recurring).**
   **vs49 s5** (t4): offered `B1. Zombie Outlander (2/2) - may block A1 (you kill it, your blocker
   lives)` against **Bloodmark Mentor (1/1 first strike)** — their team-wide first-strike lord —
   and chose **no blockers.** This is the EXACT wave-19 deck49 s9 misplay, same opponent, same
   card, recurring under an explicit guide rule that names this precise case ("especially against
   an opponent LORD or anthem-giver (e.g. first strike)"). Mixed: it TOOK the equivalent block at
   **vs49 s9** (Lord of Atlantis kills Bloodmark Mentor) and correctly DECLINED a chump-with-lord
   at **vs102 s21** (Glen chumping Tourach 5/4). Verdict: the rule is present and maximally
   explicit; obedience is inconsistent. **A model-obedience gap, not a prose gap — more words will
   not help.** (2nd instance, but same opponent -> does not promote; WATCH.)
7. **When ahead, keep it simple — PERSISTED and DISOBEYED (mutated to decode degeneration).**
   **vs49 s16** (attackers, AHEAD 10/11, lethal-soon board): the model opened correctly ("attack
   with all... 10 damage... opponent to 1"), then over-deliberated the residual mana, then
   **degenerated into "No, it's a Spirit?" repeated to truncation** -> the corpus's one `attackers`
   fallback. 2nd instance of the wave-19 ahead-giant class (deck110 s20). The guide's ahead line
   did not prevent it — but this tail is a **decode failure, not guide-addressable.** Game won anyway.
8. **When behind, best play and pass; only the listed attackers exist — SPLIT.** The
   combat-hallucination sub-rule (wave-19 deck14 s9 imagined-attacker) **DIED — no invented-board
   fallback this corpus.** The behind-spiral tax **PERSISTED**: **vs62 s18** (priority, BEHIND
   5/16, offered only "Regenerate Sanguine Guard") the model ignored the lone option and rambled a
   whole-turn attack plan to truncation -> fallback. Standard losing-spiral, in the lost game.
9. **Durdle / attack every turn — PERSISTED in the long-stall off-case (vs14).** The win-path
   attacking fired; but vs14 ran to the **turn cap (t23)** chipping ~2/turn with a lone Glen
   Elendra Liege while the ground was walled by Deep Blue. Two contributing causes: (a)
   **evasion drought** — no Inkfathom / Deepchannel / active islandwalk drawn, only one small
   flyer (construction/draw, not guide); (b) **pilot under-attack: vs14 s30** (t21, opp NOT racing
   — deck27 stable at 8) the model held **2 of 4 eligible, non-summoning-sick attackers** (Lord of
   Atlantis 3/3, Lord of the Undead 4/4), swinging only Glen + Zombie Master, and passed on
   recursion (s26/s29) while the clock ran out. **This is the guide's core anti-durdle thesis
   failing in the LATE grind** — the existing prose is scoped to EARLY piecemeal ("deploying lords
   on turn 13"), not to holding buffed attackers in a stall. Decision-evidenced -> the ONE surgical
   revision (below).

## The 3 fallbacks characterized

1. **vs49 s16 (attackers, AHEAD 10/11, 11.7k).** Ahead-position decode degeneration ("No, it's a
   Spirit?" xN to truncation). No well-formed `ATTACK:` line ever emitted -> **the extended
   salvage cannot catch this** (nothing to salvage). Heuristic answered; game won.
2. **vs62 s6 (blockers, even 20/20, 13.1k).** Caused by a **representation defect**: the option
   read `Oona's Gatewarden (2/1) wither ... A1 (both die, 2 tramples to your face)` against Birds
   3/4. The model correctly SMELLED the contradiction ("if the Birds dies it can't trample... 'both
   die' must be a mistake"), spiraled on it, and truncated with the right answer ("BLOCKS: B1:A1")
   sitting unquoted in its reasoning. See the wither-annotation engine item (notes) — **"both die"
   is wrong; a 2-wither blocker only SHRINKS a >2-toughness attacker.**
3. **vs62 s18 (priority, BEHIND 5/16, 13.5k).** Behind losing-spiral at a regenerate seam — the
   lone option was "Regenerate Sanguine Guard"; the model rebuilt a whole attack plan and truncated.

## Salvage / #N-ordinal / options_text validation (combat batch)

- **options_text present in every combat record** at this seat. Confirmed live.
- **#N / duplicate-name disambiguation fired correctly:** vs49 s19 offered A2=Lord of Atlantis and
  A4=Lord of Atlantis, A1/A5=Zombie Outlander — the model selected all five and the chosen_text
  resolved each. No duplicate-name fallback.
- **Extended loop-salvage: NOT positively exercised here.** Both combat fallbacks (s16 degeneration,
  s6 mid-confusion truncation) lack a well-formed ordinal for salvage to recover; the wave-19
  out-of-range-index case (deck14 s9) did not recur. Neither validates nor refutes the extension.

## Lift vs variance (decision-level)

**Real guide-aligned execution on the win axis; the aggregate +1 is within coin-flip variance.**
The three wins are each explained by taught classes FIRING — evasion-first (class 3) drove all
three, recursion allocation (class 4) held in the one grind. That is genuine decision-level lift on
the evasion/recursion axis. But per Step 0-ter rule 1 the record itself is a coin-flip surface: the
guide beat a 49 that fell 5/6->4/6 (weaker/variance) and a flat-3/6 35, while the tax did not
collapse. Honest verdict: **the guide made the win-path reliable where it engaged; it did NOT fix
the block-obedience or off-case tax, and it did NOT solve the durdle in the long grind.**
