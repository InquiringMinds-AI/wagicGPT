# Engine / integration / harness findings from the deck-110 review (wave 3)

Evidence base: deck110's 6 games in `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260709-220729/`
(196 logged decisions). Citations are `<epoch prefix>` + seq. Severity flags:
[GAME-LOSING] / [PACING] / [QUALITY-OF-REVIEW] / [ANNOYANCE].

---

## 1. [GAME-LOSING] "Mana in your pool: (none)" misleads the pilot; nothing states that listed actions are payable

**Observed.** The situation block always shows `Mana in your pool: (none)` (mana pools are
empty between costs — that's how Magic works), and qwen systematically reads it as "I have no
mana," declining listed, fully-payable casts: `1783657362` seq 3 ("I have no mana sources
available" with an untapped Island and `Cast Signal Pest {1}` listed), seq 6 ("my Mountain is
untapped but I have no way to produce Red mana"), `1783654819` seq 16 ("My current mana pool is
empty" with untapped Citadel + Mox Opal, passing a {1} equip). This drove 44–88% cast-nothing
rates and is my prime suspect for the corpus-wide 13/21 timeouts.

**Suggestion (representation, cheap).** Two changes to the situation block, either alone helps:
1. Reword the line: `Available mana: N untapped sources (mana pool empty — lands are tapped
   automatically when you pay costs)` — computing N (untapped lands + usable mana artifacts) is
   engine-side trivial and gives the model a number instead of a trap.
2. One static line above the options list: `Every action listed below is legal and payable right
   now.` This is cheaper than N and attacks the misconception directly. I've proposed the same
   fact as system-prompt text (see general-suggestions.md #1); stating it adjacent to the options
   at the decision site is likely stronger for a weak executor — worth an A/B.

**(d) experiment.** Would the untapped-lands count alone fix the mana delusion without any guide
text? Test: same corpus matchups, representation change only, guides unchanged, measure
cast-nothing rate. If it drops from ~70% to baseline, the whole class of guide/system-prompt
verbiage in #1 can be retired — representation fixes beat instruction fixes for interface facts.

## 2. [PACING] Decision economics: ~60–70s per LLM call × durdle-inflated decision counts = the 40-min cap

**Observed.** The loss (`1783657362`) is 36 decisions over ~40 min ≈ 67s/decision; other games
19–49 decisions. So the cap admits only ~35–50 decisions — roughly 12–17 turns. A deck that
plateaus (opponent life stuck at 9/10/16/17/18 in the five deck110 timeouts) mathematically
cannot finish. Timeouts are therefore mostly a downstream symptom of durdling (see #1), but the
harness ALSO spends calls on decisions that carry no information:

- **Duplicate consecutive asks, identical prompt state.** Mulligan asked twice back-to-back
  (`1783657362` seq 0–1, identical hand/options/reply). Casting menus re-asked with identical
  options within the same phase: seq 8/9/10 (T5, same three options, "Cast nothing" three times
  in a row), seq 6/7, 13/14, 15/16. Each duplicate is ~1 min of wall clock for zero new
  information. Suggestion: when the decision prompt (phase + board + options) is byte-identical
  to the immediately previous one and the reply was a pass/"Cast nothing," reuse the previous
  answer instead of re-calling the model. That alone reclaims several minutes per game.
- **Re-offered no-op activations** (see #5) similarly burn calls.

**Suggestion (harness).** Log per-decision latency (ms) in the translog so pacing claims like
this can be measured instead of inferred; consider a per-game decision budget alarm in the
selfplay harness (flag games that exceed N decisions by turn T — they're durdling, and the run
will time out).

## 3. [GAME-LOSING → engine fallback unclear] 16 decisions (8%) got an empty reply and `choice: -1`

**Observed.** 16 of deck110's 196 records have `reply: ""` and `choice: -1`, spread over 4 of 6
games, all kinds (`1783652860` seq 35/39/40; `1783654819` seq 0/10/11/12; `1783655260` seq
10/12/15/17/18/24/32 incl. two attackers declarations; `1783655261` seq 10/13). The mulligan
ask at `1783654819` seq 0 got no reply at all (the seq-1 retry answered).

**Why it matters.** Whatever the engine does on -1 (default option? pass? heuristic fallback?)
is silently substituting for the LLM ~8% of the time — including whole attack declarations. If
the fallback is "pass," it compounds the passivity diagnosis; either way it contaminates
guide evaluation (we attribute plays to qwen that qwen never made).

**Suggestion.** (a) Find the cause — per-call timeout? parse failure on a long reply? (several
replies are visibly truncated mid-sentence, e.g. `1783657362` seq 13, suggesting a token cap
that a stricter reply format would avoid). (b) Log the failure reason and the engine's
substituted action in the translog record. (c) On empty reply, retry once before falling back.

## 4. [QUALITY-OF-REVIEW] Plan/number mismatch — the reply protocol doesn't bind the pick to the plan

**Observed.** `1783654819` seq 6 and seq 7: PLAN text says "Cast Cranial Plating now to get it
on the battlefield, then equip it" — chosen option: "Cast nothing right now." Twice
consecutively. The model does the reasoning and then emits the wrong number.

**Suggestion (integration, cheap experiment).** Require the reply to echo the option text:
"reply with the number AND the exact text of the chosen line." Parse the number as now, but
when number and echoed text disagree, prefer the text (or re-ask). This mechanically binds the
pick to the intent and would also catch #3's truncation cases. Costs a few output tokens.

## 5. [PACING/POLISH] Engine re-offers no-op and duplicate ability lines

**Observed.**
- Equip re-offered on an already-equipped creature every turn, and qwen re-takes it:
  `1783652864` seq 13–20, Memnite already "(4/1) (printed 1/1)" yet "Equip with Cranial Plating
  targeting Memnite [cost: {1}]" keeps appearing and being paid — mana and wall-clock waste.
- Cranial Plating presents TWO lines for near-identical effects: `Equip ... [cost: {1}]` and
  `attach with Cranial Plating ... [cost: {b}{b}]` (the card's {B}{B} attach ability). A weak
  pilot can't tell the cheap line from the trap; in a deck with no black sources the {b}{b}
  line is pure noise.

**Suggestion.** Suppress (or annotate) activation lines that would be no-ops — re-attaching
equipment to its current bearer first among them. Where two lines differ only in cost, either
dedupe to the cheaper payable one or annotate ("already attached", "alternative cost"). Rules
purists can keep them behind a verbosity flag; the GPT pilot needs a clean menu.

## 6. [NEEDS INVESTIGATION] Blockers decisions nearly absent — is block-assignment reaching the GPT seam at all?

**Observed.** Deck110 logged ZERO `blockers` decisions in 6 games (kind tallies: ask/priority/
attackers only) while taking heavy damage (20→1 in the loss with creatures occasionally on
board). Corpus-wide the count is 3. Some of that is explained by boards (deck110 rarely had
untapped defenders), but 3-in-21-games corpus-wide is low enough to suspect the integration:
either the engine auto-resolves blocks in most situations without asking, or the blockers hook
rarely fires. Worth a targeted test (scripted game where blocking is clearly available) to
confirm the GPT player is actually being consulted. If blocks are silently heuristic-assigned,
all guide text about blocking is dead weight and defensive play can't improve.

## 7. [QUALITY-OF-REVIEW] Translog improvements that would have sped this review

The translogs are good — full prompt capture made confirm-before-accuse possible. Gaps:
- **No `chosen_text` field.** Every reviewer must regex the choice-index out of the prompt tail;
  attackers records make this worse because their reply protocol ("A1, A2") doesn't map to a
  single index — `choice` semantics differ by kind and are undocumented. Log the resolved
  action text(s) the engine actually executed.
- **No structured turn/phase fields** — parsed from prose. Add `turn`, `phase`, `my_life`,
  `opp_life` as JSON fields; end-state sweeps and durdle tallies become one-liners.
- **No latency / token counts** per call (see #2, #3).
- **No system-prompt/guide version stamp.** The corpus can't prove which guide text was in play;
  log a hash or copy of the composed system prompt once per game file header.
- **results.tsv** could carry final life totals and turn count — "timeout" hides how close the
  game was (deck110's timeouts ranged from dominating 20v9 to dying 5v18).
- Minor: the board label is `Opponent battlefield:` but the hand line is `Your hand:` /
  `Your battlefield:` — harmless, but one stale artifact appeared: a creature still tagged
  `[blocking Signal Pest]` in Main phase 2 after combat ended (`1783657362` seq 20 prompt).

## 8. [(d) MODEL] Weaknesses observed that guides can only partially patch — experiment ideas

- **Mana/color arithmetic hallucinations** (seq citations in #1) — the model invents constraints
  even with correct information present. Representation fix (#1) is the right lever; guide text
  is the fallback.
- **Plan/number binding** (#4) — format-level fix recommended.
- **Long rambling PLANs with self-argument** ("Mox Opal is an artifact land? No, wait..." —
  `1783654819` seq 9; mid-sentence truncations elsewhere). Longer replies = slower decisions
  (#2) and more truncation (#3). Experiment: cap/structure the PLAN ("one sentence, then your
  number") and measure both decision quality and pace; qwen's good decisions in this corpus did
  not correlate with long deliberation.
- **What the model does NOT need help with:** attack declaration logic (all 14 attacker
  decisions attacked correctly, citing battle cry/protection), carrier selection for equipment,
  and naming its deck's win plan — the strategic layer held; the mechanical layer failed. Model
  experiments should target interface grounding, not Magic knowledge.
