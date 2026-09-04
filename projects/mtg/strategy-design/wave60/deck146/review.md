# wave60 / deck146 (Orzhov Dungeons) — per-deck review

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260904-171140/`, binary master 614f8452e.
Record **5-1**. 308 decisions across the six seats, **0 fallbacks**, 0 re-asks logged, 0 hung/crashed.
Seat files below are named by their `-vs-` suffix.

## 1. Game by game

| opp | result | turns | final life | deciding decisions |
|---|---|---|---|---|
| 162 | WIN | 15 | 7/0 | seq 13/19 Vanishing Verse on Fate Unraveler then Underworld Dreams (their drain engines) before either resolved into a race; seq 32 Soul Shatter clearing Fog Bank the turn before the alpha at seq 38 (5 attackers). |
| 130 | WIN | 21 | 9/-1 | seq 13 Verse on Siege-Gang Commander; seq 16/17 Silverquill Command as pump+edict on a lone Silencer that then carried every turn (seqs 19,21,24,34); seq 29/30 Lolth -3 spiders held the ground at 9 life. |
| 123 | WIN | 15 | 22/0 | seq 17 Silverquill Command "returns NOTHING" annotation read correctly, mode taken as draw+edict; seq 23/24/25 second Command rebuying Triumphant Adventurer from the yard; never fell below 20 life. |
| 126 | WIN | 15 | 23/-1 | mulligan to 6 (seq 1-3, bottomed Lolth on a 2-land keep); seq 11 Verse on Pride Guardian to unlock the ground; dungeon Goblins widened the attack from 2 to 6 bodies by T15. |
| 125 | WIN | 27 | 24/0 | seq 16/17 Pelakka Predation stripping Supreme Verdict out of a control hand; seq 77/105 animating Hive of the Eye Tyrant as the closer; seq 102 Verse on Lightmine Field to unlock the alpha at seq 109. |
| **152** | **LOSS** | 24 | -14/31 | **seq 37 (T19)**: Kaya -3 exiled **Katilda, Dawnhart Prime (2/2 lord)** while **Sigarda, Champion of Light (4/4 trample+flying)** was on the same -3 list and tapped. Sigarda grew to 6/6 and was the unanswerable clock. **seq 48 (T22)**: the render offered `A2 [GANG BLOCK: any 3 of yours together deal 5, enough to kill this attacker]` on Intrepid Adversary (7/5 lifelink) and the seat had exactly 3 blockers; it instead put 2 Spiders in front of a 6/6 trampler (saves 2 damage, loses 2 bodies) and chumped the lifelinker with the Goblin, handing them 7 life. seq 55 was already unwinnable (19 vs 5 life; the render correctly said "no block saves you"). |

Both losing decisions read a TRUE surface and chose against it: **STRATEGY**, not perception.
Hands were real (one mulligan across six games) and no game was decided by mana screw.

## 2. Engine / interface / card items

**HIGH — the Pelakka Predation reveal seam invites an illegal answer, silently truncates it, and then narrates the illegal version as fact.**
`1788559950-ai_baka_deck146-0x5628e72db3d0-vs-ai_baka_deck125.jsonl`, seq 18 (kind `reveal`, T9).
Card (primitive `borderline.txt:82651-82652`) is *choose exactly one*: "You may choose a card from it with mana value 3 or greater. That player discards that card."
Rendered protocol: `Decide, in ONE reply, which cards go to "choose a card"` … `On the FIRST line write PUT: followed by the card numbers you send to "choose a card", comma-separated (e.g. "PUT: 1, 3")`.
The model answered `PUT: 1, 3`. Narration at seq 19 then printed
`- You revealed 4 cards and took Supreme Verdict, Final Judgment (choose a card)`
while the engine actually took one: `- You used: choose a card with Pelakka Predation targeting Supreme Verdict` … `- Opponent put the revealed Final Judgment into their hand`.
Three defects in one seam: a multi-select UI on a choose-one effect, a silent drop of the surplus pick, and a narration line that states a discard that did not happen (the model carries that false fact forward in every later prompt). A choose-one reveal should render a single-pick ask; the arms-count is the fix, not the wording.

**HIGH — the dungeon-selection ask asks the model to weigh a number the render gets wrong.**
`1788559953-ai_baka_deck146-0x559eb93763d0-vs-ai_baka_deck126.jsonl`, seq 26 (also seq 10, 17; every deck146 seat).
Rendered: `Weigh how many rooms to completion …` then
`1. Tomb of Annihilation [dungeon: 5 rooms; …]` / `2. Dungeon of the Mad Mage [dungeon: 9 rooms; …]` / `3. Lost Mine of Phandelver [dungeon: 7 rooms; …]`.
The counts are the printed ROOM LIST length. The actual ventures-to-completion in the engine's own scripts are **4 / 7 / 4** (`borderline.txt` `hascntexplore~equalto~N` ladders: Tomb tops out at 4, Mad Mage at 7, Lost Mine at 4 — `borderline.txt:67547` is Lost Mine's `completedungeon` at explore 4). Confirmed live: seq 26's narration shows LMoP finishing on `venture step 4 of that run` and paying Temple of Dumathoin.
So the one quantity the prompt tells the model to decide on is false in scope for all three rows, and it inverts the ordering it matters for (Lost Mine reads as the second-longest and is joint-shortest). Render ventures-to-completion, not list length.

**HIGH — 22.7% of this deck's decisions are pure declines, at full prompt cost.**
70 of 308 deck146 decisions resolved to "Cast nothing" / "Hold priority" / "pass" / "no blockers". Worst run: `…-vs-ai_baka_deck125.jsonl` seqs 92→101 — ten consecutive asks, prompts 20.4–21.6 KB each (~210 KB of inference), across Blockers → Main 2 → Upkeep → Draw → Main 1 → Main 1, on a menu whose only two cast rows were Soul Shatter (annotated `they control 0 creatures or planeswalkers - at 0 this does nothing`) and Vanishing Verse (annotated `the only legal targets are YOUR OWN right now`). The "Hold priority for the rest of this turn" row is scoped to the current turn, so a turn boundary re-opens the identical dead menu. A hold that survives until a row's verdict actually changes (rather than until the turn ends) would remove most of this.

**MED — the "no live cast row" banner is dropped when a row is dead only because its sole targets are the model's own permanents.**
Same file: seq 92 carries `NO LIVE CAST ROW ON THIS MENU: all 1 cast row below carry a verdict computed from the board that reads zero`. Seq 93–100 carry **no banner** although both rows are dead — Soul Shatter still reads `at 0 this does nothing`, and Vanishing Verse reads `the only legal targets are YOUR OWN right now … (warning: this would harm your own side)`. The zero-verdict test does not count own-targets-only removal as reading zero, so the banner flickers off exactly when the menu got longer and more misleading. (34 banner renders across the six seats.)

**MED — the dungeon ask's worked example is malformed and instructs the model to copy a truncated name with an unbalanced quote.**
Same file, seq 26 (and 10, 17): `e.g. "CHOICE: 1 (Tomb of Annihilation - full room path: "Trapped )"`. The short-name extractor took the option line up to a fixed width instead of the name. It did not break parsing here (the model answered `CHOICE: 3 (Lost Mine of Phandelver)`), but it is a false instruction on a live surface.

**MED — the MDFC front face renders as "Cast Card Normally", losing the card name.**
`…-vs-ai_baka_deck125.jsonl` seq 17: `1. Cast Card Normally [cost: {2}{b}] {card text: "Target opponent reveals their hand…"}` against `2. Pelakka Caverns [PLAY THIS AS A LAND: …]`. The model echoed `CHOICE: 1 (Cast Card Normally)` and the narration recorded `- You chose Cast Card Normally for Pelakka Predation`. Row 2 names its face; row 1 does not. Same shape at `…-vs-ai_baka_deck152.jsonl` seq 42.

**MED — one activated ability with N legal targets emits its full `{card text: …}` blob N times.**
`…-vs-ai_baka_deck152.jsonl` seq 37: the legal-actions section is 6,390 bytes of a 24,282-byte prompt, 14 rows, and the Kaya `Put a ghostform counter…` blob appears **10 times**. The per-target rows differ only in the target name and the `[your battlefield]`/`[opponent's battlefield]` tag. Hoisting the ability's card text above its target rows would cut ~4 KB from a decision like this one without removing a fact.

**LOW — a revealed option can arrive with no rules text while its siblings have it.**
`…-vs-ai_baka_deck125.jsonl` seq 18: rows 1–3 carry `{text: …}`, row 4 is bare `4. Sphinx's Revelation (instant)`. A silent omission on a hand-disruption decision is exactly the gap the model confabulates into.

**LOW — Kaya's Spirit token is named "Spirit Cai".**
`planeswalkers.txt:1874` creates `token(Spirit Cai)`; the token is defined at `borderline.txt:107651` (1/1 white Spirit, flying — stats and keyword are correct, and Oracle "return it to its owner's hand and create a 1/1 white Spirit creature token with flying" is faithfully scripted). Only the NAME is engine-internal junk. It renders to the model as `Spirit Cai (1/1) [flying]` (`…-vs-ai_baka_deck152.jsonl` seq 37 battlefield line, and as attacker `Spirit Cai` at seqs 39/47/53). The model reasoned about it correctly, but it is an unknown card name on a true surface.

## 3. Guide verdict: **EDIT** (one line, paid for by seq 37)

The guide is otherwise working and I am changing nothing else. Every teach I traced was executed:
Lost Mine picked by name at every three-dungeon list; Acererak treated as a repeatable venture and never
mourned as a body (cast twice in the loss, bounced twice, exactly as the guide predicts); Vanishing Verse's
`legal targets right now:` list read before firing (never once cast at an own-target-only list); Silverquill
Command cast in Main 1 when the pump mode mattered; planeswalkers never left rotting in hand. 5-1 with zero
fallbacks and zero unparsed replies.

The one gap the corpus paid for: the Kaya bullet says "-3 their best threat" without saying what best means,
and at seq 37 the seat read "best" as "their lord".

before (deck146_strategy.txt, KAYA THE INEXORABLE bullet):
```
  answer to the boards Verse cannot fight. Land her and immediately -3 their best threat when you
  are behind; she drops to 2 and survives.
```
after (`wave60/deck146/strategy.txt`):
```
  answer to the boards Verse cannot fight. Land her and immediately -3 when you are behind; she
  drops to 2 and survives. THEIR BEST THREAT IS THE BIGGEST BODY THAT WILL CONNECT - the top of
  their power line, flier or trampler first - NOT their lord or anthem. This seat exiled a (2/2)
  Katilda while a trample-flying Sigarda sat on the same -3 list; Sigarda grew and killed it.
```
Paid for by `1788559941-ai_baka_deck146-0x561bb119c220-vs-ai_baka_deck152.jsonl` seq 37 (T19), the
decision that lost the only loss. 70,343 bytes (68.7 KiB), inside the 41-71 KB band.

I did NOT add a gang-block teach for seq 48: the general guide already owns block arithmetic, the
`GANG BLOCK:` clause is a render feature evaluated by the engine seat, and on that board no block line
survived the following turn either — one marginal decision is not enough to buy guide bytes.

## 4. Proposals

None. No general-strategy or skill proposal is supported by this deck's evidence — every finding above
is an engine/render item for the core loop, and the one guide gap was deck-specific.

## What I did NOT check

- The opponent seats' translogs beyond pulling context for the six deciding decisions; I did not audit
  152/125/130/126/123/162 play quality.
- Any lane-L…Q prediction adjudication (that is the engine seat's job) — I did not check whether my
  items overlap wave60/known-bugs.md or the lane briefs.
- Latency, `async_drops`, `transport` and HOLD prefix-match census (engine seat).
- Whether the Pelakka Predation multi-pick is reproducible outside this one window, or whether other
  choose-one reveal effects render the same multi-select protocol.
- Scryfall cross-checks: card facts above are read off the primitives and off the rendered prompts;
  I did not query Scryfall for any of them.
