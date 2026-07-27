# deck146 (Orzhov Dungeons) -- wave-30 findings

Corpus: matchups-20260726-181642, binary 6e168aa05, -T 3000, REPPENALTY=1.05.
Guide under test: bin/Res/ai/baka/deck146_strategy.txt (wave-29 revision -- FROZEN this wave).
Record: **3/6** (flat vs wave-29, different pool: deck93 rotated out, deck152 in).
Own-seat fallbacks: **1** (1 unparsed_reply priority -- the campaign's 1 deck146 unparsed).

Games (from results.tsv):
- vs148: **WIN, clean kill** (14 to -3, t10)
- vs139: **WIN, clean kill** (15 to -5, t16)   <- FLIPPED from wave-29 L(adj)
- vs137: **WIN, adjudication** (146 21, 137 12, t12)  <- FLIPPED from wave-29 L(clean)
- vs122: LOSS, adjudication (146 15, 122 19, t19)  <- regressed from wave-29 W(adj); grind vs lifegain
- vs152: LOSS, adjudication (146 15, 152 20, t14)  <- new deck; mulligan-to-6 + Lolth race
- vs136: LOSS, clean kill (146 -2, t12)  <- MANA/mulligan again (Kaya mulliganed away)

Own-seat decision volume: vs122=60, vs152=54, vs139=47, vs137=45, vs136=39, vs148=36.
3 of my 6 games were cap adjudications (vs137 W, vs152 L, vs122 L) = 3 of the corpus's 12 adj.

---

## g5 -- THE "DUNGEONS COMPLETED" STATUS LINE IS LIVE; SECOND-GUESSING IS GONE (N-146f RESOLVED)

Wave-29's N-146f: the CURRENT SITUATION carried no completion status, so the model had to
INFER the anthem from boosted P/T and audibly second-guessed it (wave-29 vs122 s37, verbatim:
"the anthem applies *after* the dungeon is completed? 'As long as you've completed a dungeon'.").

**This corpus: the render fix is in.** When >=1 dungeon is completed the CURRENT SITUATION now
prints, verbatim:
    Dungeons completed (you): 1 - effects that check "completed a dungeon" (e.g. a
    completed-dungeon anthem) are ACTIVE for you.
Rendered in vs148 (21x), vs137 (9x), vs139 (31x, incl. the N=2 form "completed 2 dungeon"), and
correctly ABSENT in the three games where no dungeon completed (vs136, vs152, vs122). N>0 gating
is correct.

**The model stopped second-guessing and now ASSERTS completion, doing combat math with the
boosted stats.** Verbatim from replies:
- vs148 s21: "I have completed a dungeon, so my creatures should be buffed (Nadaar is listed as
  5/5, Adventurer as 2/2)."
- vs139 s22: "I have already completed one dungeon ... so the 'completed a dungeon' anthem is
  active, giving my creatures +1/+1."
- vs139 s37: "Triumphant Adventurer #1 is already 3/3 (1/1 base + 1/+1 from dungeon completion
  anthem + 1/+1 from previous room)."  <- correct additive stat reasoning
- vs137 s40: "I have already completed one dungeon, so my creatures have the +1/+1 anthem from
  Nadaar."

The tentative-question framing of wave-29 is replaced by confident status-assertion. **g5 VERDICT:
PASS. Render-fix landed; the motivating confusion is eliminated. Retire N-146f.**

---

## KAYA/LOLTH -- EXERCISED AT LAST (3rd corpus). LOLTH VALIDATED; KAYA STILL UNCAST.

The deck's one remaining unvalidated surface across two prior corpora. **This corpus Lolth was
cast in TWO games** (vs122 t12, vs152 t8; narration: Lolth, Spider Queen: hand -> stack ->
battlefield). Kaya was drawn in vs136 but mulliganed away (library->hand, then hand->library on a
London bottom) -- never cast, so her -3-exile-any-nonland remains card-specifically unexercised
across THREE corpora.

### Lolth loyalty decisions -- GUIDE-PERFECT
(the translog `choice` field is 1-based into the offered list; verified against narration.)

vs122 (Lolth cast t12, game to t19):
- s21 t12, immediately after casting: chose -3 (create two 2/1 Spiders). Narration confirms
  Spider (2/1): created -> battlefield x2. Exactly the guide's "-3 first to build a board."
- s27/s42/s56 (t14/16/18): chose +0 (draw a card, lose 1) -- value once the board was set.
- s25 t14: activated the Hive-of-the-Eye-Tyrant manland instead (a second threat).
- Lolth was used EVERY turn she was in play (one pass at s39 when tapped/no benefit). No
  idle-walker leak.

vs152 (Lolth cast t8, died in combat t8; game to t14):
- s35 t8: chose -3 (Spiders) -- narration confirms two Spiders created.
- s41 t10: +0 draw.  s51 t12: -3 Spiders again (loyalty regained via her death-trigger static as
  spiders traded).

The loyalty MENU renders cleanly: each option carries the full {card text: ... 0: draw ... -3:
Create ...} and a [cost: Counters] tag; the model reads all three abilities and picks the
board-building -3 first, then value +0 -- textbook per the guide. **Walker VERDICT: the walker
SURFACE (cast-when-affordable, loyalty-menu render, +/- discipline, use-every-turn) is now
VALIDATED via Lolth.** Kaya's -3-exile is mechanically the same loyalty-menu surface; her residual
non-appearance is a draw-variance gap, not a fixable engine/decision gap (see rotation projection).

### Minor: a magnitude-annotation misfire on Lolth's options (N-146g, low)
Lolth's Cast option and her loyalty options render {right now: damage 8} (later {right now:
damage 4}). Lolth's cast/abilities deal no such immediate damage -- this is a dynamic-magnitude
(WParsedInt) eval attaching an irrelevant/incorrect number to a planeswalker option (likely
evaluating the -8 emblem's drain or a death-drain expr). The model was NOT derailed (it made
correct picks), but the annotation is inaccurate. Routed to notes.md as a low-pri render item.

---

## FALLBACK DECOMPOSITION -- 1 unparsed_reply (vs152 s17), a 210s over-reasoning spiral

The campaign's 1 deck146 unparsed priority. Decomposition against the precedence spec:

- Kind/context: priority window, t4-ish, latency_ms=210290 (210 seconds). ONE non-pass option
  offered: "Flip Side with Brightclimb Pathway -> DISPLAY TOGGLE only ... It does NOT cast
  anything ... You usually do NOT need it". So the legal set was {0=pass, 1=Flip-Side-toggle}.
- What the model emitted: the reply's FIRST line was "CHOICE: 2 (Play Plains)". Option 2 does not
  exist (only 0 and 1 are valid) and "Play Plains" was not on the menu. Per the precedence spec
  the parser reads the CHOICE line first (after the plan split); an out-of-range index with a name
  absent from the offered set is correctly rejected -> unparsed_reply -> Baka answered (passed,
  the correct play). No parser bug, no engine defect: the Flip-Side toggle offering is by-design
  and the option text itself explains it does nothing.
- Root cause (model failure, not the seat's): the model's PLAN said "Play Plains"; when no
  land-play/cast appeared in the offered list (it had already made its land drop that turn), it
  refused to accept "lists are complete" and spiralled through ~3,000 tokens trying to reconcile
  its plan with the menu -- repeatedly mis-counting its own lands from the GAME LOG narration
  instead of trusting the CURRENT SITUATION board snapshot, cycling "CHOICE: 0 / CHOICE: 2 / play
  the land anyway" without ever committing, and getting truncated mid-sentence ("I can play one of
  them? No"). The correct answer (pass) was trivial; the spiral was pure over-reasoning.
- Layer routing: NOT a guide gap (a guide can't out-argue a reasoning spiral) and NOT a
  core-prompt TEXT defect (the core already states "lists are complete: if a spell is not in the
  cast list you CANNOT cast it" -- the model QUOTED that rule and still spiralled). Routed to
  notes.md as a model/latency watch (the invent-an-off-menu-option-matching-my-PLAN shape) and
  folded into the latency-tail observation.

---

## STEADY STATE

### Dungeon navigation -- LOST MINE DISCIPLINE HOLDING (answer-by-name validated)
Lost Mine selected every time (replies "CHOICE: 3 (Lost Mine of Phandelver)" -- note it was
option 3 this corpus, was 2/3 in wave-29; the guide's answer-by-NAME fix is directly vindicated).
Room picks were 100% value-aware across 14 branch decisions: goblin lair (body) x7, storeroom
(+1/+1) x3, dark pool (drain) x2, plus goblin/storeroom mixes -- ZERO scry-only picks.

### Removal on curve -- HOLDING
Vanishing Verse actually cast: vs122 x8, vs148 x3, vs137 x3, vs139 x2, vs136 x0, vs152 x0. It
fires on curve in the 4 games the deck had mana + windows; the two zero-games are the mana-screw
(vs136) and the mulligan-to-6 Lolth-race (vs152). vs122's 8 Verses is removal-heavy but correct
against deck122's lifegain grind. No hoarding leak.

### Mana durability -- f2 FIX HOLDS
0 mana-collapse lines ({b}{b}/{w}{w} from a WB deck) across all 6 games. The dual-land oracle fix
is durable a second corpus.

### Latency tail -- HEAVY (harness/server property)
272 real-call decisions: median 16.8s, p90 73s, max 216s. 32 records over 60s; several 75-216s
spikes (vs139 s35=203s/s37=209s, vs152 s46=216s, vs122 s56=175s). Matches the harness's documented
-j server-contention curve (median ~17s at saturation); not a seat issue, but the tail is where
the over-reasoning spiral turned into the one fallback.

---

## RECORD DECOMPOSITION (3/6 -> 3/6, IMPROVED W-SHAPE)
- Converted: vs139 L(adj)->W(clean kill, 15 to -5) and vs137 L(clean)->W(adj) -- two wave-29
  losses became wins. vs148 held W(clean).
- Regressed: vs122 W(adj)->L(adj) -- a t19 grind LOSS to deck122's lifegain (8 Verses cast, Lolth
  online; out-valued, not out-played). vs136 held L (mana/mulligan; Kaya mulliganed).
- New: vs152 L(adj) -- mulligan to 6 into a fast deck152; Lolth came down but died; close 15-20.

Honest read: same flat 3/6 as wave-29 but against a churned pool, with two wins converted from
losses and the deck's marquee surfaces (dungeon completion + anthem status + planeswalker play)
all validated live for the first time. The three losses are variance (screw/mulligan) and a
value-deck grind, not taught-decision misses.
