# General suggestions (core prompt) — deck109 wave-25

**PASS**, with ONE single-instance observation routed to synthesis for a convergence check (do NOT
adopt on this seat's evidence alone — the 19-PASS core streak should break only on convergence).

## Convergence CANDIDATE (single instance at this seat): coded-answer commit under prose reversal.
**vs131 s10** exposed a gap between the reply protocol and the model's answer-first-then-reverse
habit. The reply opened `CHOICE: 3 (Cast nothing)`, reasoned to the opposite conclusion, and ended
*"So I will cast Searing Spear at Young Pyromancer... So Choice 1"* with a matching PLAN — but never
re-emitted a coded `CHOICE: 1` line. The parser took the only coded line (`CHOICE: 3`) and recorded
the OPPOSITE of the model's actual final decision. b2's "last coded line" precedence is not at fault
(vs59 s13 proves it: coded lines [2,1,1] -> correctly recorded 1); the gap is that a **prose-only
reversal is invisible to a coded-line parser**.

Two possible core-prompt seams, for the synthesis agent to weigh IF other seats show the same shape:
1. **Protocol clause (reply-protocol layer):** "Your `CHOICE:` line is binding. If your reasoning
   changes your answer, you MUST emit a NEW `CHOICE: N` line as the last coded line — do not reverse
   your answer in prose only." This is the cleanest fix and matches the answer-first design intent
   (front the answer, but let the final coded line win on natural stop).
2. **Parser-side (notes.md / harness, NOT core):** on a naturally-terminated reply whose concluding
   PLAN/prose names a different option than the answer-first coded line, prefer... — but this is
   fragile (prose is numeric-noisy; the plan-split already strips it) and belongs in the ledger, not
   the prompt.

**Recommendation:** route seam (1) to the synthesis agent as a convergence candidate only. A single
outcome-neutral instance at one seat does not clear the core bar; if deck133/deck27 (the seats with
the longest deliberation spirals) show the same answer-first-then-prose-reversal, it converges and
earns the protocol clause. Otherwise PASS holds.

Nothing else at this seat routes to core. All previously-shipped core surfaces (answer-first,
attack-first, face-by-default, deploy-and-attack) held clean.
