# deck131 -- wave 27 review (UR Guttersnipe spellslinger). Favorable-block exception FIRST corpus; ROTATION test applied.

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-193608/` (binary ffec79fd8, step-1 batch
ffec79fd8, -T 3000, REPPENALTY=1.05). deck131 = 6 games. Live guide = the wave-26 REVISED guide (13264
bytes; deployed == wave26/deck131/strategy.txt AND == wave27/deck131/strategy.txt, all three byte-verified
via `cmp` exit 0). So the favorable-block exception + lethal-attack reconciliation are LIVE this corpus for
the first time. **No guide edit this wave -- strategy.txt is FROZEN (byte-copy).**

**Record: 3 W / 3 L (3/6, up from 2/6).** Seat->game map (results.tsv; deck131 is deck0 vs 137/93/134/18/22,
deck1 vs 136 -- read winner/life columns per row):

| result | vs (deck) | end life me/opp | turn | one-line cause |
|---|---|---|---|---|
| **WIN (adj)** | 137 (Selesnya Adventures, guided 2/6) | 15 / 11 | 19 | Guttersnipe engine ground a go-wide deck; ahead on life at cap; correct per-creature tapped read held Gut home; two FAVORABLE blocks DECLINED (non-decisive, see below) |
| **WIN (death)** | 93 (Relentless Rats, Step-0 guideless 2/6) | 20 / -2 | 14 | YP + token swarm raced 20->3->dead, took ZERO face damage; free win vs a deck that could not race |
| **WIN (adj)** | 134 (Urza tron control, Step-0 guideless 0/6) | 32 / 9 | 18 | dominated a control deck that could not kill it; ground opp to 9 but could NOT close (won only on life-at-cap) -- the "no reach" flag shows up even in a WIN. Two priority unparsed fallbacks (s39/s40, benign, decomposed below) |
| LOSS (death) | 18 (Kithkin anthem aggro, guided 5/6) | -6 / 20 | 11 | out-raced by wide anthem + first strike; T3 YP trade into Goldmeadow Dodger; fast aggro loss |
| LOSS (death) | 22 (mono-R Giants ramp, NOW guided 5/6) | -15 / 12 | 13 | Giants go-wide out-raced 131; correct favorable blocks (s10/s18/s22) could not stop the swarm. Last wave 22 was a FREE win (guideless); now guided it beat 131 |
| LOSS (death) | 136 (Yarok of Pranks, Step-0 4/6) | -1 / 4 | 17 | Knight of the Ebon Legion (4/5 deathtouch) wall; correct never-block holds; no reach to push past, lost by 1 (opp 4) |

**Bottom line: CONSTRUCTION-TERMINAL, unchanged from the standing 9+-wave diagnosis, now at 3/6.** The three
wins are TWO adjudications (grind-outs where 131 was ahead on life at the turn cap vs a slow control deck and
a go-wide deck it could out-durdle) + ONE free race vs a guideless Rats deck. NONE is a construction change.
The "no reach to CLOSE" flag is now visible INSIDE a win: vs134 got the opponent to 9 life but could not
finish, winning only because it was 32-vs-9 ahead at the cap. The three losses are the known limits (fast
aggro vs18, go-wide vs22-now-guided, deathtouch-wall vs136). The 3/6 is draw/matchup variance over the same
construction-capped deck -- do NOT read 0->1->2->3 across waves as a trend (n=6, single run).

**Two priority fallbacks this corpus (vs134 s39/s40, both unparsed_reply); zero elsewhere.** Both decompose
BENIGN (model-side; engine render verified CORRECT; non-decisive -- game won 32/9 adj). See MANDATE 2.

**Favorable-block exception (wave-26 edit) FIRST-CORPUS verdict: VALIDATED but applied INCONSISTENTLY.**
Fired correctly 3x (all vs22); DECLINED 2x (vs137) where a free kill was available. The declines are MODEL
inconsistency in applying an already-correct rule (a WATCH), NOT a guide defect and NOT a new corner. See
MANDATE 1.

**Guttersnipe tapped-read guard, ROUND 5: NO misread recurrence.** See MANDATE 3.

---

## MANDATE (1) -- FAVORABLE-BLOCK EXCEPTION, FIRST CORPUS

The wave-26 edit added exception (a): "FAVORABLE block -- Guttersnipe SURVIVES. If an attacker's POWER is
less than 2 ... Make this block whenever a small attacker (power 0 or 1) is coming in and no spell answers
it -- you lose nothing and remove their creature. ... this is FREE removal, not a trade." Every power<2
attacker window where Guttersnipe could block, this corpus:

### FIRED CORRECTLY (free removal, Guttersnipe survives + kills):
- **vs22 s10 (T5, life 20):** Universal Automaton (1/1) attacks; Guttersnipe (2/2) the only creature.
  Reply `BLOCKS: B1:A1`: "The Universal Automaton is a 1/1. Blocking it with Guttersnipe results in the
  Automaton dying and Guttersnipe surviving with 1 damage marked ... blocking it for free removal is
  strictly better." Engine option even labeled it "you kill it, your blocker lives". **TEXTBOOK exercise of
  exception (a). CORRECT.** This is the clean first-corpus validation.
- **vs22 s22 (T11, life 16):** opponent alpha-strikes with 4 creatures (Stinkdrinker 1/3, Automaton 3/3
  trample, Sunrise Sovereign 5/5, Blind-Spot Giant 6/5 trample = 15 power). Guttersnipe the only blocker.
  The model correctly enumerated that Stinkdrinker (power 1) is the ONLY block Guttersnipe survives, blocked
  it (survive at 1 life, preserve reach). CORRECT identification of the favorable block under pressure.
  (Parser note below -- the model wobbled the first coded line but the parser recovered the correct answer.)

### FIRED (within envelope, harmless, no kill -- minor model PT-misread):
- **vs22 s18 (T9, life 19):** Stinkdrinker Daredevil (1/3, power 1) attacks; Guttersnipe blocks. Model:
  "favorable trade: Guttersnipe (2/2) kills the 1/1 attacker while taking only 1 damage and surviving."
  Stinkdrinker is a 1/3, NOT a 1/1 -- Guttersnipe deals 2, does NOT kill a 3-toughness creature (engine
  option said "neither dies"). So nothing dies; the block prevents 1 face damage and Guttersnipe survives.
  The ACTION is within exception (a) (power<2 => Guttersnipe survives), but the REASONING was factually
  wrong (misread 1/3 as 1/1, and "trade" language for a no-kill block). Non-decisive, non-harmful. A minor
  PT-read wobble, not a guide issue.

### DECLINED where the exception CLEARLY applied (the miss):
- **vs137 s41 (T17, life 17):** attackers Faerie Guidemother (1/1, flying) + Giant Killer // Chop Down
  (1/2). Engine option: "B1. Guttersnipe (2/2) - may block A2 (you kill it, your blocker lives)". A free
  kill of the 1/2 was on offer. Reply `BLOCKS: none`: "trading it for a 1/1 or 1/2 creature is unnecessary
  when I can simply absorb the damage ... trading Guttersnipe is a net loss." The model treated a FAVORABLE
  block (Guttersnipe survives) as a TRADE -- exactly the confusion exception (a) exists to correct. MISS.
- **vs137 s45 (T19, life 15):** attackers Faerie Guidemother (1/1 flying), Giant Killer (1/2), Human (1/1).
  Engine option: "B1. Guttersnipe - may block A2 (you kill it, your blocker lives), A3 (you kill it, your
  blocker lives)". TWO free kills on offer. Reply `BLOCKS: none` again: "trading it for a 1/1 or 1/2 ... is
  a net loss." MISS.

Both misses were NON-DECISIVE (vs137 was WON on adjudication). Both left free removal on the table -- had
the model taken the ground block, it would permanently remove a recurring attacker at zero cost (Guttersnipe
does not tap when blocking). The likely trigger for the decline: a flyer the model could NOT block was also
attacking, so it reasoned "I take damage anyway, why bother" and reverted to the old "never trade
Guttersnipe" frame -- undervaluing PERMANENT removal of a ground attacker.

### CORRECT HOLDS (exception correctly did NOT over-fire):
- **vs22 s14 (T7, life 20):** Stinkdrinker (1/3, "neither dies"); `BLOCKS: none`. Fine (no kill available;
  declining a no-kill block to take 1 is equally defensible).
- **vs136 s19 (T9, life 21) + s23 (T11, life 17):** Knight of the Ebon Legion (4/5 DEATHTOUCH). `BLOCKS:
  none` both. CORRECT -- Guttersnipe would DIE (deathtouch makes any block lethal), power>=2, not lethal to
  me. The never-TRADE core held; the model explicitly cited deathtouch at s23. Clean.

**Verdict: the favorable-block edit VALIDATED (rules-correct; fired correctly when the model engaged; the
never-trade core and lethal exception did not over/under-fire).** The 2 declines are MODEL INCONSISTENCY on
an already-correct line -- a WATCH, NOT a guide defect. There is NO FOURTH corner to soften: the declined
windows are the SAME favorable-block corner the wave-26 edit already covers, not a new corner. Per the
skill discipline, a WRONG model deviation from a RIGHT line = model slipped = WATCH, never shout louder.
See notes I-2.

### Non-Guttersnipe blocks (racer creatures) -- minor deviations from the "race, don't block" rule:
- **vs18 s5 (T3, life 20):** Young Pyromancer (2/1) blocks Goldmeadow Dodger (2/2), "both die". Model traded
  its engine creature at 20 life (not lethal, not favorable). The guide says race/take-the-damage unless
  lethal. Deviation; marginal (removes an anthem-boosted attacker but loses a token-generator). Non-decisive
  (lost on construction). Below-bar; not a guide edit (judgment call, single instance).
- **vs136 s12 (T5, life 20):** Elemental token (1/1) trades with Yarok's Fenlurker (1/1). Chump/trade of a
  token at 20 life (not lethal). Same minor deviation; token-for-1/1. Non-decisive.
These are the racer-block rule ("no blockers except lethal chump") being softened by model judgment in two
low-stakes spots. Neither is a correct-deviation SIGNAL strong enough to touch the guide (both defensible-
but-not-mandated, both non-decisive, both in games lost on construction). WATCH only.

---

## MANDATE (2) -- THE TWO TURN-18 PRIORITY FALLBACKS (vs134 s39/s40), DECOMPOSED

Both `kind=priority`, `turn=18` (log header "Turn 19"; known off-by-one), `fallback=unparsed_reply`,
resolved `choice=-1`. Game WON 32/9 (adj). Full reconstruction the brief asked for.

### The board context
deck131 at 22->27->32 life vs deck134 (Urza tron) at 9. Opponent going off with a Ghostly Flicker +
Mnemonic Wall + Prophetic Prism value loop (s37 events = a wall of opponent library-reveals). deck131 is
STUCK: {u}{r}{r} available (1 blue source) and a hand of Dissipate {1}{u}{u} / Downsize {u} / Opportunity
{4}{u}{u} / Essence Scatter {1}{u} -- NONE castable (Opportunity/Dissipate need more blue; Downsize/Essence
Scatter have no legal target vs 0 opponent creatures). Land drop already used. So the engine offers ONLY one
legal action: `1. Life with Elixir of Immortality [{2},Tap: gain 5, shuffle Elixir + graveyard into
library]`. The single-option degenerate priority.

### The ask (both records)
"YOUR PLAN (as you last stated it): ... Cast Opportunity ..." + the harness's explicit stale-plan note:
"(note: the actions your plan names are no longer among the options available right now - the game state has
advanced past that plan; re-derive your choice from the current board and the options below.)" Then:
"1. Life with Elixir of Immortality". Answer format: "CHOICE: <number> (name)", 0 = pass.

### The replies
- **s39 (latency 18.8s):** `CHOICE: 2 (Cast Opportunity)` -- the model asserted its PLAN action. Option 2
  does not exist (only option 1). "Cast Opportunity" does not match "Life with Elixir". -> parser cannot map
  -> unparsed_reply. The model IGNORED the explicit re-derive note and did not read the option list.
- **s40 (latency 209.6s -- a massive decode outlier):** the model DID engage, spiralling 209 seconds. It
  correctly worked out Opportunity is uncastable, got tangled in a FALSE "contradiction" (below), and STILL
  emitted `CHOICE: 2 (Cast Opportunity)` as its first line -> unparsed_reply again.

### Root cause -- MODEL, not engine (render verified CORRECT):
1. **Plan-fixation.** Both replies default to the stated plan ("Cast Opportunity") despite the harness's
   explicit "re-derive, that action is no longer available" note. Correct answer was `CHOICE: 0 (pass)` or
   `CHOICE: 1` (Elixir). The model failed to honor an existing, correct CORE-prompt instruction -- not a
   guide gap, not deck-specific.
2. **Duplicate-copy false-contradiction spiral (the 209s).** s40 declares: "The log says Elixir went to
   library. The current situation says Elixir is on battlefield. ... Opportunity went to library ...
   Opportunity is in hand. Contradiction!" **NOT real -- the model conflates distinct copies of same-named
   cards across zones.** Traced from the log: turn 15 deck131 played TWO Elixirs onto the battlefield (MP1
   and MP2 both "Elixir: hand -> stack -> battlefield"). At turn 19, Elixir #1 was activated (s39 fallback:
   life +5, "Elixir: battlefield -> library", graveyard [Mountain/Opportunity/Aetherize/Essence Scatter/
   Counterspell/Downsize] shuffled into library). Elixir #2 REMAINED -- why CURRENT SITUATION correctly shows
   one "Elixir of Immortality" AND why Elixir is offered again at s40 (life 27->32 as Elixir #2 activates via
   the s40 fallback). The hand's Opportunity/Downsize/Essence Scatter are DIFFERENT physical copies (deck
   runs 2-3 of each) than the graveyard copies shuffled to library. The board render is INTERNALLY
   CONSISTENT. The model could not reason that same-named cards in different zones are distinct copies, and
   spent 209s manufacturing a contradiction from correct data.

### Harmful? NO.
Non-decisive: the fallback resolved (choice -1), Elixir harmlessly activated (pointless lifegain at 27-32
life; it deck-cycled the graveyard -- neutral), and at s41 the model cleanly attacked with all three
creatures. Game won 32/9 (adj). The only real costs: (a) two unparsed fallbacks the harness absorbed, and
(b) the 209.6s latency outlier on s40 (still under -T 3000, no timeout). See notes I-3/I-4.

### Routing
Engine render correct -> NOT an engine bug. Plan-fixation despite an existing correct re-derive note ->
CORE-prompt behavior (model ignored a correct instruction once, single-seat, non-decisive) -> NOT guide-
fixable, below-bar for a core change (shout-louder). Cross-zone duplicate-name conflation -> d1-adjacent
observation (d1 #N handles are BATTLEFIELD-scoped; same-name copies across hand/library/graveyard have no
disambiguation). Latency -> WATCH flag. All -> notes.md, none -> guide.

---

## MANDATE (3) -- GUTTERSNIPE TAPPED-READ GUARD, ROUND 5

Every Guttersnipe attack window (Guttersnipe on my battlefield, kind=attackers):

| game | seq | T | opp untapped blockers? | decision | verdict |
|---|---|---|---|---|---|
| vs137 | s43 | 18 | Human (1/1) UNTAPPED (Faerie/Giant Killer tapped) | held (ATTACK: none) | CORRECT per-creature read (identified the lone untapped Human, held) |
| vs134 | s23/s27/s41 | 12/14/18 | 0 creatures (tron) | SWING | CORRECT |
| vs134 | s34 | 16 | 1 x Mulldrifter (-2/2), possibly untapped | SWING Gut+YP | CORRECT (-2/2 deals 0; Guttersnipe survives any block -- harmless blocker; model did the tapped analysis) |
| vs22 | s9/s13/s17/s21 | 4/6/8/10 | untapped Mogg Sentry (3/3) each | held Gut | CORRECT (held vs a lethal-to-Gut untapped 3/3) |
| vs22 | s24 | 12 | 2 x Mogg Sentry (3/3) UNTAPPED | SWING Gut (life 2) | DESPERATION, not a misread (below) |
| vs136 | s21 | 10 | Knight (1/2) [tapped] | held | CONSERVATIVE (safe swing available; held) |
| vs136 | s31/s32/s35 | 12/14/16 | Knight [tapped] each | SWING | CORRECT (blocker tapped) |

**Round-5 result: the guard held; NO dangerous tapped-read misread.** Specifically:
- **vs137 s43** is the guard WORKING: the model explicitly distinguished the tapped Faerie Guidemother /
  Giant Killer from the UNTAPPED Human (1/1) and held Guttersnipe home. (Minor: swinging into a lone 1/1
  Human is actually FAVORABLE for Guttersnipe -- it survives and kills the Human -- so the hold was over-
  conservative. But the model correctly OBEYED the guide's attack-hold rule; there is no favorable-ATTACK
  nuance in the guide, and adding one would be speculative scope-creep. See notes I-5, below-bar.)
- **vs22 s24 is NOT a misread:** at life 2 vs a 6-creature board with no mana, the model CORRECTLY read the
  two untapped Mogg Sentries ("The untapped creatures are: Mogg Sentry #1 (3/3) and Mogg Sentry #2 (3/3).
  Both can block. ... Guttersnipe will likely be blocked and die") and swung anyway as an explicit last-
  ditch (dead next turn regardless). A correct read + desperation swing, non-decisive.
- **vs136 s21 is a CONSERVATIVE hold** (Knight tapped => safe swing was available; missed 2 damage; game was
  LOST). Same conservative-hold class as wave-26 vs102 s14; non-decisive.

**The tapped-read WATCH (I-1): NO RECURRENCE, 5th corpus running.** Stays single-SEAT (deck131 only) --
promote to a cross-seat combat-safety item only if a 2nd SEAT shows the misread. Guide UNTOUCHED on the
per-creature tapped-read prose (sanctioned-against to shout louder).

---

## MANDATE (4) -- d1-d5 + c-batch STEADY STATE + PARSER PRECEDENCE

- **(d1) R-DUPLICATE-NAME-INSTANCE: VALIDATED at this seat.** Battlefield lines carried `#N` handles for
  every same-named collision: `Elixir of Immortality #1/#2`, `Prophetic Prism #1/#2`, `Urza's Mine #1/#2/#3`
  (opp), `Mogg Sentry #1/#2` (opp), `Prism Ring #1/#2` (mine), `Island #1..#7`, `Witch's Cottage #1/#2`.
  The model referenced them correctly (e.g. vs22 s24 "Mogg Sentry #1 (3/3) and Mogg Sentry #2 (3/3)"). NO
  binding error on battlefield. **d1-adjacent GAP (new observation):** the #N handles are BATTLEFIELD-scoped;
  same-named copies across HAND / LIBRARY / GRAVEYARD have no disambiguation, and the model conflated them in
  the s40 spiral (a hand Opportunity vs a graveyard-to-library Opportunity treated as "the same card ->
  contradiction"). Not a d1 regression (battlefield #N worked); a cross-zone limit worth recording. See I-4.
- **(d2) R-CHANGELING-TYPE-INFERENCE: VALIDATED at this seat.** Universal Automaton rendered
  `[changeling: counts as Giant] [artifact] [changeling]` on the opponent battlefield across vs22; the model
  referenced it as a changeling/Giant without misread. Clean.
- **(d3) parser stash-leak fix:** no anomalous/changed ability behavior at this seat. Steady.
- **(d4) R-DFC-BACKFACE:** no Kaldheim-god DFC in pool; adventure DFCs (`Faerie Guidemother // Gift of the
  Fae`, `Giant Killer // Chop Down`) rendered cleanly with the `// ` split + `[adventure]` tag. Steady; no
  flip-thrash.
- **(d5) deck-load loud rejection:** deck131 loaded clean -- NO rejection/warning on any of the 6 stderr
  logs. (Benign steady-state stderr noise across ALL games: "card id collision! : 446807 -> Nighteyes the
  Desecrator", "AttemptNew failed to load. Deleting cache item", texture/backdrop load misses -- graphics/
  DB, not deck131-specific, no crash/assert/segfault.)
- **(c1) full target enumeration:** UNEXERCISED/IMMATERIAL (counter/bounce build; no burn-at-creature
  target-preview). **(c2) ETB pay-or-tap:** UNEXERCISED (basics only). **(c3) PT-pump render:** UNEXERCISED
  (no own-creature +N/+N). **(c5) may-ask render:** UNEXERCISED (0 may-ask records).
- **(c4) parser hardening -- CORPUS GATE:** 2 fallbacks (vs134 s39/s40), BOTH model-side unparsed replies
  (the model emitted a nonexistent option 2 "Cast Opportunity"; parser correctly could not map it). NO
  dropped/mis-picked LEGITIMATE reply. **PARSER PRECEDENCE, positive datapoint (vs22 s22):** the model
  emitted TWO contradictory `BLOCKS:` lines -- first `BLOCKS: Guttersnipe blocks Sunrise Sovereign` (WRONG,
  kills Guttersnipe), then reasoned to and re-emitted `BLOCKS: Guttersnipe blocks Stinkdrinker Daredevil`
  (RIGHT, favorable) as the final line. The parser resolved to the CORRECT final line. So **BLOCKS is NOT
  answer-first** -- it takes the model's corrected/final coded line. Precedence ASYMMETRY vs wave-26 I-4,
  where ATTACK / PUT (answer-first) took the PRE-reasoning WRONG line. Recorded so the divergence is not
  misread as a contradiction: here the wobble was RECOVERED by the parser, not entrenched. See notes I-4.

---

## MANDATE (5) -- 2/6 -> 3/6 DECISIONS-READ + CONSTRUCTION-TERMINAL REASSESSMENT

Two fallbacks (both vs134, both benign, MANDATE 2). Every other game fully piloted.

**The 3 WINS -- none raises the ceiling:**
- **vs137 (adj, 15/11):** Guttersnipe engine ground a go-wide Selesnya deck; ahead on life at the cap.
  Correct tapped-read hold (s43); two favorable blocks DECLINED (non-decisive, MANDATE 1).
- **vs93 (20/-2, T14):** FREE win vs guideless Relentless Rats -- YP + token swarm raced 20->3->dead, ZERO
  face damage. A signal about a weak un-guided opponent, not deck131's ceiling.
- **vs134 (adj, 32/9):** dominated a guideless Urza-tron control deck that could not kill it -- but 131
  could NOT close (opp at 9 at the cap). The "no reach" flag INSIDE a win.

**The 3 LOSSES -- construction-terminal, restated (owner's ledger, not relitigated):**
- **vs18 (-6/20, T11):** Kithkin anthem aggro out-raced 131 by T11; no early defense.
- **vs22 (-15/12, T13):** mono-R Giants go-wide (NOW guided, 5/6 -- last wave it was a guideless free WIN)
  out-raced 131; correct favorable blocks could not stop a 4-6 creature swarm.
- **vs136 (-1/4, T17):** Knight of the Ebon Legion (4/5 deathtouch) wall; correct never-block holds; no
  reach to push the opponent past 4. Lost by 1.

**CONSTRUCTION-TERMINAL flag: CONFIRMED, STANDS.** Threat density 6/60, no reach to CLOSE (visible even in
the vs134 win), no early defense vs aggro (vs18/vs22). Against the GUIDED field (137/18/22) 131 went 1W/2L,
the win an adjudication. The 3/6 is matchup/draw variance over the SAME construction-capped deck. Lever
remains DECK CONSTRUCTION (owner's call; sideboard already carries Spellheart Chimera reach / Aetherling
finisher). See notes deck-construction ledger.

**Ceiling reassessment (higher than flagged?): NO.** Two of three wins are adjudications where 131 could not
actually kill; one is a free race vs a guideless deck. The ceiling is where it has been for 9+ waves.

---

## MANDATE (6) -- REPPENALTY=1.05 round 6

- Reply prose coherent; no decode-collapse, no token-loop. The 209.6s s40 tail is an ON-TASK (if spiralling)
  deliberation, not a decode failure -- the model reasoned coherently the whole 209s, just to a wrong first-
  line answer. Retry fired 0x. The 2 unparsed fallbacks are model plan-fixation, not decode degradation. No
  timeout-draw under -T 3000.
