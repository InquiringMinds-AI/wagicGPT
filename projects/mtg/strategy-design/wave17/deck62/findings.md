# deck62 (Enchantresses) — FIRST-GUIDE VALIDATION findings (wave 17)

Corpus: `matchups-20260716-151559`, binary `/tmp/wagic-7cdcf9c73`. deck62's FIRST guided corpus
(capability-table-lead guide shipped wave-16, byte-identical live at Res/ai/baka/deck62_strategy.txt).
6 seat translogs. Record **4/6, up from 2/6 guideless** — but per Step 0-ter the record is context;
the guide's JOB is whether the taught classes died and the reasoning-TAX collapsed. Both did,
decisively.

## Headline: the tax collapsed. This is the guide working.
- **Fallbacks 3/143 (~2.1%)** — down from wave-16's **17/147 (~11.6%)**: 3 unparsed, 0 stale_echo,
  0 defer, 0 empty. (Wave-16 was 12 unparsed + 5 stale.)
- **Giant replies (>8k chars) went 14 -> 1.** Wave-16 had 14 replies over 8k (combat/aura-math
  agonizing); this corpus has exactly one (vs49 s17, 12710 — Blanchwood Forest-recount, below).
  Every other decision is short and decisive; most casts/attacks are 300-1500 chars.
- No enchant-opponent gift, no hallucinated-removal reasoning, no durdle-at-1-life anywhere.

## Record (4/6)
- **WIN vs deck135** (adj1 in favor, T12 L20 vs L14) — POSITION was correctly ahead: wide board
  (Canopy Spider + 4 Saprolings + Birds) vs opponent with no blockers, building lethal (s28-29).
  Adjudication is a latency artifact; the board was won.
- **WIN vs deck21** (T15, opp -1) — ground game: single Yavimaya Enchantress pumped with Web to
  4/6 trample+reach, attacked every turn (s18/20/23/26/29), Birds chump-blocked Raging Goblin.
- **WIN vs deck35** (T10, opp -6) — clean go-wide: Canopy Spider pumped (Web/Gaea's/Blanchwood to
  2/6 trample-reach) + Fists Saprolings, alpha-struck a Slivers board with 8 creatures (s22).
- **WIN vs deck49** (T10, opp -4) — go-wide Saprolings under Blanchwood/Primal Rage; the guide's
  win line fired despite the corpus's single worst tax reply (s17).
- **LOSS vs deck17** (T10, L0) — flyer race lost to Faeries. NOT a play failure: the model
  correctly held its sole reach-blocker back vs 5 flyers (s20, ATTACK: none, correct). Just behind
  on the race — a ground enchantress deck losing to a mono-flyer tempo deck is a matchup floor.
- **LOSS vs deck110** (adj0, T8, behind) — run over by affinity in 8 turns; only ever fielded
  Argothian (0/1) + Yavimaya, never assembled a threat before the clock. Construction/matchup, not
  a taught-class failure.

## Per-taught-class: died / persisted / mutated

| Taught class | Verdict | Evidence |
|---|---|---|
| Auras DRAW/PUMP, not removal (hallucinated-removal) | **DIED** | 0 recurrences. Every aura cast framed as pump/token/draw (vs49 s17 pure token+Primal-Rage math; vs135 s16-17, vs35 s7/s15/s17). No "cast to kill/trade" anywhere. |
| Enchant-opponent GIFT (targeting) | **DIED** | Every target chosen was OWN creature: vs135 s8 ignored opp Ice-Fang Coatl for own Druid; vs21 s7/s12/s17; vs49 s9; vs17 s6/s8/s12; vs35 s8/s12/s16/s18/s21. Reinforced by new engine label "the only legal targets are YOUR OWN right now." |
| Argothian-first sequencing | **DIED** (where drawn) | vs135 s4, vs110 s4/s10 cast Argothian promptly on curve; no enchant-before-Argothian mis-sequencing observed. Other games didn't draw her early and correctly ran the beater/go-wide plan. |
| Attack every turn / no durdle | **PERSISTED (working)** | Attacked every turn with real power: vs21 Yavimaya x5, vs49 Saprolings x4, vs35 x4, vs135 s22. Correctly held sole blocker vs lethal (vs17 s20). No passive-at-low-life recurrence. |
| Defensive reach / block flyers | **PERSISTED (working)** | vs17 Birds blocks Wasp Lancer (s14), Web-reach Saproling blocks (s21); vs135 Druid blocks Ice-Fang Coatl (s10); vs21 Birds blocks. Reach usage correct. |
| TRUST printed [X/Y], don't re-derive aura math | **PERSISTED-and-DISOBEYED (tax relocated)** | Belief present but the model still recounts, now concentrated on **Blanchwood Forest-count at the CAST step**: vs49 s8 (10528), s16 (9962), s17 (12710) all recount Forests despite the prompt showing "Saproling (6/6) (printed 1/1)". Tax RELOCATED from wave-16's combat-UNPARSE to aura-cast recount — but no longer unparses. Biggest residual; guide trust-line tightened this wave (Blanchwood-specific). |
| Attack from the A-list (eligibility) | **MUTATED** | Wave-16: counted summoning-sick and drowned. Now: the model reads its OWN board, not the offered list — vs135 s19 named "Canopy Spider, Saproling, Saproling" (all made T9, summoning-sick) when the A-list held ONLY "A1. Argothian Enchantress" -> unparsed (choice=-1). Correct answer was ATTACK: none. Guide tightened: "never name a creature absent from the list." |

## Fallback repros (all 3)
- **vs135 s18 (unparsed) — SUB-MENU MISREAD.** Prompt was "Choose the target for Web" (own
  creatures: Canopy Spider, 2 Saprolings). The model ignored the sub-menu, wrote a PLAN about
  attacking, and emitted "CHOICE: 0 (Attack with...)" — out of the 1-3 range -> unparsed. This is
  MY seat showing the same sub-menu-confusion SHAPE flagged for deck135 (target sub-menu read as a
  different decision type). Representation/engine item, not guide-fixable (see notes N1).
- **vs135 s19 (unparsed, choice=-1) — attacked summoning-sick creatures absent from the A-list**
  (the eligibility MUTATION above). Guide-addressable; tightened.
- **vs110 s5 (unparsed) — NO-BLOCK FORMAT GAP.** Decision was CORRECT (don't block 0/1 Argothian
  into 4/1 Memnite). The model wrote "BLOCKS: (no assignments)"; the blockers prompt only
  demonstrates a per-blocker "B2:none" token and has NO all-none example (unlike the attackers
  prompt's explicit "ATTACK: none"). Parser rejected the improvised phrase -> unparsed. HARNESS
  item (see notes N2).

## Residual tax pocket (guide-addressed, watch next wave)
Blanchwood Armor is the last big tax generator. The model does not trust the pre-computed P/T on
Blanchwood-wearing creatures and re-derives "+1/+1 per Forest" from the land count every time
(vs49 s7-s17). The wave-17 trust-line now names Blanchwood and the "(printed 1/1)" annotation
explicitly. If s-level reply lengths at Blanchwood casts do not drop next corpus, route to a
representation nudge (notes N3), not more guide prose.
