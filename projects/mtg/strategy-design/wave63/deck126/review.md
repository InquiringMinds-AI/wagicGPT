# Wave-63 per-deck review — deck 126 (Sanguine Blood, WGB wall prison + Sanguine Bond/Exquisite Blood)

Corpus: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-191148/`, binary 98d13050f.
Seat files below are abbreviated to `<epoch>…vs-deck<N>`; all are
`<epoch>-ai_baka_deck126-<ptr>-vs-ai_baka_deck<N>.jsonl` in that directory.
Record: **1 win, 5 losses**. No `fallback` on any of the 209 seat records in the six games; no
re-asks visible, no `async_drops` in the seat gameends.

---

## 1. Game by game

### G1 — vs deck123 (Bloodline Keeper tokens). LOSS, turn 14, life -68/18. `1788653519…vs-deck123`
17 asks, 0 blockers asks. Real-hand game: the seat drew three Tribute to Hunger and its first
white source on turn 5.
- seq 6 (T5) first creature (Pride Guardian) — killed by the opponent's own Tribute to Hunger on T8.
- seq 9 (T9) Sanguine Bond; seq 11/13/15 (T11) three defenders in one turn. Curve was land-gated,
  not decision-gated: every earlier window's cast menu was empty or one card.
- **Decided at T12, in the opponent's turn, with no decision for this seat**: Intruder Alarm +
  Bloodline Keeper + Lightning Greaves produced 22 flying Vampires in one activation
  (narrated correctly and collapsed as `Opponent used: Create vampire with Bloodline Keeper (x22)`).
  Every attacker had flying; all three walls are ground blockers, so no blockers ask was ever
  issued — correct, and the CRACK-BACK line said so exactly
  (`44 from 22 attackers nothing you control can legally block`, seq 18 prompt).
- seq 17/18 (T13) two Tribute to Hunger with Sanguine Bond out, gaining 2 and draining 2 each
  against an opponent at 22 — futile but the only rows on the menu. Not a misplay; the game was
  already unwinnable at T12.
No STRATEGY misplay found in this game.

### G2 — vs deck162 (Ob Nixilis / Fate Unraveler draw-ping). LOSS, turn 15, life 0/9. `1788653525…vs-deck162`
**The best-played loss and the most informative game.**
- seq 11/13 (T9) two Tribute to Hunger eating Master of the Feast (toughness 5) and Shield Sphere
  (toughness 6) under Sanguine Bond: +11 life, opponent 20 → 9 in one turn. Excellent.
- seq 20/21 (T13) Idyllic Tutor → Exquisite Blood; seq 23 casts it. **Both halves of the pair were
  on the battlefield at end of T13 with the opponent at 9.**
- The seat still lost, and correctly so: every point of damage it took came from Ob Nixilis + Fate
  Unraveler triggering on each card DRAWN (opponent ran Dictate of Kruphix and Howling Mine), i.e.
  from a source no wall can block and no Tribute answers. It went 23 → 11 across T11-T13 and died in
  T15's draw step before it ever had a life-gain event to start the loop.
- seq 25 (T13 cleanup) discard `Sorin, Lord of Innistrad, Chromatic Lantern` from a 9-card hand of
  six lands + Sorin + Lantern + Wall of Omens. This looks like pitching the deck's only in-hand
  life-gain enabler, but it is **correct**: a Sorin was already on the battlefield at 5 loyalty and
  Sorin is Legendary (`type=Legendary Planeswalker`, planeswalkers.txt:3062), so the second copy is
  dead. The guide's discard order (send Lantern, then a duplicate wall, then a land past the sixth,
  then Sorin) produced exactly this answer.
Verdict: lost to an uninteractable clock, not to a decision.

### G3 — vs deck146 (WB aggro + Silverquill Silencer). LOSS, turn 15, life -8/19. `1788653532…vs-deck146`
- seq 8/9 (T8) Idyllic Tutor → Sanguine Bond; seq 11 (T10) casts it into the Silencer tax. The
  `[NAMED BY THEIR Silverquill Silencer: … you would be at 17. This price is PER CAST…]` annotation
  is accurate and the model reasoned from it explicitly in its PLAN. Good surface.
- **Deciding window, seq 13 (T12), life 12, zero creatures, CRACK-BACK `up to 8 - you would be at 4`:**
  the model tapped out for Staff of Nin {6}. The board needed a body, not a card-draw engine.
  Mitigating: the only other row was a second Sanguine Bond at a 3-life Silencer tax, so no row on
  that menu blocked a creature. This is a curve/draw problem, not a clear guide violation — the
  seat had drawn no wall since T1.
- seq 16 (T14, life 4, CRACK-BACK `you would be at -4; that would KILL you`) it took the one
  survival row available (Pride Guardian over the 3-life-tax Sanguine Bond) — correct — and died to
  four attackers a single 0/3 could not stop.

### G4 — vs deck125 (UW control). WIN, turn 68, life 23/90 — opponent **decked out**. `1788653534…vs-deck125`
95 asks; 865 hold windows and 705 mana-only windows skipped, so the wave-63 hold machinery carried
this game (17 held rows vs 95 asks over 68 turns).
- **Four Idyllic Tutors were countered** (Dream Fracture ×2, Cancel, Fall of the Gavel — seq 43, 68,
  82 and one earlier). That is why the seat has zero `reveal` records in this game; nothing is
  broken. Each was jammed alone into 2-3 open blue sources with 12-21 of its own sources untapped.
- seq 48 (T36) Exquisite Blood lands; **Sanguine Bond only at seq 97 (T67)** — the pair never both
  resolved in time, and the win came purely from the opponent running out of library.
- seq 50/51 (T37) two Tribute to Hunger against a lone Emrakul, the Aeons Torn (17/15): +15 life
  each with Exquisite Blood out — the seat went 9 → 39 in one turn. That is the play that survived
  to the deck-out.
- The one avoidable pattern: the seat held 12-21 untapped sources for ~25 turns and never once used
  the surplus to protect or bait; the guide has no anti-counterspell teach and neither does the deck
  (it has no protection to teach).

### G5 — vs deck130 (RG goblins / Siege-Gang). LOSS, turn 22, life -4/20. `1788653538…vs-deck130`
- 3 blockers asks, all reasonable and all matching the engine's suggested assignment
  (seq 12, 18, 21).
- seq 6/7 (T5) Idyllic Tutor → Exquisite Blood, i.e. it tutored the half it could not cast for 16
  turns while Sanguine Bond was already in hand. Defensible (Bond was uncastable too) but it is the
  slower half of the pair against an aggro board.
- Land-screwed: no land drop was available T7 or T9 (`Land drop: you have no land you could play
  right now`, seq 9 prompt) and it drew two Chromatic Lanterns instead.
- seq 21 (T20) block was optimal and the **LOOP HALF PENDING** clause fired correctly and usefully
  (see §2, item 4). Sanguine Bond landed T19 and Exquisite Blood T21, one turn after the seat was
  already at 2 life. Lost the race.

### G6 — vs deck152 (WU Luminarch aggro). LOSS, turn 11, life -5/20. `1788653551…vs-deck152`
**The only game with a clear, guide-contradicting deciding decision — and there are two of them.**
- **seq 2 (mulligan #1)**: 7 cards, 2 lands (Savannah, Sunpetal Grove) + Tribute ×2, Exquisite
  Blood, Sorin, Chromatic Lantern; coverage line `would not cover any spell in it`. The live guide's
  mulligan section says verbatim: *"Two lands and a coverage line reading 'would not cover any spell
  in it' … on a seven, KEEP"*. The model shipped it, reason given in its PLAN: *"2 lands but cannot
  cast my cheapest spell"* — exactly the thought the guide names and rejects.
- **seq 3 (mulligan #2)**: 7 cards, 3 lands + Wall of Omens, Exquisite Blood, 2 Lanterns; coverage
  line **named two cards** (`would cover the cost of: Chromatic Lantern, Chromatic Lantern`). The
  guide's first bullet is *"Two or more lands and a coverage line that names even one card: KEEP, at
  any price."* The model shipped again, to five, reasoning *"zero white mana"*.
- seq 4/5 kept and bottomed at five. The seat then had one creature on the board by T8, blocked once
  (seq 13) and died on T11.
Classification: **STRATEGY** on both. The surfaces were true, complete and matched the guide's own
trigger wording; the model read them and decided against the guide.

---

## 2. Engine / interface / card items

### HIGH — the actor label on a `targetedplayer`-granted ability is INVERTED in the narration (5 of 5 games)
Repro: `1788653525…vs-deck162`, the longest prompt, turn-9 block of the GAME LOG:
```
- You cast Tribute to Hunger (that Tribute to Hunger was 1 of 2 copies in your hand; …)
- Your Tribute to Hunger resolved and went to your graveyard
- Opponent used: Gain life equal to its toughness with Tribute to Hunger targeting Master of the Feast
- Opponent's Master of the Feast died
- You gained 5 life (now 20)
```
The reciprocal error is in `1788653519…vs-deck123` at turn 8: the OPPONENT casts Tribute to Hunger
and the log reads `You used: Gain life equal to its toughness with Tribute to Hunger targeting Pride
Guardian` followed by `Opponent gained 3 life (now 23)`.
Both directions appear in every game where a Tribute to Hunger resolved (deck123, deck162, deck146,
deck125, deck130 seat files — 8 occurrences total).
Cause is visible in the primitive (mtg.txt:124075): the granted sacrifice ability rides
`… sacrifice!$ targetedplayer`, so the ability's controller is the SACRIFICING player, and the
narrator credits the ability's controller rather than the source spell's controller — while the
life actually accrues to the spell's controller. This is a narration that states the wrong actor for
a life-gain event in the one deck whose entire win condition is keyed on *who gains life* (Sanguine
Bond / Exquisite Blood). It is mitigated — the immediately following `You gained N life` /
`Opponent lost N` lines are correct, and the model recovered every time (its PLANs consistently
described the gain as its own) — but under the trust doctrine a rendered actor is an instruction,
and this one is false. The defect is not Tribute-specific: it will hit any `targetedplayer`- or
opponent-granted ability whose effect benefits the granting player.

### MED — two lines in the same blockers prompt give different post-combat life totals
Repro: `1788653551…vs-deck152` seq 13 (T9, Blockers). The prompt says:
```
INCOMING THIS COMBAT: 3 attackers, 10 unblocked damage - you would be at 1 … - best case with
every blocker assigned: you would be at 7 - one legal assignment that reaches it, …:
Pride Guardian blocks Luminarch Aspirant #1; 1 of those blockers die
…
BLOCKING THIS COMBAT: each of your 1 blocker that blocks gains you 3 - up to 3 life for you
```
The "best case … you would be at 7" figure excludes the block-trigger life gain that the very next
line advertises. The seat took that assignment and finished the combat at **10**, not 7 (`my_life`
on the following record). The summed trigger price (lane AF) is computed and printed, but the
best-case life projection is not fed from it, so the two numbers disagree by exactly the trigger.
Concrete fix shape: add the summed trigger gain into the best-case total, or state the total as
"…at 7 before your blocking triggers (+3)".

### MED — the survivable-block suggestion and the header above it give opposite advice
Same prompt, same seq. The header reads `NOT lethal: block only where the trade favors you`, and
three lines above it the engine's own suggested assignment is a **chump block that kills the sole
blocker** (`Pride Guardian blocks Luminarch Aspirant #1` against a 6/6; the per-attacker line for
that pairing reads `your blocker dies, attacker lives`, while A2 and A3 both read `neither dies`).
The suggestion is life-optimal for this combat and the material clause is present in the per-row
annotations, so it is not wrong — but a reader told "block only where the trade favors you" and then
handed a suggested assignment that loses a permanent has two authorities in one window. The seat
followed the suggestion, lost its only blocker, had nothing to block with on T10 and died on T11.
This is the one place in deck 126's corpus where the interface plausibly steered a deciding decision.

### LOW — no way to tell, from the ask, which permanent an opponent's removal just took
Repro: `1788653534…vs-deck125` seq 14 (T10, 827 s latency). The window is
`CHOOSE A CARD TO PUT ONTO THE BATTLEFIELD: Path to Exile's effect …` with three basic lands. The
ask itself never says a creature of yours was exiled to produce it; the model must infer the whole
context from the GAME LOG. The guidance line ("with basic lands, the colour your hand and your board
are shortest of") is good and the option rows are clean; one clause naming the exiled permanent
would make it self-contained.

### LOW — single-decision latency outliers
`1788653534…vs-deck125` seq 14 = 827 s; `1788653532…vs-deck146` seq 16 = 618 s; two opening-hand
asks at 233 s and 203 s. Seat medians were 8-54 s. Flagged for the engine seat, not diagnosed here.

### Things that WORKED and should not be re-litigated (deck-126 evidence)
- **LOOP HALF PENDING** (`1788653538…vs-deck130` seq 21): rendered exactly when Sanguine Bond was on
  the battlefield and Exquisite Blood in hand, including the honest negative
  (`You CANNOT cast Exquisite Blood in this window … its cost is {4}{b}; your untapped sources: 1`).
  This is the single best-targeted clause this deck sees.
- **LIFE-TO-DAMAGE CONVERTER** clause: present and correctly one-sided in G1 and G5.
- **Per-attacker blocking-trigger prices** (`{blocking trigger, this combat: you gain 3}`) matched the
  primitive (`Pride Guardian … life:3 controller`, mtg.txt:88358) exactly.
- **Silverquill Silencer per-cast tax annotation** (G3 seq 11/13/16): accurate, and the model's PLAN
  cited it — a surface that demonstrably changed reasoning.
- **CRACK-BACK unblockable breakdown** in G1 seq 18 (`nothing you control can legally block`) was
  true: all 22 attackers had flying, all three defenders were ground creatures.
- **Battlement `VARIABLE output` source annotation** appeared in every window that listed it; no
  wasted upkeep Battlement tap occurred in any of the six games (guide RULE #1 held).

---

## 3. Guide verdict: **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck126_strategy.txt`, 70,992 bytes (at the top of the 41-71 KB band).
No `strategy.txt` written.

Why KEEP is the right verdict here, not a default:
- Every rule the guide owns was **followed** in the corpus. RULE #0 (play the land every turn): no
  land drop was declined in any of the six games; the two turns with no land played (G5 T7, T9) had
  `Land drop: you have no land you could play right now`. RULE #1 (pass the upkeep, never hand-tap
  the Battlement): zero Battlement taps outside a same-window cast across 209 records. RULE #5
  (blocking): all four blocker declarations took the engine's assignment. Cleanup discard: G2 seq 25
  reproduced the guide's send-order exactly, and the result was correct play (Legendary Sorin).
  Staff/Sorin section: G4 seq 13/17/21 took the Sorin +1 and the -2 emblem as taught.
- The five losses trace to (a) hands and mana — G3 drew no wall between T1 and T14, G5 missed two
  land drops; (b) matchups a 5-mana two-card combo behind 0/3 and 0/4 walls simply loses to — G1's
  22 flying tokens in a single activation, G2's 4-damage-per-turn-cycle draw-ping engine that walls
  cannot interact with, G4's four counterspells on the Tutor. None of those is a guide teach.
- The one genuine misplay pair (G6 seq 2 and seq 3, mulliganing a 2-land seven and then a 3-land
  seven whose coverage line named two cards) is **already covered verbatim** by the guide's mulligan
  bullets — including the exact reasoning the model used and the guide's rejection of it. Rewriting
  correct text that the model ignored is the wave-58 busywork pattern; it would add bytes to a guide
  already at the band ceiling and change nothing about the failure.
- What that failure IS worth reporting, as information for the core loop rather than as a guide
  edit: the mulligan rules the model violated sit at roughly line 660 of a 71 KB guide (the section
  starts at line 639 of 730), and the two decisions that ignored them are the *first two decisions of
  the game*. Whether an instruction that far into a band-ceiling guide is read at the pre-game seam
  is a question about guide length/placement and prompt assembly, not about this guide's wording; the
  synthesis seat and the engine seat are better placed to judge it than a per-deck edit is.

## 4. No proposals filed
No `general-proposals.md` and no `skill-proposals.md`. The mulligan-adherence observation above is
the only candidate, and it is an assembly/placement question I have no cross-deck evidence for —
one deck's two decisions does not pay for a general-guide or skill amendment, and a proposal that
restated the existing mulligan doctrine would be noise against amendments 1-331.

## 5. What I did NOT check
- I did not read the opponents' seat translogs except through this seat's GAME LOG narration; the
  opponent-side view of the G1 Intruder Alarm loop and the G4 counterspell windows is unexamined.
- I did not open the two HUNG games or the reruns in `matchups-20260905-214946` /
  `matchups-20260905-215345` — deck 126 is in neither pairing.
- I did not adjudicate any lane-AB/AC/AD/AE/AF prediction; the §2 "worked" list is incidental
  deck-126 evidence, not a verdict, and the engine seat owns the counts.
- I did not check `softlock-blockers-1788650768.md` against this deck: no seat game shows a Blockers
  seam with a calm stack and no advance — the three blocker asks all resolved and the three games
  with zero blocker asks had no legal block (G1: all flying; G2 and G4: no attack was declared into
  the wall board in a window this seat saw).
- I verified Pride Guardian, Perimeter Captain, Overgrown Battlement, Wall of Omens, Sanguine Bond,
  Exquisite Blood, Tribute to Hunger, Staff of Nin, Idyllic Tutor and Sorin against the primitives
  (`mtg.txt`, `planeswalkers.txt`) only — I did **not** cross-check any of them against Scryfall, so
  the §2 HIGH item's cause is read off the engine script, not off Oracle text.
- I did not measure hold-row prefix-match census, `async_drops`, transport failures or wall misses;
  those are the engine seat's counts and I report only the per-game gameend fields quoted above.
