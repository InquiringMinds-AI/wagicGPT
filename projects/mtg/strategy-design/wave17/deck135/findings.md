# Deck-135 (Modern Snow) — Wave-17 game review

Corpus: PRIMARY `matchups-20260716-151559` (21-game round-robin over 110/135/17/21/62/49/35;
binary `/tmp/wagic-7cdcf9c73` = wave-16 engine batch: PLAN-carry stale-intent caveat on 319
decisions; legalCasts normalEntry fix; own-target-only BAD-effect suppression). deck44 & deck133
rotated OUT; deck49 (Dragons) + deck35 (Slivers) entered. All six deck135 seat games read
decision-by-decision; Force resolution recovered from the seat log itself this corpus (Force
fired at THIS seat, no opponent-log cross-check needed). Doctrine: win column is CONTEXT ONLY;
unit of analysis is the DECISION with seq repros, layer-attributed; adopt-a-revision bar is HIGH
(veteran seat, guide FROZEN since wave-11). deck135 owns this cycle: (a) Force-played-WELL check;
(b) stale `[attacking]` tag (3rd-wave watch); (c) the multi-target sub-menu unparsed cluster
(s29/s31/s38 vs62); (d) the 1/6 record construction-vs-play split.

## RECORD (context only — NOT a verdict) — 1W / 5L

| game    | opp                    | outcome (deck135)          | my/opp life | turn | class |
|---------|------------------------|----------------------------|-------------|------|-------|
| 135v49  | 49 Dragons (new)       | **WIN** (real, 11/-6)      | 11 / -6     | 14   | Rule#1/#2 working — curved out, raced a slow deck |
| 135v62  | 62 Enchantresses       | LOSS (adj, behind)         | 14 / 20     | 12   | winnable slow matchup LOST — Azcanta cluster fingerprint |
| 135v110 | 110 Etched Affinity    | LOSS (adj, raced to 2)     | 2 / 6       | 14   | well-played close race; construction clock too slow |
| 135v21  | 21 Goblin Gang         | LOSS (real, dead)          | -5 / 20     | 15   | green-screw + fast burn; 2 face-burn Forces |
| 135v17  | 17 Faeries             | LOSS (real, dead)          | -3 / 20     | 9    | evasion (flyers); Treefolk too late |
| 135v35  | 35 Mighty Slivers (new)| LOSS (real, dead)          | -6 / 20     | 9    | fast aggro; flooded 2x DEAD Force vs all-creature deck |

Win table (brief) assigns deck135 1/6; matches. **Two adjudicated losses this corpus** (vs62,
vs110 — deck135 alive at 14 and 2), THREE real deaths (vs21/-5, vs17/-3, vs35/-6). The ONE win
(vs49) is a genuine kill to -6. Record path: 1/6(w13) -> 0/6(w14) -> 2/6(w15) -> 1/6(w16) ->
**1/6(w17)** — variance around an unchanged construction floor, BUT the WIN moved (vs62 last wave
-> vs49 this wave) and the wave-16 win (vs62 Enchantresses, 19/5) FLIPPED to a loss this corpus,
which charges (c)+(d) tie to the Azcanta representation defect, not just variance.

---

## (a) FORCE-PLAYED-WELL CHECK (owned). Verdict: ENGINE surface CLOSED + wave-16 #1b NOW VALIDATED; PLAY quality NOT clean — 2/2 Forces were face-burn casts (guide's hardest "NEVER" line), 3rd corpus of the reflexive-cast class.

Only TWO Force offers this corpus, BOTH vs deck21, BOTH the FREE exile-pitch mode, BOTH chosen,
BOTH FIRED cleanly at THIS seat (verified in-seat, not opponent-recovered):

| seq | window | mana line | exiled | target | resolution |
|-----|--------|-----------|--------|--------|------------|
| s6  | T4 opp turn, 19 life, 0 creatures | **(no untapped sources)** | Search for Azcanta | Volcanic Hammer **[spell] targeting you** | s7 events: `Force: hand->stack` \| `Volcanic Hammer: stack->exile` \| `Force: stack->graveyard` — COUNTERED |
| s9  | T10 opp turn, 10 life, 0 creatures | `{g}` (1 untapped) | Ice-Fang Coatl | Volcanic Hammer **[spell] targeting you** | s10 events: same clean cast->exile cycle — COUNTERED |

### ENGINE: CLOSED, and the wave-16-carried #1b path is now VALIDATED (upgrade).
- **s6 VALIDATES ENGINE #1b (was UNVALIDATED wave-16).** The prompt shows `Mana available: (no
  untapped sources)` and the free exile-pitch was offered, chosen, and PUT FORCE ON THE STACK and
  countered (no silent fizzle). This is exactly the trigger shape wave-16 lacked (a free alt-cast
  chosen with "(no untapped sources)" that counters). The 69793930a tapped-out free-cast fix is
  now demonstrably working end-to-end. Close the #1b validation loop.
- **No wave-16 #1c recurrence (hardcast-unpayable offer).** Only the free mode was offered both
  times (deck135 held a legal blue card to pitch each window), so the hardcast-while-unpayable
  path had no trigger. Not exercised; carry #1c's watch, do not mark fixed on absence.
- **#1a regression watch CLEAN:** both targets went stack->EXILE (countered), never
  stack->graveyard/battlefield.

### PLAY: both Force casts were WRONG by the guide's own verbatim line.
Volcanic Hammer was `[spell] targeting you` in BOTH prompts — FACE burn, and deck135 had ZERO
creatures on board both times, so there was no creature target. The guide states verbatim:
*"NEVER Force a burn spell pointed at your own face — take the damage and keep the blue card to
cast your creatures."* Both s6 and s9 violate it.
- **s6 is the sharp error:** 19 life, exiled **Search for Azcanta** — the dig engine that finds
  the blue/green source deck135 was screwed on — to save 3 face damage while nowhere near dead.
  Textbook 2-for-1 against face burn with negative value (it burned its own fixing to stay at 19).
- **s9 is off-guide but defensible-in-context:** 10 life vs a burn/goblin deck that had already
  thrown Lightning Bolt x2 + Volcanic Hammer, and the exiled Ice-Fang Coatl was UNCASTABLE anyway
  (color-screwed, no blue), so the "keep the blue card to cast creatures" rationale was moot. Still
  a face-burn Force; deck135 died -5 the same game regardless.
- **Class:** the reflexive-cast-on-face-burn class PERSISTS a 3rd corpus (wave-15 Cranial Plating
  over-Force; wave-16 s19 Volcanic Hammer face; wave-17 s6/s9 Volcanic Hammer face). The guide
  ALREADY bold-says NEVER and lists the sanctioned targets — this is MODEL over-application, not a
  guide gap. Route to model watch + general-suggestions #1. NOT guide-fixable (a 4th restatement is
  not warranted; the line is already maximal).

## (b) STALE `[attacking]` BOARD-TAG (owned): REPRODUCED, benign, 3rd+-wave watch.

Own TAPPED creatures still render `[tapped - untaps and can attack next turn] [attacking]` in
**Main phase 2 (post-combat, YOUR turn)** — no combat active:
- **deck110 s31** (MP2, your turn): Boreal Druid, Icehide Golem, **both** Ice-Fang Coatls tagged
  `[attacking]` (4 creatures).
- **deck49 s13 / s14** (MP2, your turn): Icehide Golem tagged `[attacking]`.
Opponent-battlefield `[attacking]` tags seen (deck21 s8, deck17 s18, deck110 s9/s13, deck35
s6/s11) are all Blockers-step / opponent's-turn windows = LEGIT in-combat, not stale. The stale
own-creature instances drove NO wrong decision (every tagged creature was tapped and inert; deck110
s31 was an unparsed ramble anyway). Cosmetic representation-noise, 0 cost — but the identical
wave-13/15/16 defect stands on the ledger. FIX (unchanged): clear `[attacking]` on any creature
not in the current declared-attackers set. -> notes ENGINE #2.

## (c) MULTI-TARGET SUB-MENU UNPARSED CLUSTER (owned): Azcanta, the Sunken Ruin's dig ability, rendered as an opaque "Choose target N of exactly 4" ORDERING menu. ESCALATED from wave-16's "benign" read — MATERIAL cost this corpus.

**What the engine was actually showing.** deck135 transformed Search for Azcanta -> Azcanta, the
Sunken Ruin and activated `{2}{U}, {T}: Look at the top four cards... You may reveal a NONCREATURE,
NONLAND card and put it into your hand. Put the rest on the bottom in any order.` The engine
exposes this as a sequence of prompts: `Choose target 1 of exactly 4 for Azcanta` ... `target 2 of
exactly 4` ... `target 3 of exactly 4`, the option list SHRINKING by one each step as cards are
assigned. This is an ORDERING of all 4 revealed cards onto the bottom of the library, with "put one
into hand" folded into the same undifferentiated selection.

**Two full activations, BOTH whiffed to zero cards-in-hand:**
- Activation 1 (s28 target1=Abominable Treefolk [a CREATURE], s29 target2 unparsed, then s30
  events: **all 4 revealed cards -> library**, NOTHING to hand).
- Activation 2 (s36 target1=Ohran Viper [CREATURE], s37 target2=Diamond Faerie [CREATURE], s38
  target3 unparsed, then s39 events: **all 4 revealed cards -> library**, NOTHING to hand).

**Root cause (representation, likely + engine).** The prompt gives the model NO way to know:
(1) the EFFECT ("put ONE noncreature-nonland into hand, rest to bottom"); (2) that "target N" is a
bottom-ordering slot, not a keep-this-card pick; (3) which revealed cards are ELIGIBLE for hand
(Azcanta may only grab NONCREATURE-NONLAND — so Treefolk/Viper/Faerie/lands are INELIGIBLE, but
they are listed as choosable "targets" with no eligibility mark); (4) what was already assigned in
prior steps (no running state). The model, wanting its finisher, picked a CREATURE as target 1 in
BOTH activations, the put-to-hand silently voided (ineligible type), and every card went to the
bottom. The model's own replies show total menu confusion ("This is impossible"; "the prompt has a
formatting error") and a degenerate repeat-loop at s31 — it believed its uncastable Treefolk was
still findable and spent the whole turn digging for a creature that Azcanta could never hand it.

**Cost — MATERIAL, not benign.** vs62 produced 6 of the seat's 7 fallbacks (s21 unparsed, s22/s23
stale_echo, s29/s31/s38 unparsed) — all clustered on / downstream of the Azcanta resolution. Across
TWO activations deck135 paid `{2}{U}`+tap twice and drew ZERO cards to hand, in a game where it was
NOT screwed (8+ snow permanents, full mana) against a SLOW Enchantress deck it BEAT 19/5 last wave.
It never deployed a finisher and drifted to an adjudicated loss at 14/20. Had the eligible cards
(Force/Gelid/Search in act 1; Arcum's Astrolabe in act 2) been MARKED, the model would have banked
a real card each time. This is the deck135-owned engine/representation item -> notes ENGINE #4 (NEW,
HIGH VALUE). NOTE the s22/s23 stale_echoes (settled correct catches) are a SEPARATE Into-the-North
target sub-menu misread in the same game — the model echoed the unoffered "Snow-Covered Forest"
when the sub-menu offered Scrying Sheets/Island (s22) / Prismatic Vista (s23); correct downgrades,
not relitigated.

## (d) 1/6 RECORD — CONSTRUCTION vs PLAY SPLIT (owned). Verdict: construction-dominant + variance, BUT one loss (vs62) carries an ENGINE/REPRESENTATION fingerprint, not pure construction.

**Creature deployment by game:**
- vs49 (WIN): Icehide Golem T1, 2x Icehide Golem, 2x Ice-Fang Coatl, Ohran Viper, Diamond Faerie —
  a full board, raced the slow Dragons deck (burn + Crucible of Fire, no dragons landed). Rule #1
  ("deploy a creature every main phase") and Rule #2 ("attack every turn that profits") executed
  cleanly. This is the ceiling working.
- vs62 (LOSS, adj): had Ice-Fang Coatl early (traded), then the Azcanta cluster (c) denied it card
  advantage twice and it never landed Treefolk — a WINNABLE slow matchup lost with an engine
  fingerprint, NOT construction.
- vs110 (LOSS, adj): Boreal Druid, Ohran Viper (traded for the Cranial-Plating'd 5/1 Memnite — a
  good block), 2x Icehide Golem, 2x Ice-Fang Coatl. Raced back to put the opponent to 6, died to
  adjudication at 2 life. Well-played; the clock (small bodies, Treefolk a turn late as an 11/11)
  was just too slow vs Affinity. Construction + adjudication latency.
- vs21 (LOSS, real): green-screwed off a `{u}{u}{c}` board, 0 effective creatures, dead T15. The
  2 face-burn Forces (a) are the only real misplays but the game was lost to screw. Construction.
- vs17 (LOSS, real): Golem + Coatl deployed, overrun by evasive Faeries; drew Treefolk too late and
  then MISCOUNTED its mana (thought `{g}{u}{u}` = 3 could pay the 4-cost {2}{g}{u}) and PASSED three
  decisions "waiting" on an unaffordable Treefolk (s19/s20/s21) — the exact losing thought the guide
  names. Model arithmetic/adherence, not a new guide class; game lost to evasion regardless.
- vs35 (LOSS, real): flooded on **2x Force of Negation** (DEAD cards vs an all-creature Sliver
  aggro deck — Force counters only NONCREATURE) + fetches; drew no threat after T4, run over T9.
  Construction (noncreature top-end useless vs aggro).

**PLAY errors this corpus:** the 2 face-burn Forces (a, vs21), the Treefolk mana-miscount pass
(vs17), and model-side Azcanta mis-selection (c). NO Rule #1 hard creature-skip this corpus (repeat
of wave-16's clean deployment axis). **Split verdict:** construction-dominant + variance, unchanged
ceiling (slow clock, no evasion answer, gold/double-pip screw, dead Force vs creature decks) —
EXCEPT vs62, where the loss traces to the Azcanta representation defect rather than the decklist. The
one win (vs49) is Rule #1/#2 producing a clean kill when the deck is not screwed.

## GUIDE DISPOSITION: FROZEN (byte-identical copy of the live guide; `cmp`-verified).

No new GUIDE-fixable decision class. Every residual routes off the guide: Azcanta multi-target
ordering menu + zero-to-hand whiff -> engine/representation (ENGINE #4, deck135-owned, ESCALATED);
stale `[attacking]` -> engine (ENGINE #2); face-burn Force reflexive-cast -> MODEL over-application
of an already-maximal verbatim "NEVER" line -> model watch + general-suggestions #1; Treefolk
mana-miscount + Azcanta-grabs-a-creature + (carried) card-color/fetch-color -> model card-knowledge
watch -> general-suggestions #3; losses -> construction (restate; user owns decklist) + variance.
A guide KEY-CARDS line teaching "Azcanta's dig grabs only a NONCREATURE-NONLAND card" was CONSIDERED
and REJECTED this wave: it cannot clear the ordering-menu TIMEOUTS (the dominant failure), it rides
on a possibly-mis-scripted engine put-to-hand, and per wagicgpt doctrine the representation/engine
is the correct layer to fix first. Reconsider only if the engine fix does not land. Guide diff = 0 bytes.

## GAME -> FILE MAP (run = matchups-20260716-151559)
| opp | outcome | turns | final (me v opp) | deck135 seat file |
|-----|---------|-------|------------------|-------------------|
| vs49  | WIN (real)         | 14 | 11 v -6 | `1784235367-ai_baka_deck135-0x5652283d5150-vs-ai_baka_deck49.jsonl` |
| vs62  | LOSS (adj)         | 12 | 14 v 20 | `1784232967-ai_baka_deck135-0x55fde88562b0-vs-ai_baka_deck62.jsonl` |
| vs110 | LOSS (adj)         | 14 | 2 v 6   | `1784235356-ai_baka_deck135-0x55d1692fe190-vs-ai_baka_deck110.jsonl` |
| vs21  | LOSS (real)        | 15 | -5 v 20 | `1784232963-ai_baka_deck135-0x55e3cfd58080-vs-ai_baka_deck21.jsonl` |
| vs17  | LOSS (real)        | 9  | -3 v 20 | `1784234421-ai_baka_deck135-0x5583a9fac250-vs-ai_baka_deck17.jsonl` |
| vs35  | LOSS (real)        | 9  | -6 v 20 | `1784238229-ai_baka_deck135-0x55ea7f8f01a0-vs-ai_baka_deck35.jsonl` |
