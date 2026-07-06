# Additional findings from deck131 work (catch-all)

Observations beyond the strategy guide, the skill, and the general-guide suggestions. Each cites what was seen in the pool7 self-play logs (`~/.Wagic/ai/gpt/selfplay-runs/pool7-20260705-232254/*ai_baka_deck131*`).
(Persisted by the orchestrator from the agent's returned text — its Write tool hit a report-file guard.)

## A. (Most useful) The legal-action list offers reactive spells that have NO legal target
Root cause behind the whole "fires reactive spells into nothing" class of misplays — fixable at the ENGINE level, more reliable than teaching a weak model to avoid it. INDEPENDENTLY CORROBORATED by the deck44 agent (Go for the Throat offered vs an all-artifact board).
- Game `1783313459`, decision 21: opponent's board was ALL lands (no creatures), yet **"Cast Downsize"** (target = a creature you don't control) was offered as legal. qwen took it and wasted the card — no legal target exists.
- Same game, decision 42: at the opponent's **end step with an empty stack**, **"Cast Essence Scatter"** (counter target creature spell) was offered. qwen cast it into nothing.
The offered set is "legal to begin casting" but includes spells that fizzle for lack of a target; a weak pilot reads "on the list" as "reasonable play." **Recommendation:** filter the casting seam so a targeted spell is offered only when at least one legal target exists (counter only when a counterable spell is on the stack; "target creature you don't control" only when one exists). If full filtering is hard, annotate the option (`Cast Downsize {U}  (no legal target right now)`).

## B. Modal / "choose one" menus strip the reason for the choice — qwen defaults to a fixed position
The Prism Ring color menu rendered as bare labels (`1. green / 2. blue / 3. red / 4. black / 5. white`) with nothing stating what the choice does. **qwen chose "3. red" in all five games it saw the menu** — a heavy-blue deck where blue was correct every time. Order was identical each game, so red is always option 3; a consistent middle-option pick looks like **positional anchoring, not color reasoning.** Fixes: (1) menus should carry a one-line reminder of the ability (header "Choose Prism Ring's color — you gain 1 life whenever you cast a spell of that color:"); (2) cross-deck behavior to know: on a contextless menu qwen falls to a fixed positional default.

## C. Mana-cost formatting in options doesn't match the format the prompt teaches
Option lines show costs as `{{1}{u}}`, `{{2}{r}}`, `{{u}}` — **double braces, lowercase** (`Cast Young Pyromancer {{1}{r}} (2/1)`). The system prompt teaches `{2}{R}` — single brace, uppercase. Double brace is a templating artifact (symbols already `{}`-wrapped, then re-wrapped). Cosmetic but a needless mismatch for a weak reader; normalize to the taught single-brace uppercase form.

## D. Event log spams routine phase transitions
Some "Events since your last decision" blocks list long runs of empty phase changes (game `1783313459` dec 21: "End / Cleanup / --- / Untap / Upkeep / Draw"). Low-information noise competing for a small model's attention; consider collapsing phase-only transitions and keeping board-changing events.

## E. Behavior patterns worth carrying forward (cross-deck)
- **Under-attacks with small creatures.** Game `1783313459` repeatedly declared no attackers while holding Young Pyromancer + tokens; stalled at parity ~20 turns. The one game it pressured (`1783314186`), token + Guttersnipe attacks took the opponent 16→6.
- **Durdles at parity.** Several games sat at even life for many turns casting nothing — it doesn't distinguish "nothing is happening and that's fine for me" from "…and I'm the one who needs to act."

## F. Minor translog note
For `kind=attackers`/`blockers` records, the logged `choice` field is not an option index — it appears to be a parsed count/selection artifact (reply `A1, A2, A3` logged `choice=3`). Analysis scripts should read `reply`, not `choice`.

**Priority:** A is high-value (source-level fix removes a misplay class); B and C are cheap representation cleanups; D–F are minor.
