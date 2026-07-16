# deck133 wave-13 — development notes (engine / harness / model / deck-construction)

Corpus: PRIMARY `matchups-20260715-183228` (wave-13b, first clean post-crash corpus), binary
`/tmp/wagic-a75e2625e` (Force r2/r3 + auto-tap two-pass + name-echo + BLOCKS:none + 240s timeout),
Spark GPU_MEM_UTIL=0.75 (latency med ~11s, max 155-200s/game). Real games: 110(W), 131(W-adj),
140(L-adj), 135(L), 109(L), 44(L-adj) — 2W/4L. Layer-routing enforced: engine/harness/model items
live HERE with seq repros, never in prompt text. Self-contained repros.

## HARNESS / REPLY-PROTOCOL

### A. [NEW · HIGH — deciding once] Name-echo reconcile: echo-names-no-option → index-wins executes an unrelated in-list card. Route: HARNESS reconcile arm (+ MODEL hallucination).
The model writes `CHOICE: N (Cast Gray Merchant of Asphodel)` when Gray Merchant is NOT among the
offered options (it hallucinates GM in hand, computes a drain). The name-echo reconcile has no
remap target (echoed name matches no option), so index-wins-on-conflict executes option N — an
unrelated in-list card. 6 firings this corpus:
- **vs140** (`1784164978-...-vs-...deck140.jsonl`) **seq45 T13 my16/opp15 — DECIDING.** Options were
  only `[Cast Thoughtseize (opp 1 card) / Cast nothing]`. Reply reasons entirely about Gray Merchant
  ("opponent loses 10 ... wins the game immediately ... CHOICE: 1 (Cast Gray Merchant of Asphodel)").
  Recorded choice=1 → executed `Cast Thoughtseize`, `Your life -2` → my14. Game lost adj 14 vs 15
  (by 1). But-for the 2-life self-payment, deck133 is my16>opp15 at the cap → WINS.
- Non-deciding siblings (index landed safe): vs110 seq31 idx0→pass (already won); vs140 seq47 idx0→
  pass, seq49 idx0→pass; vs135 seq17 idx2→Cast nothing; vs109 seq17 idx3→Decline.
- **Candidate fix (engine/harness ledger, HIGH):** when the echoed card name matches NO offered
  option AND the parsed index maps to a card whose name ≠ the echo, treat as an unresolved conflict
  and FALL BACK to the heuristic (choice=-1) rather than trusting the index. Index-wins is safe only
  when the index's card is the echoed card; an echo of an ABSENT card makes the index arbitrary.
  (Cheaper partial: if the ONLY safe outcome differs by index, prefer the least-committal in-list
  option — but heuristic-fallback is the principled move.)
- Related MODEL surface: same GM-fixation appears in an attacker-phase ramble (vs110 seq27: "I
  should have cast Gray Merchant" → unparsed fallback). Root is model hallucinating an absent finisher.

### B. Last-CHOICE-line-wins reconcile CONFIRMED and works for self-correction (positive).
vs110 (`1784158354-...-vs-...deck110.jsonl`) seq13: reply emits `CHOICE: 1 (Cast Fatal Push)`, then
reasons self-Fatal-Push is "suicide," then closes `CHOICE: 2 (Cast nothing)` → recorded choice=2.
Multi-CHOICE self-correction is honored (last wins). The finding-A cases are all single-CHOICE (no
correction pass), which is why they leak.

### C. Fallbacks: 8, all unparsed_reply ramble-discards, choice=-1, heuristic played. empty_reply = 0.
vs110 s27, vs135 s18, vs109 s19, vs44 s25/s26/s29/s38/s40 — all long-reasoning timeouts (486 to
15k chars) correctly discarded (ramble guard verified). None deciding; verified no self-Fatal-Push
resolved at any fallback window. The wave-12/13a empty-reply flood (Spark death) stays CLOSED here.

### D. Attacker fallbacks are ramble timeouts, NOT the "ATTACK: <name>" name-format shape.
vs110 s27, vs44 s29/s40 (Attackers phase) are unparsed rambles, not name-vs-A#-index parse misses.
The corpus-wide `ATTACK: <name>` ledger candidate (mirroring parseChoice for attacker indices) did
NOT surface at this seat; carry as a corpus item, not a deck133 repro.

### E. `resolved:` field still not emitted (reviewer-facing, carried wave-9/10/11).
The `events` field carried the deltas used to verify GM resolution (x2), the self-Fatal-Push
resolution, and the block. An explicit `resolved: battlefield|countered|graveyard` would remove the
manual narration-scrape. Low priority.

## MODEL / REPRESENTATION

### F. [RECURRENCE of wave-11 class E, MED, non-deciding] Stack-targeting self-Fatal-Push.
vs110 seq15 (opp casting Memnite, hand→stack): option `Cast Fatal Push {b} - the only legal targets
are YOUR OWN right now`; reply "cast Fatal Push *now* in response to the stack to destroy Memnite
before it enters ... CHOICE: 1"; events: own Bloodghast battlefield→graveyard, Memnite stack→
battlefield untouched (Bloodghast returned via landfall). Reasoning DISOBEYS the present annotation
(stack-vs-battlefield / destroy-vs-counter conflation). 1 instance this wave (2 in wave-11) — now a
3rd across waves at this seat. Self-limiting, WON game.
- **Carried representation fix (from wave-11 notes #1):** on a removal option whose only legal
  targets are the pilot's own permanents WHILE an opponent creature-spell is on the stack, name WHY
  the on-stack creature is excluded (e.g. append `(the opponent's <name> is a SPELL on the stack —
  Fatal Push destroys a creature on the battlefield, it cannot target a spell)`). Representation, not
  prompt text; the annotation is present-and-obeyed at other windows (vs110 seq13 self-corrected).

## ENGINE / REPRESENTATION — inherited items CONFIRMED STEADY
- **GM bare-N shown==resolved** (2 real casts). vs110 seq30 drains12 → opp-12/my+12 (lethal); vs135
  seq24 drains4 at my7≤10 → my+4/opp-4 (correct stabilizer). No N→N+2 regression. The "+2"
  workaround stays DELETED. Finding-A is a card-PRESENCE hallucination, distinct from the number layer.
- **Thin/empty-hand Thoughtseize annotation STEADY.** Obeyed at every window reasoned directly
  (vs140 s43, vs135 s17, vs109 s13 all → nothing). The sole thin-hand CAST (vs140 s45) was the
  finding-A collision, not a rule failure.
- **BLOCKS: no misparse at this seat.** The one block window (vs135 s26) was a real declared block
  (spent GM body chump/trade of a 6/6 trampler), not a BLOCKS:none all-decline that swallowed an
  intended block.
- **Fetch / Liliana coexist** — clean; no re-ask inflation, no no-legal-target loop, 0
  fails-validation across stderrs (both Lilianas cast+activated vs131/vs140/vs44).

## DECK CONSTRUCTION (upstream decklist flags — RE-CONFIRMED; do NOT fix with guide length)
- **No early blocker; Bloodghast cannot block.** vs109 T8 blowout (Hellrider+Goblins → my3 by T7,
  no wall available). Won last wave only on a curve-perfect draw (variance). Consider a cheap early
  blocker. Carried from wave-10 #10 / wave-11 #11.
- **No reach / no flyer answer except Gray Merchant.** vs44 (Faeries): dealt 11 (opp 20→9, an
  IMPROVEMENT over wave-11's static-20) but bled to 4 on its own fetch/discard self-damage and lost
  the adjudication. GM density 4/60, no selection; never drawn vs44 or vs140. Carried from wave-9/10/11 #9/#10.
- **Self-damage suite (Thoughtseize 2, Phyrexian Arena 1/turn, fetch 1 each) is disproportionately
  costly under the life-adjudication regime.** vs140 lost by 1 life with self-inflicted Arena+
  Thoughtseize bleed; vs44 lost by 5 with the same pattern. A 1-copy Thoughtseize→Inquisition shift
  and cutting Phyrexian Arena would reduce self-damage in life-capped games. Construction, not a
  guide gap. Carried from wave-10 #11 / wave-11 #12.

## WHAT WORKED (freeze — do not churn)
GM bare-N correct on both real casts (lethal vs110, stabilizer vs135). Thin-hand Thoughtseize
discipline obeyed wherever reasoned. Control-lock line executed cleanly vs131 (legit adj win, ahead
for the right reasons). Last-CHOICE self-correction rescued a would-be self-Fatal-Push (vs110 s13).
Ramble guard verified (8 fallbacks, all choice=-1, zero prose casts). Guide is a ZERO-edit frozen diff.
