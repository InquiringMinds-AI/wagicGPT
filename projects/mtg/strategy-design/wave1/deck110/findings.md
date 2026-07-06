# Additional findings from deck110 (beyond the guide, skill, and general-guide suggestions)

Concrete observations from the deck110 self-play logs
(`selfplay-runs/pool7-20260705-232254/*-ai_baka_deck110-*.jsonl`). These are
representation / engine / harness issues, not strategy-guide content — a
per-deck guide cannot fully paper over them, so they are worth fixing at the
source.

## 1. Equipment ATTACHMENT is invisible in the board state (highest-value fix)

The board line lists Cranial Plating as a standalone permanent and never shows
which creature it is attached to. Example (game 5, rec 27):

    Your battlefield: Darksteel Citadel; Signal Pest (0/1); Glimmervoid;
    Cranial Plating {{2}}; Ornithopter (4/2) (printed 0/2); Mountain; Island

Ornithopter reads 4/2 vs printed 0/2, so the buff IS there — but the model must
INFER that the +4 comes from the Plating, and that the Plating is already
equipped. When there are TWO Platings (game 2 rec 24/26 had two, then three on
the battlefield), the model literally cannot tell the equipped copy from a
spare, unattached one. This almost certainly fed the observed "cast a 2nd/3rd
Plating and never notice one is already working" flooding.

The general prompt even promises this: it says the state includes "anything
attached to it." It does not. FIX: render attachment, e.g.
`Ornithopter (4/2) (printed 0/2) [equipped: Cranial Plating]` and/or
`Cranial Plating {{2}} [attached to Ornithopter]`. This is the single most
useful engine/representation change I'd recommend from this deck.

## 2. Metalcraft / artifact-count is never surfaced

Nothing in the state tells the model how many artifacts it controls or whether
metalcraft (3+ artifacts) is ON, and no permanent is labeled as an artifact.
Yet for this deck metalcraft silently changes: Galvanic Blast (2 vs 4 damage),
Etched Champion (protection from all colors on/off), and Mox Opal (mana on/off).
A weak model will not reliably re-count artifacts across a long battlefield line
every decision. Etched Champion is shown only as `(2/2)` / `(7/2)` with no
indication it currently has protection from all colors (why it's unblockable/
unkillable). This is a class of problem for any threshold/state-triggered deck
(metalcraft, delirium, threshold, city's blessing, etc.).

FIX options: add a one-line "You control N artifacts (metalcraft: ON)" to the
state header, and/or annotate cards whose text is currently active. Cheapest
high-value version is just the artifact count line — it directly feeds burn
targeting and combat math.

## 3. Internal heuristic scores leak into the model-facing action list

Every activated-ability option carries the Baka heuristic's debug score, e.g.
(game 4, rec 20):

    1. Equip with Cranial Plating targeting Etched Champion [cost: {1}] (heuristic score 100)
    2. Equip with Cranial Plating targeting Ornithopter [cost: {1}] (heuristic score 20)

And elsewhere the SAME equip is offered as `(heuristic score 0)`. This is an
internal signal from the fallback AI bleeding into the LLM prompt. Two problems:
(a) it can actively mislead — a great equip shown as "score 0" reads as "don't
bother," and I saw qwen pass (choose 0) on equips labeled score 0 while taking
the score-100 one; the model may be anchoring on this number instead of the
board; (b) it's noise that shouldn't be part of the decision surface at all.
FIX: strip `(heuristic score N)` from the GPT-facing action rendering (keep it
in the translog/debug path if useful). Note: this appears only on the
"priority"/activated-ability seam, not on casting/attack/block prompts.

## 4. The equip action IS otherwise well-presented

Positive finding: on the priority seam the equip reads clearly and is
distinguishable from casting — "Equip with Cranial Plating targeting X [cost:
{1}]", one option per legal target, with `0 = pass`. The label names the
equipment, the target, and the cost. Aside from the score leak (#3), this seam's
wording is good and did not need guide compensation. The real reason qwen missed
equips in the losing games was upstream: it tapped out, so the equip ability was
never a legal action (no priority window offering it appeared in games 2 or 6 at
all).

## 5. qwen behavioral patterns worth noting for the eval loop

- Strong bias toward passing / low option numbers. Across all six games many
  decisions were "cast nothing" (1) or pass (0); when it did equip it took the
  first-listed target. Can't separate genuine judgment from positional/first-
  option bias from these logs alone — worth a controlled check (shuffle option
  order, see if choice distribution follows position).
- It never once cast Thoughtcast, Master of Etherium, or activated Steel
  Overseer in the sampled games (they rarely reached hand/board, but when a
  Steel Overseer or refuel opportunity existed it wasn't used). The deck's
  engine cards are systematically under-used relative to its cheap beaters.
- Land drops are always taken (chose "Play" every time) — the separate
  "Land drop: play X now?" ask seems low-value; qwen never holds a land, so
  this seam mostly spends a round-trip. Could be auto-resolved unless holding
  the land is meaningful (e.g. it enables nothing this turn).

## 6. Minor / harness

- The first two decisions of every game render as an "ask" with no clear
  question (my parser saw no question line); these look like mulligan-keep
  decisions and qwen always chose 1. Worth confirming the mulligan prompt is
  well-formed, since it's the same seam for every deck.
- Game outcomes are not in the translog (known — the harness notes win/loss is
  unlogged). For deck-guide A/B work, wiring `gameFinished` win/loss into the
  per-deck logs would let this exact analysis (winning vs losing game deltas,
  the method my skill.md leans on) be done by outcome instead of by inference.
