# Wave-58 synthesis notes

Inputs: `deck{123,125,126,130}/general-proposals.md` (12 proposals), `deck{123,125}/skill-proposals.md`
(2 proposals), all seven `deck<N>/review.md`, and `engine-seat.md`. Outputs:
`wave58/general-strategy.md` (wave-57 body carried verbatim + R319-R330 adopted, two forward-pointers
added at R295 and R225) and `wave58/strategy-writing-skill.md` (byte-verbatim copy of
`wave57/strategy-writing-skill.md` — prefix `cmp`-verified over the first 1,299,175 bytes — with
amendments **330-331** appended).

**Score: 14 proposals, 14 ADOPT, 0 REJECT** (one adopted with a scope condition). No proposal
restated an existing R-item or an existing amendment; every one names the wave-57 item it extends.

## General-guide proposals

| # | proposal | verdict | why (one line) |
|---|---|---|---|
| deck130 P1 | annotate a `moveto(hand)` activation with what the returned card does | **ADOPT → R327** | 14 consecutive passes on the deck's only remaining reach, with the opponent at 4; every CAST row on that seat carries the clause this row lacks. |
| deck130 P2 | price a REPEATABLE activation over the payments affordable now | **ADOPT → R325** | `SURVIVES` is true of one payment and false of the window (seq110/113); the repeat count already gates the row's legality, so it is a withheld fact, not a new computation. |
| deck130 P3 | sum damage already ON THE STACK against the pilot's own life | **ADOPT → R326** | seq114 at 1 life prints a FUTURE draw-step forecast and nothing about the 3 pending damage in front of it, on the last window a response was possible. |
| deck123 G1 | own-turn crack-back forecast, counted over creatures that WILL untap | **ADOPT → R319** | R295's own first sentence names the own-turn window; lane C shipped D9 gated on the opponent's turn (engine-seat: PASS 10 of 10 there), so this is the residual, not a restatement — and an able-to-attack-now count renders 0 on the board that killed the seat. |
| deck123 G2 | cast menu names the hand cards omitted for mana | **ADOPT → R320** (merged with deck126 P3) | Same defect filed by three seats (deck123 7 re-asks, deck126 s34, deck162 HIGH-3); the emitter already computed the shortfall when it suppressed the row. |
| deck126 P1 | a HOLD take is not re-opened by a life-total change alone | **ADOPT → R322** | 32 byte-identical asks inside one upkeep (seq 56-87) while the header's life ran 17/17 → 33/0; scoping the latch to `options_text` + stack + permanents removes no window the pilot did not itself close, so the standing anti-cap ruling does not reach it. deck146 MED-3 files the same shape independently. |
| deck126 P2 | a search/reveal row carries the price its cast row will carry | **ADOPT → R321** | s47 fetched a card that s48 then badged `this KILLS you`, spending the game's last live window; every input was on the board at search time. |
| deck126 P3 | say why a hand card has no cast row | **ADOPT → R320** | Merged with deck123 G2 above — one item, three seats' evidence. |
| deck125 P1 | `empty_reply` is a ~2.5 s deadline, not a refusal | **ADOPT → R328** | 89 of 89 in a 56 ms latency band, in runs up to 10, with nothing in stderr; engine-seat HIGH-2/HIGH-3 derived it independently and ties it to F2's precondition. |
| deck125 P2 | `PUT:` parser accepts one trailing name gloss | **ADOPT → R329** | The protocol teaches the gloss on `CHOICE:` and refuses it on `PUT:` (s83); adopted with its negatives, per R313's pattern. |
| deck125 P3 | damage-to-attackers row prints its kill count | **ADOPT → R323** | The row prints N two rows from a sweeper row that prints its own kill count; the evaluator already enumerates the able-to-attack set. |
| deck125 P4 | the `[<- largest affordable X]` marker must read its own row's NET tail | **ADOPT → R324** | Not a restatement of R298 (that is the `best trade` definition): this is a marker asserting "no listed X does more" over a row the render had already priced lethal, taken 1 of 1, game over. Same rule as R303 — a printed recommendation is conservative in the non-lethal direction or it is not printed. |
| deck125 P5 | the converter paragraph names the SAFE direction | **ADOPT WITH SCOPE → R330** | True and cheap for the converter on the board, but a blanket "life you gain is never converted" would be false against a converter that triggers on an opponent's life gain; adopted only as a clause generated from the named converter's own primitive direction. |

## Strategy-writing-skill proposals

| # | proposal | verdict | why |
|---|---|---|---|
| deck123 S1 | scope every mandatory PLAN ritual to a named window class, in the ritual's own text | **ADOPT → #330** | Not #181 (both rituals pass #181's observable gate) and not #282/#309 (which govern one ritual's content and actively mint new ones): this is the collision between two, with the receipt — the pilot wrote Rule 3's number `M=0` under Rule 1's label at s22 and never entered Rule 3. |
| deck125 S1 | a constant FLOOR is a proxy; write it as the card's own test where the render prints the operands | **ADOPT → #331** | Extends #319 (arithmetic for a render marker) and #321 (re-key an unactionable key) to the guide's own constants; the Lightmine Field floor held at its ceiling — 43 rows, 1 cast, no argument — and was wrong on the card. Keeps its own non-licence clause (a value floor with no printed threshold stays a constant). |

## Deck-guide edits checked against the adopted general changes

No contradiction found. Three convergences, each of which becomes a `perception status: CLOSED`
obligation (#310) for the seat's NEXT revision if the render ships:

- **deck123 E1** (`CHECK 0 - THE CRACK-BACK`, adding tapped power by hand on the pilot's own turn) is
  the guide half of **R319**. It compensates for an ABSENT number from true surfaces, which #287
  permits — it is not a hedge against a printed bound (#324, the wave-57 rejection).
- **deck125 E1** (Lightmine Field's constant floor → a three-branch kill test) is the guide half of
  **R323** and the worked case behind **#331**. Consistent: the guide derives the kill count the
  render would print.
- **deck125 E3/E4** (the converter runs one way; announce the largest X whose NET leaves life above 0)
  are the guide halves of **R330** and **R324**. Consistent — the render items make the same fact
  visible at the window rather than replacing the rungs.

deck126, deck130 and deck162 EDIT verdicts and the deck146 / deck152 KEEP verdicts contain nothing
that an adopted item contradicts. **No deck guide was edited by this step.**

## Not carried by this step

- The engine seat's own defect list (HIGH-1's 11 false `{dead right now:}` discard badges, MED-4's
  unpriced marker X, MED-5, MED-6, MED-7, LOW-8) is an ENGINE-ledger input, not a proposal, and no
  seat filed it as a general-guide item — it is left for the docket, not folded in here.
- Lane E and wave-57 D22 are UNTESTED by construction in this corpus (headless AI-vs-AI;
  `WAGIC_BLOCKLOG` never set by the harness). Nothing in the adopted set depends on either.
- I did not re-derive any seat's counts from the translogs; every number above is quoted from the
  proposal or review that filed it, cross-checked against `engine-seat.md` where the two overlap
  (the 89/2502-2558 ms `empty_reply` band, the 10 `named_row_reask`, the HOLD census, D9's 10 of 10).
  Where a proposal's count had no second source (deck130's 14 upkeeps, deck125's 43 Lightmine rows,
  deck126's 32-ask run), it is carried on the seat's authority.
