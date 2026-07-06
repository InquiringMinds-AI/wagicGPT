# Findings from the deck44 work — beyond the guide / skill / general-guide suggestions

Catch-all for things I observed reviewing deck44's decklist and 6 self-play games that
don't belong in a strategy artifact. All items cite the specific log evidence.

---

## 1. ENGINE/REPRESENTATION BUG (highest value): a targeted spell with NO legal target is offered as castable, and qwen loops on it

In game `1783311789`, the opponent's only creature was **Icehide Golem**, which the
engine types as a **"Snow Artifact Creature"** (verified in the set file). **Go for the
Throat** reads "Destroy target NONARTIFACT creature" — so it had **no legal target** on
that board. Yet across decisions **15, 16, 17, 18** (and again 20-23) the casting seam
kept offering `4. Cast Go for the Throat {{1}{b}}`, qwen kept picking it (`choice=4`),
and it **never resolved** — Go for the Throat stayed in hand every single time, the
option kept reappearing, and the "Events since your last decision" block was empty
between those records. That is a stuck **re-ask loop**: the model spends decision after
decision (each an inference call) on an action the engine cannot complete, and the game
state does not advance.

Two distinct problems, both worth fixing:
- **Engine/seam:** the `FindCardToPlay`/casting seam offers a targeted spell whose
  only candidate targets are all illegal (here: all opponent creatures are artifacts vs
  a nonartifact-only removal spell). The legal-action list should exclude a targeted
  spell when it has zero legal targets — otherwise the model is handed a phantom option.
  This mirrors the project's own principle that "illegal choices are structurally
  impossible (options come from the engine's legal set)"; targeting legality isn't being
  applied at the cast-offer stage.
- **qwen behavior:** even granting the bad option, qwen **fixates** — it re-selects the
  same non-resolving action instead of falling through to "Cast nothing." A weak model
  will not self-correct out of an offered-but-inert option; the engine has to not offer
  it. (This also inflates inference cost and can stall a game.)

This is the most actionable non-strategy finding: it's a correctness bug, it wastes
inference, and it silently corrupts any win-rate/decision-quality signal from the
harness (many "decisions" are the same stuck no-op).

## 2. Baka's internal heuristic score LEAKS into the GPT option label

Game `1783314193`, decision 9 (a `priority` kind), the option shown to qwen was:
`1. Untap with Sleep-Cursed Faerie [cost: {1}{u}] (heuristic score 0)`.

The `(heuristic score 0)` is the fallback Baka AI's internal ranking score bleeding into
the prompt the GPT pilot sees. This is a representation leak: qwen shouldn't see an
internal engine number, and "score 0" is actively misleading — it reads as "this action
is worth nothing." It appeared once here only because `priority`-kind decisions are rare
in these logs, but it will recur on every activated-ability priority seam. Strip the
heuristic-score annotation from labels rendered to the GPT model.

## 3. What the representation gets RIGHT (keep it)

Counters and tap state are surfaced well. After Sleep-Cursed Faerie was cast it showed
as `Sleep-Cursed Faerie {{u}} (3/3) [counters: 2x stun] [tapped]` — qwen can see it's
stunned and can't attack yet. Rakdos Cackler showed `(2/2) (printed 1/1) [counters: 1x
+1/+1]` — current vs printed P/T both visible. Faerie Bladecrafter showed its grown
`(3/3) (printed 2/2)`. This current-state rendering is good and load-bearing for correct
play; don't regress it. The "(printed X/Y)" annotation is helpful, not noise.

## 4. qwen behavior patterns worth knowing (deck-agnostic)

- **Passivity is the dominant failure** (covered in the general-guide suggestions):
  full grips of interaction, empty board, repeated "Cast nothing."
- **Fixation/looping** on a re-offered inert action (finding #1) — qwen does not
  self-correct out of a bad-but-legal-looking option; the fix must be upstream.
- **Basic mechanics are fine:** it makes its land drop nearly every turn, and when it
  has creatures available to attack it does attack with them (in game `1783311796` it
  swung Faerie Miscreant every turn and correctly only had summoning-sick creatures
  held back). Attacking and land-sequencing are not where the losses come from.
- **Reactive counterspell use was essentially never exercised** in these logs — almost
  every decision was an own-turn `ask`; I saw only one `priority` seam total across 6
  games. If reactive interaction (countering on the opponent's turn) is a design goal,
  the harness/logs barely surface those seams, so it's untested here — worth checking
  whether the opponent-turn interaction windows are firing for the GPT pilot at all.

## 5. Minor / cosmetic
- Mana costs render with doubled braces and lowercase, e.g. `{{u}{u}}` / `{{1}{b}}`.
  Cosmetic; didn't appear to confuse qwen, but single-brace `{U}{U}` would match the
  rules-preamble convention the same prompt uses ("costs are written like {2}{R}").
- `Mana in your pool:` is usually blank (only populated mid-cast). Fine as-is.

---

**Bottom line:** finding #1 (phantom cast option for a no-legal-target spell, causing a
decision loop) is a real engine correctness bug that also pollutes the harness's
decision signal, and finding #2 (heuristic-score leak into GPT labels) is a quick
representation cleanup. The rest of the representation is solid.
