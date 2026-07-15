# deck131 wave-10 -- dev notes (engine / harness / model / deck-construction; self-contained)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-031825/`, 6 deck131 games, 0W/6L
(3 death, 3 life-adjudicated cap). Live guide = wave-9 FROZEN revision. Binary `/tmp/wagic-c8c054be8`.
Answer-last reply protocol, max_tokens 2048. Seat files (from `gamestart`):
vs135 1784103512 (L -2) | vs133 1784105445 (adjL 15, T12) | vs110 1784106735 (adjL 6, T8) |
vs109 1784110077 (L -6) | vs140 1784110759 (adjL 26, T32) | vs44 1784112264 (L -3).

Layer-routing note: engine/harness/model items live HERE, never in prompt text. Deck-construction
flags are RESTATED for the user, not relitigated.

## Reproducible evidence

### R1 -- mulligan intent-collapse CLOSED under answer-last (the headline; full repro)
- vs135 `...deck131...vs...deck135.jsonl` seq1: hand `Elixir; Island; Island; Prism Ring; Island;
  Counterspell; Young Pyromancer` (0 Mountain, no draw spell) -> reply ends `CHOICE: 2`,
  `chosen_text`="Mulligan". CORRECT + LANDED.
- vs133 `...deck131...vs...deck133.jsonl` seq1: hand `Counterspell; Island; Island; Essence Scatter;
  Island; Young Pyromancer; Prism Ring` (0 Mountain, no draw spell) -> `CHOICE: 2`, "Mulligan".
- vs110/vs109/vs140/vs44 seq1: Mountain+YP hands -> `CHOICE: 1`, "Keep this hand".
- Tally both directions: no-Mountain reasoning-correct 2/2, OUTCOME-correct 2/2 (wave-9 was 0/1);
  keepable kept 4/4. The wave-9 head-first false-Keep did not recur.
- Repro: `python3 -c "import json;r=[json.loads(l) for l in open(F) if l.strip()];x=[y for y in r if
  y.get('seq')==1][0];print(x['chosen_text']);print(x['reply'][-200:])"` per file.

### R2 -- Mountain-first 10/10
- Co-offers (options_text has BOTH 'Play Mountain' and 'Play Island'): 10 (vs135 1, vs133 3, vs110 1,
  vs109 2, vs140 1, vs44 2). Chose a Mountain option 10/10, incl. Island-listed-first windows.
- Script: for each deck131 jsonl, filter asks whose options list contains both substrings; check
  'Mountain' in chosen_text. All 10 true.

### R3 -- Guttersnipe cast-every-turn / NEVER-block
- NEVER-block held: vs135 seq7 (T6, 18 life, Icehide Golem attacking) -> `BLOCKS: none`.
- Cast-every-turn as FUEL: vs140 seq43 (T29) + seq46 (T31) cast Aetherize {3}{u} with no attackers
  to bounce, reply explicitly calls it "waste, no targets" then casts it for the 2 face damage.
  Guttersnipe attacked seq30/36/42 (T21-27). 0 fuel-declined leaks.

### R4 -- damage>=toughness: 0 recurrence
- vs135: 2/2 Guttersnipe took 2 from a 2/2 Icehide Golem and died correctly; no survival
  misjudgment. Grep all 6 replies for toughness+survive/kill/die -> 0 hits. Single-seat (wave-8).

### R5 -- engine-change firing at this seat
- #3 legal-target-names RENDERED: vs110 seq6 `Cast Cyclonic Rift {1}{u} - legal targets right now:
  Memnite, Ornithopter, Cranial Plating, Mox Opal` (picked Mox Opal); vs109 seq19 `Cast Essence
  Scatter {1}{u} - NO legal target right now - can target on the stack: Ash Zealot` (cast on stack).
  No fabricated-target class.
- #2 fetch consume-on-choose: N/A -- deck131 has no fetchlands.
- #7 adj-row sanity: vs110 seat-last my_life=6 == tsv 6; vs133 =15 == tsv 15; vs140 seat-last=27 vs
  tsv 26 (timeout cap at T32 one lower than last logged ask T27; jsonl has no gameend for timeouts).

## HARNESS / MODEL ledger (with repros)

### H1 -- 7 truncation fallbacks; NONE changed a game (KNOWN class, do not re-diagnose)
- 6 `unparsed_reply` + 1 `empty_reply`: vs135 seq7 (blocks:none intent, correct), vs135 seq11
  (Guttersnipe already dead, tapped out -- no fuel lost), vs135 seq17 (empty; Elixir vs nothing),
  vs110 seq5 (single land drop; heuristic plays it), vs140 seq16 (Elixir vs nothing, 22 life),
  vs44 seq9/11/14 (mana-screwed, no mana to act -- heuristic couldn't do better; game lost to
  screw+tempo). Repro: filter asks where `r['fallback']` is truthy.
- MODEL TRIGGER worth a harness eye (not a guide item): the non-blocker spirals are triggered by
  MANA-AVAILABILITY confusion -- the model distrusts "Mana available: (no untapped sources)" or
  loops on generic-mana payment ("{2} can be paid with... no...") until it hits the 2048 token cap.
  Same family as the core's "(none) read as I cannot cast" correction. Because it burns to the cap
  and self-heals to a heuristic, it appears as truncation rather than a wrong pick. If the token cap
  were raised further OR the mana line reworded, these would likely resolve to a real pick. Left as
  a harness/model observation; changed no game this corpus.

## DECK-CONSTRUCTION ledger (RESTATED for the user; user owns; not relitigated)
The standing 5-wave diagnosis holds and is the dominant loss cause again (0/6, opp untouched at 18-27
in every game):
- **Threat density 6/60** (3 Guttersnipe + 3 Young Pyromancer). A 2-damage-per-spell engine with 6
  payoffs cannot reliably present a clock: vs133 mulliganed to 6 and drew NO payoff all game (DIG
  gear obeyed, nothing to find); vs110/vs44/vs109 raced out with only a lone Young Pyromancer or no
  payoff at all.
- **No non-incremental finisher / no reach vs lifegain.** vs140 (lifegain grind) went to the T32 cap
  and deck131 lost adjudication by ONE life (26 vs 27) -- with a single burn/reach finisher that
  game likely flips. The deck can grind even but cannot close.
- **Manabase 14 Island / 8 Mountain leans slightly wrong** for two red win conditions; both mulligans
  and much of the vs44 mana screw were red-source shortfalls. (Interface is clean -- Mountain-first
  is 10/10; this is a draw/count problem, not the wave-7 unofferable-menu artifact.)
- These are the user's to decide; recorded, not argued. Sideboard already carries Spellheart Chimera
  (a reach body) + Aetherling (a finisher) as candidate swaps.

## WATCH (single-seat, unscored -- below the bar to touch the frozen guide)
- **Attack-seam partner to NEVER-block-Guttersnipe.** vs135: Guttersnipe traded with a 2/2 Icehide
  Golem while ATTACKING (both dealt 2, both died) around T7, removing the deck's only reach and
  leaving it unable to rebuild (lost -2/18). NEVER-block was obeyed (seq7 no-block); this was the
  attack side, which the guide does not guard. No attack-DECISION record was captured (the declare
  step wasn't logged as its own ask), so it cannot be attributed to a pilot choice with confidence,
  and it is one instance. Wave-5 method noted a block-seam survival rule needs its attack-seam
  partner; if a 2nd seat shows the pilot deliberately attacking its only reach into a fatal trade,
  consider an attack-seam clause ("do not attack Guttersnipe into an untapped blocker that trades").
  Do NOT add it off this single, decision-record-less instance.
- **Prism-Ring-in-a-race.** vs109 (mono-red aggro) seq8 (T4) + seq14 (T6) cast Prism Ring (pure
  lifegain, not an instant/sorcery so it does not even trigger Young Pyromancer) during a race; the
  guide says Prism Ring only with leftover mana and nothing better. Whether a better spell was
  castable each window is unclear (color/mana), and the Elixir at 5 life (seq17) was a correct
  below-12 stabilize. Marginal, single-seat, matchup likely unwinnable regardless. Not scored.

## For a future deck131 reviewer -- gotchas
- n=6, single run: win-rate is noise. The guide is at its terminus (6 waves obeyed-but-losing);
  wins come from CONSTRUCTION (threat density / reach) not guide prose. Resist churning frozen lines.
- The reply-protocol intent-collapse this seat witnessed in wave-9 is now CLOSED under answer-last;
  do not re-open it.
- `choice` is the 1-based number the model emitted (`CHOICE: 1` = first option); `chosen_text` is
  authoritative for what the engine did -- score against chosen_text, not `choice`.
- Isolate `Your battlefield (creatures: N):` for on-board checks (a loose "Guttersnipe" substring
  matches hand/cast-list/guide text); the opening-hand line for mulligan; `A#.`/attacker records for
  combat. Timeouts write no per-seat `gameend`; use results.tsv adj rows for final life.
