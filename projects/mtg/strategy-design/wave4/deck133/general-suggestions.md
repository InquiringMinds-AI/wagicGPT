# Core-prompt suggestions from the deck133 wave-4 review

Evidence: 12 games (runs `matchups-20260712-192832`, `matchups-20260712-230636`; 3/12,
3 timeouts all winning-but-slow). Audited against the LIVE
`bin/Res/ai/gpt/system_prompt.txt`. The wave3 batch (affordability, hold-limits,
"that time is NOW", cost-promptness) is now IN the live core and this corpus shows it
WORKING — see the audit table at the end. Proposals below are only where the residual
defect is general. Format: quote → replacement/addition → why → layer verdict.

---

## 1. Complete the plan-execution anchor: cover the NOT-LISTED branch (ship core NOW)

**Current line (end of the costs paragraph):** "…and your chosen number must EXECUTE
your own plan: if your plan says to cast or activate something that is in the list,
pick that number; never describe an action and then choose 'Cast nothing' or pass."

**Gap.** This anchors the case where the planned card IS listed. The failure I
observed is the complement: the pilot plans a card that is NOT in the list (not yet
affordable), and with its plan unexecutable it defaults to "Cast nothing" instead of
re-planning from what is offered.

**Proposed addition (append to that sentence):** "And write your plan FROM the list:
before naming a card in your plan, check it appears in the numbered choices. If the
card you wanted is not listed, it is not available right now - re-plan from the cards
that ARE listed and pick the best of them; a missing card is never a reason to choose
'Cast nothing'."

**Why.** vs135 run2 (1783918306) seq30 (T11) and seq31 (T12): reply text is "PLAN:
Cast Gray Merchant of Asphodel immediately" while the chosen action is "Cast nothing
right now" - Merchant costs 5, the pilot had 4 lands, so it was never offered. Two
consecutive turns lost in a game it lost at -9. Same shape milder elsewhere (plans
naming Obliterator on 3 lands).

**Layer: CORE, NOW.** Argument for shipping on one game's evidence: (a) the failure
is mechanical, not strategic - a weak executor with an unexecutable plan needs a
stated fallback, and every deck's pilot writes plans; (b) it is a one-sentence
COMPLETION of an existing core line, not a new concept, so the attention cost is
near zero and it cannot conflict with any guide; (c) the deck-side copy already
shipped in wave4/deck133/strategy.txt - if the core takes it, deck guides can drop
it later (the wave3 promotion pattern). If the synthesis agent wants more evidence,
check other decks' corpora for plan-names-unlisted-card + chose-nothing pairs - it
is grep-able (reply PLAN card name absent from the prompt's numbered lines).

## 2. Make the beatdown decision board-readable: add the damage-flow test (ship core; keep it minimal)

**Current line:** "Decide who is the beatdown. Compare clocks: how many turns of
attacks each side needs to kill the other. …If you cannot clearly tell who is the
beatdown, be the aggressor by default…"

**Problem.** "Compare clocks" asks the executor for a two-sided projection it never
performs; in practice the tie-breaker ("aggressor by default") always wins. deck133
stayed aggressor through both vs44 Faerie games while flat-out losing the race - its
attacks never connected (ground 2/1s into flying blockers; opponent ended BOTH games
at 20 life) while their flyers killed it. The clock comparison was decided by facts
sitting in its own prompt, but the line gave no board-level test to read them with.

**Proposed addition (insert after "This role can flip mid-game."):** "Read the race
off the last few turns, not from theory: if THEIR attacks are landing on you and
YOURS are not (your attackers get blocked or cannot get through, your life is
falling while theirs is not), you are LOSING the race - switch to defense NOW:
remove their best attacker, keep blockers home, stop paying life, and win through
your deck's non-combat route if it has one. If your damage is landing and theirs is
not, you are winning it - keep attacking."

**Why.** The trigger facts (life trajectories, attacks blocked/unblocked) are in
every prompt's narration and board snapshot - this is the executable form of the
clock comparison. My per-deck ROLE CHECK instantiates the same test with named cards
(Obliterator wall etc.); the core can only carry the deck-agnostic test + posture,
and that split is right. On deck131's reviewer's runnable blocking checks: my
corpus neither adds nor contradicts blocking-decision evidence (deck133 logged ONE
blockers decision - its main creature literally can't block); what my evidence
does support is the upstream half - the pilot must first NOTICE it is defending,
or no blocking rule ever fires. The damage-flow test is that notice. I'd ship the
posture test core-wide and leave per-blocking-decision checks to decks with real
blocker data.

**Layer: CORE** (the test), **PER-DECK** (the named-card defensive line), as split
above. Cost: ~4 lines, symmetric, and it tightens an existing principle rather than
adding a new one.

## 3. Winning-but-slow / pressing a won position: NOT a core-prompt problem

**Candidate examined and rejected for the core.** All three deck133 timeouts were
the pilot AHEAD at the cap (opp at 2, 5, and -10). I looked for a core line that
would have converted them to wins:
- vs140 run2 (1783921425): opponent at **-10 life** from seq58's Gray Merchant
  drain, game continued to seq61, scored `timeout`. No prompt text presses a
  position the ENGINE has already failed to adjudicate - a player at <=0 life must
  lose to state-based actions. **ENGINE bug, highest priority of the three.**
- vs140 run1 / vs110 run1: pilot attacking with everything, correct lethal
  declarations, opponent surviving on lifegain/removal while the wall clock ran
  out on two seats' inference (median 8-11 s/decision × 40-60 decisions/seat).
  **HARNESS economics** (cap vs latency), plus opponent Elixir-of-Immortality
  grind. The core prompt already orders "run the lethal check TWICE every turn" and
  the pilot was doing it.
**Layer: ENGINE/HARNESS.** Asks: (a) fix negative-life adjudication (or find why
Elixir/replacement effects mask it); (b) results.tsv timeout rows should carry
final life totals so winning-but-slow is visible without opening translogs; (c) the
repeated single-option priority ask (wave3 #4, still live: 62 priority records in
1783922461, 36 of them bare "pass") remains the biggest decision-count multiplier -
a within-turn decline memo would shorten every grindy game at zero play-quality
cost.

## 4. Audit of existing core lines across my 12 games (keep-list + no dead weight found)

| Core line | Verdict from deck133's corpus |
|---|---|
| Affordability / auto-tap + "every listed choice is payable" | **FOLLOWED - the wave's headline fix.** Cast-nothing 75%→36%, casts ~25→98. Residual "no mana" phrasing in replies is now mostly attached to correct passes (in 1783906271, 8 of 18 such replies still cast anyway). Keep verbatim. |
| "Your own turn is for ACTING" + cast-nothing conditions (a)/(b) | **FOLLOWED, including the exception**: the 32 cast-nothings in 1783922461 were condition (a) - hand was targetless removal vs a creatureless board. The line's structure (default + enumerated outs) is doing real work. |
| Hold-limits ("holding a card must never hold your TURN") | **FOLLOWED.** No wave3-style Fatal-Push-freezes-the-turn games. |
| "Choose by IMPACT" targeting | **FOLLOWED** (Searing Spear vs burn, Cranial Plating vs affinity, Counterspell vs control taken correctly). Keep verbatim. |
| Combat free-case + lethal check | **FOLLOWED** (multi-attacker lethal sums correct in plans; free-case attacks declared). |
| "that time is NOW" / stale-plan | **PARTIAL**: deferral loops much milder than wave3 but plan staleness still echoes; the finding-1 addition attacks the sharper residual (plan vs list divergence). |
| Beatdown/clock comparison | **NOT EXECUTED as written** - see proposal 2. |
| Mulligan (2-5 lands + castable spell) | **FOLLOWED**; the one bad keep (all-discard six, 1783918460) passes the core rule and is fixed per-deck. |
| Burn-at-face / X-spells / flash lines | **NOT EXERCISED by this deck** (no burn, no X, no flash) - no opinion on dead weight from this seat; defer to decks that exercise them. |

No line in the live core misled this pilot this wave. The two proposals above are a
completion (1) and an operationalization (2) of lines that exist, not new doctrine.
