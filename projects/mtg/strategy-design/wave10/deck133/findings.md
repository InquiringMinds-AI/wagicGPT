# Wave 10 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260715-031825` (21 games, round-robin over 44,109,110,131,133,135,140;
binary c8c054be8; wave-9 guides; **FIRST corpus under the answer-after-plan reply protocol** — scratch
reasoning -> `PLAN:` -> `CHOICE:`/`ATTACK:`/`BLOCKS:` final line; max_tokens 2048). deck133 was the GPT
seat in 6 games.

**Real record 4W / 2L** — WON vs110, vs131(adj1), vs135(adj1), vs140; LOST vs44, vs109.
Different round-robin from wave-9 (3W/3L, lost vs110/vs44/vs109) — vs110 flipped W. Per the binding
doctrine this is draw variance, not the guide (byte-identical to wave-9 output — freeze-check below).

| opp | result | end my/opp | last T | decs | fb | note |
|-----|--------|-----------|--------|------|----|------|
| 110 | **WON**  | 21 / **-1** | 13 | 35 | 4 | affinity; GM drain-6 lethal (N=6>=opp5); a self-Fatal-Push blunder (won anyway) |
| 131 | **WON** adj1 | 20 / 15 (cap) | 12 | 37 | 0 | Liliana coexist-casts + activations; grind, ahead at cap |
| 135 | **WON** adj1 | 3 / 2 (cap)  | 14 | 37 | 7 | razor 3-2 adjudication; heaviest fb cluster, none deciding |
| 140 | **WON**  | 16 / **-6** | 15 | 39 | 0 | beatdown; GM drain-6 stabilizer + Obliterator/Geralf's/GM beats; self-Fatal-Push blunder (won anyway) |
| 44  | LOST | **0** / 12 | 17 | 50 | 3 | Faerie flyer tempo; no reach/flyer answer, GM never drawn; Yawgmoth grind bled life vs static opp12 |
| 109 | LOST | **-2** / 18 | 8  | 20 | 0 | Hellrider aggro T8 blowout; threat-light draw, no wall (Bloodghast can't block) |

**Mode: OBEYED / TURNAROUND-PRESERVATION.** Wins execute the guide's own lines (beatdown curve; GM as
finisher vs110 AND stabilizer vs140; Liliana coexist-casts vs131). Losses are construction races the
deck cannot answer (no reach vs Faeries; no early blocker vs Hellrider — same flags as wave-9 #9/#10).
Disposition: **guide FROZEN, byte-identical** (`diff -q` clean vs wave9/deck133/strategy.txt).

## Harness / representation sweep — CLEAN
- **0 fails-validation, 0 DESYNC** in all 6 game stderrs; **0 defer** records in the JSONL.
- **14 fallbacks** (deck135 x7, deck110 x4, deck44 x3), all truncation/format at ~100k+ms latency (the
  known 2048-cap class — do NOT re-diagnose). **None changed a game outcome** (audited below).
- **Label compliance ~97.5%**: 199/204 non-fallback ask/priority replies carry a CHOICE label; the 8
  without still parsed (truncated-but-recoverable). Attacker labels 17/20 correct `A#` format.

---

## VERIFICATION LIST (brief items 4/3 + frozen classes + carried watches)

### (A) HEADLINE — Thoughtseize thin-hand annotation FIRED and is OBEYED; the wave-9 fabrication class is CLOSED.
The wave-9 signature leak (vs131 s58: Thoughtseize into a KNOWN-EMPTY hand, reply fabricating "the
opponent may have a removal spell") was routed to an OPTION-LINE annotation. **That annotation SHIPPED**
and renders as `Cast Thoughtseize {b} - legal targets right now: the opponent, you - the opponent's
hand is EMPTY: nothing to strip` (and `- the opponent holds only N card(s)` for thin non-empty hands).

**Empty-hand windows (annotation fired) — Thoughtseize declined 100% at every real-choice window:**
- deck109 seq16 (T7, oppHand0): chose Fatal Push (killed a Goblin). seq18/seq20: Cast nothing.
- deck110 seq13 (T9, oppHand0): Fatal Push. seq15/22: Cast nothing. seq19: Collective Brutality.
  seq25: Fatal Push (Master of Etherium). seq27: Collective Brutality. seq29: fb->CB (choice 0, not TS).
- **Zero Thoughtseize casts into an empty hand this corpus.** No fabricated-card-over-empty-hand reply
  recurred. The read-then-disobey-hallucinated-empty-hand wall of wave-9 is GONE at this seat — a
  wave-9-proposed representation fix landed and closed the class.

**Real Thoughtseize casts this corpus (both non-empty):**
- deck44 seq7 (T3, oppHand5): sanctioned early seize (guide turns 1-3). CORRECT.
- deck44 seq16 (T7, oppHand1, "holds only 1 card" annotation): cast anyway under Faerie-flyer race
  pressure, blind-stripping the last card. Marginal — 2 life while losing a race — but the annotation
  here is INFORMATIONAL (thin, not empty), a real card exists, and the guide permits late seize. WATCH,
  not a leak; the self-damage feeds the construction margin (notes #4), not a decision-failure class.

Not a demotion trigger — see freeze-check (the guide clause covers a superset of the annotation's surface).

### (B) Gray Merchant bare-N frozen class — STAYED DEAD (2 casts, shown == resolved).
- deck44 seq32 (T13, my5/opp18): `{right now: drains 6}` -> my5->11, opp18->12 (+6/-6). Cast under the
  `<=10 stabilizer` trigger (my5) — correct panic-button use. Shown 6 == resolved 6.
- deck110 seq35 (T13, my15/opp5): `{right now: drains 6}` -> my15->21, opp5->-1 (game end, WIN).
  N=6 >= opp5 -> lethal rule-1 win, correctly cast. Shown 6 == resolved 6.
No N->N+2 regression. The wave-8 "+2"-deletion stays SAFE. **FREEZE.**

### (C) Liliana coexist-casts / no-legal-target loop — STAYS FIXED.
Liliana of the Veil cast + activated (vs131 seq16 cast, seq17/22/31 activations); Liliana, the Last Hope
cast + activated (vs131 seq20 cast, seq21/33 activations); Liliana of the Veil activations vs135
(seq33/36). All resolved; **0 fails-validation, 0 defer, 0 loops** in all 6 games. Fixed class dead.

### (D) Liliana-the-Last-Hope +1 as removal (dormant watch) — STILL DORMANT (no diagnostic window).
vs131 seq33/34: the +1 (`up to one target creature gets -2/-1`) had NO opponent creature to target
(opp board creature-empty); the pilot correctly DECLINED (seq34 chose "Decline - do nothing", reasoning
"the opponent has no creatures, so the -2/-1 ability has no valid target"). No window that tests the
watch (a removable creature present). Watch stays dormant, unchanged from wave-9 notes #8.

### (E) Legal-target names + own-targets annotation — RENDER and OBEYED-IN-REASONING.
Fatal Push renders real target names (`- legal targets right now: Abominable Treefolk, Geralf's
Messenger, Bloodghast`) and the own-targets marker (`- the only legal targets are YOUR OWN right now`).
The wave-9 deck44 fabricated-no-legal-target class is DEAD — every Fatal Push cast at an opponent named
a real creature (deck44 Surveilling Sprite; deck109 Legion Loyalist; deck110 Vault Skirge / Master of
Etherium; deck135 Icehide Golem / Abominable Treefolk with revolt) — all sound removal targets, none a
mana-dork/harmless miss. Across 29 own-target windows the pilot declined/cast-nothing in 27 and
**verbally rejected Fatal Push in all** ("Fatal Push has no valid targets... I will not cast it"). The
annotation works. The 2 residual self-casts are a NUMBER/PARSE-layer leak, not an annotation failure —
see finding (F).

### (F) NEW FINDING — self-Fatal-Push-at-own-creature via the number/parse layer (2 instances, both WON games, non-deciding; route to HARNESS/reply-protocol, NOT guide).
The own-targets annotation was OBEYED IN THE REASONING in both cases, yet the recorded choice landed on
the self-Fatal-Push the plan had just condemned — the leak is downstream of the reasoning:

- **deck140 seq19 (T7, my19/opp17) — unoffered-desired-card numeric mismatch.** The 2-option list was
  literally `1. Cast Fatal Push {b} - the only legal targets are YOUR OWN right now` / `2. Cast nothing
  right now`. The PLAN reasons correctly: *"Fatal Push useless... dangerous if it targets my own
  creatures. I will cast Phyrexian Obliterator..."* — but Obliterator is NOT in the list, and the model
  emits **`CHOICE: 1`** = the self-Fatal-Push it just rejected. Resolved onto own Bloodghast (returned
  via landfall the same turn when a Swamp was played, seq21). The guide's list-anchor rule ("confirm the
  card you name appears in the list; if it does not, cast the best card that IS listed") was disobeyed —
  the pilot picked the WORST listed option instead of the safe "Cast nothing". Answer-after-plan does NOT
  prevent this: it guarantees the answer follows the reasoning, but when the desired card is UNOFFERED
  the model can still map its fallback number onto an actively-harmful option.

- **deck110 seq20 (T9, my17/opp5) — format-noncompliance + truncation int-grab.** The reply DROPPED the
  `PLAN:`/`CHOICE:` format entirely, rambled ~7000 chars of mana confusion to the token cap, and
  concluded in prose *"the correct choice is 4"* / *"Cast nothing"* repeatedly — but was cut before
  emitting an answer line. The parser (no leading `PLAN:`, so the truncation-FAIL guard did not fire)
  int-grabbed a stray `2` from the reasoning = Cast Fatal Push, which resolved onto the pilot's own
  Geralf's Messenger (4/3, +1/+1 counter -> undying did NOT return it): a self-inflicted 2-for-1 that
  also erased 3 of the 5 power that was lethal-next-turn. deck133 still WON at T13 (Collective Brutality
  drain + Gray Merchant), so non-deciding — but a genuine value blunder. The truncation-FAIL heuristic
  guard keys on a reply LEADING with `PLAN:`; a reply that omits the prefix evades the guard and can be
  int-grabbed into a harmful option.

Both route to the reply-protocol/harness layer (notes #1) — the guide cannot fix a number that
contradicts a correct plan (representation-beats-instruction). Skill wrinkle: verify annotation
obedience at the REASONING level separately from the recorded choice (skill.md).

### (G) Intent-collapse — GONE at this seat.
Under answer-after-plan every reply head is the word `PLAN` (198/198 "non-numeric heads" — the
head-based arms of tools/intent-collapse-metric.py are now moot, expected). The reversal-detector flagged
12 deck133 records; **all are the design-doc's known future-tense false positives** — every `CHOICE:`
label matches the plan's actual final decision (spot-verified deck140 seq14/30, deck135 seq23/29,
deck131 seq37, deck44 seq13/32). The wave-9 one-shot head-first collapse (deck131 mulligan) cannot
recur — the number is emitted after the reasoning. (The lone `choice=-1` in that list, deck135 seq19, is
a truncation fallback, not a collapse.)

---

## MANDATORY AUDITS

### (1) Fallbacks — none changed a game.
- **deck44 (LOSS)** seq47/49/50: combat-damage and MP2 priority windows AFTER damage was assigned, at
  my2->my1, opp static at 12 with no reach on deck133's side. The pilot had already grinded Yawgmoth
  -1/-1 counters (seq38-46) killing Faeries but bleeding its own life (Yawgmoth 1/activation + 2 fetch
  cracks + the seq16 Thoughtseize) with no clock. The fallbacks were post-damage / lost-position —
  non-deciding. Construction loss (no reach vs a static opp12), confirmed.
- **deck135 (WON adj 3-2)** x7 and **deck110 (WON opp-1)** x4: clustered but non-deciding (both won).
  deck135 seq32 attackers reply emitted `ATTACK: Geralf's Messenger, Bloodghast` (card NAMES not `A#`)
  -> flagged unparsed -> heuristic; the heuristic declared attackers, which is what the pilot wanted
  (it was behind and racing). deck110 seq20 is the self-Fatal-Push (F) — delayed, not deciding.

### (2) Obeyed-but-losing on the two losses.
Obedience HIGH: curve deployed, attacked with all available bodies (deck44 4 attack windows with
Bloodghast/Geralf's/Yawgmoth), GM cast as stabilizer/lethal in the wins. vs109's single attacker window
and thin board were forced (T8 blowout, no board assembled — threat-light draw). Both losses attribute
to construction (no reach / no early blocker), not play. NOT a rewrite trigger.

### (3) Convergence watch — reflexive high-life blocking: NEGATIVE for this deck.
**0 blocker windows** for deck133 all corpus (structural: Bloodghast can't block, walls draw-dependent,
games were beatdowns/races). No `[deals 0]` blocker windows either. No reflexive-block possible.

### (4) Fetch behavior — re-ask multiplier tamed.
20 fetch-crack choices / 6 games (~3.3/game), all cracking when a land was needed. The wave-9-flagged
single-option "Put in Play" re-ask inflation did not recur as a decision-count problem — consistent with
the consume-on-choose engine change. Not a leak.

### (5) adj-row sanity check.
- `131 133 adj1 15 20 12`: deck133 seat last life my20/opp15 (T11); results deck133=20 vs deck131=15 at
  the T12 cap -> deck133 AHEAD -> adj1. Consistent.
- `135 133 adj1 2 3 14`: deck133 seat last life my3/opp7 (T13); the game ran one more turn to the T14 cap
  where deck135 dropped to 2 and deck133 held at 3 -> deck133 AHEAD 3-2 -> adj1. Consistent.

---

## FREEZE-CHECK (turnaround-preservation — every kept line accounted for; ZERO edits)
Live guide is BYTE-IDENTICAL to the wave-9 output (`diff -q` clean). Every line stands:
- **Identity + beats-are-the-win paragraph** — vindicated by 4 wins (GM lethal vs110; grind vs131);
  KEPT VERBATIM.
- **Impact-ranked cast list + GM #1 ELSE (bare-N)** — OBEYED; bare-N validated on 2 casts (B). KEPT.
- **GRAY MERCHANT block (bare-N "use the shown number directly")** — CONFIRMED shown==resolved (B). KEPT.
- **Thoughtseize rule (impact-list #5 empty-hand prohibition + DECIDING SITUATIONS empty-hand line)** —
  the class is now OBSERVED OBEYED via the shipped option-line annotation (A). **Demotion caveat (wave-9
  conditional-surface granularity): DO NOT demote.** The annotation's surface covers only the hand=EMPTY
  case; the guide clause ALSO covers "holds nothing that threatens you" (a non-empty but harmless hand),
  which the annotation does NOT surface (opponent card CONTENTS are hidden), plus alternate option
  formats where the annotation may not render (e.g. the escalate-discard sub-menu). Not fully
  double-covered -> KEEP the guide prohibition at full prominence. FREEZE.
- **ROLE CHECK + DEFENDER/Obliterator-wall line** — untestable at deck133's seat (0 blocks, structural);
  the wall mechanic renders opponent-side (wave-9 C). KEPT VERBATIM.
- **own-targets / legal-target handling** — the annotations render and are obeyed in reasoning (E/F);
  the residual is a number/parse-layer harness item, not guide-writable. KEPT VERBATIM.
- **VS RED/BURN, Bloodghast, Fatal Push, Collective Brutality, Liliana, fetch, Yawgmoth, Geralf's,
  Arena, attack, mulligan, DECIDING SITUATIONS** — untested or obeyed; KEPT VERBATIM.
Nothing dropped. **The only change this wave is ZERO — the guide is frozen and stands alone.**

## In my words
The engine caught up on the exact leak wave-9 diagnosed: the empty-hand Thoughtseize annotation shipped,
and the pilot now reads `hand is EMPTY: nothing to strip` and declines every time — the hallucinated
card in a 0-card hand did not recur once. Gray Merchant's bare-N display and the Liliana coexist-casts
stayed frozen-dead. The one genuinely new thing this corpus surfaced is subtle and NOT a guide problem:
in two spots the pilot reasoned correctly ("Fatal Push only hits my own creatures, I won't cast it") but
the emitted NUMBER still landed on the self-Fatal-Push — once because the card it actually wanted
(Obliterator) wasn't offered and it mis-mapped its fallback digit, once because a formatless, truncated
reply got int-grabbed by the parser. Both were in games it won, so they cost nothing this time, but they
mark where the answer-after-plan protocol still leaks: it orders the answer after the reasoning without
guaranteeing the answer AGREES with the reasoning when the wanted card is missing or the format is
dropped. That belongs to the harness, not another paragraph of guide. So the guide freezes clean.
