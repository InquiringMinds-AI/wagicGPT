# wave-57 proposals for the strategy-writing skill — from the deck125 seat

Evidence base: `matchups-20260903-174505`, deck125's 909 decisions across 6 seat logs. Each
proposal names the corpus fact that pays for it. Nothing here is proposed on taste.

## #S1 (STRONG) — A RULE MUST BE TRUE AT THE WINDOW IT FIRES IN, NOT AT THE WINDOW IT WAS LEARNED IN

Amendment #188 makes us verify literals. Nothing yet makes us verify a rule's PREMISE against the
window the rule is answered at.

deck125's `YOUR X IS 3` rule was written from cast-menu evidence (over-drawing into a seven-card
hand, discard punishers) and it is answered at the ANNOUNCE_X window — a different window, whose
own first line reads `Mana available: 0 total (no untapped sources) | Already in pool:
{u}{u}{w}{w}{w} (5 mana ALREADY produced and floating right now)`. At that window the mana is
already out of the lands, so the rule's implicit "save the rest for a counterspell" is false by
construction, and its explicit "cards above seven are discarded" is a cost of zero against a
benefit of one life per point. The model obeyed the rule perfectly — 3 of 3 where the largest X
exceeded 3 — and threw away 4 life and 4 cards, one of them at 5 life in a game lost at −3.

Proposed skill rule: **when a rule fires at a menu the pilot reaches through ANOTHER menu (an
X announce, a target chooser, a mode chooser, a blocker assignment), the rule's cost claim must be
re-derived from the SECOND window's own resource line.** In practice: quote the second window's
resource line in the rule, the way we already quote the row.

## #S2 (STRONG) — ONE RULE, ONE PLACE: A GUIDE MAY NOT STATE THE SAME RULE THREE TIMES WITH THREE TRIGGERS

deck125's low-life Revelation floor existed in the live guide in three forms, 70 lines apart:
a "unless your life is 10 or less" clause inside the max-affordable-3 paragraph, an
attacking-power override, and a "5 life or less whenever their header's second number is 1 or
more" sentence. All three were true. The seat, at 3 life with `max affordable X=2` on the menu,
answered `Cast nothing right now` **five times across two turns** and died at −1.

The failure mode is not that the pilot missed a rule. It is that the pilot has to decide which of
three overlapping exceptions governs a board, under a paragraph whose headline says the opposite.
Proposed skill rule: **when a corpus shows a rule broken at a board that TWO OR MORE of the guide's
own clauses already cover, the edit is to MERGE them into one clause at the head of the section,
not to add a fourth.** A count of how many clauses cover a broken board is a cheap review metric
and we should record it in the seat file when we find one.

## #S3 (STRONG) — A CEILING THAT HAS NEVER BEEN TESTED AGAINST SURVIVAL IS NOT A CEILING

The skill treats a 0-of-N ceiling as a rule at rest. Two of deck125's ceilings held this corpus and
one of them lost the game: `Lightmine Field 0 casts of 24 rows` is the same ceiling that, at 3 life
against two creatures both dead to the card's own printed damage, forbade the play that survives.
The ceiling was a VALUE floor ("it only punishes a crowd") applied at a SURVIVAL board.

Proposed skill rule: **every value floor in a guide must carry an explicit survival carve-out, or
be marked in the seat file as untested against survival.** The test is mechanical: does the rule
still say "no" on a board where the printed incoming damage meets or exceeds the seat's life? If it
does and the reviewer cannot cite a window where saying no there was right, it is a defect.

## #S4 (MEDIUM) — THE MANDATED PLAN LINE IS AN INSTRUCTION CHANNEL, AND WE HAVE BEEN USING IT AS A LABEL

deck125's guide prescribes a verbatim PLAN string, and the engine echoes the seat's last PLAN into
the header of every subsequent prompt (`YOUR PLAN (as you last stated it, 2 windows ago on turn
45): ...`). That echo is the only guide-authored text guaranteed to be in front of the model at
every window, at zero prompt cost we do not already pay.

The Emrakul-under-a-converter stop has now failed in four corpora as guide PROSE, in progressively
louder paragraphs, including this corpus where the row itself printed the finished price. Wave 57
moves the stop into the mandated PLAN string. Proposed skill rule: **a rule that has broken in
three or more corpora as prose should be re-carried, not re-worded — and for guides that mandate a
PLAN line, the PLAN line is the first re-carrier to try.** Falsifier: W123.

## #S5 (MEDIUM) — WHEN THE RENDER COMPLETES, SAY SO, AND STOP ADDING PROSE

Wave 56's HIGH #3 asked for a converter price on the Emrakul row. Lane C's D7 shipped it, exactly
as asked, on exactly that row. The seat took the row anyway. The correct reviewer move is to
record **"the perception half is closed"** as an explicit verdict line, because it changes what the
next wave is allowed to propose: once the render is complete, another paragraph is the option we
have already falsified four times, and the remaining moves are structural (carrier, ordering,
deletion) — see #S4.

Proposed skill rule: **a seat file should carry, per repeated failure, a one-line PERCEPTION STATUS
(open / closed) so the next reviewer cannot re-request an ask that already shipped.**

## #S6 (LOW) — CEILING CLAIMS IN A GUIDE MUST CARRY THEIR DENOMINATOR AND THEIR DATE

Two of the live deck125 guide's ceiling sentences were stale enough to mislead this corpus's read
("The list is bare card names with no {right now:} verdict on any row" — false as of lane C's D8;
"Eighty-five land windows last corpus, eighty-five lands played"). Both are cheap to keep true and
both are load-bearing: the first one told the pilot not to look at the tags the engine had just
started printing for it.

Proposed skill rule (small): **any sentence in a guide that describes what the ENGINE prints is a
literal claim under #188 and must be re-verified against the current corpus every wave, not only
the numbers.**

## What this seat does NOT propose

- No change to the trust doctrine's direction. See `general-suggestions.md` #G2: this corpus has
  the first record of a rendered number teaching the pilot to distrust the render, and the fix
  belongs in the engine, not in the doctrine.
- No change to the perception-vs-strategy routing itself. It routed all four of this seat's items
  correctly; #S5 only asks that its verdicts be written down per failure rather than per wave.
