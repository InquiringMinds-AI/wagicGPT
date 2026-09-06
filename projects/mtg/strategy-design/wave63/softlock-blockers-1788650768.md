# Vita softlock 1788650768 (vpk26, 2026-09-05): declare-blockers advance refused "regardless of configuration" — OPEN

Owner: "a bizarre softlock where i could assign and cancel blockers but not go to the next step" and, asked whether it
refused with zero blockers: "yes, it refused regardless of configuration".

**Evidence (pulled to psp-work/logs/20260905-vpk26/):** softlock dump: turn 13, phase Blockers(7), combatStep=0,
settled 13/7/0, phaseTicks 1759, currentPlayer=p2 (AI attacker), currentActionPlayer=currentlyActing=p1 (human),
stack items all state=1 (RESOLVED) incl. a NextGamePhase, no chooser, no menu, no display, extraPayment=0.
Attackers include Nullpriest of Oblivion (MENACE). Transcript: NO `p1.next` logged anywhere in turn 13 (the log is
written only after every refusal gate passes) — the human's presses never became effective requests.

**Reproduced headless:** `WAGIC_HEADLESS=1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1 WAGIC_REPLAY=<transcript>` replays
to turn 13 phase 7. A synthetic `p1.next 1 7` inserted before `[end]` was CONSUMED by the replay's engine-action
matcher ("already performed by the engine") — i.e. the ENGINE itself was requesting the advance in that state and
not getting it. Replay copy: ~/.gatelogs/replay-b.txt; logs replay-{a,b,c,d}.log.

**Ruled out:** W43-1 illegal-block gate (needs >=1 blocker; owner had zero at some point); unresolved stack; open
target chooser / menu / reveal-scry display at dump time (all none; the reveal/scry key-swallow needs a display);
MTGGamePhase `activity` latch (resets after 4 frames); `nextCombatStep` (unconditional BLOCKERS->TRIGGERS).
A dev-only probe `WAGIC_ADVANCEPROBE=1` (this commit) names every early-return gate in
`GameObserver::userRequestNextGamePhase`; during the replay it printed NOTHING, so no probed gate refused —
the request either never reaches the function, or reaches `AddNextGamePhase()` and the stack item's resolve
does not move the step.

**Current suspicion (unverified):** (a) `MTGGamePhase::NextGamePhase` gates before the observer is called:
`(asked && !asked->isAI()) || (!asked && interrupting && !interrupting->isAI())` redirects the press to
`stack->CheckUserInput` as a decline, and `currentActionPlayer == currentlyActing()` — the STALLPROBE showed
`wait=` (actionLayer waiting action) flickering on/off every frame with gtc=nil: something re-arms a waiting action
each tick (the AI attacker's Act? the human's block ability re-created by MTGBlockRule?) and may flip
currentlyActing each tick; (b) `NextGamePhase::resolve`'s W36 hold is AI-defender-only, but the item DID resolve
(state=1) with no step change — check what `userRequestNextGamePhase(false,false)` did on that resolve (probe it).

**Next probes:** extend WAGIC_ADVANCEPROBE to log ENTRY (allowInterrupt, caller) and the AddNextGamePhase branch;
trace MTGGamePhase::NextGamePhase's three gates; in the replay, exempt synthetic lines from engine-action
consumption (or place the synthetic next one action earlier); dump `currentlyActing()` per tick at t13.

**Shipped meanwhile:** 349a4d5b4 — a refused advance now shows a 4 s banner (illegal-block case). That does NOT
cover this lock (no probed gate fired). The owner reads it as a softlock; it is reproducible from the transcript.
