# deck14 (Deep Blue) — findings.md — wave-18 GUIDELESS FIRST PASS

Seat: deck14, mono-blue tempo. Corpus `matchups-20260716-185930`, binary f664539a3.
Record **3/6** (WIN vs49, WIN vs110, WIN-adj vs62; LOSS vs35, LOSS vs135; TIMEOUT/draw vs21).
Played with NO strategy guide (core prompt only). This is the initial-guide derivation.

## 1. Deck read — engine reality (from rendered option/hand text, not Oracle memory)

Mono-blue. Every card casts off Islands; mana is trivial. Card set (engine `{cost}` + `[X/Y]`):
- **Merfolk beatdown core**: Merfolk of the Pearl Trident {u} (1/1), Coral Merfolk {1}{u} (2/1),
  **Lord of Atlantis {u}{u} (2/2)** ×4 = Merfolk LORD (other Merfolk get +1/+1 and islandwalk),
  **Gravelgill Axeshark {4}{u/b} (3/3) [islandwalk, persist]** — a MERFOLK (Lord buffs it; persist
  = returns as a 2/2 with a -1/-1 counter when it dies). Note {u/b} hybrid pays with U here.
- **Evasive finishers (the reliable clock)**: **Air Elemental {3}{u}{u} (4/4) flying** ×2,
  **Mahamoti Djinn {4}{u}{u} (5/6) flying** ×2. Most opponents in the pool have no flyer/reach,
  so these connect for full every turn — they are the primary win condition.
- **Tempo/interaction**: Unsummon {u} [instant] (bounce a creature) ×4, Boomerang {u}{u}
  [instant] (bounce ANY permanent) ×4.
- **Steal**: Control Magic {2}{u}{u} + Persuasion {3}{u}{u} — gain control of a creature (Aura).
- **Card advantage**: Counsel of the Soratami {2}{u} [sorcery] = draw 2.

**Engine divergences from paper (route to notes.md, guide uses the engine):** Counsel is {2}{u}
(3 mana), not {3}{u}. Gravelgill Axeshark is a 3/3 islandwalk+persist (not paper's 4/3). Coral
Merfolk 2/1. **Islandwalk is near-dead in this pool** — it makes a creature unblockable only if the
DEFENDER controls an Island, and the opponents (Slivers, Dragons, artifacts, green) mostly do not.
FLYING (Air Elemental / Mahamoti) is the evasion that actually matters.

**Win path:** mono-blue TEMPO. Deploy a clock — Merfolk under a Lord of Atlantis, then evasive
flyers — and ATTACK every turn, while bounce (Unsummon/Boomerang) clears blockers / buys tempo,
Control Magic steals the opponent's best threat, and Counsel refills. It is NOT a durdle-control
deck; its blue toolkit (draw, bounce, steal) makes it LOOK like control and that mislabel is the
deck's central failure mode.

## 2. Failure-class analysis (per-decision, all 6 games)

### FC1 — THE DURDLE-INTO-DEFENSE LOSS (highest value; the 0-attacker signature fires clean)
vs **deck35 (Slivers), LOSS turn 10 at -2: ZERO `attackers` decisions the entire game.** The deck
cast Lord of Atlantis t3, then immediately BLOCKED with it (t4, chump into Heart Sliver), then
spent t5/t7 Unsummoning and Boomeranging individual slivers (Muscle, Striking) — creatures that
recast for 1 mana, so the swarm just rebuilt while life went 20→19→15→9→6→dead. It never once
declared an attack, never deployed a flyer, never established its own clock. Classic tempo-loses-
to-aggro: it flipped to full reactive/control mode and got raced. The blue toolkit invited it.
Contrast: all 3 WINS have 6-8 attack-decisions (vs110 deployed 3 flyers, won turn 21 opp -7). The
0-attacker game is the loss. **This is the #1 rule the guide must countermand — "you are tempo,
not control; attack every turn; bounce serves the race, it is not the plan."**

### FC2 — COMBAT REASONING-TAX SPIKE + FORMAT DRIFT (dominant fallback source; 3 of 5 fallbacks)
Every big unparsed reply is a COMBAT decision where the model enumerated every possible block/trade
across flyers, lords, and pumped bodies, then drifted OUT of the required label format:
- vs62 **s34 blockers, 8,654-char reply** — ended `BLOCKS: Gravelgill Axeshark: Saproling (1/3),
  Lord of Atlantis: Saproling (1/1)` — used CARD NAMES, not `B#:A#`. Unparsed.
- vs62 **s37 blockers, 10,206-char reply** — ended `BLOCKS: Air Elemental:Canopy Spider, Lord of
  Atlantis:Canopy Spider`. Card names again. Unparsed.
- vs135 **s26 attackers, 13,116-char reply** — a spiraling "Maybe I can cast Unsummon? No.
  Boomerang? No…" ramble at a 2-attacker declaration (A1/A2 Lords) while at 8 life facing lethal;
  never emitted a clean `ATTACK:` line. Unparsed, at the game's decisive turn (LOSS turn 14).
The reasoning tax is a deck idiosyncrasy: mono-blue combat is math-heavy (flying evasion, Lord
pump already in the [X/Y], persist recursion, "block while ahead?"). The guide must (a) collapse the
tax with "trust the rendered [X/Y]; you rarely block — take damage while ahead," and (b) hard-teach
the answer FORMAT: one short line, `BLOCKS: B#:A#` / `ATTACK: A#` using the CODES, never card names.

### FC3 — INVENTED OPTION NUMBER (obey the offered list)
vs110 **s8, unparsed ask**: the cast menu offered only `1. Cast Boomerang` / `2. Cast nothing`,
but the model wanted a body from hand and answered `CHOICE: 3 (Cast Gravelgill Axeshark)` — a
number that does not exist. It reasoned from its HAND/PLAN, not the OFFERED options. If a card you
want is not on the list, it is not castable this decision — pick from what is offered.

### FC4 — STALE_ECHO at a bounce target (non-deciding, known-class)
vs21 **s24, stale_echo**: model chose `CHOICE: 14 (Raging Goblin)` to bounce a Goblin that had
already left the stack; the option was gone. Correct intent (bounce the incoming haste threat),
stale target. This is the known corpus-wide stale_echo class (TARGET-CHOICE framing validated at
scale) — note only, do not re-diagnose.

## 3. Play-quality observations (not fallbacks, but guide-worthy)

- **Bounce used defensively/low-value**: repeatedly Unsummoned a 1/1 Bloodmark Mentor (vs49 t5/t7/t9)
  and bounced 1-mana slivers (vs35). Bounce a cheap recastable creature is weak tempo. Best uses:
  clear a BLOCKER to push a lethal/near-lethal swing, bounce a creature that cost a lot to recast,
  bounce your OWN persist/attacked-down creature to reset it, or (Boomerang) bounce a land/key
  permanent. Do not spend bounce re-bouncing a 1-drop every turn on defense.
- **Control Magic on a weak target**: vs49 s27 stole a 1/1 Bloodmark Mentor. Steal the opponent's
  BEST creature (biggest body / best blocker / a bomb) — it removes their threat AND adds to your
  clock. A 1/1 is a wasted 4-mana steal.
- **Flyers win when deployed**: vs110 deployed Air Elemental + Mahamoti and ground the opponent out
  (turn 21, opp -7). vs35 loss deployed zero flyers (partly draw). Prioritize casting and attacking
  with the flyers — they are the evasive clock the ground stall cannot stop.
- **Counsel timing**: 4 draws across the corpus, all fine — but do not durdle on card draw when you
  should be deploying a threat or attacking (it is a sorcery; cast it when NOT racing for tempo).

## 4. What the core prompt already gets RIGHT (do NOT re-teach)
- Mana affordability: no "I can't pay" false-declines observed off Islands (mono-blue, trivial).
- TARGET-CHOICE sub-menu: bounce/steal target menus resolved correctly (except the one stale echo);
  the wave-17/18 sub-menu framing is doing its job — the guide need not re-teach the flow.
- Basic attack declarations parse fine when the reply stays short; the failures are the tax spirals.

## 5. Rotation verdict
NEWCOMER — writes its first guide this wave; does not rotate. Strong new-work signal (FC1 durdle
loss + FC2 combat-tax cluster are load-bearing and both addressable in-guide).
