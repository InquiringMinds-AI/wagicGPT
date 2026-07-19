# deck27 (Zombies / "Undead Infiltrator") — wave-19 findings

**Seat:** GUIDELESS FIRST PASS (Step 0-bis). No prior guide. This corpus is the baseline read;
`strategy.txt` here is the NEW initial guide.

**Binary:** /tmp/wagic-72b05535d. **Corpus:** matchups-20260719-092058, 6 games round-robin.

## Record

- **2/6.** WIN vs110 (L14/-9, t13), WIN vs135 (L19/-2, t11).
  LOSS vs14 (L-5/10, t14), LOSS vs49 (L-2/16, t10), LOSS vs35 (L-21/20, t10), LOSS vs62 (L-4/3, t14).
- No adj/timeout/draw games — all six decisive. No 0-attacker-cap durdle to adjudicate, but the
  durdle signature shows up in the *tempo* of the losses (below), not at the turn cap.

## Fallback / tax profile

- **3 unparsed_reply fallbacks (~2% of 141 decisions), 0 stale_echo / 0 defer / 0 empty.**
  All three are GIANT replies (>8k). Matches the brief's "3 unparsed."
- **6 giants (>8k) total; 5 of 6 in a BEHIND board state, 1 ahead (in a won game).** The tax
  localizes to the losing off-case, exactly the corpus-wide pattern. The 3 that did NOT fall back
  (deck62 s12/s28/s38, all behind) rambled but still emitted a parseable choice.

### The 3 fallbacks characterized

1. **deck110 s20** (ask, t11, **ahead** L14/2, WON game, 15.4k). Over-deliberation on a trivial
   Main-2 "which creature to cast" — spiraled on "what if the opponent's 1 card is removal,"
   re-deriving the whole board. AHEAD-but-still-rambles class (rare here; 1 of 6). Not a losing
   spiral — a keep-it-simple failure when already winning.

2. **deck14 s9** (blockers, t8, behind L16/17, 11.9k). **The important one.** The prompt offered
   exactly ONE attacker — `A1. Lord of Atlantis (2/2) deals 2` — and ONE blocker
   (`B1. Lord of the Undead (2/2) - may block A1 (both die)`). The model narrated a nonexistent
   **"Gravelgill Axeshark 4/4 persist"** as A2 (a threat from an EARLIER turn) and answered
   `BLOCKS: B1:A2`. A2 is not in the offered set -> illegal assignment dropped -> unparsed.
   **NOT a name-tolerance or trade-annotation parser failure** — the engine rendering was clean
   and correct, name-tolerance and the `(both die)` annotation both present and right. The failure
   is the model **importing a stale/imagined board into combat and blocking an attacker that does
   not exist.** salvageLoopedChoice cannot rescue it: `B1:A2` is well-formed but references an
   out-of-range attacker index (illegal, not malformed).

3. **deck35 s12** (ask, t9, behind **L2/20**, 15.7k). Textbook losing spiral: "I have 2 life, any
   damage kills me, blocking doesn't help, but the game forces me to choose" — churns to
   truncation. The off-case-spiral tax, unchanged from prior waves.

## VALIDATION FOCUS answers (wave-19 engine batch)

- **Combat name tolerance:** the seat's block-assignment fallback (deck14 s9) is a NEW shape, not
  the old code/name-format discard. Rendering was correct; the model hallucinated an attacker.
  Name tolerance itself never mis-fired at this seat. Seq: **deck14 s9.**
- **TRADE OUTCOME annotations:** present and **correct** in every block prompt sampled
  (`(both die)`, `(you kill it, your blocker lives)`, `(your blocker dies, attacker lives)`).
  **No misleading annotation observed.** They did their job in deck49 s9 (flagged a FREE favorable
  block) — the model ignored a correct annotation. Annotation good, obedience bad.
- **Pitch surfacing:** N/A — deck27 has no Force-class / pitch cards.
- **salvageLoopedChoice:** 3 unparsed; 0 code-loop-salvageable (one ahead-ramble, one illegal-index
  combat hallucination, one losing spiral). What still rambles = the behind-position giant, plus a
  rarer ahead-position over-deliberation.

## Play-pattern read (the archetype)

**UB (blue-black) tribal GO-WIDE with stacked LORDS + EVASION + recursion/regeneration grind.**
The deck has NO spells — every decision is deploy / attack / block. Engine mechanics (from
`auto=`/`text=`, authoritative):

- **Lords (this is the engine):** Glen Elendra Liege (2/3 flying) = a DOUBLE anthem — +1/+1 to
  other BLUE *and* other BLACK creatures, i.e. the whole deck. Lord of the Undead (2/2) buffs
  Zombies +1/+1 **and** `{1}{B},{T}: return a Zombie from your graveyard to hand` (recursion).
  Zombie Master (2/3) gives Zombies swampwalk **and** `{B}: regenerate`. Lord of Atlantis (2/2)
  gives Merfolk +1/+1 **and islandwalk**. Deepchannel Mentor (2/2) makes **ALL your blue creatures
  unblockable** (never drawn this corpus — huge unrealized card).
- **Evasion (the win path):** Inkfathom Infiltrator (unblockable), flyers (Wasp Lancer 3/2, Glen
  Elendra Liege, Oona's Gatewarden), islandwalk (Merfolk under Lord of Atlantis), all-blue
  unblockable (Deepchannel). Vodalian Zombie & Zombie Outlander have protection from green;
  Marauding Knight protection from white.
- **Resilience:** regeneration everywhere (Zombie Master grants it to all Zombies; Sanguine Guard,
  Metathran Zombie, Walking Dead self-regen) + Lord of the Undead recursion. Oona's Gatewarden is a
  DEFENDER (can't attack) — a pure wall with wither.

### WINS = the intended line

- **vs135:** led with **Inkfathom Infiltrator (unblockable) attacking every turn from t5**, added
  Wasp Lancer (flyer), Lord of Atlantis, Zombie Outlander, Glen Elendra Liege — clean evasive
  beatdown 20->-2. The deck's A-plan, executed.
- **vs110:** held the ground with Oona's Gatewarden walls, deployed Lord of the Undead + double
  Vodalian Zombie, attacked every turn. Won on the ground.

### LOSSES = passivity, mis-valued blocks, recursion misallocation

- **vs62 (durdle):** 22 priority passes; cast tiny bodies (Walking Dead 1/1, lone Vodalian),
  attacked piecemeal, and **deployed both Lord of Atlantis and Zombie Master on turn 13** — far too
  late. Never assembled a lord-buffed board; ground out by Enchantresses. Pure durdle.
- **vs35 (slow development -> swarm):** only 1 creature by t7; took **17 in one turn** at t8 once
  the Sliver swarm (5 first-strike flyers) assembled. The block itself (no blockers; the only
  blocker was the key Glen Elendra Liege trading down into first strike) was DEFENSIBLE — the loss
  was decided earlier by not developing/pressuring. Durdle-adjacent.
- **vs49 (declined a FREE block + recursion durdle):** at deck49 s9 the model was OFFERED
  `B1. Lord of the Undead (2/2) - may block A1 (you kill it, your blocker lives)` against Bloodmark
  Mentor (their first-strike-granting lord) and **declined it (no blockers)** — a clean misplay
  (free kill of their team-wide first-strike enabler, blocker survives). Then it used Lord of the
  Undead's recursion to return a **1/1 Walking Dead** (s10) and burned three priorities
  **regenerating that 1/1 at 2 life** (s20/s21/s22) while getting raced by Dragons.
- **vs14 (out-tempo'd + combat hallucination):** the s9 blockers hallucination (above); overall
  raced by Deep Blue tempo while deploying evasion too slowly.

## Loss signature (dominant, portable)

**Passive/slow: durdle with small un-buffed bodies, deploy the lords too late, attack piecemeal,
and mis-value blocking (declining FREE favorable blocks AND failing to develop enough for blocks to
matter).** The deck's power is lords + evasion; the pilot under-uses both and babysits 1/1s with its
recursion/regen tools. The wins prove the A-plan works when run; the guide's job is to make the
pilot run it.
