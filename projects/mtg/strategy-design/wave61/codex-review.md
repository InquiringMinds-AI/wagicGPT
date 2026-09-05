# Wave-61 step-one adversarial review (Codex gpt-5.6-sol, read-only, on diff 718e99d2a..b6f60b47e; verdict: NOT safe to ship as-is)

# Adversarial findings

1. **HIGH — `projects/mtg/src/AllAbilities.cpp:1182`, `projects/mtg/src/AIPlayerGPT.cpp:35031`:** A bare one-target chooser is classified as optional solely because `targetMin` is false, allowing the model to decline choices that Oracle requires.  
   **Trigger:** Resolve Pelakka Predation against a hand containing a legal mana-value-3-or-greater card. Oracle says “You choose a card,” but the prompt explicitly offers `PUT: none`; the reveal driver then takes no card and the mandatory discard never occurs. PARSETEST at `AIPlayerGPT.cpp:54728` pins the wrong expectation that declining this Pelakka-shaped choice is legal.

2. **MED — `projects/mtg/src/AIPlayerGPT.cpp:19817`:** A HOLD remains active when rows disappear because `holdStillStands` checks only whether every current row belonged to the held set, contradicting the promised “any row change re-opens” rule and suppressing a changed legal menu.  
   **Trigger:** The model holds a menu containing legal actions A and B; an intervening state change makes A illegal while B’s rendered bytes remain identical. The current menu `{B}` is accepted as a subset of `{A,B}`, so B is auto-declined without asking even though the menu changed.

3. **MED — `projects/mtg/src/AIPlayerGPT.cpp:26867`:** The tapped-land classifier treats any `tap(noevent)` anywhere in a land’s script as an enters-tapped effect, so it emits false availability claims.  
   **Trigger:** Offer Bant Panorama as a land drop. Its script contains `tap(noevent)` only inside its activated fetch ability, for the fetched basic land, yet the Panorama row says it “enters TAPPED — it makes no mana this turn.” Blood Crypt is another counterexample: the `tap(noevent)` is one branch of its pay-2-life choice, but the row states unconditional tapped entry. The supplied PARSETEST cases cover only straightforward Arcane Sanctum/Isolated Chapel shapes.

4. **MED — `projects/mtg/src/AIPlayerGPT.cpp:11453`:** Timeout classification is decided from empty body plus elapsed time before considering successful transport/HTTP status, so a late HTTP error becomes `timeout` rather than `http_error` and receives the wrong retry budget.  
   **Trigger:** The endpoint returns HTTP 503 with curl success after 855 seconds of a 900-second deadline. `gptHttpPost` returns an empty body with `httpStatus=503`, then `timedOut` becomes true at the 95% threshold; `noAnswerClassFor` consequently selects `timeout`, and `pollCompletionRetry` grants a fresh full deadline instead of treating the completed HTTP response as an HTTP/transport failure. Scalar transport-phase PARSETESTs do not exercise this integration order.

5. **MED — `projects/mtg/src/AIPlayerGPT.cpp:1719`:** The damage-wipe roster equates `damage >= toughness` with death while considering only indestructible, so `DIES` and survivor-power claims are false under protection, prevention, regeneration, and replacement effects.  
   **Trigger:** Lightmine Field deals red damage to an attacking 2/2 with protection from red. The roster prints that the creature dies when the damage count reaches two, although the damage is prevented and it survives. PARSETEST constructs `WipeVictim` values directly and therefore cannot expose missing live abilities.

6. **MED — `projects/mtg/src/AIPlayerGPT.cpp:17574`:** `crackBackFloorSources` labels every non-triggered player-targeting damage script as presently usable without checking that the source is untapped, can untap, or that its activation cost is payable, then promotes the combat figure to a purported floor.  
   **Trigger:** The opponent controls a tapped Staff of Nin under a does-not-untap effect, or a mana-intensive pinger with insufficient sources. Its `{T}:damage:1 target(anytarget)` line still produces “they can also aim ability damage at you” and makes the crack-back line say the number is a floor, although no activation is available next turn. The lane brief specifically required activated damage from untapped sources.

7. **MED — `projects/mtg/src/AIPlayerGPT.cpp:9399`:** One-shot draw analysis silently hard-caps rendered modal branches at four, withholding the converter/punisher price from later legal outcomes in direct violation of the no-hard-caps doctrine.  
   **Trigger:** A scripted spell has five legal `choice` branches containing distinct immediate draw effects. All five remain executable, but `oneShotDrawGrantScan` stores only the first four, so the fifth choice receives no draw price or converter count. PARSETEST exercises only short synthetic branch lists and protects no uncapped invariant.

8. **MED — `projects/mtg/src/AIPlayerGPT.cpp:33467`:** The blocking-lifegain ceiling counts every creature for which parameterless `canBlock()` is true without proving it can block any offered attacker, producing a false “life back” ceiling that can erase a valid guaranteed-kill conclusion.  
   **Trigger:** The defender controls Perimeter Captain and a flying Wall, while every offered attacker has shadow and cannot be blocked by either body. Both bodies pass `canBlock()`, their hypothetical blocking triggers are added to `blockGain`, and the attack total claims blocking can restore life even though no legal block—and therefore no blocking trigger—exists.

## Verdict

**Not safe to ship as-is.** The diff explicitly authorizes an illegal decline for Pelakka Predation-class mandatory choices, retains a subset-based HOLD cache that suppresses changed menus, and adds several confident tapped-land, wipe-survival, crack-back, and combat-life claims not implied by the engine state. Transport classification still orders elapsed-time inference ahead of an actual HTTP result, while the new PARSETEST coverage both misses these composed paths and, for the reveal protocol, actively pins the rules-wrong behavior.