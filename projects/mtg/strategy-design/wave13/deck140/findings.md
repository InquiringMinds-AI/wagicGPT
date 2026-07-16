# Deck-140 review — wave 13b (name-echo era; first clean validation after the Spark-crash contamination)

Deck: **"Wipe Them Out!"** — RBW control, 14 board wipes, ~2 creatures (Blightsteel Colossus x2),
finishers = Rakdos's Return / Staff of Nin / Blightsteel Colossus.

Corpus: PRIMARY `matchups-20260715-183228`, 6 deck140 seat games, binary `/tmp/wagic-a75e2625e`
(Force r2+r3 + auto-tap two-pass d5d7b1f2e + name-echo c3e6d95b2 + BLOCKS:none 42e8bc180 + 240s timeout).
Wave-11 guide, FROZEN byte-identical. Spark at GPU_MEM_UTIL=0.75 (latency med 11.2s / p90 87s / p99 185s;
all 6 deck140 games hit the 2400s cap and were **life-adjudicated** — this is latency, not play quality).
Every deck140 seat file read decision-by-decision; the deck109 opponent seat read at seq33 for the
brief's `ATTACK: Hellrider`. `choice` 1-based; `choice -1` = heuristic fallback; `CHOICE: 0` = pass action.

**Record: 5 W / 1 L** (140:5/6 — best of corpus, up from 3/6 at wave-11). All 6 games adjudicated by life.

| game | opp | outcome | end life | class |
|---|---|---|---|---|
| vs109 | mono-R Hellrider | **WIN adj0** 21-5 T32  | swept + Staff clock; 2 defects below (missed sweep s9, Rakdos X-underpay s43) — won anyway |
| vs135 | snow control      | **WIN adj1** 29-15 T18 | clean sweeps (BSZ/Damnation/Pyroclasm); comfortable |
| vs133 | mono-B blitz      | **WIN adj0** 15-14 T14 | won by 1; swept; 1 discard-menu misread (s12) |
| vs131 | UWx control       | **LOSS adj1** 29-31 T34| STRUCTURAL reach-starvation in a control mirror; lost adj by 2, no blunder |
| vs44  | UB faeries        | **WIN adj1** 16-12 T32 | held Rakdos ~14 windows THEN name-echo cluster dumped Rakdos X=8 + Wrath into C=0 (s40-42) |
| vs110 | affinity          | **WIN adj0** 18-14 T24 | swept every creature; 1 latency fallback self-healed (s13) |

Adjudication rows cross-checked: exact on vs109/vs133/vs131/vs110; vs135/vs44 show the known
life-snapshot-timing nit (deck140 gained 1 after the last logged decision) — verdicts unambiguous.

Tags: [GUIDE] model-fixable-by-wording · [REPLY-PROTOCOL] name-echo reconcile layer · [ENGINE] C++ ·
[STRUCTURAL] deck/infra.

## Headline
The **SWEEP TRIGGER validates a 5th corpus** (read correctly at every window; guide FREEZES) and the
**#1 Rakdos-hold is obeyed at ~every window** — but the wave-11 **UNLISTED/UNAFFORDABLE-CARD FIXATION did
NOT go away under the name-echo protocol; it RECURS and is now HIGHER-impact.** Because name-echo resolves
echo-vs-index conflicts by INDEX-WINS-BY-DESIGN, when the model's echoed name matches NO listed option the
reconciler falls straight to the raw index (an arbitrary "Cast X"), which twice **dumped a Rakdos kill
shot** this corpus (vs44 name-echo cluster; vs109 a separate engine X-underpayment). Neither cost the game
(both adjudicated wins), but both are real decision-level defects sitting BELOW the guide.

## TOP FINDINGS

### 1. [FREEZE — 5th corpus] Count-anchored SWEEP TRIGGER read correctly at every window
Machine-scanned all 6 games: ~28 windows fired a payable sweeper at `(creatures: N>=1)` with the trigger
cited and applied faithfully — the "16 or lower (16 counts)" boundary (vs110 s13/s21-25 all at life 16-17),
the empty-board HOLD (vs131 s32/s34/s35 held at C=0), tapped-still-counts (vs109 s33 Cruel Edict on a
tapped Hellrider), and WHICH-SWEEPER derivation (vs135 s6 BSZ over Pyroclasm vs a 2-creature snow board;
vs133 s17 Wrath for toughness-3+). **The two "cast nothing at life<=16" windows are NOT trigger misreads**
(Finding 3). **Trigger GUIDE stays FROZEN.**

### 2. [FREEZE] #1 Rakdos-hold + BSZ-X + Staff-face-ping obeyed
Rakdos's Return was held at every non-lethal window it was legitimately offered (vs44 s22-38 held ~14
straight turns; vs131 s9-17 held at opp 21 with only X=1-2 affordable). The one CORRECT lethal fire —
vs109 s43, opp at 9, X=9 affordable — was INTENDED correctly (Finding 4 covers the engine underpaying it).
BSZ X always picks option 1 = biggest affordable X (vs135 s7 X=2, vs133 s15 X=2 — both max). Staff of Nin
takes "Deal 1 Damage" at a player (face) every upkeep and never wastes the ping on a toughness-3 Hellrider
(vs109 s40/s41/s45/s46 all targeted the player, not the 1/1 Legion Loyalist or the 3/3). Freeze.

### 3. [REPLY-PROTOCOL — route to the name-echo reconcile A/B, NOT the guide] The wave-11 UNLISTED/UNAFFORDABLE-CARD FIXATION RECURS under name-echo, now dumping a kill shot
The wave-11 residual (plan's NAMED action is a card ABSENT from the options because it is
unaffordable/already-used-this-turn; the trailing answer mis-maps onto a WRONG listed option) **is not
fixed by name-echo.** Name-echo added a "(name)" to every CHOICE (100% compliance, 204/204 at this seat)
and resolves echo-vs-index conflicts by INDEX-WINS. But when the echoed name matches NO option, there is
nothing to reconcile TO, so the engine takes the bare index — an arbitrary "Cast X". The most damaging
instance this corpus:

**vs44 seq40-42 t29-31 — the "Play Badlands" absent-action fixation (3 consecutive mis-maps):**
The model played its land at s39 (Play Badlands, correct). At s40-42 it kept WANTING to "Play Badlands"
again — but the land drop is spent, so "Play Badlands" is not offered. Its PLAN each time was RIGHT
("hold Rakdos, non-lethal", "Wrath has no targets, don't cast"), yet it echoed `CHOICE: 1 (Play Badlands)`
against option lists whose index 1 was always a *Cast*:
- **s40** options `["Cast Rakdos's Return", "Cast nothing"]` -> echo "Play Badlands" -> index 1 =
  **Cast Rakdos's Return** (the model explicitly reasoned NOT to cast it).
- **s41** X-menu `[X=8..X=0]` -> echo "Play Badlands" -> index 1 = **X = 8** -> the held kill shot fires
  non-lethal (opp 20 -> 12).
- **s42** options `["Cast Wrath of God", "Cast nothing"]`, board C=0 -> echo "Play Badlands" -> index 1 =
  **Cast Wrath of God into an empty board** (wasted sweeper; a fresh Wrath is drawn next turn).
The correct intent-consistent target in every case was "Cast nothing" (option 2). This is the wave-11
class verbatim, at HIGHER impact (a wasted Rakdos, not just a wasted Pyroclasm-into-C0). It won anyway —
the accidental 8 damage ironically secured the 16-12 adjudication — but the DECISION was defective.

**vs109 seq9/seq10 t8 life16 C=2 (2x Goblin 1/1) — UNDER-cast direction:** plan wanted **Damnation**
("must use Damnation to destroy all creatures... I will not cast Cruel Edict as it only removes one").
Damnation `{2}{b}{b}` was unaffordable (no black shown; Finding 5). Options were `[Cruel Edict (legal
targets: the opponent), Rakdos, Venser's, Cast nothing]`. The model emitted **CHOICE: 4 (Cast nothing)**,
declining an OFFERED payable Cruel Edict at a MANDATED sweep window (life exactly 16, C>=1). Per the guide's
"cast the FIRST sweeper you can pay for / partial removal still beats casting nothing," Cruel Edict (kill 1
of 2 Goblins) was the correct payable play. This blends the reconcile-fixation with the mana-annotation
defect (Finding 5) and with a model judgement error (declining partial removal to wait for an unaffordable
full sweep). It recurred identically at s10 (same turn, re-prompt). deck140 recovered (swept at s11 with
Cruel Edict next window, ground opp to 5, won adj) — the win was sound in TRAJECTORY, defective at s9/s10.

**Attribution:** all reply-protocol, NOT guide. The guide is read correctly throughout (the model NAMES
the right intent in prose every time). This RE-CONFIRMS the wave-11 promotable ask (notes.md MODEL #4,
general-suggestions #1): the reconcile-named-action arm must handle "named action ABSENT from options" ->
route to the intent-consistent best-available option (payable removal if the plan wanted removal;
Cast-nothing/pass if it wanted to hold or deploy an unaffordable card), NEVER the raw index. Now with a
2nd mechanism-independent, HIGHER-impact witness (the vs44 kill-shot dump).

### 4. [ENGINE — notes ledger, HIGH] Rakdos's Return X auto-resolved to 3 (non-lethal) when lethal X=9 was affordable and INTENDED; no X-menu surfaced
**vs109 seq43 t30 life22/opp9:** the model correctly cast Rakdos's Return reasoning "X=9... which is
lethal... win the game" with 11 untapped sources ({r}{b}{b}{b}{w}{w}{w}{w}{w}{w}{w}), where {r}{b}+9
generic = X=9 is fully payable. But **no X-menu was ever surfaced as a decision** (contrast vs44 s41 where
the X-menu `[X=8..0]` DID appear as its own prompt). The next record (s44) shows `Already in pool: {w}{w}
{w}{w}{w}{w}` (6 white unspent) and `Damage: 3 dealt by Rakdos's Return` — the engine cast it for **X=3**,
tapping only 5 of 11 sources. Opp dropped 9 -> 6, the game dragged two more turns to a T32 adjudication
that SHOULD have been a T30 win. This is an auto-tap / X-payment defect (the X was not maximized and the
choice was never presented), distinct from the name-echo mis-map. HIGH: it converted a game-ending lethal
into a slow grind. Repro: read s42 (fallback) -> s43 (cast) -> s44 (pool + events).

### 5. [ENGINE/REPRESENTATION — notes ledger; brief #5 CONFIRMED] "Mana available" line defaults duals to one color and hides a color the model needs, causing declines of AFFORDABLE offered casts
**vs109 seq9 t8:** prompt shows `Mana available: {r}{w}{w}{w}{w}` (NO black) while the SAME prompt OFFERS
`Cast Cruel Edict {1}{b} - legal targets right now: the opponent` — a direct contradiction (the offer
asserts castable, needing black; the mana line shows none). The model REASONED from the mana line ("since
I lack Black mana") and declined. That black WAS available is proven by s11 (t10, next window) where the
model casts Cruel Edict from the same manabase. The "Mana available" annotation renders a single defaulted
tap-plan (duals shown as white), not the achievable color space, so a dual the model needs for black reads
as unavailable. This is the exact "Auto-tap Mana-available annotation trust" question in brief #5 —
answered YES, a payable offered cast was declined off a mana line that didn't hold. Related candidate
(brief #2 auto-tap stranding at the LEGAL-MOVE layer): **vs133 seq11 t8** — only Elixir was offered though
the model had Plateau(R/W)+Chapel(W/B) untapped, from which Pyroclasm `{1}{r}` should be castable; the
legal-move generator appears to use the same defaulted `{w}{w}` plan and excluded it. Uncertain (Pyroclasm
may not have been in hand) — flag, corroborates the deck44 reviewer's stranding finding, do not assert.

### 6. [STRUCTURAL — the whole loss column, unchanged] vs131 loss = reach-starvation in a control mirror
The single loss (adj 29-31 T34) is not a decision blunder. deck140 out-survived (30 life most of the game)
but never drew reach late: Rakdos's Return appeared only early (s9-17, opp at 21, X=1-2 non-lethal — held
correctly), Staff of Nin resolved only at s41 (t34, the FINAL turn — too late to ping), Blightsteel never
affordable. It lost the life-adjudication by 2 because deck131 chipped 2 more. No play at s41 changes the
adjudicated life (Staff/Venser's only pay off on future upkeeps that never come). Consistent with the
wave-9/10/11 reach-starvation loss column: 14 sweepers is the right density; the deck needs a faster,
counter-resilient CLOSER, not guide text. User owns; flag only.

## Fallbacks (9 unparsed_reply at the deck140 seat; 0 empty_reply — matches corpus)
All 9 are long-latency reasoning rambles (150-199s, KV preemption at 0.75 util) correctly discarded by the
label guard -> heuristic, EXCEPT vs133 s12 (fast 14.5s, the discard misread below). **None cost a game.**
The only one on a critical sweep window — **vs110 seq13 t6 life16 C=1, payable Cruel Edict offered** —
SELF-HEALED: the heuristic cast Cruel Edict (s14 events show the Vault Skirge dying, board back to C=0).
Latency harness class, not pilot quality; the 240s timeout + backstop held. No fallback landed on a C>=1 +
payable-sweeper + life<=16 window with a bad heuristic outcome.

## Name-echo protocol audit (brief #4)
- **Compliance 100%** (204/204 final CHOICE lines carry "(name)" at this seat).
- **No echo-vs-index conflict resolved WRONG** — index wins by design everywhere, as intended. Benign
  conflicts where the echo named an unlisted card and index-wins produced the right outcome: **vs109 s31**
  (Staff-upkeep-trigger menu; model echoed "Cast Cruel Edict" confusing it for a main-phase sweep, index 1
  = Staff ping at a player = correct). Protocol nits, harmless: **vs135 s7** / **vs133 s15** BSZ X-menus
  where the model echoed the spell name instead of "X = N" (index correct). The DAMAGING cases (Finding 3
  vs44) are NOT conflicts the reconciler can catch — the echoed name matches no option, so index-wins is
  the ONLY behavior and it is wrong. That is the gap the absent-named-action arm must close.

## ATTACK: Hellrider (brief) — located, and it is NOT a deck140 decision
The `ATTACK: Hellrider` name-instead-of-index unparsable is at the **deck109 OPPONENT seat, seq33**
(deck109 attacking with its own Hellrider; `ATTACK: Hellrider` -> choice -1 unparsed_reply -> heuristic,
correctly discarded). **The deck140 seat emits ZERO ATTACK:/BLOCKS: lines all corpus** — it deploys no
creature as an attacker (Blightsteel never reached) and has no creatures to block with, so BLOCKS:none is
untested here too. The attackers name->index reconcile arm (mirroring parseChoice) is a confirmed real
engine gap corpus-wide, but it does NOT touch any deck140 decision. Noted, not re-diagnosed.

## OVERSHOOT / DISPLACEMENT AUDIT (mandatory)
1. SWEEP TRIGGER + belief-kills: read correctly 5th corpus. The "cast nothing at life<=16" cases are the
   reply-protocol under-cast (s9) and a latency fallback (s13), not trigger misreads. FREEZE.
2. Displacement: the residual did NOT move to a new guide seam. It sits at (a) the NAME-ECHO RECONCILE arm
   (absent-named-action -> arbitrary index), (b) the ENGINE (Rakdos X-underpayment, mana-annotation,
   auto-tap stranding, attackers name->index), (c) DECK construction (reach). None is a displaced guide leak.
3. Rakdos hold (#1): obeyed at every legitimate window; the two "wasted Rakdos" events are BELOW the guide
   (name-echo reconcile s40-41; engine X-underpay s43). The guide text was not violated by the model.
4. BSZ-X / X-menu / Staff face-ping / Lightmine / Mulligan / Lands: obeyed where exercised. Freeze.
5. No attack-floor issue (deck runs ~no creatures; Blightsteel never deployed as an attacker this corpus).

## WATCH (do NOT legislate — single seat / precondition-absent)
- **Forced-discard menu misread** (vs133 s12) — Liliana of the Veil +1 forced deck140 to discard; the
  discard-selection prompt renders like a cast menu (`Card [sorcery] [your hand] - "text"`), the model
  read it as a stack response and emitted CHOICE: 0 (Pass) -> unparsed -> heuristic discarded. Low impact
  (forced either way). Representation/label candidate; flag for a 2nd witness (notes ENGINE #x).
- **Early premature sweep** (vs110 s5 t2 life20 C=1, cast Pyroclasm) — trigger strictly not met (life>16,
  no recent attack) but a control deck killing an affinity 1-drop early is defensible. Single, low-stakes.
  WATCH, do not legislate.
- **Phantom-board stale-plan sweep** (carried from wave-11) — no clean recurrence this corpus. Hold.
