# wagicGPT

A fork of [Wagic, the Homebrew](https://github.com/WagicProject/wagic) that gives the
card game a **language-model opponent** — an AI that reads the board, plans, and explains
itself, in place of the engine's heuristic player.

> Status: **alpha, in development.** Playable on Linux/PC. Builds exist for Android, PSP
> and PS Vita. Not yet released.

## Why

Wagic is a 15-year-old C++ engine with something unusual at its core: a compact `auto=`
scripting language that expresses roughly 26,000 unique cards. The engine does not "know"
how to play Magic — it knows counters, zones, phases, triggers and costs. The card set is
*content*, interpreted at runtime.

That makes it an unusually good host for a language model. The model is not asked to
invent rules; it is handed a legal action set and asked which action to take. Every
decision it makes is a choice the engine already guarantees is legal — which means the
model can be wrong about *strategy*, but never about *rules*.

The long arc runs through original cards — our own designs, our own art, freely
distributable — toward an open platform for user-created, shareable card games. The LLM
opponent is what makes an engine like that worth playing without a human on the other
side.

## What's actually built

**An engine-owned decision layer.** The fork replaces ad-hoc UI-driven play with an
explicit contract:

- `LegalActionsOracle` — a pure legality oracle. What can be cast, what lands can be
  played, what can attack or block, what can respond at instant speed.
- `ManaEngine` — player-agnostic mana: potential mana, payment planning, auto-tap.
- `DecisionContract` — a `DecisionRequest → Action` API. The PC UI, the handheld UI and
  the AI policies are peer consumers of the same contract; none of them is privileged.
- Engine-owned priority, with auto-pass and stops: a window that offers no legal action
  is not presented at all.

**The language-model player** (`AIPlayerGPT`) overrides decision *seams*, not the game
loop: casting, targeting, modal choices, X announcement, attackers, blockers, damage
order, mulligans. It is built so that:

- Illegal choices are structurally impossible — options come from the engine's legal set.
- No model call is made when only one outcome exists.
- Any failure at any seam — unreachable endpoint, malformed reply, a platform that
  refuses to start a thread — falls back to the heuristic AI for that decision. The game
  never stops on the model.
- A whole decision is bundled into one ask where the engine would otherwise ask N times.
  Blocking assignments are one question with one answer, because sequential per-blocker
  asks cannot coordinate.

**Any OpenAI-compatible endpoint.** Local llama.cpp or Ollama, or a hosted provider.
Endpoint, model and key are configuration, not code.

## Platforms

| Platform | State |
|---|---|
| Linux / PC (SDL) | Primary development target |
| Android | Builds; bundled resource pack; JNI HTTP transport |
| Sony PSP | Builds and runs on hardware |
| PS Vita | Builds and runs on hardware |
| Windows | Planned |

## Testing

The engine carries a scripted regression suite of ~1030 fixtures plus AI-specific tests,
runnable headless:

```sh
cd projects/mtg/bin
WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 ./wagic
```

There is also a self-play harness for bulk AI-vs-AI games, and a parser/render self-test
corpus for the model's reply handling.

## Building

See [BUILDING.md](BUILDING.md), and [BUILD_VITA.md](BUILD_VITA.md) for the Vita.

## Cards and card images

This repository ships **no Wizards of the Coast card art**, and no release artifact
contains any. That restriction is about copyright, not about art — original cards with
original art are part of where this is going, and those are meant to ship with the game
and be freely distributable.

The card *scripts* — the `auto=` definitions the engine actually interprets — came from
upstream, and this fork extends and repairs them as first-class work. Two Oracle-drift
sweeps have corrected 217 cards whose scripted behaviour no longer matched their current
Oracle text, each with an accompanying parser fix, and cards continue to be scripted as
gaps surface. A card that plays wrongly is usually a data bug rather than an engine bug,
and is fixed in the script without recompiling.

Magic: The Gathering is a trademark of Wizards of the Coast. This project is not
affiliated with or endorsed by Wizards of the Coast.

## Credits

Wagic, the Homebrew is the work of the **[WagicProject](https://github.com/WagicProject/wagic)**
team and its long line of contributors. This fork exists because of theirs, and inherits
their engine, their card-scripting language, and their resource set.

The PS Vita port this fork builds on is the work of **[Brendonm17](https://github.com/Brendonm17/wagic-vita)**.

Community fixes adopted into this fork are credited individually in
[CREDITS.md](CREDITS.md).

## License

Same terms as upstream Wagic — see [LICENSE](LICENSE). Note that resource files
(graphics, sounds) do **not** follow the same license as the source code.
