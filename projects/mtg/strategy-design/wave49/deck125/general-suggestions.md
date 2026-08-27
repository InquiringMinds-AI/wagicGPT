# deck125 - render / core-prompt proposals (wave 49), layer-routed

Engine bugs are in `wave49/seats/seat-125-126.md` (engine list); these are render/prompt items.

G1 (HIGH, render) - the `legal targets right now:` clause on a cast row is the last uncollapsed
enumeration. `...deck125-0x561c77675570-vs-ai_baka_deck123.jsonl` seq 13 (t10, Main phase 1) is
the corpus's largest prompt at 34,333 chars; 28,755 of them are ONE row: `3. Cast Path to Exile
{w} ... - legal targets right now: Lord of Lineage (creature 5/5) [...], Vampire #2 (creature 4/4)
[flying, doesn't untap during its controller's untap step], Vampire #3 (creature 4/4) [...], ...
Vampire #322 (creature 4/4) [...]` - 321 identical-text token rows written out one by one, while
the battlefield line two paragraphs up already collapsed the same tokens (lane O grouping) and
the opponent header read `329 permanents listed, of which 323 are creatures`. Proposal: apply the
battlefield/A-row range collapse to the target clause (`Vampire #2-#322 (creature 4/4) [flying,
...] x321`), same grammar the model already reads. Lane R's "no prompt > 30,000" prediction fails
on this one record and nothing else.

G2 (MED, render) - the sweeper row's second number `(K able to attack)` is 0 on the caster's own
turn for summoning-sick and tapped creatures as well as for walls, so on the seat's turn it reads
0 for a board of 323 fresh tokens (seq 13 above: `destroys 323 of their creatures (0 able to
attack)`) and 0 for five walls (`...vs-ai_baka_deck126.jsonl` seq 60: `exiles 5 ... (0 able to
attack)`). The guide keys the decision to the header's "N of them without a restriction against
attacking" instead, which separates those two boards; the row could print that same count
(`5 of their creatures (all 5 defenders)` / `323 of their creatures (323 able to attack next
turn)`) so the row alone carries the decision the guide makes from two surfaces.

G3 (LOW, core prompt) - late-game land-drop declines. Four `Play no land right now` replies this
corpus (`...vs-ai_baka_deck146.jsonl` seq 94; `...vs-ai_baka_deck130.jsonl` seq 80, 98, 100), all
with 12+ lands out and 7-8 cards in hand, bare replies. Not a guide matter beyond #0's existing
text; recorded so the land-drop counter (W44) is read against 88 offers, not assumed 100%.

G4 (INFO) - Emrakul's `card text` promises "take an extra turn after this one" and the engine
DOES give it (`...vs-ai_baka_deck146.jsonl`: "Turn 36 - YOUR turn ... cast Emrakul ... Turn 37 -
YOUR turn"); checked because the primitive's auto= lines show only the annihilator - the extra
turn is implemented elsewhere. No item.
