# Wave-26 reviewer brief (shared context for all 7 deck agents + synthesis)

Twelfth cycle. Pool: 27, 102, 131, 133, 137 + **deck18 (Kithkin, mono-W tribal aggro) and
deck22 (Giants!, mono-R tribal ramp) — GUIDELESS Step-0 debuts** (deck59 + deck109 rotated
out last wave; the Step-0 -> initial-guide pipeline is proven — deck59 went 3/6 -> 6/6).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary 77e91ef4e, commits 79e39511f + 77e91ef4e)

(c1) **R-TRUNCATED-TARGET-PREVIEW fixed** — single-target cast previews now enumerate ALL
legal targets, opponent side FIRST; the "(+N more)" cap is gone. Watch: targeted-removal /
burn decisions citing targets that were previously hidden; any prompt-length side effects.
(c2) **ETB pay-or-tap annotations NOW ACTUALLY FIRE** (the wave-24/25 annotation had never
fired live — case mismatch; wave-25's "validated" b4 was the card-TEXT annotation only).
Options now carry `[... enters UNTAPPED - usable this turn]` / `[... enters TAPPED ...]`.
Watch at shockland/pathway seats: decision quality + no double-annotation confusion.
(c3) **PT-pump options render delta + result** (`+3/+3 until EOT (2/2 -> 5/5)`) — ability-
class general. Watch any activated pump seat: did stat-conflation spirals die?
(c4) **Parser hardening shipped** (example-echo exclusion at the parseChoice choke point +
line-anchored answer selection). ⚠ CORPUS GATE: this is the live misfire check — the
standing lesson (N9 131x over-fire) says unit tests prove firing-where-it-should, only a
corpus proves not-firing-where-it-shouldn't. EVERY reviewer: any legitimate reply whose
answer was dropped/mis-picked (fallback where the reply looks parseable, or a choice that
contradicts the reply's clear final answer) is a POTENTIAL HARDENING MISFIRE — decompose
against the precedence spec (natural-stop last-coded-line; truncated first; ATTACK
answer-first) and report the exact reply text.
(c5) **Tergrid steal chain CLOSED end-to-end** (parse repair + may-ask render fix; see
strategy-design/wave26/probe-deck199/report.md): may-asks with a resolvable object now
render "Triggered ability of X - you MAY act on OBJECT (from ZONE)..." Watch at ANY seat
with may-triggers: legibility improvement or new confusion.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-115216 — 21 games, binary
  77e91ef4e, -T 3000, REPPENALTY=1.05.
- Win table: **133:6/6** (up from 1/6!), 27:4/6, **18:4/6 (Step-0 debut)**, 137:3/6,
  22:2/6, 131:2/6, **102:0/6** (down from 4/6). 0 timeouts/draws; 4 adjudicated at cap
  (34 gameend logs = 42 - 8, exactly the 4 adj games — integrity clean).
- **Fallbacks: 16/1,322 decisions** — 15 = deck137 by-design convoke defers; **1 = a
  priority fallback at deck133 (vs102 game)** — deck133 reviewer must decompose it (kind
  =priority, the only non-defer fallback corpus-wide). Retry fired 0x.

## Per-seat focus

- **deck18 (Step-0, 4/6)**: FIRST READ — decision-quality survey for the INITIAL guide
  (deck59 pattern: what does the model already do right, where does it leak). Kithkin =
  tribal anthem weenie aggro: wide boards, lord effects, combat math. Draft the initial
  strategy.txt per the wave-25 skill.
- **deck22 (Step-0, 2/6)**: FIRST READ — initial guide draft. Giants = big-mana tribal
  ramp/midrange: cost-reduction, damage-doubling, curve discipline. Same Step-0 method.
- **deck102 (0/6, ROTATION PENDING)**: the steal chain is DISCHARGED by probe (take-the-
  steal fired and was taken on the fixed render — probe report). Your job: (a) decompose
  the 0/6 collapse honestly (pool-rise? decision regressions? the mulligan edit's first
  corpus?); (b) confirm no NEW work signal beyond ledgered items; (c) rotation verdict —
  synthesis expects ROTATE OUT unless you find a live new decision-class.
- **deck133 (6/6)**: what changed? Last wave was threat-drought 1/6; validate the Arena
  edit's first corpus (did the low-life Arena casts stop?) + decompose the corpus's ONE
  priority fallback (yours, vs102). 6/6 can hide taxes — apply the deck59 6/6 rung.
- **deck27 (4/6)**: bottom-phase rule-#1 clause first corpus; wither-FS steady-state;
  natural-stop reversals (any new rescues or misfires — c4 gate applies).
- **deck131 (2/6, up from 1/6)**: the two CORRECT-DEVIATION edits' first corpus (bottom-
  step recalibration + lethal-only-blocker exception) — did the model follow the fixed
  rules? Guttersnipe all-tapped WATCH round 4; construction-terminal reassessment (2 wins
  now — is the ceiling higher than flagged?).
- **deck137 (3/6, up from 1/6)**: adventure-body fix first corpus (any Lovestruck-class
  declines left?); blockers-seam round 2 (floors held?); convoke defers (15 again,
  still harmless?); spiral-relocation ceiling check at the new reply-length profile.
- **EVERYONE**: c1-c5 behavior shifts (above); may-ask renders (c5) wherever encountered.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave25/strategy-writing-skill.md (CURRENT skill). Skill -> skill.md. Core ->
general-suggestions.md (20 consecutive PASSes). Win column context-only. Rotation test:
no-guide-mod AND no-new-work-signal = candidate. Termination reframed (wave-25 synthesis):
deck supply is unbounded (146 guideless remain) — termination gates on NEW-WORK EXHAUSTION;
verdicts stay unsentimental, name plainly when a seat produces nothing new.

## Open ledger carried into this wave (do not re-derive; cite if exercised)

R-DFC-BACKFACE (fix map at AllAbilities.cpp:5422 + autostack paid(alternative); deferred,
no suite coverage); doubleside parse-poisoning generality (Aclazotz same-structure suspect,
unverified); primitives-only card + set hint silent drop (MTGDeck.cpp:1043, loud-rejection
candidate); ETB menu card-name-at-seam (engine work); prose-only answer reversal
convergence watch; affirmative untapped-tagging core candidate (below-bar); N8
blocker-gang-chump unexercised; Azcanta probe; undying trade-annotation; commit-lock watch;
mulligan surface now 3 corpora unexercised; Yawgmoth/Bloodghast-fodder unexercised at 133.
