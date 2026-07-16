# Wave 13 review — deck133 (Phyrexian Asphodel, mono-black devotion midrange)

Corpus: PRIMARY `matchups-20260715-183228` (wave-13b, first CLEAN post-crash corpus; 21 games
round-robin over 44,109,110,131,133,135,140). Binary `/tmp/wagic-a75e2625e` (Force r2/r3 +
auto-tap two-pass + name-echo protocol + BLOCKS:none + 240s timeout). Wave-11 guides FROZEN
(byte-identical, `diff -q` clean — freeze-check below). deck133 was the GPT seat in 6 games.
Spark ran at GPU_MEM_UTIL=0.75 → decision latency med ~11s / max 155-200s per game; 13/21 corpus
games life-adjudicated (deck133: 3 of its 6). Per brief, adjudication count is NOT a play-quality
signal — the adjudicated POSITIONS are evaluated below.

**Real record 2W / 4L — down from wave-11's 5W/1L, SAME opponents.**

| opp | result | end my/opp | last T | mode | note |
|-----|--------|-----------|--------|------|------|
| 110 | **WON** | 26 / **-5** | 13 | clean kill | GM drain-12 lethal (shown==resolved); 1 stack-target self-FP (won anyway) |
| 131 | **WON** | 22 / 18 | 6(adj) | adj, ahead-right | control lock: discard+2xLiliana emptied opp board; legit adj win |
| 140 | LOST | **14 / 15** | 14(adj) | adj by **1 life** | **DECIDING decision error** — GM-hallucination CHOICE collision → thin-hand Thoughtseize (-2 life) flipped it (finding #1) |
| 135 | LOST | **-8** / 12 | 14 | clean kill vs it | fast 6/6-Treefolk beatdown; GM stabilizer only drain-4; no early wall (structural) |
| 109 | LOST | **0** / 20 | 8 | clean kill vs it | red aggro (Hellrider+Goblins); no early blocker, Bloodghast can't block — variance revert of wave-11's flip-WIN |
| 44  | LOST | **4** / 9 | 15(adj) | adj | Faerie no-reach; dealt 11 (opp 20→9, better than wave-11 static-20) but self-damage race lost on life |

**Verdict on the 5/6→2/6 drop:** DOMINANTLY opponent/draw variance + the latency-driven
adjudication regime, which snapshots deck133's slow devotion development mid-build and exposes its
STRUCTURAL weaknesses (no early blocker, no reach, self-damage suite) as life deficits. Of the four
losses, THREE are structural/variance (vs109, vs135, vs44) and were flagged as such in wave-11
(wave-11's own notes called the vs109 WIN "variance, not a construction change"). EXACTLY ONE loss
(vs140) carries a genuine, deciding, decision-quality failure — and it is a NEW harness/model
collision class, NOT guide drift. Guide-obedience where the model reasoned directly stayed high
(GM bare-N, thin-hand Thoughtseize, GM-as-stabilizer all obeyed). **Guide FROZEN byte-identical.**

---

## FINDING 1 — [HIGH · DECIDING · NEW class] Gray-Merchant-not-offered hallucination + name-echo reconcile index-collision. Route: MODEL + HARNESS (ledger notes #A). NOT guide-writable.

The model repeatedly writes `CHOICE: N (Cast Gray Merchant of Asphodel)` when Gray Merchant is NOT
among the offered options — it hallucinates GM in hand/castable, computes a devotion drain, and
emits an index. The name-echo reconcile CANNOT rescue this: the echoed name ("Gray Merchant")
matches NO offered option, so there is no unique remap target → index-wins-on-conflict fires →
the engine executes whatever in-list option sits at index N. Because N is essentially arbitrary
when the model is hallucinating an absent card, safety is luck.

**6 firings this corpus (all single-CHOICE-line, no self-correction):**
- vs110 seq31 (T13, my26/opp2): idx=0 → `pass`. SAFE (game already won).
- **vs140 seq45 (T13 MP2, my16/opp15): idx=1 → `Cast Thoughtseize` (thin, opp 1 card) → my16→my14. DECIDING.**
- vs140 seq47 (T13, my14/opp15): idx=0 → `pass`. safe.
- vs140 seq49 (T15, my13/opp15): idx=0 → `pass` (option list was a single fetch put-in-play). safe.
- vs135 seq17 (T7, my17/opp19): idx=2 → `Cast nothing`. safe.
- vs109 seq17 (T7, my3/opp20): idx=3 → `Decline - do nothing`. safe.

**Repro of the deciding one (vs140 seq45):** options were exactly
`["Cast Thoughtseize {b} - ... the opponent holds only 1 card", "Cast nothing right now"]`
(Gray Merchant absent). The reply reasons ENTIRELY about Gray Merchant — "casting Gray Merchant of
Asphodel ... opponent loses 10 life (15-10=5) ... This spell wins the game immediately. The
strategy guide explicitly states: 'N >= their life -> cast him, you win now.' ... CHOICE: 1 (Cast
Gray Merchant of Asphodel)." Recorded `choice=1`; `chosen_text="Cast Thoughtseize ..."`; events
`Your life -2`. The game was lost on adjudication 14 vs 15 (by 1). The 2-life self-payment ≥ the
1-life margin: had index 1 mapped to nothing (or had the reconcile fallen back to heuristic),
deck133 sits at my16 > opp15 at the cap and WINS the adjudication.

**Attribution & why not the guide:**
- The guide ALREADY forbids this exactly: *"Choose FROM THE NUMBERED LIST ONLY: before you write a
  plan, confirm the card you name appears in the list. If it does not, cast the best card that IS
  listed."* At seq45 the model named a card NOT in the list and, instead of casting the best listed
  card, emitted an index that executed the guide-forbidden thin-hand Thoughtseize. This is a
  PRESENT-AND-DISOBEYED instruction → louder guide prose is the sanctioned-against escalation
  (representation-beats-instruction). Guide stays frozen.
- MODEL layer: hallucinating an absent finisher (also miscounts devotion as 10 for a card it can't
  cast). Same GM-fixation surfaces at vs110 seq27 (attacker-phase ramble "I should have cast Gray
  Merchant" → unparsed fallback).
- HARNESS layer (the durable lever): the name-echo reconcile should treat "echo names a card that
  matches NO option AND the parsed index maps to a card whose name ≠ the echo" as a conflict that
  FALLS BACK to the heuristic (choice=-1), not one that trusts the index. Index-wins is safe only
  when the index's card is what the echo names; when the echoed card is absent, index-wins can
  execute an unintended harmful in-list option. Ledger notes #A (HIGH — deciding once).

Contrast (mechanism is real, not a scan artifact): the harness takes the LAST CHOICE line, which
DOES rescue self-correction. vs110 seq13 the model first wrote `CHOICE: 1 (Cast Fatal Push)`, then
reasoned that self-Fatal-Push is "suicide," and closed `CHOICE: 2 (Cast nothing)` → recorded
choice=2. Multi-CHOICE self-correction works; the GM-hallucination cases are all single-CHOICE with
no correction pass.

---

## FINDING 2 — [MED · non-deciding · RECURRENCE of wave-11 class E] Stack-targeting self-Fatal-Push. Route: MODEL + REPRESENTATION (ledger notes #B, already parked wave-11).

vs110 seq15 (T10... T7 opp turn, my16/opp19): opponent casting Memnite (hand→stack). Option:
`Cast Fatal Push {b} - the only legal targets are YOUR OWN right now`. Reply reasons "I can cast
Fatal Push *now* in response to the stack to destroy Memnite before it enters ... CHOICE: 1."
`choice=1` → events `Your Fatal Push: hand→stack / Your Bloodghast: battlefield→graveyard /
Opponent's Memnite: stack→battlefield` (own Bloodghast destroyed, returned via landfall; Memnite
resolved untouched). Identical mechanism to wave-11 findings E (stack-vs-battlefield / destroy-vs-
counter conflation), reasoning DISOBEYS the present annotation. **1 instance this wave** (2 in
wave-11); self-limiting (own Bloodghast returns), in a WON game — non-deciding. Now a 3rd instance
of this class at deck133 across waves → carry the wave-11 representation refinement in the ledger.
NOTE for synthesis cross-seat grep: this is a second-wave recurrence at the SAME seat; the wave-11
parked reactivation trigger asked for a SECOND SEAT — a synthesis-level check, not declarable here.

---

## VERIFIED-GOOD (no regression; the wave-11 termini hold)

- **GM bare-N shown==resolved** (no N→N+2). vs110 seq30 `drains 12` → opp-12 (→2)/my+12 (→26),
  lethal. vs135 seq24 `drains 4` at my7≤10 → my+4/opp-4, correct STABILIZER cast per guide. Both
  when GM was actually offered. The number layer is clean; finding #1 is a card-PRESENCE hallucination,
  a different failure than the wave-10 number leak.
- **Thin/empty-hand Thoughtseize rule OBEYED wherever reasoned directly.** vs140 seq43 (same turn as
  the deciding error, opp 1 card) DECLINED (`Cast nothing`); vs135 seq17 (opp 2 cards) → nothing;
  vs109 seq13 (opp 2 cards) → nothing. The single thin-hand CAST (vs140 seq45) was the GM-hallucination
  collision, NOT a Thoughtseize-rule violation — the rule works; the collision bypassed it.
- **Blocks: the one block window was a real block, not a BLOCKS:none misparse.** vs135 seq26 (my11)
  declared `Gray Merchant of Asphodel blocks Abominable Treefolk` — a SPENT GM body (ETB drain
  already used at seq24) chump/trading a 6/6 trampler, reducing 6→2 face damage and enabling a
  potential Liliana-return second drain. Defensible; not a finisher thrown away.
- **Attacker fallbacks are ramble-discards, not the "ATTACK: <name>" name-format shape.** vs110
  seq27, vs44 seq29/seq40 are long-reasoning timeouts (13-15k chars) correctly discarded to
  choice=-1 (heuristic). No name-vs-A#-index attacker parse miss at this seat.

## MANDATORY AUDITS

- **Fallbacks (8, all unparsed_reply ramble-discards, choice=-1):** vs110 s27, vs135 s18, vs109 s19,
  vs44 s25/s26/s29/s38/s40. empty_reply = 0 (Spark-death class CLOSED). None were deciding: every
  one fell to a safe heuristic line; verified no self-Fatal-Push resolved at any fallback window.
- **vs140 adjudicated LOSS — ahead-for-wrong-reasons at the cap:** deck133 held 17 life most of the
  game (opp 25→15) but its OWN self-damage dropped it below opp at the cap. Contributors, in order:
  the deciding GM-hallucination Thoughtseize (-2, finding #1); Phyrexian Arena cast seq36 (T11) for
  ~3 upkeep-life — reply is lucid ("1 life negligible ... find Gray Merchant"), the guide's "skip
  Arena" is scoped to DEFENDER and deck133 read itself as beatdown (opp no creatures, 2 attackers),
  so DEFENSIBLE-but-costly under a life-adjudication regime, not a clean violation. GM never drawn/
  offered all game (density 4/60 — construction, notes #E).
- **vs131 adjudicated WIN — ahead-for-RIGHT-reasons:** at the T6 cap, both boards empty of creatures,
  deck133 22 vs opp 18 with a Liliana of the Veil at 4 loyalty vs a Young-Pyromancer deck it had
  discard-locked. Legit control-role adjudication; the "zero creatures by T7" is correct for the
  control matchup, not a durdle.
- **vs109 fast LOSS is structural, not misplay:** T1 Thoughtseize took Pillar of Flame, T3 Fatal
  Push killed Stromkirk Noble, T5 Bloodghast (can't block). Hellrider + 2 Goblins put deck133 to
  my3 by T7; Collective Brutality (seq16/17, seq19/20) removes only one body — cannot save my3 vs
  Hellrider's attack-triggers. No early wall exists in the deck (wave-11 note #11). Variance revert
  of wave-11's draw-dependent vs109 win.

## FREEZE-CHECK (every kept line accounted for; ZERO edits)
Live guide BYTE-IDENTICAL to wave-11 output (`diff -q` clean).
- **Identity + beats-are-the-win + impact-cast list** — vindicated (vs110 GM-12 kill, vs131 control
  lock). KEPT.
- **GM block ("use the shown number directly", bare-N)** — shown==resolved x2 (verified-good). KEPT.
- **Thoughtseize thin/empty-hand rule + DECIDING-SITUATIONS line** — OBEYED wherever reasoned; the
  one violation was a GM-hallucination collision the guide already forbids ("confirm the card you
  name appears in the list"). Making it louder = sanctioned-against escalation. KEPT at full prominence.
- **own-targets Fatal Push annotation** — obeyed at seq13 via self-correction, disobeyed once at
  seq15 via the stack-targeting model misconception (finding #2, representation not guide). KEPT.
- **DEFENDER/Obliterator-wall, VS RED/BURN, Bloodghast, Fatal Push, Collective Brutality, Liliana,
  fetch, Yawgmoth, Geralf's, Arena, mulligan** — obeyed or untested. KEPT VERBATIM.
Nothing dropped. **Only change this wave is ZERO.**

## In my words
The record halved on the same opponents, but it did not break the way the number suggests. Three of
the four losses (vs109 red aggro, vs135 fast beatdown, vs44 Faeries) are the deck's known structural
holes — no early blocker, no reach, a self-damaging discard/fetch suite — being exposed by a
latency-adjudicated corpus that freezes the game mid-devotion-build. Wave-11 itself called the vs109
result variance; this wave the variance simply landed the other way. The one loss that is genuinely
a decision failure is vs140, and it is a clean, deciding, NEW class: the model wanted its Gray
Merchant finisher so badly it wrote "CHOICE: 1 (Cast Gray Merchant)" when Gray Merchant wasn't even
offered — and index 1 was the guide-forbidden thin-hand Thoughtseize, whose 2 self-life was the
exact 1-life margin it lost the adjudication by. The name-echo protocol can't catch this because the
echoed card matches no option, so index-wins executes an unrelated in-list card; five other firings
of the same hallucination happened to land on pass/nothing and cost nothing. That's a harness
reconcile fix (fall back when the echoed card is absent) plus a model hallucination — the guide
already tells the pilot to confirm its card is on the list, and the pilot ignored it. Everything the
model reasoned about directly held: Gray Merchant's bare-N was shown==resolved on both real casts,
the thin-hand Thoughtseize rule was obeyed at three windows, the lone block was a sane spent-body
chump. So the guide freezes clean, and the wave's real product is one ledger item for the reconcile
arm.
