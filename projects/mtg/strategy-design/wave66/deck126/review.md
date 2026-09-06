# Wave 66 — deck 126 per-deck review

Seat: `ai_baka_deck126` (Sanguine Bond / Exquisite Blood + defenders). Record **3/6**
(W vs 130, 123, 125; L vs 146, 152, 162).
Corpus dir: `/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-064038/`.
Seat census: 281 decisions (218 ask, 37 priority, 7 reveal, 6 blockers, 6 attackers, 1 bottom),
**24 `unparsed_reply` (8.5%, vs the corpus 3.6%)**, 3 `named_row_reask`,
**2 `reveal_stall_forced` — both of the corpus's two are on this seat**.
HOLD takes 47 : plain `Cast nothing` 9. No `repeat_past_stop`, no discard ask, no wall miss.

---

## 1. Game by game

**L vs 146** (`1788694840-…deck126-0x55e5f2ecd420-vs-…deck146`, turn 17, 0–21).
Sanguine Bond landed t10 (seq14) and was **exiled by Vanishing Verse** on t11; Silverquill
Silencer then named Exquisite Blood, so seq15 (t12) cast the Blood for 3 life at 15 life —
guide-correct under Rule #7 entry 1's printed-price clause (the row did not say "KILLS you").
Deciding decision **seq16, t14, life 6**: menu row 1 was `Cast Tribute to Hunger` against a
board of 3 attackers; the model took row 3, a second `Idyllic Tutor`, and reached t16 at 1 life
with **0 creatures on its battlefield** (seq19 rendered `Your battlefield (5 permanents … of
which 0 are creatures)`) while holding 2 Pride Guardians and a Perimeter Captain since t12.
Guide-compliant (Rule #7 entry 2 outranks walls) and lost anyway; the removal, not the ordering,
is what decided it.
seq7 (t6) was an `unparsed_reply` — see HIGH-1.

**L vs 152** (`…-0x55bfc4171d20-vs-…deck152`, turn 13, −5–20). Katilda aggro. seq9 the model
answered `CHOICE: 1 (Cast Sanguine Bond)` to a menu whose row 1 was Tribute and which had no
Bond row at all — `named_row_reask` caught it correctly and the re-ask (seq11) cast Sorin.
Deciding decision **seq17, t12, life 4**: rows 4/5 were walls leaving 3 sources, row 1 was
Sanguine Bond "casting this taps you out". It took row 1 per Rule #7 entry 1 and died on the
crack-back. Bond alone with no life-gain source and no Blood is a zero-impact card at 4 life; the
guide's own entry-1 override has no life floor. Flagged, not paid for — one wall would not have
stopped five attackers either.

**L vs 162** (`…-0x560b2cac8e40-vs-…deck162`, turn 21, 0–17). **The clearest engine-caused loss
in this seat.** t5 seq9 cast Idyllic Tutor; seq10's reveal reply was refused (HIGH-2); seq12
forced `reveal_stall_forced` and the rendered log reads
`- The reveal could not be completed - the engine returned the 52 revealed cards to your library
so the game could continue`. Three mana and the deck's only tutor bought **nothing**, and 126
never assembled either half in that game. It then lost entirely to non-combat life loss
(Underworld Dreams ×2, Fate Unraveler, Ob Nixilis, Liliana's Caress ×2) which walls and blocks
cannot touch. Deciding decision **seq34, t17, life 19**: took
`Cast Tribute to Hunger {right now: they control 1 creature - Shield Sphere (0/6) [defender] is
sacrificed, you gain 6}` — the exact row, card and life total the guide names verbatim in Rule
#7b's defender carve-out as a game-losing take. Reply was a bare `CHOICE: 1 (Cast Tribute to
Hunger)` with **no REASONING and no PLAN**, 2 055 ms. Three turns later (seq43) the row read
`Fate Unraveler (3/4) is sacrificed, you gain 4` — the real threat — and that reply was refused
as `unparsed_reply`. Dead t21.

**W vs 130** (`…-0x55913c9defa0-vs-…deck130`, turn 11, 35–0). Bond t9 (seq12) + Blood t11
(seq16), then seq21 attacked with the Sorin lifelink Vampire and the pair chained 20→0 / 20→35
in one damage step. **Rule #2 and Rule #7a are validated end-to-end.** Four `unparsed_reply`
(seq9, 13, 17, 19) cost tempo but not the game.

**W vs 123** (`…-0x55ac3bcc4ff0-vs-…deck123`, turn 25, 21–0). Won on double Staff of Nin pings
plus Tribute edicts against a 88-Human token board. 8 `unparsed_reply` + 1 `named_row_reask`
+ 1 `reveal_stall_forced` (seq51→53, tutor voided again, this time while already ahead 28–11).
seq44/55 cast Tribute at "they control 23 creatures" / "20 creatures" — near-worthless (they
choose), permitted by Rule #7 entry 8; MED-3.

**W vs 125** (`…-0x5583a97bdd00-vs-…deck125`, turn 78, 5–60). 138 decisions, 609 hold windows
skipped. Blood t10, Bond t14 — but Blood was destroyed and sat in the graveyard from ~t20
(seq49 renders `LOOP HALF PENDING: … Exquisite Blood, is in your graveyard`), so the pair never
closed; 126 won only because **125 decked out at 60 life**. Deciding *misplay*: **seq83 and
seq84, t34** — see HIGH-3.

---

## 2. Engine / interface / card items

### HIGH-1 — `ANSWER: CHOICE:` is refused at every ask/priority seam; `ANSWER: <other verb>` is not
The system `reply_protocol` (kind=`system`, `reply_protocol` field) reads *"Write your REASONING
first, then your ANSWER, then your PLAN"* and *"THE ANSWER LINE comes next, on a line of its
own"*. The per-menu footer, unchanged, reads *"On a line of its own write CHOICE: followed by the
number of your choice"*. The model resolves the two by writing the section name as a literal
label. Corpus-wide, keyed on `^\s*ANSWER:\s*<VERB>:`:

| shape | seam | outcome |
|---|---|---|
| `ANSWER: CHOICE:` | ask | 24 / 24 `unparsed_reply` |
| `ANSWER: CHOICE:` | priority | 19 / 19 `unparsed_reply` |
| `ANSWER: CHOICE:` | attackers | 1 / 1 **OK** |
| `ANSWER: ATTACK:` / `ANSWER: BLOCKS:` | attackers / blockers | 3 / 3 **OK** |

**43 of the corpus's 83 fallbacks (52%), and 20 of this seat's 27**, are this one string. It is a
per-seam parser inconsistency, not a model failure: the attackers seam already tolerates the
prefix. Repro: `…deck126-0x560b2cac8e40-vs-…deck162.jsonl` seq14, reply ends
`ANSWER: CHOICE: 3 (Cast Perimeter Captain)` → refused; seq16 re-ask, identical intent written
`CHOICE: 1 (…)` → accepted. **Fix is one line in the ask/priority parser (strip a leading
`ANSWER:`), or one line in `kReplyProtocol` (say "the answer line" and never "your ANSWER").**
Wave 65 had 0 heuristic fallbacks; this is lane AR H2a's first live cost.

Second shape, 36 corpus replies (5 on this seat): **no coded line at all**. The protocol's
*"write the answer LABEL … exactly ONCE … never inside your reasoning"* is being over-complied
with — the model finishes reasoning in prose ("Therefore, I cast nothing", "I will cast Idyllic
Tutor to find Sanguine Bond") and jumps straight to `PLAN:`. Verbatim, `…vs-deck146` seq7 (2 290
chars, no `CHOICE:` anywhere): *"… Entry 7b is a 'Survival Floor'. Since I am not in immediate
danger, I should follow Entry 2. I will cast Idyllic Tutor to find Sanguine Bond.\n\nPLAN: …"*.

### HIGH-2 — a refused `reveal` reply VOIDS the spell; it does not degrade to a legal pick
Both of the corpus's `reveal_stall_forced` are here, both one seq after an `ANSWER: PUT:`
refusal. `…vs-deck162` seq10 answered `ANSWER: PUT: 44`, and row 44 rendered
`44. Sanguine Bond (copy 1 of 2 in this list) (enchantment) … [eligible for "choose card"]` — a
legal, eligible, correctly-reasoned pick. seq12's `chosen_text` is
`The reveal could not be completed - the engine returned the 52 revealed cards to your library so
the game could continue`, and the game log renders the same line. Same at
`…vs-deck123` seq51 → seq53 (`PUT: 1`, row 1 `Sanguine Bond … [eligible]`, 28 cards returned).
Two notes: (a) the reveal footer still asks for a bare `PUT:` while the system protocol asks for
an ANSWER line, same seam mismatch as HIGH-1; (b) **the fallback is not a fallback.** Every other
seam's refusal costs a window; this one costs a resolved spell and a card. The Baka answer here
changed a game: it took 126's only Idyllic Tutor out of the vs-162 game on turn 5, and 126 lost
that game at turn 21 having never seen either half. Repro is deterministic in shape but not in
outcome — `…vs-deck146` seq12 sent a structurally identical `ANSWER: PUT: 37` (row 37 eligible)
and it **parsed fine**, so the refusal is not explained by the label or the index alone.
Recommend the reveal seam degrade to *the first eligible row* rather than to a null resolution.

### HIGH-3 — `plan_contradicts_noop_row` (AR H8) did not fire on the textbook case
`…vs-deck125` seq83, t34. Row: `Cast Tribute to Hunger {2}{b} {right now: they control 0
creatures - at 0 this does nothing}`. The reply, in full:

> `CHOICE: 1 (Cast Tribute to Hunger)`
> `PLAN: Cast Tribute to Hunger. Opponent has 0 creatures, so this does nothing. This is a waste
> of mana and cards. …`

The answer line contradicts the plan **in the same reply**, on a row whose own annotation says
`this does nothing` — the exact `rowSaysNoOp` + plan-contradiction pair H8's re-ask was built
for. No re-ask fired. seq84, same turn, same row, same answer: **both Tributes burned in one
turn** at t34 of a game that ran to t78. Guide Rule #7 entry 8 already forbids it
("at 'at 0 this does nothing' it is never cast at all"), so the guide is not the gap — the
detector is.

### MED-1 — identical DRAW FORECAST re-charges repeat 3–4× per turn (AQ H1)
`…vs-deck162`: seq26/27/28 all render `3 x 2 = 6 … you would be at 21`; seq31/32/33 all render
`2 x 2 = 4 … at 15`; seq39/40/41/42 all render `3 x 3 = 9 … at -4; that KILLS you`. Three runs of
≥3 on one seat. The *content* is correct — the mid-step retirement clause works
(seq23 `1 of them has ALREADY been drawn … so 1 is still to come`; seq25 `2 … so 0 are still to
come`) — and I saw **no `drawsUnattributedClause` and no under-charge** on this seat. The waste
is repetition, not accuracy.

### MED-2 — the deciding reply of the vs-162 loss carried no reasoning at all
`…vs-deck162` seq34, `latency_ms 2055`, reply is the single line
`CHOICE: 1 (Cast Tribute to Hunger)`. A 3-row menu whose row 1 is the guide's named
game-losing take got no REASONING and no PLAN, and the protocol requires REASONING first. If a
short-menu path is skipping the protocol footer, that is worth the engine seat's eye.

### MED-3 — Tribute rows at high creature counts read as live but are near-dead
`…vs-deck123` seq44 `{right now: they control 23 creatures - they choose which one}`, seq55
`20 creatures`. The row prints no floor on what you gain and no note that at N≫1 the opponent
sacrifices their worst body; the model took both. A `{you gain at least N (their smallest
toughness)}` clause already exists on the 3-creature form (`…vs-deck152` seq9) — extend it.

### LOW-1 — 47 HOLD takes : 9 plain declines on this seat
AS H7 is working here (wave 65's corpus ratio was 4:182). `hold_windows_skipped` reached 609 in
the vs-125 game and 83 in vs-162 with no observed over-hold. No `repeat_past_stop` on this seat.

### LOW-2 — `named_row_reask` fired correctly 3/3
seq9 (vs 152), seq32 (vs 123), seq63 (vs 125): each named a card that was on the hand line and
not in the menu; each re-ask produced a legal row. The safety net is doing its job.

---

## 3. Guide verdict: **KEEP as is**

`projects/mtg/bin/Res/ai/baka/deck126_strategy.txt`, 70 992 bytes (in band, at its ceiling).
No `strategy.txt` written.

Why this is a success verdict:

- **Every misplay I traced is already named in the guide, verbatim, with the same card.** The
  vs-162 deciding take is Rule #7b's own worked example — *"You declined 'Shield Sphere (0/6)
  [defender] is sacrificed, you gain 6' twice at 20 life this corpus — right — and then took it at
  19 in the third window … both games were lost"* — and the model took it at 19 life against a
  Shield Sphere. seq83's Tribute-at-0 is Rule #7 entry 8's closing clause. seq9's phantom row is
  Rule #7's re-ask block. These are compliance failures against text that is already maximally
  explicit; rewording them would restate, and the file has no bytes to spend.
- **The rules that were followed won.** vs 130 (t11, 35–0) is Rule #2 + #7a executed cleanly —
  both halves down, Sorin Vampire attack, pair chained to 0 in one damage step. vs 123 is Rule
  #7a's Staff-of-Nin line. Rule #8's hold row skipped 609 dead windows in vs 125 with no
  over-hold.
- **The losses are engine-caused or variance.** vs 162's tutor was voided by a parser refusal
  (HIGH-2). vs 146 lost its Bond to Vanishing Verse and had its Blood named by Silverquill
  Silencer. vs 125 lost its Blood to removal and won on a deck-out. Hands were real.
- The one place I could argue for text — a life floor on Rule #7 entry 1, from vs-152 seq17
  (Bond tapping out at 4 life) — is a **single window in a game five attackers were going to end
  regardless**, and entry 1's no-floor rule is argued from two prior corpora in the guide's own
  body. One ambiguous window does not pay for reversing it.

## 4. Proposals

None. No `general-proposals.md`, no `skill-proposals.md`. My three HIGH items are all engine-side
(parser, reveal degradation, an H8 detector that did not fire); none is a general-guide or
strategy-writing-skill question, and a proposal restating Rule #7's existing text would be noise.

## 5. Not checked

Card primitives were not re-verified against Scryfall — no card behaved contrary to its rendered
text on this seat, so no verify-the-Oracle trigger arose. I did not read the opponent seats of
the three wins beyond the vs-130 close, did not audit `async_drops` / `transport` / timeout fields
(engine seat's census), did not adjudicate the lane prediction tables, and did not diff
`wave62/general-strategy.md` (not loaded at runtime). AT H5 (discard ordering) and AS H3
(`repeat_past_stop`, `loop_autopass`) are **UNTESTED on this seat** — no discard ask and no
repeat-past-stop window arose in 281 decisions.
