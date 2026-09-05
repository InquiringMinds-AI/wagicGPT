# Wave-63 step-1 lane brief — KNOWN BUGS ONLY

Base: master 41fbad879 (wave-62 close). Baselines on caa5d6ea3, hermetic `make -B`: PARSETEST 4111/0, suite 1254
with 2 known concurrency-only failures (+ intrepid_adversary_repeated_payment.txt, a genuine intermittent: see E17) (lifeline + merrow_reejerey; 0 at
WAGIC_TESTSUITE_THREADS=1), 65 AI/0, 0 timeouts. Model for any live probe: Spark 35B at
http://100.116.136.74:8084, model `qwen36-35b-a3b`, key via `ssh spark 'cat
~/.config/vllm/qwen35-api-key'` (never print it). Live probes are optional; fixtures and
PARSETEST are the gate.

Your items are wave-62 findings; read each in FULL in the files cited by
`projects/mtg/strategy-design/wave63/known-bugs.md` (item numbers below). The
ledger's "Ask" is a proposal, not an order — fix the mechanism the finding proves, in the
rules-correct general form, and state one falsifiable prediction per item for the wave-63
corpus. Do not widen: nothing beyond your items, no refactors, no style edits.

Rules (all binding):
- Doctrine: enforce legality without constraining choice — NO hard caps on legal choices, NO
  window removed, NO blind caches; every change adds a true token / true row / stamp or one
  re-ask and deletes nothing the model relies on.
- Every parse/render change ships PARSETEST cases (a positive, a must-NOT-match negative, and the
  echo shape of any new bracketed/braced annotation). Every engine-behaviour change ships a suite
  fixture RED on base, GREEN after (state how you verified RED).
- Byte-splice (python rb/wb) AIPlayerGPT.cpp, MTGRules.cpp, GameObserver.cpp (CP1252+CRLF),
  MTGAbility.cpp, AllAbilities.cpp, primitives/*.txt, _cards.dat — non-UTF-8 bytes; after any
  edit verify `git diff | /usr/bin/grep -c $'\357\277\275'` prints 0. Use /usr/bin/grep for
  searches (bare grep is a wrapper that drops matches).
- MEMORY CAP on every wagic run: prefix `systemd-run --user --scope -q -p MemoryMax=4G
  -p MemorySwapMax=0 --`; detached units get `-p MemoryMax=4G -p MemorySwapMax=0`. A kill under
  the cap is a RED test pointing at an allocation loop in your change.
- Build from <wt>/projects/mtg: `rm -f bin/wagic && make -f Makefile.sdl -j4` (qmake only with
  `-o Makefile.sdl`, only if you add sources). Suite from bin: `env -u WAYLAND_DISPLAY -u DISPLAY
  WAGIC_HEADLESS=1 WAGIC_TESTSUITE=1 ./wagic` — count BOTH `==Test Failed !==` and `==Test timed
  out`. PARSETEST: `WAGIC_GPT_PARSETEST=1 ./wagic` (must read ` 0 failed`). Scoped runs:
  `WAGIC_TESTSUITE_FILE=test/_probe.txt` (Res-relative; delete the probe file after).
- Gates run DETACHED: `systemd-run --user --unit=w63-<lane>-gate -p MemoryMax=4G
  -p MemorySwapMax=0 --working-directory=<wt>/projects/mtg/bin bash -c 'unset WAYLAND_DISPLAY
  DISPLAY; <legs> > ~/.gatelogs/w63-<lane>-*.log 2>&1; touch ~/.gatelogs/W63_<lane>_DONE'`, then
  WAIT with a FOREGROUND `until [ -f ~/.gatelogs/W63_<lane>_DONE ]; do sleep 10; done` (timeout
  1200 s). Never a Monitor, never a background task, never end your turn with work pending.
- `cd` into your worktree inside EVERY Bash call (cwd does not persist). All scratch, logs and
  done-files go under ~/.gatelogs/ or <wt>/projects/mtg/strategy-design/wave63/ — never ~ or
  ~/Projects.
- Diagnostics you add are compile-time gated `#if defined(_DEBUG) || defined(WAGIC_DEVLOGS)`.
- Tag every code comment for your change `#W63-<lane> (En)`.
- COMMIT on your lane branch in your worktree when the gate is green (suite 0 new failures,
  PARSETEST 0 failed). Stage explicit files only (`git add -f` for anything under bin/Res; never
  CLAUDE.md; never `-A`). NO AI attribution of any kind in the message: no Co-Authored-By, no
  Generated-with, no session link.
- Write `<wt>/projects/mtg/strategy-design/wave63/lane-<lane>.md`: what shipped (files,
  mechanism), gate numbers, predictions, and what you did NOT verify. Never write a mechanism as
  the owner's position. Report the commit hash at the end of your reply.

Common to every lane: the deck reviews cited in known-bugs.md are the evidence — read the cited seq
records in the corpus (~/.Wagic/ai/gpt/selfplay-runs/matchups-20260905-133058 (+ the 152v125 rerun matchups-20260905-161801)/, read-only) before
designing; every render change ships PARSETEST cases (positive, must-NOT-match negative, echo shape);
every engine change ships a fixture RED on base. Tag comments `#W63-<lane> (En)`.

Standing rule from wave 60 (still binding): after your lane merges, a Codex adversarial review reads the combined diff
(wave61/codex-review.md and wave62/codex-review.md show what it catches — first-match breaks, source-only gates, false render
claims, PARSETEST cases pinning wrong expectations); write so that review finds nothing. Every
aggregate you print must either fold every trigger the per-row tags already price, or say what it
excludes. Read wave62/lane-X.md, lane-Y.md, lane-Z.md, lane-AA.md for the emitters you will touch, wave62/engine-seat.md for the
adjudication, and wave61/corpus-livelock.md + wave62/corpus-hang.md (two corpus-killing regressions in two waves; a lane that
touches a shared choke point - a chooser, a key, a marker - must ask what ELSE reads it):
ANYTHING you print that can change between two rebuilds of the SAME window (a counter, a clock, a running total of asks)
must never enter the async slot key (mPromptTail) or the ask key — pass the note-free tail as keyTail. That defect cost an
11-hour corpus.

## Lane AB — E1, E3, E4 (combat renders: the block side of the life loop, the block suggestion, gang blocks)
E1 (deck123 HIGH-1; wave-62 synthesis R331): D2 shipped attack-side only. 123v126 seq 76: the blockers header
promised `best case with every blocker assigned: you would be at 16` and B rows priced `they gain 1 from this block
only` beside `(you kill it, your blocker lives)` with Sanguine Bond + Exquisite Blood on THEIR side; the block took
the seat 16 -> 0 in eleven lose-1/gain-1 pairs off a won board. Apply D2's fail-closed rule to the blockers header
and every lifelink/lifegain price on a B row: no survival number, name the loop, say declining is the survivable
line when it is. E3 (deck152 HIGH-1/2): the survivable-block suggestion is life-maximising only and the model copies
it verbatim (146v152 seq 25: named a chump while the same line offered two kill-and-live results; seq 28 then had
no survivable line); on a lethal screen it withdraws entirely and the unguided seat picked the damage-MAXIMISING
block. Rank: survive AND keep material > survive; on a lethal screen still name the least-damage line, labelled as
such. E4 (deck162 HIGH; deck146 HIGH-2): `- GANG BLOCK:` sits behind three friendly per-blocker parentheses on an
uncollapsed attack tag (162v146 seq 16; the reply reasoned it out loud and attacked into it); attack A-lines price
`(blocking trigger:` per lone block and never say prices SUM (146v126 seq 32: worst tag 5, three walls, opp gained 9).
Hoist the verdict to the front of the tag; sum blocking-trigger prices with a `(N if all block)` figure.
Read wave62/lane-X.md and lane-Z.md (D12) first.

## Lane AC — E2, E5, E9, E13, E15, E16 (prices and rows that lie by omission)
E2 (deck125 HIGH-1): the CLEANUP PRICE tail ignored four draw triggers already ON THE STACK in the same prompt and
priced the floor off pre-cast life (125v162 seq 89 "up to 7 discards ... at worst 4"; seq 94: 10 discards, 20 life,
25 -> 5). Fold stacked draw triggers and the cast's own life delta. E5 (engine HIGH-1, MED-10, LOW-11; deck130): ONE
Lair of the Hydra reads five times in CRACK-BACK (130v152 seq 17-19; `AIPlayerGPT.cpp:18317-18330` appends per
script line — lane Y's D4 per-ability ranking leaked into lane X's walk); the animate clause prices nothing and misses
the comma power form; Lair's own rung list stops one short of affordable. Collapse per permanent, price the best
affordable rung, fix the rung ceiling. E9 (engine HIGH-4; D19 FAIL): the crack-back relief clause fired 0 times
corpus-wide including its two repro windows — find why (`crackBackNextTurnDue` gate? rows never reach the emitter?)
and make it fire, PARSETEST the composed path. E13 (engine MED-8/9; deck162 MED): the DRAW PUNISHERS board paragraph
still carries D3's direction error; "N life to you from your punishers" reads as GAIN; `{feeds:}` converters vs
discard punishers invite conflation. E15 (deck123): the `mana of ANY colour pays a generic cost` clarifier renders on
1922/1922 nonzero-mana lines and 0/177 zero-mana lines — preceded a 7,680 B reply concluding the deck uncastable; state
it on zero-mana lines too (or the cheaper truth: what would make mana). E16 (deck125 MED): `{spare:}` never tags a
land while Emrakul is in hand. Read wave62/lane-X.md, lane-Y.md (D4), lane-S.md (wave 61) first.

## Lane AD — E6, E7, E10, E11, E14 (the record, the latch, the hold, the decline row)
E6 (deck146 HIGH-1, deck126 HIGH-1, deck152; engine HIGH-7): `answer_replaced` overrode the model's first `CHOICE:`
from a coded line inside trailing chain-of-thought (146v162 seq 41, `latched_coded_line: 2`, 2,803 B of CoT) and
`reply_trimmed_bytes` cut the deciding line OUT of the record — an override nobody can audit. It won games (152v162
seq 34) and lost windows (126: 7 latches, two dropped Staff pings). Three things: (a) the record keeps the latched
line verbatim whatever the trim; (b) decide, with the wave-62 lane Z D9/D10 bound in hand, whether a coded line
inside the PLAN/CoT paragraph may override at all — check where these seven fell relative to `planParagraphBound`,
and if inside, the bound is not holding; (c) the reply protocol (`kReplyProtocol`) is in ZERO logged prompts (it is
in the system message) — put the system message, or its hash + edition, in the record so protocol claims can be
adjudicated. E7 (engine HIGH-5; D5/R1 FAIL): the ANNOUNCE_X Decline row vanished on one 4-row menu after mana was
committed — make it unconditional on the ask or print why it is absent. E10 (engine HIGH-2; D13 FAIL; deck130/125
MED): the hold promise is broken 15x at one seam (a hold in main 1 is void in main 2; only `(combat comes next this
turn)` differs on the pass row); hold row offered 459x taken 6, declined-N tag 204x held 4 — scope the promise to
what the latch honours, and make the pass-row phase clause not break the row identity. E11 (engine HIGH-6): every
`reveal` record carries `choice: 1` and a false parse stamp. E14 (deck126 HIGH-1 second half): the carried plan
asserted two exiled enchantments "on the battlefield" for eight turns — the carry is a scratchpad; bound it to the
plan sentence (D16(a)) and drop a carried plan that names a permanent no longer in the zone it claims.
Read wave62/lane-Z.md and lane-AA.md (findings 2, 8) first. Anything you print that varies per rebuild stays out of
both keys (wave61/corpus-livelock.md).

## Lane AE — E8, E12, E17, E18 (a wrong witness, a card script, an intermittent, a missing half)
E8 (engine HIGH-3; D1-p3 FAIL): `enters UNTAPPED: you control X` names a witness that lacks the named subtype —
the witness search matches by name where it should match by subtype (or vice versa); fix and PARSETEST the shape
with a Plains-typed nonbasic vs a same-named non-Plains. E12 (deck130 HIGH, card): Nadaar, Selfless Paladin
`borderline.txt:75387` is `lord(creature|myBattlefield)`; Oracle (Scryfall — verify it yourself) says "Other
creatures" — express the Oracle in the card script (`other`), no engine change; suite fixture RED on base (Nadaar
alone prints 3/3, not 4/4). E17: `intrepid_adversary_repeated_payment.txt` is a GENUINE intermittent — fails 1-2 of 3
solo at THREADS=1 on both 66afd72bb and caa5d6ea3 (run it 10x to see the rate); diagnose the nondeterminism (map
iteration over pointers? unordered container? uninitialised field — lane Y found `AIAction::id` uninitialised last
wave) and fix the CAUSE, not the test. E18 (D7's missing half): the `{paying this taps: ...}` clause on the
pay-repeat row had no window in the corpus; verify by PARSETEST on a composed board that it renders when the payment
would tap an attacker, and that D7's mana half and the taps half agree. Read wave62/lane-W.md, lane-Y.md, lane-T.md
(wave 61) first.

## Lane AF — the nine findings of wave63/codex-review.md (fix or refute, each with evidence) + E19
Worktree: worktrees/lanes/w63-AF (base = master after the AB-AE merge + the review commit + the Mossborn fix
76e4f58b9). Read wave63/codex-review.md in full, then the lane report each finding touches (lane-AD.md for #2/#4/#5,
lane-AB.md for #6, lane-AC.md for #3/#7/#8, lane-AD.md + wave62/lane-AA.md for #9; #1 is wave-62 lane Y/AA's
ANNOUNCE_X route); wave62/lane-AA.md shows the expected shape of a CONFIRMED/REFUTED table. For EACH finding: VERIFY
against the code first (REFUTED with the line that proves it, or CONFIRMED with a reproduction — a PARSETEST case or
fixture failing on the current tree); fix every CONFIRMED one in the general form. #1 is HIGH and a doctrine breach
(no hard caps on legal choices): the ordinary ANNOUNCE_X route must offer every affordable X and the renderer must
not call an affordable X unaffordable — if the menu needs a bound, it is the affordable maximum, computed, not 50.
#2 is HIGH but ADJUDICATE it against E10's evidence (wave62/engine-seat.md HIGH-2: the owner-facing breaks were a
hold in Main 1 retired in Main 2 on a BYTE-IDENTICAL board): a hold honoured in Main 2 when nothing moved is the
promise kept; a hold honoured in Main 2 after combat or any board change is the defect — the board key already
re-opens on change, so CONFIRM only the case where the phase clause was the ONLY difference AND the model could
not have known Main 2 differs; if you keep the normalization, the hold row must say it spans both main phases.
#3: the `lands + 2` reach cap is a heuristic that pins a false "spare" — use real reachability (the most expensive
castable card's cost) or drop the claim. #4: "Wait" as an operative verb must not cut the plan — narrow the
self-correction markers to correction shapes ("Actually", "Wait, no", "Correction:", "Scratch that"). #5: the
absent-permanent guard must respect ownership words ("I control", "my", "their"). #6: material rank compares
lexicographically (fewest blockers lost first), not by sum. #7: the relief clause recomputes the crack-back total
without the killed body's static effects, or says "before static effects". #8: the pending-draw scan covers
`DrawAction` objects as well as `StackAbility`. #9: headers received is not a completed round trip — a body-phase
deadline expiry is `timeout`. **E19 (owner, Vita vpk26; Mossborn fixed 76e4f58b9 with fixture
mossborn_hydra_doubling_one_event.txt as the model):** eight more cards script "double the number of +1/+1
counters" as `thisforeach(counter{1/1.1}) counter(1/1)` (N one-counter events) — Kalonian Hydra (mtg.txt), Primordial
Hydra (mtg.txt), Hydra's Growth, Solarion, Tanazir Quandrix, Biogenic Upgrade, Invigorating Surge, Visions of
Dominance (borderline.txt). Verify each Oracle at Scryfall, rewrite as ONE event `counter(1/1,counter{1/1.1})` (for
targeted ones confirm the expression evaluates on the TARGET — `WParsedInt(nbstr, spell, target)` — with a fixture),
and ship a Hardened-Scales fixture per card (RED on the old script). Do NOT touch the Modular/other `thisforeach`
users — their Oracle is "put its counters on", a different shape. Gate as usual (suite THREADS=1 0 failed, AI count,
PARSETEST 0 failed, RED evidence per confirmed finding). Tag `#W63-AF (Rn)` / `#W63-AF (E19)`. Write
wave63/lane-AF.md with the CONFIRMED/REFUTED table.
