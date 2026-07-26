# deck136 (Yarok of Pranks) - findings.md
# Wave-27 STEP-0 GUIDELESS DEBUT. Mono-B midrange/attrition (walker value). Record 4/6.
# Binary ffec79fd8. Corpus matchups-20260725-193608. All seq refs = deck136 seat.

## Record: 4/6
WINS:  vs137 (adj, ahead 12-9), vs134 (adj, 19-9), vs131 (won, 4 life t17),
       vs18 (adj, 35-1 - dominant).
LOSSES: vs22 (adj0, mono-R Giants ramp, 7 life at cap - close), vs93 (0, Relentless
       Rats go-wide, -7 t16).
Fallbacks: 4, ALL the Legion's End phantom chooser (see notes.md TOP item). No other
fallback class. Parser/mana/render otherwise clean.

## What the model already does WELL with a walker-midrange deck (Step-0 read)
This is the STRONGEST guideless debut behavior seen at this seat class. With NO guide:
1. REMOVAL is cast correctly and targeted well. Legion's End cast seam
   ("legal targets right now: Mogg Sentry/Soldier/Faerie Guidemother") picks the right
   creature every time (seq6 vs22, seq16 vs18, seq5 vs137). Cavalier of Night ETB
   "may sacrifice -> destroy" fires cleanly and kills the biggest threat (vs22 seq36-39:
   cast Cavalier, Sacrifice, chose Knight to sac, DESTROYED Inferno Titan 6/6).
2. MULTI-TARGET / BATCH choreography works. Command the Dreadhorde reanimation
   (vs18 seq44-46): cast, chose Knight from graveyard, then "Done - no further targets."
3. LIBRARY-SEARCH (reveal kind, Mausoleum Secrets tutor) is clean: 1/game, 0 fallbacks,
   real card selected; the "does NOT qualify - goes to shuffle" MV annotation renders.
4. MANA is a non-issue. Mono-black + generic Ugin = zero color-anxiety, zero mana-veto
   tax observed. The wave-25/26 mana-simplification failure class does NOT appear here
   (no hybrid/devotion confound). The trust-line in the guide is cheap insurance only.
5. Knight of the Ebon Legion {2}{B} deathtouch/+3+3 pump is activated at priority when
   useful; planeswalker (Liliana) is cast when castable.
The deck's floor is high because its plays are mostly unconditional value (removal, card
draw, recursion) that the core prompt handles competently. Deck knowledge is load-bearing
mainly at CARD-IDENTITY (deep card pool, moderate fabrication risk) and MATCHUP nuance
(Ritual whiffs on MV4; Legion's End as anti-wide), not at basic decision competence.

## Deciding levers (ranked by reasoning-tax x correctness, per Step-0-bis rule 2)
### 1. TOP tax spike is ENGINE, not a guide fact. The three largest replies in the whole
corpus (15.9k / 15.5k / 15.4k chars) are ALL the Legion's End phantom-chooser refusal.
CLASSIFY BEFORE TEACHING: this tax is engine-caused (R-RESOLVED-SPELL-PHANTOM-CHOOSER,
notes.md) and the model's refusal is CORRECT. It is NOT a deck fact to pre-answer in the
guide, and a guide crutch would risk the WORKING Legion's End cast-target seam. Route to
notes.md; teach nothing. (This is the key Step-0 methodology note - see skill.md.)
### 2. PASSIVITY at the attackers seam (guide-addressable). vs22 seq19 t7 "no attackers"
burned 10.8k chars; vs93 attacked with ONLY Knight of the Ebon Legion turn after turn
(seq26 t13, seq30 t15) while the Rats went wide - under-attacking/under-racing. Midrange
under-deploys without a per-seam attack floor (this skill's standing deck135 rung). The
guide's attack floor is board-state-keyed (beatdown vs slow, hold blockers vs aggro) with
Rankle/pumped-Knight named as the lead attackers.
### 3. MATCHUP card-facts that the model cannot infer:
   - Ritual of Soot (MV<=3) MISSES Relentless Rats (MV4) - it is a dead sweeper vs the
     deck's go-wide nemesis. Contributed to the vs93 loss posture. Guide names this + the
     real anti-wide tools (Liliana -4, Legion's End name-strip).
   - Legion's End as an anti-tribal/anti-wide NAME-STRIP (exiles a whole name from board
     + hand + graveyard) - a non-obvious use the model treats as generic 1-for-1 removal.
   - Swift End / Ugin -3 hit PLANESWALKERS and permanents, not just creatures - flexible
     removal the model should aim at the biggest threat/PW.

## The two losses (matchup + passivity, not decision-breakdown)
- vs93 (Relentless Rats): go-wide MV4 bodies dodge Ritual of Soot; the model raced with a
  single Knight instead of leaning on Liliana -4 / Legion's End / blockers. Part matchup
  (sweeper structurally misses), part passivity (attack-floor gap). Guide addresses both.
- vs22 (Giants ramp): out-sized by a 6/6 Inferno Titan line; the model DID remove it
  (Cavalier sac) but fell behind on the race. Close (7 life at cap). Sound decisions,
  lost the tempo war - the guide's "you are the aggressor vs slow, grind the long game"
  framing + keep-blockers-vs-big-swing is the lever.

## Unexercised classes (carry-forward, NOT scored as clean - see notes.md)
Rankle "choose any number" may-ask NEVER fired; Liliana LOYALTY activations never asked
(cast once, game ended 3 turns later); Ugin never cast. The walker-loyalty and Rankle
multi-mode may-ask render/decision quality are UNVERIFIED at this seat.

## Layer routing summary
- Engine: R-RESOLVED-SPELL-PHANTOM-CHOOSER (notes.md, NEW ledger item, HIGH-visibility).
- Guide: attack floor, capability table, removal-priority, Ritual-whiff + anti-wide
  matchup facts, Legion's-End-as-name-strip. -> strategy.txt (this wave's initial guide).
- Skill: "classify the top tax spike as engine-vs-deck-fact BEFORE ranking teaching
  points; an engine phantom-ask can be the #1 tax yet must NOT enter the guide." -> skill.md.
- Core: no signal -> PASS (general-suggestions.md).
