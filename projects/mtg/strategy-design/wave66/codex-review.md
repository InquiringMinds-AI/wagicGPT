# Adversarial findings

## 1. HIGH — HOLD normalization creates a blind cache over materially changed verdicts

**Defect:** [`AIPlayerGPT.cpp:24107`](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:24107) replaces every numeric payload inside annotations with `#`, and [`holdHonoured()` at line 24326](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:24326) consequently auto-answers HOLD even when an option’s kill count, damage, price, or survivor count has materially changed.

**Trigger:** The model holds on a removal row reading `{right now: deals 3, kills 0 of their 1 creatures}`; before the next priority window the target loses toughness, making the same legal row read `kills 1 of their 1 creatures`. Both rows normalize to the same key, so the newly effective removal is never shown to the model. This violates the lane doctrine’s prohibition on blind caches, rather than merely suppressing volatile life-total noise.

## 2. HIGH — Draw-step accounting subtracts unrelated draws from pending scheduled draws

**Defect:** [`AIPlayerGPT::receiveEvent()` at line 18075](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:18075) counts every library-to-hand move during the draw phase, while [`drawsStillAhead()` at line 11095](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:11095) subtracts that count from only the base-plus-static-extra draw-step forecast, producing false remaining-draw and lethal/survival claims.

**Trigger:** After the turn-based draw, a Howling Mine trigger remains on the stack; the active player casts an instant cantrip in response. The base draw and cantrip raise `resolvedInStep` to two, so a two-card base-plus-Mine forecast reports zero remaining, although the Mine draw—and its Underworld Dreams/Fate Unraveler damage—is still pending. The PARSETEST cases exercise scalar arithmetic, not event provenance.

## 3. MED — One-shot token triggers are classified as repeatable engines

**Defect:** [`engineKindForScript()` at line 19250](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:19250) treats any `token(` preceded by a colon on the same line as a repeatable “one per activation” engine, although Wagic’s colon syntax also denotes one-shot triggered abilities.

**Trigger:** A creature scripted as `@movedto(this|mybattlefield):token(Soldier,Creature Soldier,1/1,white)` is labeled `TOKEN ENGINE (it makes more permanents, one per activation)`, and sweep/sacrifice renderers then claim it “keeps producing while it is on board” at [`AIPlayerGPT.cpp:26154`](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:26154). The negative PARSETEST at [`AIPlayerGPT.cpp:68689`](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:68689) uses an unrealistic colon-free stand-in for a one-shot trigger and therefore misses the actual grammar.

## 4. MED — The attackers renderer grants future attack permission not implied by summoning sickness

**Defect:** [`attackersHeldSickLine()` at line 3653](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:3653) states both that every omitted sick creature “entered this turn” and that it attacks from the next turn onward, while its caller at [`AIPlayerGPT.cpp:40632`](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:40632) checks neither proposition.

**Trigger:** A newly entered Defender, a creature with `CANTATTACK`, or a creature whose attack cost remains unpaid is absent from the legal attacker vector and summoning sick, so it is listed as able to attack next turn despite still being unable; a creature gained during the current turn also has sickness without having “entered this turn.” PARSETEST explicitly pins the unsupported `NEXT turn` promise at [`AIPlayerGPT.cpp:68341`](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:68341).

## 5. MED — The all-dead exemplar still instructs the model to choose a dead row

**Defect:** [`askExemplar()` at line 31699](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31699) defaults to row 1 when every row satisfies `rowSaysNoOp`, directly contradicting the lane requirement that the re-ask “must never exemplify” such a row.

**Trigger:** A mandatory mode or target menu contains only currently ineffective legal choices such as `Cast A {right now: this does nothing}` and `Cast B {right now: this does nothing}`; the prompt supplies `CHOICE: 1 (Cast A)` as its worked answer, recreating the documented answer-attractor that H8 intended to eliminate. PARSETEST deliberately enshrines this behavior at [`AIPlayerGPT.cpp:68970`](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:68970).

## Verdict

**Unsafe to ship as-is.** The HOLD key can suppress a legal option after its tactical outcome changes, and draw-event misattribution can emit false survival or lethal guidance while damage remains pending; both are decision-corrupting HIGH defects that a green helper-oriented PARSETEST cannot expose. The token-engine and summoning-sickness renderers also make affirmative claims not implied by the state they inspect, and two PARSETEST expectations preserve or conceal those errors. I found no defensible wave-66 regression in the reviewed timeout/`transport_error`/`empty_reply` ordering or a concrete new NULL/token dereference, but that does not offset the shipping blockers above.