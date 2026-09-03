# Wave-53 synthesis (2026-09-02) — TENTH FAIR-HAND CORPUS

Inputs read, in order: `wave52/synthesis-notes.md`; `wave52/strategy-writing-skill.md` (the CURRENT
edition, amendments 1-186 — tail read, the rest grepped); `wave52/general-strategy.md` (R1-R160);
`wave52/engine-ledger.md` (the wave-53 docket D1-D28); `wave53/review-carry.md`;
`wave53/reviewer-brief.md`; the twelve lane reports `wave53/lane-{M,N,O,P,Q,R,S,T,U,V,W,X}.md`; the
four seat files (`seat-engine-narration.md`, `seat-123-130.md`, `seat-125-126.md`,
`seat-146-152-162.md`); all seven `deckN/skill.md`, `deckN/general-suggestions.md` and
`deckN/edit-texts.md`, plus the seven `deckN/strategy.txt` (verified byte-identical to the live
`bin/Res/ai/baka/deckN_strategy.txt` — the guides are ALREADY DEPLOYED, commit baf36b936); and the
corpus `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260902-023342/` (read with python over the JSONL,
including the NEW `recovery` record kind, and the `.stderr` files with `/usr/bin/grep`) for every
reconciliation below. Card facts were re-verified against `projects/mtg/bin/Res/sets/primitives/*.txt`
with `/usr/bin/grep`; **four emitter/parser mechanisms were pinned by READING**
`projects/mtg/src/AIPlayerGPT.cpp`, `src/GuiPlay.cpp` and `include/AllAbilities.h` (read-only —
nothing was built, run, deployed, or committed).

Outputs: `wave53/strategy-writing-skill.md` (the wave-52 edition **verbatim** — the 1,125,187-byte
prefix is `cmp`-verified — plus **amendments 187-208**), `wave53/general-strategy.md` (R1-R160 with a
wave-53 verdict each, **R161-R192** new, layer-routed, twelve measurement items, PASS/KEEP, plus a
carried set for the Vita/replay engine items), `wave53/engine-ledger.md` (the **wave-54 docket
D1-D38**, ranked, engine/UX only, each with a record citation; both discharge tables including lanes
R-X; fallback classes; per-kind prompt chars; specimens), and the boundary findings below.

**Ownership (owner doctrine 2026-08-26).** Reviewers own every `deckN/strategy.txt`; this step owns
the skill and the general strategy; the core loop — engine correctness, LLM UX, bugs — is the
purpose. **No guide file was edited by this step**, and none could have been usefully: the wave-53
guides were deployed before this step ran. Step 4 was run as a REPORT (B1-B10). Three proposals that
would have removed a legal window are rejected in that form; one is adopted in a shape that caps
PROSE and no choice. Hard caps on legal choices are rejected on sight. Every mulligan text quoted by
the seats is odds-form. **No deck-content question is open for the owner** — see B10 and
reconciliation 11. No file under `projects/mtg/bin/Res/**` or `src/**` was modified, no git command
was run, wagic was not launched, nothing was deployed.

---

## Reconciliation against the records

1. **Health, re-derived on disk over all 42 JSONL files.** 3,572 records = 42 `gamestart` + **40
   `gameend`** + **7 `recovery`** (the new kind) + **3,483 decisions** (ask 2,754 / priority 519 /
   attackers 120 / discard 34 / blockers 30 / reveal 21 / bottom 5). **15 `fallback` fields =
   0.43%**: `plan_choice_conflict` 8, `unparsed_reply` 3, `named_row_reask` 3, `retracted_choice` 1
   — **no `empty_reply`, no `timeout`, no `multiblock_reask`, no `stale_livelock`**. Baka executed
   **4** decisions (0.11%). `answer_replaced` **12** — and they are *exactly* the 12 records carrying
   `latched_coded_line: 2`, which is a clean internal consistency check nobody claimed and which
   holds. `coded_answers > 1` **43**, `commit_retracted` **1**, `retry` **0**, `pass_row_named` **0**.
   The engine seat's whole health table is confirmed by string; the two deck seats' per-seat sums add
   to it.
2. **THE HANG, and the rerun question (invariant 00).** Both seats of `152v125` lack a `gameend` —
   the only such pair — and `results.tsv` reads `152 125 adj0 20 9 15`. `152v125` seq 28 is the ONLY
   record in the corpus with `latency_ms < 0` (`-1`), a zero-length `prompt`, `options: 5`,
   `options_text` null, and **no `fallback` field**. Lane X pinned the mechanism end to end
   (`MTGRevealingCards::toResolve` arms option TWO when option one has zero legal targets; phase 0
   finalizes against that chooser; phase 3 waits for a chooser already consumed) and shipped both
   halves with a GPT-free fixture RED on base and GREEN after. **Ruling: the matchup is recorded as a
   FAILED test, excluded from every rate, and NO RERUN IS OWED FOR WAVE 53.** A rerun on `c628dd531`
   reproduces the hang; a rerun on the fixed binary is a wave-54 datum on different code, and mixing
   it into wave-53 rates is precisely the relabelling invariant 00 forbids. The replacement datum is
   the wave-54 corpus. What IS owed now is the falsifier (no seat log without a `gameend`) and a
   **no-progress watchdog**: the harness's feasibility watchdog measures LATENCY, and a hang produces
   none — the process wrote nothing for 13 hours and nothing fired (docket D7).
3. **LANE N FAILED ITS OWN FALSIFIER, and the three seats' disagreement resolves into ONE item with
   three fixes.** The seats did not contradict each other; they measured three different things and
   all three are true. Engine seat: 1,052 windows / 58 casts / 26.0% runs. seat-146:
   `hold_windows_skipped` 102 across 40 gameends, 0 on 38. seat-125-126: 0 on all 11 of ITS gameends,
   with the mechanism named — `holdBoardKeyOf` keeps the opponent's hand and library counts.
   Re-derived here: HOLD offered **1,173**, LAST row **1,173/1,173**, opponent's turn only, **exactly
   one distinct row string**, **29 takes** across six seats; `hold_windows_skipped` **102**, non-zero
   on exactly **2 of 40** gameends (88 at `146v125`, 14 at `130v125`); opponent-turn casting windows
   **1,052** on the wave-52-comparable key (1,126 / 1,243 on looser keys), **58 casts**; runs of 3+
   byte-identical declined menus **912 / 3,483 = 26.2%**, **147.4 min**. **The mechanism is confirmed
   in the source, read-only:** `holdBoardKeyOf` (`AIPlayerGPT.cpp:12904`) drops only the leading
   phase line, and `serializeGameState` emits `opponentZoneCountsLine(opp->game->hand->nb_cards, …)`
   at **12239** (`Opponent hand size: N | Opponent library: M cards`) and ends at **12314** with
   `yourLibraryLine(game->library->nb_cards, …)`. **Both counters are in the key, and the opponent's
   draw step changes both on every opponent turn** — repro `125v126` seq 128 (Upkeep, HOLD taken) ->
   seq 130 (Draw, byte-identical menu, asked again) with one `hold re-opened … the board changed` in
   stderr. seat-146's `plan_choice_conflict` reading is right on the shape and its arithmetic needs
   one correction: **5 of the 8** (not 8 of 9) are the HOLD-row verb colliding with pass-priority
   prose; s392 and s555 are the same vocabulary collision on a Hive animation, and `123v126` s22 is
   the one true false positive. And `CHOICE: 0 (Hold priority)` appears exactly **twice** and both
   times executed a plain pass with no note — because lane N's own D9 fix makes `0`/`pass` a RESERVED
   echo that binds ahead of every name verdict, so the HOLD name can never win. **One docket item
   (D2) with three fixes: key without the hidden-zone counters, bind the HOLD name the way `pass` is
   bound, and exempt the row from the pass-verdict conflict test.**
4. **Path to Exile's compensation search, 8/8, and the mechanism is now pinned.** All 8 renders of
   `EXILE ONE OF YOUR OWN CARDS` are Path to Exile. `handRemovalVerb` (`AIPlayerGPT.cpp:20084`)
   matches the lowercased *source-name + ability-verb* string in order and `"path to exile"` contains
   `exile`, so the verb is decided by the CARD'S NAME before anything reads the destination;
   `buildHandRemovalAsk` (20105) then correctly picks the SELF branch, because the search really is
   granted to `targetcontroller`, and prints loss wording for `moveTo(mybattlefield)` — a gain
   (`mtg.txt:84676`). The fix is a destination-zone gate, not a name list. It is also the cause of two
   of the corpus's three mega-menus (17 library Mountains, 14 library Swamps).
5. **The lethal face row: the class is FIVE TIMES bigger than the seat found, and it has two
   halves.** (a) **26** `{kills …}` rows enumerate `the opponent` among their legal targets and
   exclude it from the count — nineteen of them in `130v125` alone, including seq **125** at
   `Opponent life: 14` (declined) and seq **127** at 8 (taken). (b) At `130v162` seq **62** the cast
   row's own enumeration **omits the player entirely** (`- legal targets right now: Shield Sphere …,
   Fate Unraveler …`) though Hammer of Bogardan is `target=anytarget auto=damage:3`
   (`mtg.txt:52046`). **Correction to seat-123-130:** the row was **taken**, not declined — the
   misplay is at the target ask (seq 63), where the pilot chose the planeswalker. And that ask is the
   sharpest exhibit in the corpus: it prints `{right now: takes 3 damage - SURVIVES (toughness 6)}`
   on the Shield Sphere and `- SURVIVES (toughness 4)` on the Fate Unraveler and **nothing at all on
   `0. The opponent (player, life 1)`** — a survival verdict for every creature and none for the
   player the spell wins the game against. Lane O's own note documents the exclusion by design.
6. **The PLAN capture is bounded, and seat-123-130's H3 is REFUTED on disk.** The filed claim is that
   `123v130` s57's `PLAN:` swallowed ~2,000 characters including a false Morbid assertion and that
   s63/s64 were served all of it. Read on disk, the served block at s63 and s64 is **151 characters**
   (`"Fetch a dual (Tundra or UGS), then cast Intruder Alarm. …"`) with no Morbid claim and no phase
   claim. `consumePlan` anchors on the **LAST** line-leading `PLAN:` marker (s57's reply carries
   markers at char 35 and char **3,755**) and bounds the result at **1,600 bytes** cut at a sentence
   boundary; the largest served PLAN block anywhere in the corpus is ~1.8 KB. **Both mechanisms
   worked.** The false Morbid assertion is in s64's and s65's OWN replies — which is a real defect of
   a different class (reconciliation 7). What remains open about the capture is a SHAPE bound rather
   than a byte bound, and one genuine staleness instance the seat did not cite (`162v152` s11 -> s12:
   the reply's plan names `Cast Master of the Feast` while the row it took in the same breath made
   Master unaffordable, and s12 was served that plan verbatim). **A cap on PLAN prose is not a cap on
   a choice** — it bounds the pilot's own quoted text and removes no window — which is why it is
   adopted where a menu cap would be rejected.
7. **`decision_reversed_in_prose`: the wave-52 rejection STANDS, and the cost this wave is a
   different thing.** Wave 52 rejected narrowing the check because 3 of its 5 firings carried
   `coded_answers: 2` + `answer_replaced` and had changed the executed action. This corpus has **8
   firings and every one carries `coded_answers: 1`** — neither the wave-52 grounds for the check's
   value nor the grounds for narrowing it recurred. What DID appear is a collision the check was
   never built for (reconciliation 3), removable by exempting one row and narrowing nothing. The
   opposite failure is separately real and is adopted as an ADDITIVE stamp, never a suppression:
   `126v125` seq 73/74 wrote `CHOICE: 1 (Cast Tribute to Hunger)` above PLAN lines arguing against
   exactly that, on a row the engine itself labelled `at 0 this does nothing`, and both executed
   unflagged; `123v125` s84 cast a dead Damnation under prose reaching the opposite verdict with no
   second coded line. seat-123-130 and seat-125-126 found the two halves independently.
8. **The annihilator framing is 18 renders, not 1, and the fix is already in the tree one lane
   over.** Of the corpus's **26** `your own effect` renders, **18 are the OPPONENT's Emrakul** —
   `126v125` seq 111-116 (turn 53) *and* `130v125` seq 181-186 and 189-194 (turns 48 and 50), which
   is the very sequence seat-123-130 proposed as its prompt-review specimen. The `byOpponent` branch
   exists at `AIPlayerGPT.cpp:20105-20120` and was not taken because
   `selfInflicted = tc->source && tc->source->controller() == this` (20470) reads TRUE: an ability
   GRANTED to a player parses onto a nameless dummy card controlled by the victim. **Lane P's D4
   pinned exactly this indirection for the stack line** — the real granter is
   `MTGCardInstance::storedSourceCard`, set at `include/AllAbilities.h:4470`
   (`myDummy->storedSourceCard = source;`). The same one-line indirection fixes the framing.
9. **The `[DRAW PRICE:` dispute: three seats, three units, and the emitter has TWO tags.**
   Corpus-wide there are **85 row-instances in 71 records** in exactly two forms. The **Forced
   Fruition form** states a damage total and is tailed **27 of 27**, with **16 carrying
   `; this KILLS you`** across 8 records, all at `125v162`, **0 casts from a KILLS row** — i.e.
   **every lethal row in the corpus carries the tail, 16/16**, and lane O's D3 is a clean PASS. The
   **cost form** states a cost and is tailed **0 of 58** (37 cast rows + 21 cycling ability rows).
   seat-125-126's "8/8 records" is exactly right; the engine seat's "85 rows, 16 lethal tails" is
   exactly right; seat-123-130's "0 of 58" has the right DENOMINATOR to the row (its two decks render
   exactly 58 DRAW PRICE rows) and a numerator off by six (**6** of its 58 carry the tail — `130v162`
   meets the Fruition form), and its diagnosis "the tag states a damage total in all 58, re-check the
   call site's `life` argument" is wrong: the call site is fine and the two tags are different
   functions. The real finding underneath is the 58 cost-form rows with no total (docket D10, MED —
   none of them was lethal this corpus).
10. **The long replies: SIX, not three, and only one crosses lane Q's threshold.** `126v125` seq 13
    at **868,729 ms = 96.5%** of the 900 s wall on a 3-row, 5,780-char menu; `126v146` 22 (718,196)
    and 23 (608,145); `162v126` 10 (703,161); `123v126` 1 (660,402, the opening mulligan on a
    1,545-char prompt); `152v123` 16 (629,642, a three-row land drop). Five seats. **All six
    ANSWERED**, so lane Q's arm — which requires an EMPTY body — had no window and is UNTESTED, not
    PASS. Only the 868,729 ms record exceeds 95%. seat-125-126's "three" is right on its own two
    seats. The docket item is a `long_reply` stamp on the branch `noAnswerClassFor` does not cover;
    `WAGIC_GPT_TIMEOUT` is the owner's dial and is not touched (the same ruling wave 52 made for D10).
11. **The OWNER QUESTION seat-146 flagged is discharged here: nothing remains for the owner.**
    `152v162` s1 (2 lands making {G}{G}, five white spells) was shipped against that guide's explicit
    floor (*"TWO OR MORE LANDS AND AT LEAST ONE SPELL: KEEP. That is the floor and it is not a close
    call"*), with the reply naming the colour — the guide's own FORBIDDEN THOUGHT — as its reason;
    s2 mulled again at 1 land, s3 kept at 5 lands, and the seat won that game 11-0. Under the
    2026-08-26 doctrine deck-content calls are the reviewers': the seat that owns deck152 flagged it
    as the brief requires, diagnosed it (#171's missing scoping sentence, absent in deck152 and
    present in deck146, which shipped **0 of N** two-plus-land sevens with the same literal), wrote
    the fix (152-B) and pinned it (P5). **That is the whole of it, and it is theirs.** Two things
    remain and neither is a question: the cross-guide control it provides is merged as skill **#203**,
    and the render half — the coverage line collapsing "too few lands" and "enough lands of the wrong
    colour" into one string, when the engine prints both facts on adjacent lines — is docket **D16**.
12. **The UNTESTED render items, with the window each needs.** Three lane items shipped and never
    rendered: the edict such-clause (`- a sacrifice is already on the stack aimed at this permanent`,
    **0 renders** — needs a second edict aimed at a permanent an unresolved stack object already
    picked; the wave-52 triple did NOT recur, `146v125` T50 resolved cleanly and the T64 copy was
    countered), the converter fold on edict gain rows (**0 renders** — needs an edict/gain row while a
    Sanguine-Bond-class converter is on the board; the fold DOES exist elsewhere, 28 occurrences
    inside combat blocker annotations, and the static CONVERTER block renders 551 times), and the
    from-exile cause (**2/2 PASS, N = 2** — needs an Elite Spellbinder game where the granter leaves
    play, which is the shape wave 52 measured at 4/17). All three carry forward with those windows
    named; none is re-derived and none is scored a pass on absence.

---

## The headline: eight lanes shipped, the render is the cleanest it has been, and the wave was decided by one hang and one row the model would not take

**(a) THE CORPUS IS 20/21 AND THAT IS THE WAVE.** One game hung for thirteen hours in the
interactive-reveal driver on the single path where option one has zero legal targets — a shape that
fired **1 in 5** Sigarda Coven triggers and **1 in 21** reveal windows, left **zero stderr** because
its only tracer is env-gated, and was invisible to the harness's watchdog because that watchdog
measures latency and a hang produces none. Lane X pinned it end to end and shipped the fix with a
RED-on-base fixture after the review opened. Invariant 00 says a corpus completes a full game for
every matchup or the test failed: **this one failed**, the matchup is excluded from every rate, and
no rerun is owed — the wave-54 corpus on the fixed binary is the replacement datum.

**(b) EVERY WAVE-53 HIGH SHIPPED AND FOUR OF THE FIVE DISCHARGED CLEAN.** `multiblock_reask` **0**
against last wave's silent zero-block, with 30 blockers asks paired 1:1 to 30 stderr settlement lines
and `multiblock_first_wins` firing live for the first time; the DRAW PRICE lethal tail on **16/16**
lethal rows with **0 casts**, including eight declines at `125v162` on the exact menu that took the
counter and lost the game a wave ago; stack lines **706 emitted, 0 bare**; the battlefield land count
**3,429/3,429**; the `recovery` record **7/7 paired, 0 unpaired**, with the new `retracted_choice`
class catching its first live firing; the `[second copy:` usefulness verdict turning **5 takes of 5
into 0 takes of 6** on the Chromatic Lantern **with no guide change at all**. Fallbacks **0.43%**,
Baka-executed **0.11%**, `stale_livelock` 0, receipts 496 with **0 with no source tapped**, and
`blockers` prompts **34% smaller**.

**(c) THE ONE HIGH THAT DID NOT DISCHARGE IS THE ONE THE WHOLE WAVE WAS ABOUT, AND IT FAILED THREE
WAYS AT ONCE.** The HOLD row rendered exactly as designed — 1,173 times, last row every time,
opponent's turn only — and every headline number moved the wrong way: windows **+19%**, casts
**-15%**, identical-declined runs **26.2%** against a target under 10%. It is not the row. It is (i)
a hold key that carries the opponent's hand and library counts, so their own draw step retires the
hold one phase after it is taken, on every opponent turn — which is why `hold_windows_skipped` reads
0 on **38 of 40** gameends while six seats took the row; (ii) the same lane's `pass` echo reservation
making `CHOICE: 0 (Hold priority)` bind row 0 silently, 2 of 2; and (iii) the plan-conflict detector
reading the model's own word for the act ("I must pass priority") as a reversal of it, 5 of the
corpus's 15 fallbacks, at a full extra model call each. All three are keying and vocabulary. **None
of them is a reason to remove a window**, and three proposals that would have are rejected.

**(d) THE OTHER NEW HIGHS ARE THE RENDER OWING A NUMBER IT ALREADY HAS, TWICE OVER.** A kill summary
that counts creatures and hides players printed `{kills 0 of the 2 legal targets at 3 damage}` on a
row that wins the game at opponent life 1 — 26 rows corpus-wide, and the target ask one record later
gave a `SURVIVES` verdict to both creatures and no verdict at all to the player. A compensation
search that puts a basic land onto your battlefield tapped is announced as **"EXILE ONE OF YOUR OWN
CARDS … each option is a card YOU will LOSE"**, 8 of 8, because the verb is read off the source
card's NAME. A modal spell prints its whole Oracle text and never which of its four modes is live —
28 cast rows and 9 mode sub-menus, zero annotations — and the pilot declined a free
body-plus-a-card about twenty times across turns 29-83 of an 84-turn loss while asserting an empty
graveyard the log above it contradicted.

**(e) TWO SEAT VERDICTS ARE REVERSED ON DISK AND ONE MECHANISM IS FOUND THAT NO SEAT LOOKED FOR.**
The PLAN capture was filed as unbounded and is bounded — the served block at the cited repro is
**151 characters**, `consumePlan` anchors LAST and caps at 1,600 bytes, and the false Morbid claim is
in the model's own reply, not in the carried plan. The DRAW PRICE tail was filed as `0 of 58` and is
`6 of 58` at that seat and `16 of 16` where it matters. And the annihilator misframing, filed as one
citation at one seat, is **18 renders across two seats** — including the exact six-record sequence
the other seat nominated as its prompt-review specimen — with its fix already written one lane over:
lane P's `storedSourceCard` indirection, found for the stack line, is the same one line.

---

## What merged into the skill (187-208)

**Convergence merges (>= 2 seats):**
- **188** the #168 literal audit is counted from the ROWS and scoped per (literal, CARD FAMILY) — a
  string rendering 1,586 times corpus-wide can render 0 times on the row the guide keyed to it
  (deck123 S4 + deck146 S-1, with a scope correction this step owes deck123: the guide is NOT in the
  `prompt` field — 0 of 826 at its own seat — so the phantom-render risk is the log and the plan
  echo, not the guide).
- **190** #177's diff runs per-EDIT, not as a pass at the end, and gains a step 0 enumerating the ASK
  KINDS (deck125 S3 + deck146 S-3; a correct twelve-line cleanup bullet filed 500 lines from its
  ask scored 0 of 3 and the seat died at 0 against an opponent on 2).
- **189** a guide sentence keyed to a COUNTING literal names what is outside the count (deck123 S2 +
  deck130's grounding; `{kills 0 of the 2 legal targets}` on the row that wins the game).

**Decisive single-seat merges:**
- **187** #154 gains a deadline: a derivation the literal now answers is cut in the SAME wave
  (deck123 S1).
- **191** carve-out reachability — the exception's allowed actions and the primary rule's requirement
  in one sentence (deck123 S3; Marsh Flats fetches only Plains or Swamp and the carve-out asked for a
  basic while the rule needed blue).
- **192** a threshold is a CONSTANT against a printed number, never a relation between two moving
  totals (deck125 S1; the loss — a rule that was followed and was wrong).
- **193** re-key to the most DEDICATED literal, not the most detailed composite (deck125 S2; a
  353-render single-purpose line vs a substring inside a 25-permanent battlefield listing).
- **194** refuse "nothing else on the menu was castable" by name, once, with the seat's own decline
  ceiling as proof (deck125 S4).
- **195** name a plausible NON-member of every listed mechanical class (deck126 S6; the pilot
  invented a sixth win-button starter out of a wall that gains nothing).
- **196** prose-right / answer-wrong belongs in the ANSWER-FORMAT section, and its detector in the
  engine list (deck126 S7).
- **197** a render re-key can close a rule the guide could not — make the tag answer the question the
  pilot IS asking (deck126 S8; 5 takes of 5 -> 0 takes of 6, no guide change).
- **198** report a break rate with its distinct-TURN count (deck126 S9; "2 of 39" is one failure with
  two records, two windows apart, in one turn).
- **199** in an ordered procedure the step that WINS THE GAME is step 0 and overrides the rest,
  floors included (deck130 S6; the lost game).
- **200** #132 gains a third verdict, UNTESTABLE-AT-THIS-SEAT, which earns a compression, not a
  deletion and not a restatement (deck130 S7; three corpora, 0 ships).
- **201** score a floor or ritual on LIVE rows and report both numbers (deck146 S-2; 15/165 raw,
  15/26 live, protecting 0 decisions -> DELETE).
- **202** an absolute keyed to a substring the emitter prints once per LIST MEMBER carries the list's
  cardinality (deck152 S-1; one gain clause held a 4/4 and a 7/5 for three combats).
- **203** #171's scoping sentence joins the #182 port list and #144 runs as a MATCHED PAIR — the
  passing guide's sentence is the fix (deck152 S-2; same literal, two guides, opposite results).
- **204** #157's shared-fact step runs on SURVIVING breaks even while the count falls, and the
  missing condition may be a cardinality inside the quoted literal (deck152 S-3).
- **205** a survival gate lives at the ask kind that SPENDS the resource (deck162 S-1; the block
  window inherits a board the cast window bought).
- **206** #183's imperative-count diagnostic is run on the EDIT — closing a rule's empty case creates
  a mirror failure on its exempt case (deck162 S-2; 1 over-brake window became 11).
- **207** TRUST-DOCTRINE BOUNDARY: the carried `YOUR PLAN` block is not a render, so a guide may
  prefer a `{right now: …}` clause over it (deck123 S5, adopted with the repro corrected — see
  reconciliation 6).
- **208** (this step's own, forced by the reconciliation): a seat file states the FIELD
  (`options_text` vs `prompt` vs `chosen_text`), the UNIT (row-instance vs record vs window) and the
  MASK it counted with, and synthesis re-derives every load-bearing number and names who was right on
  which denominator — **four such disagreements in one wave, and not one of them was an error.**

## What was REJECTED, and why

- **"Withhold, or stop re-offering, a cast row the engine has itself marked dead" (deck146 R-2,
  restated with a bigger denominator: `at 0 this does nothing` 613 renders / 2 takes).** REJECTED,
  second wave running: it removes a legal action from the menu, the ceiling is the model's and not
  the engine's, and wave 52 showed such a clause can be stale against a pending stack object. The
  613/2 number is evidence the annotation WORKS, not evidence the row should go.
- **"Skip the opponent-turn window whose stack is empty at a phase this seat has never cast in"
  (deck125 G1).** REJECTED: a phase's cast history is not a legality fact, and this corpus supplies
  its own counter-example — `125v152` seq **15**, a real `Cast Path to Exile` at the **Attackers**
  step, one of only four casts outside Main phase 1. The independent lever the seat wanted exists and
  is adopted: fix the hold key (D2c), which is what actually closes those windows.
- **Any blind identical-menu cache.** Not re-opened: wave 52 measured **30 real actions** following a
  decline on a byte-identical option set inside one turn.
- **"Require the prose verdict to NAME a row before `decision_reversed_in_prose` counts."** The
  wave-52 rejection STANDS and this corpus neither supports nor needs it — all 8 firings carry
  `coded_answers: 1`, and 5 of them are removable by exempting ONE row from ONE test, narrowing
  nothing (reconciliation 7).
- **seat-123-130's H3, "the PLAN capture swallows the whole reply tail and is replayed as current."**
  REFUTED on disk — reconciliation 6. The served block is 151 characters, the anchor is the LAST
  marker and the cap is 1,600 bytes. Adopted in a smaller form (a SHAPE bound plus a
  contradiction clause), and recorded explicitly as a bound on the pilot's own prose, not on a choice.
- **seat-123-130's "the DRAW PRICE tail rendered 0/58; re-check the call site's `life` argument."**
  Refuted — reconciliation 9. Six of its 58 carry the tail, 16 of 16 lethal rows carry it, and the
  two DRAW PRICE tags are different emitter functions. The finding underneath survives as a MED item.
- **seat-123-130's reading of `130v162` s62 as "deck130 declined a win."** Corrected: the row was
  TAKEN; the misplay is at the target ask. The render item is stronger for it, not weaker
  (reconciliation 5).
- **"Answer internally when the de-duplicated option list has length 1" (deck152 R-6), as a first
  move.** Adopted only SECOND, behind extending lane P's collapse to those menus: it is not a hard
  cap only once the collapse has demonstrated the choice is vacuous, and until then the model keeps
  the ask.
- **A guide compensation for any of D1-D5.** Trust doctrine: a guide describes cards and printed
  strings. deck123's A53-6 (reading the blockers hint against the two life totals) is accepted as a
  teach about a printed string, and it RETIRES when D21 gates the clause — see B3.
- **Any win-rate attribution.** deck146 4-2, deck162 4-2, deck125 4-1 natural, deck123 2-4, deck126
  2-4, deck130 2-4, deck152 2-3 (+1 adjudicated), n <= 6 per deck on real hands. Counts only (#87,
  #167); every rotation call is made on open lanes, and all seven seats are correctly recorded as NO
  EXIT — including deck162, which was exit-eligible a wave ago and opened a mirror lane instead.

---

## Layer routing (`general-strategy.md`)

RENDER/CORE/ENGINE, ranked: the interactive-reveal hang (R161) · the HOLD row's three defects
(R162) · the compensation search framed as a loss (R163) · the kill summary's hidden player target
(R164) · modal rows with no per-mode legality (R165) · **MED**: the annihilator's `your own effect`
(R166), the reveal driver's unbounded wait and the harness's blind watchdog (R167), the collapse
that does not reach ability/sacrifice/reveal menus (R168), the unstamped 96.5% reply (R169), the
DRAW PRICE cost form (R170), unconditional removal with no victim (R171), the PLAN shape bound and
the stale-plan diff (R172), the latch executing a third row (R173), the PLAN-vs-CHOICE stamp (R174),
the echo matcher binding an annotation name (R175), the mulligan coverage line's two causes (R176),
the missing blocker count (R177), the menu that never says what fits together (R178), the doubled
effect label (R179), fetch rows with no colours (R180), the blockers hint's unchecked premise
(R181), the tap clause's missing count (R182), the CONVERTER block's missing consequence (R183),
the `{spends}` clause naming the wrong card (R184), the battlefield-ordered sacrifice list (R185) ·
**LOW**: the `(xN)` collapse (R186), the payee (R187), the ability row's missing life total (R188),
the bare `[second copy:` read as safe (R189), the priority seam's turn attribution (R190),
`pass_row_named` (R191), the log window (R192, the owner's).
Of R136-R160: **R136, R138, R139, R141, R143, R145, R146, R147, R148, R149, R154, R158, R159
SHIPPED**, of which R136/R141/R146/R147/R149/R158/R159 **PASSED**, R138 passed on the form it
reaches, R139/R143/R145/R148/R154 are **UNTESTED for want of a window** (each with the window named),
and R137/R140/R142/R144/R155 shipped and each exposed a residual (R162, R164+R171, R168, R191, R186).
R150-R153, R156, R157, R160 carried; R151 and R152 CLOSED in effect by behaviour.
**Carried engine items with no corpus surface**: replay's four independent failures (RNG-stream
drift, the `read the bones` chooser order, lane W's two dumps, lane R's dump) — which four lanes
independently reported as the reason every owner play report is investigated from a transcript and a
fixture rather than the board that produced it — plus `kMaxOptSources` 14, the `&&`-wrapped-producer
blindness at three call sites, the Dominating Vampire livelock hypothesis,
`GuiPlay::receiveEventPlus`'s unconditional `Replace()` per ETB, and the missing `assertmtgid`.
GUIDE-only: listed per deck in `general-strategy.md`.
MEASUREMENT: twelve items, headed by the HOLD row's per-gameend `hold_windows_skipped`, the kill
summary's player targets, and the DRAW PRICE audit **by FORM** with rows AND records reported.
PASS/KEEP: every lane M-Q string in the ledger's Set B; **one render falsehood this corpus** (R181's
"while ahead on LIFE") after three clean waves, and the two mechanisms filed as falsehoods were
refuted on disk rather than fixed.

---

## Guide boundary findings for the deck reviewers

Step 4 was run as a REPORT over the seven wave-53 `deckN/edit-texts.md` proposals and the seven
`deckN/strategy.txt` files, which are **already deployed** (verified byte-identical to
`bin/Res/ai/baka/deckN_strategy.txt`; commit baf36b936) — so every finding below is an obligation for
wave 54, not a request to change a pending edit. Sweeps: engine-behaviour vocabulary
(#99/#103/#136/#154), the hard-floor family, quoted render literals per row family with the
three-number audit (#168/#188), the cross-guide KEY diff (#144/#203), the Oracle/primitive class and
effect words (#145/#164), the mulligan single-property rule (#153/#171), the list-answer emit form
(#143/#152), the prose-vs-bullet derivation check (#177/#190), and the 72c mood-word sweep.

**TRUST DOCTRINE: CLEAN, all seven, and one guide actively repaired it.** No guide teaches doubt of a
rendered value. The mood sweep returns **one** hit corpus-wide (`deck152` line 254), and it is inside
a quoted FORBIDDEN THOUGHT the guide is refuting ("the thought 'that parenthetical is probably a
template…'") — the #174 device, not a hedge. PASS.

| # | file : site | amendment | finding | suggested wording |
|---|---|---|---|---|
| **B1** | `deck125` W84 (the `=== YOUR MANA AND YOUR WINDOWS ===` block) | **#154 / #187** | **PASS, AND IT IS THE WAVE'S BEST BOUNDARY RESULT — a guide that had gone FALSE was corrected TO the render.** The guide asserted *"You get a window on the opponent's turn only when they cast a spell"* and *"You are never offered an end-of-their-turn window"*. Both were made false by lane I a whole wave ago (884 windows then, 479 at this seat now, including 35 End and 34 Cleanup windows) and survived a full corpus uncorrected. The reviewer found them by the audit and rewrote them to what the engine does. **This is exactly the failure #187 now puts a deadline on, caught from the other direction: a lane can make a guide sentence false without changing any string the guide quotes.** | none — but the wave-54 brief should ask every reviewer for the reverse audit too: not only "did my literal change" but "did a lane make one of my SENTENCES false". |
| **B2** | `deck126` D91b / D91d (the Chromatic Lantern re-key, both occurrences) | **#168 / #188** | **PASS, and it is the proof of #197.** The three-number audit is stated in the edit text exactly as the brief asked — old string **0 renders**, new partial form **6 renders**, **0 takes** (was 5 of 5) — and **both** occurrences in the deployed `strategy.txt` (lines ~393 and ~628) carry the full new literal, verified on disk. This is the wave-51 B3 obligation honoured, and the measured result is that a render re-key closed a rule the guide could not. | none. |
| **B3** | `deck123` A53-6 (the blockers-hint bullet) | **#154 / #187, with a DEADLINE** | **PASS now, RETIRES on D21.** The bullet quotes `"you would be at K - NOT lethal: block only where the trade favors you; taking damage while ahead on LIFE is often correct"` and teaches the pilot to read it against the two life totals. That is correct today and is a teach about a printed string. **But D21 gates the clause so it is not printed when the pilot is behind** — at which point the bullet is a derivation the literal answers, and #187 says it is cut in the SAME wave, not left beside it. | At D21: *"The blockers prompt prints `- NOT lethal` with your two life totals above it. When their life is higher than yours the hint's second half is not printed at all: a token goes in front of the biggest attacker."* |
| **B4** | `deck125` W78a/W78b (the Fruition threshold keyed to `- you would be at K`, floor 15) | **#192 / #188** | **PASS, with a scope obligation.** The key is satisfiable and the audit proves it: all **27** tailed DRAW PRICE rows in the corpus are the Forced-Fruition form and all 27 are at this seat's matchup. The threshold is now a CONSTANT against a printed number, which is #192 in its first shipped instance. **The obligation is D10:** when the tail also ships on the cost form (58 rows, 1-2 life costs, 21 of them on cycling rows), a flat floor of 15 read against `- you would be at K` would forbid a 1-life cycle. | At D10: scope the floor to the row that names Forced Fruition — *"…only on a row whose DRAW PRICE names Forced Fruition; a cycling row's own `- you would be at K` is a 1-2 life cost and this floor does not reach it."* |
| **B5** | `deck123` A53-2 and `deck130` D53-1/D53-2 (rules keyed to `{kills …}`) | **#189 / #187** | **PASS, and #189 is why.** Both guides now key to the kill literal and deck130's rule promotes the winning step above the procedure. **Two re-keys fall due together on D4**: the literal becomes `{kills 0 of the 2 CREATURE targets …}` plus a lethal tail, and deck130's step-0 rule then reads off the row instead of off the pilot's arithmetic. Note the direction: **if D4 does NOT ship, deck130's rule must stay exactly as written**, because it is the only thing on either surface that knows a face shot can be lethal. | At D4: deck123 re-keys to the new denominator; deck130's step 0 gains *"…and the row will say so: `- and N to the opponent at life K WINS THE GAME`."* Until then, no change. |
| **B6** | `deck146` 146-A (cast-order entry 5 re-keyed off `{right now:` on a Command row) | **#188** | **PASS — and this is the amendment's origin.** The old key rendered **0 of 28** on the card it was about while rendering 1,586 times corpus-wide, so a whole-corpus grep passed and the entry was dead. The reviewer found it with the per-card audit and re-keyed. **Obligation on D5:** when the modal clause ships, the new key is `{modes live right now: …}` and the current graveyard-reading instruction is CUT, not left beside it (#187). | At D5: *"The Command row prints `{modes live right now: …}`. Read it. Do not reconstruct your graveyard from the log."* |
| **B7** | `deck152` 152-A (the converter hold given the blocker list's cardinality) and `deck152` 152-B (#171's scoping sentence ported from deck146) | **#202 / #203** | **PASS on both, and 152-B is a PORT, not an invention — which is the point.** The pool supplied its own control: deck146 and deck152 quote the same coverage literal, deck146 carries the scoping sentence and shipped **0** two-plus-land sevens, deck152 did not and shipped one for the third corpus running. **Obligation on D17:** when the attackers header prints `They have N untapped creatures able to block`, 152-A's "count the distinct names in `their untapped blockers`" is a derivation the literal answers and is cut. | At D17: *"The attackers header prints how many of their creatures can block. Declaring more attackers than that number leaves the surplus unblocked — that is the trade, and the tag on any one blocker is not."* |
| **B8** | `deck162` 162-A (the survival gate moved to the cast window) and 162-B (the first-Mine exemption given imperatives) | **#205 / #207** | **PASS, and 162-B is the wave's cleanest demonstration of #207.** Wave 52 gave the rule's FIRING branch three imperatives and its EXEMPT branch a half-sentence; the brake then worked perfectly (3/3 declines at K=0, 4/4 takes at K>=1) while the exempt first Mine was declined **11 times across three games**, arriving four turns late twice, against a wave-52 over-brake count of 1. The discriminator is exact and free — a second copy's row carries `[second copy: …]` and a first copy's does not. **This deck is no longer exit-eligible and the reason is an edit, not a deck.** | none now; 162-A's render companion is D18 and re-keys to it. |
| **B9** | `deck123` A53-4 and `deck130` D53-4 (the HOLD-row teaches) | **#154 / D2** | **PASS — and a watch with a short fuse.** Both guides now teach a row that renders 1,173 times and is taken 29. The risk to watch is identical to wave 52's B9: the sentence drifting into "do not answer these windows", which would be a guide encoding an engine cadence. It has NOT happened — 234 of 240 declines at deck123's seat are correct. **Obligation on D2:** the row's own text changes (it will lead with "Pass priority") and BOTH sentences re-key to the new literal on the same wave; a guide quoting the retired wording is the exact shape that cost deck126 its Lantern rule. | At D2: quote the new row verbatim, once, in each guide, and cut the old quotation. |
| **B10** | all seven `deckN/strategy.txt` — SIZE, and the guides' surface | **#146** | **A STANDING FINDING, not a defect of any one guide: the files have outgrown the skill's own stated ceiling and nobody is measuring them.** The skill's size discipline is written against "27-38 KB"; the deployed wave-53 guides are **39.6 KB (deck162), 53.9, 56.0, 57.1, 58.3, 61.7 and 70.2 KB (deck123)** — deck123 is nearly double the top of that band. Two facts that bound the alarm, both checked on disk: the guide is **NOT** in the per-decision `prompt` (0 of 826 prompts at deck126 contain a guide-only phrase — it lives in the system message), so it does not compete with the game log for D32's 30 KB tail; and every edit this wave is justified by a cited seq. But no seat file reports its guide's net size change, the skill's band is now stale by a factor of two, and #146's discipline cannot be applied to a number nobody prints. | wave-54 brief: every seat file states each guide's byte size before and after, and its net change with the trims that paid for it. Synthesis re-states the band from the measured distribution rather than from the 2026-08 figure. |

**Not findings — checked so nobody re-files them.** deck126 D84 (the answer-FORMAT check: read the
number you wrote against the sentence you are about to write after it): #196 exactly, and its
detector half is docket D14 — PASS, and it is correctly a format rule and not a card rule.
deck126 D87a (Overgrown Battlement gains nothing on a block, quoted against `mtg.txt:83596`): #195's
named non-member — PASS, and the primitive was quoted correctly by two seats.
deck125 W77a-c (the Emrakul stop re-keyed from a battlefield-line substring to the dedicated
`LIFE-TO-DAMAGE CONVERTER` block, 353 renders): #193 — PASS. deck125 W81 ("nothing else was
castable" refused by name, with the seat's 155-of-155 sweeper decline ceiling as proof): #194 —
PASS. deck130 D53-6 (the mulligan anecdote compressed under the UNTESTABLE verdict rather than the
rule deleted or restated): #200 exactly — PASS. deck146 146-T DELETED on its live denominator
(15/165 raw, 15/26 live, 0 decisions protected): #201/#181 — PASS, and a deletion is the right
outcome of a ritual gate, which is worth saying because it is the first time one has been taken.
deck146 146-C (the cleanup order given a DECIDING SITUATIONS surface keyed to
`"your hand has N cards and your maximum hand size is 7"`): #190's step 0 — PASS.
deck152 152-C (the tap clause's stop re-keyed to the clause's NAME COUNT rather than restated):
#204 — PASS, and its surviving "breaks" were correct play in a won game, which is #184.
deck123 A53-1 (the fetch carve-out fenced so the exception cannot contradict the rule above it):
#191 — PASS. Every guide's card facts were spot-checked against the primitives: Marsh Flats
`target(*[swamp;plains]|myLibrary)` (71436), Tundra `subtype=Plains Island` (124691), Underground Sea
`subtype=Island Swamp` (125732), Hammer of Bogardan `target=anytarget auto=damage:3` (52046), Pyrite
Spellbomb (90168), Path to Exile (84676), Overgrown Battlement (83596), Silverquill Command /
Silencer / Triumphant Adventurer `mana={B}{W}`, Sigarda and Ranger Class (borderline.txt) — **no
primitive/Oracle divergence found by this step**, and no seat needed Scryfall.

---

## For the next wave

The wave-54 docket is engine correctness at the top for the fifth wave running, and its shape has
changed character: **the top five are one shipped-but-unverified hang fix, one mechanism that works
and cannot be honoured, and three numbers the render computes and points at the wrong screen.**
D1 is a verification item — lane X's fix is in master and the only thing that discharges it is a
21/21 corpus. D2 is the wave's whole story: the HOLD row is the correct instrument, the model took it
29 times, and it was retired one phase later by a key that watches the opponent's library; three
keying and vocabulary fixes cost nothing, remove no window, and are the only remedies the records
survive. D3, D4 and D5 are one-clause emitter changes with PARSETEST cases and a suite pin each — a
destination-zone gate on a compensation ask, a player target counted and priced, and a per-mode
live/dead clause on modal rows — and D4's is the only defect this corpus that lost a game by itself.
D6-D21 are the render's remaining silences, and **six of them (D4, D5, D10, D17, D18, D21) are
re-keys guides are already waiting on (#187)** — the wave-54 reviewer brief must carry them in its
#136 checklist BEFORE the corpus, because B3-B9 name the exact sentences that go stale on the day
each ships. D34-D38 are the carried engine items with no corpus surface; **D34 (replay) is the one
that compounds**, because four independent lanes reported the same thing this wave — every owner play
report is investigated from a transcript and a fixture rather than from the board that produced it,
and one lane could not build a red-on-base fixture at all as a result.

The guide lane's work this wave is three audits the corpus proved, all run BEFORE the corpus: the
per-(literal, card family) three-number audit read from the ROWS (#188 — it found a condition that
was unsatisfiable on its own card while its literal rendered 1,586 times elsewhere), the reverse
audit B1 discovered by accident (not "did my literal change" but "did a lane make one of my SENTENCES
false" — deck125 carried two sentences a lane had falsified a whole wave earlier), and the
per-EDIT #177 diff with its ask-kind step 0 (#190 — a correct rule filed under the wrong heading
scored 0 of 3 and the seat died at 0 against an opponent on 2). **deck162 is no longer exit-eligible
and the reason is an edit, not a deck**: closing a rule's empty case opened its exempt case, 1
over-brake window became 11, and #207 is the diagnostic that would have caught it in the edit.
deck146's cast-order entry 5 and its cleanup order are both first-corpus findings with first
statements and need a corpus. deck152 has three lanes open and its mulligan floor has now broken on
the same shape in three consecutive corpora — the fix is a PORT of deck146's sentence and #203 says
so. deck125 is 4-1 with a single named loss whose cause is a threshold SHAPE, and its two drifting
gates (the tap-out gate at 8 breaks, the Emrakul converter stop at 2 breaks and 30 life) are both
re-keyed to literals rather than to arithmetic — re-test before any freeze talk. deck126's Lantern
result is the strongest argument in the corpus for the render-first order: **5 takes of 5 became 0
takes of 6 with no guide change at all.** deck123's Damnation plan-text lane is at its THIRD corpus
at 0 of N and is now formally exhausted — the row already computes N, K and M, so the ask becomes a
row annotation, not a fourth restatement. deck130's face-damage floor is at its third corpus
non-zero and is the same call one wave from now.

**Specimen for the owner's per-wave prompt review (invariant 0).** **Keep the harvest default** —
`wave53/lategame-specimen.txt` = `1788334427-…deck125-…-vs-…deck146.jsonl` seq **357**, turn **84**,
Main phase 1, 28,481 chars, 6 rows: the deepest decision turn in the corpus and the decking endgame,
carrying the `#1-#4 x4` collapse, the `{X pricing:}` block, three honestly-priced dead sweeper rows,
the PLAN age stamp and the log-trim header — seven annotation families, and **84.2% game log**, which
makes it the D32 exhibit as well. **Recommended alongside it, and this step endorses the engine
seat's companion:** `wave53/lategame-specimen-render-dense.txt` = `1788334424-…deck125-…-vs-…deck126.
jsonl` seq **66**, turn 22, Draw, **10,412 chars**, 4 rows, 27 life vs 20 — verified on disk to carry
**ten** annotation families including the **HOLD row**, `[you declined this exact list N times
already this turn]` and the PLAN age stamp, which makes it **the only single screen in the corpus
carrying all three of this wave's new forms**, at a size a human reads end to end. That is what the
owner's criterion — decision value per token — is actually measured on. **seat-123-130's proposal
(`130v125` seq 181-186) is declined for this purpose and promoted for another:** those six records
are turn-48 annihilator sacrifices of ~20 KB each at ~99% game log, growing 675 chars per step, to
pick a Mountain from a battlefield-ordered list of 18 — they are the best single EXHIBIT in the
corpus for three docket items at once (D6's `your own effect` on the opponent's Emrakul, D8's
fourteen uncollapsed Mountains, D25's unsorted list), and they show the engine at its worst rather
than the render at its most informative, which is the opposite of what invariant 0 asks. **The size
ceiling, if he wants the worst case:** `1788334464-…deck146-…-vs-…deck123.jsonl` seq **92** (32,068
chars, turn 33, priority) — the corpus maximum and the D32 exhibit, which is his call and no one
else's.

---

## What this step did NOT check

- **I did not run wagic, build anything, run git, deploy anything, or edit any file under
  `projects/mtg/bin/Res/**` or `src/**`.** Four source files were READ (`src/AIPlayerGPT.cpp`,
  `src/GuiPlay.cpp`, `include/AllAbilities.h`, and greps over `include/MTGCardInstance.h`) to pin the
  hold key, the removal-verb seam, the `selfInflicted` discriminator, the PLAN capture and the
  `Replace()` call; every attribution from those reads is stated as a read, not as a debugger trace.
- **I did not re-score any deck's guide rules.** Set A is summarised from the seat files' own
  verdicts; the only Set A numbers I re-derived are the ones a docket item rests on.
- **I did not read all 21 `.stderr` files.** I read the hang's in the seams the seats cited and
  grepped the rest for the specific lines quoted (`declared blocks from`, `hold re-opened`,
  `no reply after`, `with no source tapped`).
- **I did not verify lane U's, V's, W's or X's gates by running them.** Their suite/PARSETEST numbers
  are quoted from the lane reports; they are recorded as SHIPPED-UNTESTED because they are not in
  this corpus's binary, and each carries its own falsifier.
- **I did not audit `- Paid` receipts pip by pip** (496 deduped lines; counts, no-source and no-payee
  only), and I did not re-run the wave-52 opponent-turn cost analysis at per-seat phase granularity.
- **I did not check the Vita transcripts myself.** Lanes R/S/T/U/V/W read them; I read the lane
  reports and verified only `GuiPlay::receiveEventPlus`'s `Replace()` call site in the source.
- **The `[second copy:` per-card census, the modal-menu census and the `your own effect` census were
  taken from `options_text` and `prompt` respectively**; where a literal appears both on a row and in
  the log, I counted the row and said so (#208).
- **I did not attempt to reproduce the hang, the Dominating Vampire livelock, or any replay
  divergence.** All three are recorded with the next step that would settle them.
