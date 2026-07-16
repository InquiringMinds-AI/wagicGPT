# deck133 wave-13 — core-prompt (general-strategy.txt) proposals

**Verdict: PASS (no core prompt-body edit).** No 2+-seat core candidate at this seat.

deck133's one deciding failure this wave (the Gray-Merchant-not-offered hallucination + name-echo
index-collision, findings #1) is a HARNESS/MODEL item, not a core-prompt-writable one — the core/
guide already instruct the pilot to confirm the card it names appears in the list, and the pilot
disobeyed. Louder core prose against a present-and-disobeyed instruction is the sanctioned-against
escalation. The convergence gate also forbids a single-seat item adding a core line. Layer-routing:
the reconcile-arm fix and the stack-targeting representation refinement go to notes.md, never prompt
text. Core stays on its standing PASS.

## Single-seat parked item (NOT proposed — flagged for a 2nd-seat grep by synthesis)

**A name-echo CHOICE can name a card that is in NO offered option (a card-PRESENCE hallucination,
not a wording variant); the reconcile's index-wins-on-conflict then executes an unrelated in-list
option, which can be a harmful, guide-forbidden cast. The durable lever is the HARNESS reconcile
arm (fall back to heuristic when the echoed card matches no option and the index maps to a different
card), never a core prose line.**

Grounded in vs140 seq45 (deciding: echo "Gray Merchant" absent from options → index 1 → thin-hand
Thoughtseize → -2 life → flipped a 1-life adjudication loss) plus 5 non-deciding sibling firings
(vs110 s31, vs140 s47/s49, vs135 s17, vs109 s17, all landing on pass/nothing/decline).

Why PARKED, not proposed:
1. **Single seat, one corpus, 1 of 6 firings deciding** — fails the convergence gate.
2. It is a **model hallucination + harness reconcile** issue, not an instruction gap: the guide
   already forbids naming an unlisted card and the pilot reasoned past it (instruction-beaten-by-
   hallucination). The candidate durable fix is the reconcile arm (notes #A), not prose.
3. Five of six firings were self-limiting (index landed on a safe pass/decline).

**Reactivation trigger for synthesis:** a SECOND seat this wave whose name-echo CHOICE names a card
that is in NO offered option AND whose parsed index executes a different, non-trivial in-list card
(especially a beloved finisher/bomb the deck is fixated on — GM here; watch for the analogous
"missing-bomb" fixation at other seats). If it recurs cross-seat, promote the reconcile-arm
fallback (echo-names-no-option → heuristic) to the engine ledger as a shipped item; still no core line.
