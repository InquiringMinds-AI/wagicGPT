# Deck-135 (Modern Snow) — Wave-19 game review

Corpus: PRIMARY `matchups-20260719-092058` (21-game round-robin over 110/135/62/49/35/14/27;
binary `/tmp/wagic-72b05535d` = wave-19 batch: combat name tolerance + TRADE OUTCOME block
annotations + alt-cast pitch surfacing + salvageLoopedChoice; **GPT REVEAL SEAM f4a2ea891 —
`Player::decideReveal`, translog kind `reveal`**; suppression->annotation replacement; Oracle sweep).
deck21 rotated out; deck27 (Zombies) entered guideless. deck135 OWNS THE REVEAL SEAM CHECK — all
18 reveal decisions this corpus sit at this seat. All six deck135 seat games read decision-by-decision.
Doctrine: win column CONTEXT ONLY; unit of analysis is the DECISION with seq repros, layer-attributed;
adopt-a-revision bar HIGH (guide FROZEN since wave-11). Charges: (a) reveal-seam verification — all 18
decisions, the non-contiguous ordering residual, the Azcanta-class optionone question; (b) 9 fallbacks
(worst seat) — old giant-reply tax or new shape?; (c) pitch-surfacing validation; (d) durdle read.

## RECORD (context only — NOT a verdict) — 2W / 4L

| game    | opp                | outcome (deck135)      | my/opp life | turn | class |
|---------|--------------------|------------------------|-------------|------|-------|
| 135v35  | 35 Mighty Slivers  | **WIN** (adj, ahead)   | 18 / 16     | 10   | slow-but-ahead grind; stabilized vs Slivers (was a LOSS wave-18) |
| 135v14  | 14 Deep Blue       | **WIN** (real, kill)   | 13 / 0      | 14   | curve + Treefolk/Coatl/Diamond-Faerie finish; clean |
| 135v62  | 62 Enchantresses   | LOSS (adj, behind)     | 13 / 18     | 12   | under-powered small-creature clock; out-raced |
| 135v49  | 49 Dragons         | LOSS (adj, behind)     | 10 / 12     | 10   | **Glacial Revelation mill BUG stripped its gas** + Dragon clock |
| 135v27  | 27 Zombies (new)   | LOSS (real, dead)      | -2 / 19     | 11   | out-raced; only 2 attack declarations; deployment stall |
| 135v110 | 110 Etched Affinity| LOSS (adj, 1 life)     | 1 / 20      | 11   | **Glacial Revelation mill BUG milled its only blocker at 11 life** + ground overrun |

Win table assigns deck135 2/6; matches. Path: 1/6(w13)->0/6(w14)->2/6(w15)->1/6(w16)->1/6(w17)->
3/6(w18)-> **2/6(w19)**. **3 of 4 losses adjudicated at the turn cap** (brief said "2 of 3" — actual is
3 of 4); deck135 sits in 4 of the corpus's 5 adj games. The decline from 3/6 is NOT a play regression:
two of the three adj losses (vs49, vs110) were materially worsened by a NEW game-affecting engine bug
the reveal seam exposed (ENGINE-R1). See durdle section.

---

# NEW SURFACE — THE REVEAL SEAM (deck135 owns this — PRIMARY DELIVERABLE)

18 reveal decisions, ALL at this seat. They route FOUR distinct card effects through one
`Player::decideReveal` seam. Verdict per effect, then the two brief questions.

## Effect map (which card drives each reveal)

| effect (card)                        | # | seqs | seam verdict |
|--------------------------------------|---|------|--------------|
| Search for Azcanta UPKEEP look-top-1 | 8 | d14 s11/18/24/29/34, d49 s24/28, d27 s22 | **WORKS both ways** OK |
| Into the North SEARCH (snow->battlefield) | 5 | d35 s10*, s18; d62 s17/23; d49 s11 | outcome CORRECT, rendering MISLEADING |
| Glacial Revelation (top-6, snow->hand)   | 4 | d27 s8*, d49 s30, d27 s12, d110 s18 | **CONFIRMED BUG — to-hand partition dropped** |
| Azcanta, the Sunken Ruin activation (top-4) | 1 | d27 s24 | eligibility not surfaced -> 0 to hand |

(* = fell to `unparsed_reply` fallback; see fallbacks section.)

## (R1) Glacial Revelation — CONFIRMED BUG: the "get snow" (to-hand) partition is silently dropped; every revealed card goes to graveyard. GAME-AFFECTING.

Glacial Revelation Oracle/text: *"Reveal the top six cards of your library. You may put any number
of snow permanent cards from among them into your hand. Put the rest into your graveyard."* Three
parsed instances, **ZERO cards reached hand in all three** — the model's chosen snow permanents were
milled along with everything else:

| seq | model chose ("get snow"->hand) | engine outcome | consequence |
|-----|-------------------------------|----------------|-------------|
| **d110 s18** | `PUT: 2,6` = Snow-Covered Island + Ohran Viper | "put Snow-Covered Island, Ohran Viper to get snow" -> then **all 6 incl. both "goes to graveyard"** | at **11 life vs lethal Master of Etherium**, the model correctly grabbed Ohran Viper (its ONLY blocker/answer) — engine MILLED it. Collapsed to 1 life, adj LOSS. |
| **d49 s30** | `PUT: 1,2,3,4,6` = Coatl,Viper,Coatl,Treefolk,Viper | "put ... to get snow" -> then **all 6 incl. the 5 picks "goes to graveyard"** | at **10 life vs 12**, tried to rake 5 creatures incl. **Abominable Treefolk (its finisher)** into hand — all milled. adj LOSS. |
| **d27 s12** | `PUT: 1,3,4,5,6` (skip Boreal Druid) | "put ... to get snow" -> then **all 6 "goes to graveyard"** | 18 life, non-critical, but Snow-Covered Island + Arcum's Astrolabe (both legal snow permanents) milled. |

**Not an eligibility rejection.** d110 s18 chose Snow-Covered Island — unambiguously a snow permanent
— and it was milled. The to-hand move fails for definitely-legal picks.

**Root-cause hypothesis (card-script layer, VERIFY before fixing).** The interactive `auto=` line
for Glacial Revelation (`Res/sets/primitives/borderline.txt:45643`) is:
`auto=name(Look) reveal:6 optionone name(Get Snow) target(<anyamount>*[snow]|reveal) moveto(hand)
optiononeend optiontwo all(*|reveal) moveto(graveyard) optiontwoend revealend`.
optiontwo's `all(*|reveal) moveto(graveyard)` sweeps EVERY revealed card to graveyard — apparently
including the ones optionone's `<anyamount>` multi-select just moved to hand. Contrast the WORKING
reveals: Search-for-Azcanta upkeep uses `optiontwo target(<1>*|reveal) moveto(ownerlibrary)`
(targets the REMAINDER, not `all`), and it resolves correctly. The break is specific to the
`<anyamount>` multi-to-hand interactive path colliding with an `all(*|reveal)` optiontwo. NOTE: the
NON-interactive `aicode` line for the same card is CORRECT
(`all(snow[zpos<=6]|mylibrary) moveto(hand) && all(*[-snow;zpos<=6]|mylibrary) moveto(mygraveyard)`)
— so the reveal-seam routing (GPT player -> interactive `auto=` per the ishuman lens) EXPOSED a latent
interactive-script bug that Baka never hit. `all(*|reveal)` appears 538x corpus-wide; single-target
optionone cards (e.g. reveal-until-land) may be fine, so the fix is scoped to the multi-to-hand shape,
not a blanket `all` ban. -> notes ENGINE-R1. Effect today: Glacial Revelation is a `{2}{G}` sorcery
that MILLS your own top 6 — a strict card-DISADVANTAGE nothing spell, actively hurting a deck whose
guide calls it a card-advantage engine.

## (R2) The non-contiguous-subset ordering residual — VERDICT: the "chose 2,3 / engine moved 1" ORDERING hypothesis is REFUTED as the mechanism; a MORE SEVERE confirmed bug (R1) occupies the seam. Seqs d110 s18 / d49 s30 / d27 s12.

The pre-corpus suspect was a skip-first / index off-by-one (model picks indices 2,3; engine moves
index 1). What actually happens is worse and index-independent: the ENTIRE to-hand partition is
dropped to graveyard regardless of which indices were chosen. All three exercised picks WERE
non-contiguous / skip-first — d110 `2,6` (skip 1), d49 `1,2,3,4,6` (skip 5), d27 `1,3,4,5,6` (skip 2)
— and in every case the engine moved the chosen cards to the WRONG zone (graveyard), never to hand.
The single pre-corpus "engine moved index 1" observation is consistent with an early glimpse of this
same `all(*|reveal)` sweep (index 1 went to graveyard because ALL go to graveyard), not an ordering
fault. **Status: CONFIRMED-BUG (partition drop), not an ordering bug.** The one seam that DOES honor
the chosen SET — Into the North (R3) — takes the first legal snow land in the model's list in index
order, correctly, with no ordering error. So there is no live non-contiguous ORDERING defect; there is
a partition-drop defect.

## (R3) Into the North SEARCH — outcome CORRECT, rendering MISLEADING (representation defect).

Into the North (*"Search your library for a snow land card and put it onto the battlefield tapped"*)
is rendered through the reveal seam as *"you looked at the top 49 cards of your library. Decide which
go to 'choose card' vs 'put back'"* — the WHOLE library, no snow-land filter, verbs describing a
hand-partition rather than a search-to-battlefield. The model reads it as Glacial Revelation and picks
a huge arbitrary subset (choice=16/39/15/18). **Engine outcome is CORRECT in all 4 parsed instances**:
the engine takes the FIRST legal snow land in the model's chosen set and puts exactly it onto the
battlefield —

| seq | first snow land in chosen set | engine -> battlefield |
|-----|-------------------------------|-----------------------|
| d35 s18 | Snow-Covered Forest (pos 8) | Snow-Covered Forest OK |
| d62 s17 | Snow-Covered Island (pos 3) | Snow-Covered Island OK |
| d62 s23 | Snow-Covered Forest (Prismatic Vista non-snow skipped) | Snow-Covered Forest OK |
| d49 s11 | Snow-Covered Forest (pos 4) | Snow-Covered Forest OK |

So the legality filter salvages a correct result. But the misrendering is expensive: it produced a
12k-char confusion spiral at every instance and TWO of the seat's unparsed fallbacks (d35 s10, d27 s8
— fallbacks section). REPRESENTATION defect (offer only legal snow-land targets, label it a search-to-
battlefield), not a play-quality loss. -> notes ENGINE-R3.

## (R4) Azcanta-class optionone question — VERDICT: the seam CLOSES the old optionone AUTO-DECLINE for the Search-for-Azcanta UPKEEP reveal (model now drives it; 8/8 correct). Two residuals replace it.

The wave-17/18 KNOWN-OPEN item was: the engine AUTO-DECLINED the optional reveal chooser before the
model could answer (the model never had the choice; the 4b fingerprint). **That fingerprint is GONE** —
the model now drives all 8 Search-for-Azcanta upkeep looks, and BOTH branches resolve correctly:
- **put-to-graveyard fires**: d14 s11 (Arcum's Astrolabe -> graveyard, filling toward the 7-card
  transform), d14 s29 (Prismatic Vista -> graveyard). Events confirm `... goes to graveyard`.
- **keep-in-library fires**: d14 s18/24/34, d49 s24/28 all chose `PUT: none` and the top card stayed
  in library and was drawn on the normal draw step next turn.
The upkeep script (`borderline.txt:98940`) uses the safe `target(<upto:1>*|reveal)` / `target(<1>*|reveal)`
idiom — it is correct. **Charge (R4) = the auto-decline is CLOSED.** OK

Two residuals now stand in its place:
1. **Glacial Revelation to-hand drop** (R1, new confirmed bug — a RESOLUTION break, not an auto-decline).
2. **Azcanta ACTIVATION eligibility not surfaced** (d27 s24, the only Azcanta transform+activation this
   corpus): the transformed-Azcanta ability (`borderline.txt:7118`) offers `optionone Get a card
   target(<upto:1>*[-land;-creature]|reveal)` — only a NONCREATURE-NONLAND may go to hand. Options were
   shown as [Windswept Heath, Diamond Faerie, Boreal Druid, Gelid Shackles] with NO eligibility marks;
   the model picked Diamond Faerie + Boreal Druid (both CREATURES = ineligible) -> 0 to hand, all 4 ->
   bottom. The zero-to-hand here is the MODEL picking ineligible cards, NOT provably the R1 all-sweep
   (no eligible card was tested — only Gelid Shackles was legal and it wasn't chosen). Separately: the
   opaque wave-17 "Choose target N of exactly 4" ORDERING menu is GONE (now a clean reveal seam) — that
   half of ENGINE #4 is CLOSED. -> notes ENGINE-R4 (representation: mark/filter eligible cards).

## Reveal-seam bottom line
- optionone auto-decline (Search-for-Azcanta upkeep): **CLOSED** — model drives it, 8/8 correct.
- Azcanta opaque ordering menu (ENGINE #4 first half): **CLOSED** — clean reveal seam now.
- Glacial Revelation to-hand: **NEW CONFIRMED BUG** — milled, 3/3, game-affecting (R1).
- Non-contiguous ordering residual: **REFUTED as an ordering bug**; the seat's residual is the R1
  partition drop (R2).
- Into the North search rendering: **MISLEADING** (outcome correct; representation defect, R3).
- Azcanta activation eligibility: **not surfaced** -> model picks ineligible cards (R4).

---

# (b) THE 9 FALLBACKS (worst seat) — old giant-reply tax PLUS two NEW reveal-seam shapes PLUS one combat-name-tolerance residual.

Profile: 2 stale_echo, 7 unparsed_reply (0 defer, 0 empty). salvageLoopedChoice did NOT rescue any
of the 9. Six of the nine replies are >12k chars = the giant-reply repetition/spiral tax is still
dominant. Breakdown:

| seq | kind | class | len | note |
|-----|------|-------|-----|------|
| d35 s5  | ask | stale_echo | 562 | "CHOICE: 2 (Play Misty Rainforest)" — parenthetical option-label echo. Old shape, benign (adj-WIN game). |
| d35 s25 | ask | stale_echo | 968 | "CHOICE: 1 (Horned Sliver ...)" — target-label echo. Old shape, benign. |
| **d35 s10** | **reveal** | **NEW: Into-the-North misrender** | 12182 | spiral reconciling "Into the North cast" with a 49-card "choose subset" prompt ("pick 1,7,11,38. Wait 37 and 49 are also..."). Caused by R3. |
| **d27 s8** | **reveal** | **NEW: Glacial-Rev snow-eligibility confusion** | 13345 | spiral on which of 6 are snow permanents ("pick 4 and 6... maybe Misty Rainforest is a snow land?"). Seam doesn't mark eligibility. |
| d35 s16 | ask | repeat-loop (carried) | 12630 | mana-payment loop ("play Into the North and pay {1} with {U}? No. Wait...{G}? No..."). |
| d35 s21 | priority | repeat-loop (carried) | 15058 | markdown-bullet spiral ("Maybe I need a land to tap for mana..." xN). |
| d35 s30 | ask | repeat-loop (carried) | 12796 | lethal-math loop ("Total 11. Opponent 16. I need 5 more. Is there..."). |
| d110 s25 | ask | repeat-loop (carried) | 13845 | lethal-math at **1 life** ("Treefolk deals 9 to Master. Master survives... not lethal."). |
| **d62 s27** | **blockers** | **combat-name-tolerance residual** | 923 | NOT a loop — a well-formed single block `BLOCKS: Ice-Fang Coatl: Saproling (1/1) #1` DISCARDED because the "(1/1) #1" disambiguation suffix defeated the name->label second pass. |

Read:
- **The reveal seam ADDED 2 of the 9 fallbacks** (d35 s10, d27 s8) — its eligibility/effect opacity
  feeds the giant-reply tax directly. Fixing R1/R3/R4 representation would likely remove both.
- **4 carried repeat-loops** (d35 s16/s21/s30, d110 s25) — same decode-time spiral tax as wave-18, all
  on lethal-math / mana-confusion at hard spots, all in eventually-lost or already-won contexts (none
  flipped a winnable game). salvageLoopedChoice did not fire on them.
- **1 combat-name-tolerance residual** (d62 s27): the brief flagged that block fallbacks should have
  COLLAPSED. At this seat one did NOT — but it is a NEW shape, not the old computed-P/T distrust: the
  reply is short and well-formed; the parser choked on the `(1/1) #1` count-suffix the model appended to
  disambiguate two Saprolings. The name-tolerance second pass needs to strip trailing `(P/T) #N`
  annotations. This discarded a legal single block (Ice-Fang Coatl on a Saproling) — non-fatal (adj-LOSS
  game) but a real dropped decision. -> notes HARNESS-1.
- **2 old stale_echo** (d35 s5/s25): the option-label parenthetical echo — the model appends a
  human-readable label after `CHOICE: N`. Benign, both in the adj-WIN. Carried, cosmetic.

salvageLoopedChoice verdict at this seat: **0 of 9 salvaged.** Most damning is d62 s27 — a short,
well-formed `BLOCKS:` line that a last-well-formed-CHOICE salvage should trivially recover but did not
(salvage keys on CHOICE/ATTACK, and the block line's format mismatch is upstream of salvage). -> gen-sugg.

---

# (c) PITCH-SURFACING — VALIDATED. The wave-18 ENGINE #1d fix (name the auto-pitched card) works; the finisher was NOT eaten this corpus.

deck135 was offered + cast Force of Negation once (d62 s19), free exile-pitch mode on the opponent's
turn. The option line now carries the annotation this seat asked for in wave-18:

> `Cast Force of Negation with its exile a blue card from hand cost  (auto-exiles ONE of: Search for
> Azcanta, Ice-Fang Coatl, Search for Azcanta - the engine picks for you here) - can target on the
> stack: Gaea's Anthem`

Model cast it; events (d62 s20): `Your Search for Azcanta: hand -> exile / Your Force of Negation:
hand -> stack / Opponent's Gaea's Anthem: stack -> exile`. The engine auto-picked **Search for Azcanta**
(a dig spell) to pitch — **NOT** Abominable Treefolk / a finisher. **The wave-18 finisher-eating problem
(vs62 s12: Force ate the only blue card = Treefolk) did NOT recur** — the naming annotation let the
model commit knowing the pitch pool, and the engine's auto-pick landed on a low-value card. Charge (c)
answered: the fix is validated.

Two residuals (both low-priority, carried):
1. **Still reflexive off-whitelist Force.** Gaea's Anthem is a `+1/+1` team anthem — NOT a wipe /
   mass-bounce / only-threat-kill (the guide's whitelist). Same model over-application as wave-18's
   Primal Rage. Non-fatal (the Force wasn't decisive in the adj loss). Model-watch, carried; do NOT add
   a 4th Force line (already-maximal, prior-wave ruling). The incoming off-whitelist-target annotation
   (owner ruling) is the right lever. -> gen-sugg #1.
2. **Engine auto-picks the pitch; the model can't choose.** deck135 held THREE blue cards (2x Search for
   Azcanta + Ice-Fang Coatl); the annotation says "the engine picks for you here." Wave-18's deeper ask
   (surface the pitch-card SELECTION when holding >1 blue card) remains OPEN, but is now LOW priority —
   the auto-pick chose well, and the "exiles your ONLY eligible card" warning correctly did NOT fire
   (3 blue cards, not 1). -> notes ENGINE-1d-followup.

---

# (d) DURDLE READ — deck135 is NOT durdling. The adj losses are under-powered-clock games; two of three were WORSENED by the R1 reveal bug, not by 0-attacker durdling.

Attacker declarations (Rule #2 compliance) across the 3 adj losses + adj win:
- **vs35 (adj WIN 18/16):** attacked T5 Golem, T7 no-attack (a correct defensive hold), T9 Golem, T10
  Golem+Treefolk. Slow small-creature clock, but AHEAD -> deserved adj win. Not a durdle.
- **vs62 (adj LOSS 13/18):** attacked EVERY combat (T7 Coatl, T9/T11 Coatl+Viper), blocked T8. Pure
  under-powered clock vs Enchantress lifegain. Not a durdle.
- **vs49 (adj LOSS 10/12):** attacked EVERY combat (T3/T5 Golem, T7/T10 Golem+Druid+Faerie). Loss
  materially worsened by **R1 milling 5 creatures incl. Treefolk at s30 (10 life)** — the deck's own
  card engine stripped its gas. Not a durdle; a bug-assisted grind.
- **vs110 (adj LOSS 1/20):** the ONLY near-durdle — T7 "no attackers", T8 "no attackers" while blocking
  Master of Etherium with Boreal Druid at collapse. But the collapse was **R1-assisted**: s18 milled
  Ohran Viper, its only answer to Master, at 11 life. Ground into 1 life. Not idle durdling — got run
  over AND milled its own answers.
- **vs27 (real LOSS -2/19):** only 2 attack declarations (both Ice-Fang Coatl, T6) — a genuine
  deployment stall vs a faster Zombie clock (closest to a durdle, but a deploy-tempo problem, not a
  no-plan durdle). No R1 instance here.

Verdict: Rule #2 is honored; there is NO 0-attacker durdle signature. The 3/6->2/6 dip is partly an
**engine-bug artifact** (R1 stripping card advantage in 2 of 3 adj losses), partly under-powered-clock
+ matchup variance (the standing construction flags below). vs35 flipping LOSS->adj-WIN and vs14's clean
kill show the deck's plan still executes when the reveal bug doesn't bite.

---

# CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)
- **Under-powered small-creature clock — STANDS (6th corpus).** vs62/vs49/vs110 all reached the turn
  cap because Golem/Coatl/Viper couldn't close; Abominable Treefolk / Diamond Faerie remain the only
  real closers. The reveal bug (R1) makes this WORSE by milling the closers off the top.
- **Force DEAD vs all-creature aggro — softened this wave.** vs35 (Slivers) flipped LOSS->adj-WIN — the
  deck stabilized without needing Force. Flag stands in principle (Force counters only noncreatures) but
  did not cost a game this corpus.
- **Thin blue base + fetch-color mismatch — STANDS.** Windswept Heath still cannot fetch Island; the
  gold/double-pip dependence remains the fragility.

# GUIDE DISPOSITION: FROZEN (byte-identical to live `Res/ai/baka/deck135_strategy.txt`; `cmp`-verified).
Every residual this corpus routes OFF the guide. The dominant new finding (R1 Glacial Revelation to-hand
drop) is a card-script/engine bug — no guide text can make a milled card reach hand. R3/R4 are
representation. The fallbacks are decode-time + seam-opacity. The reflexive Force is model over-
application of an already-maximal rule (prior-wave ruling holds). Guide diff = 0 bytes. See notes for
the layer-routed ledger.

## GAME -> FILE MAP (run = matchups-20260719-092058)
| opp | outcome | turns | final (me v opp) | deck135 seat file |
|-----|---------|-------|------------------|-------------------|
| vs35  | WIN (adj)   | 10 | 18 v 16 | `1784470862-ai_baka_deck135-0x55d4cf1ef840-vs-ai_baka_deck35.jsonl` |
| vs62  | LOSS (adj)  | 12 | 13 v 18 | `1784472048-ai_baka_deck135-0x56057f8526b0-vs-ai_baka_deck62.jsonl` |
| vs14  | WIN (real)  | 14 | 13 v 0  | `1784474658-ai_baka_deck135-0x55e65ab666a0-vs-ai_baka_deck14.jsonl` |
| vs49  | LOSS (adj)  | 10 | 10 v 12 | `1784476971-ai_baka_deck135-0x55b453029890-vs-ai_baka_deck49.jsonl` |
| vs27  | LOSS (real) | 11 | -2 v 19 | `1784478369-ai_baka_deck135-0x556cf66e27d0-vs-ai_baka_deck27.jsonl` |
| vs110 | LOSS (adj)  | 11 | 1 v 20  | `1784478470-ai_baka_deck135-0x560a586329f0-vs-ai_baka_deck110.jsonl` |
