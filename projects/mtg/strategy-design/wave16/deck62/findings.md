# deck62 (Enchantresses) — GUIDELESS FIRST-PASS findings (wave 16)

Corpus: `matchups-20260716-084532`, binary `/tmp/wagic-69793930a`. deck62 entered the pool
this wave with NO guide (core prompt only). Six seat translogs, one per matchup. This is the
information-rich guideless corpus: it shows what the core already handles and, via the
reasoning-tax spikes, exactly which deck facts are load-bearing.

## Record (2/6, matches the win table)
- **WIN vs deck21** (T13, opp -12) — assembled a 9/10 trample+flying Birds of Paradise (Ancestral
  Mask + Gaea's Anthem + Primal Rage stacked) and swung. The voltron kill line, live.
- **WIN vs deck44** (adjudicated AHEAD, T17 L9 vs L2) — ground out with Canopy Spider + a pumped
  Yavimaya Enchantress under Gaea's Anthem/Primal Rage.
- **LOSS vs deck110** (T10, -7) — run over by affinity while durdling on auras; passive at 1 life
  ("no attackers" s19), then unparsed twice (s17/s18, 14.7k/14.0k chars).
- **LOSS vs deck17** (T10, -2) — lost the flyer race to Faeries; unparsed on combat twice.
- **LOSS vs deck135** (adjudicated BEHIND, T11 L5 vs L19) — three unparsed combat/main loops
  (s17/s20/s22), never closed a 7/7 Yavimaya into a tapped-out opponent.
- **LOSS vs deck133** (adjudicated BEHIND, T12 L6 vs L18).

## Deck identity (derived from engine `auto=`/`text=`, mono-green)
Mono-green ENCHANTRESS engine-aggro. Draw engine (**Argothian Enchantress**) refills your hand off
cheap enchantments; you then either (a) **voltron** — stack pump auras onto ONE creature and give
it trample, swing for the kill — or (b) **go wide** — Fists-of-Ironwood Saproling tokens under
Gaea's Anthem + Primal Rage. Kills ~T10-13. Mana is trivial: every spell is {G}-based off Forests
(+ Birds/Druid dorks); confirmed `Mana available: {g}` — NO hybrid/color idiosyncrasy (unlike
deck17). The load-bearing idiosyncrasy is the ENGINE and the AURAS, not mana.

## THE DOMINANT IDIOSYNCRASY (earns rule #1) — auras/enchantments are DRAW + PUMP, NOT removal
qwen has NO model of this deck and repeatedly hallucinated its cards as removal:
- **vs110 s3/s4 (7.1k tax across the pair):** PLAN "cast Fists of Ironwood targeting the opponent's
  Ornithopter **to kill it (1 damage to a 0/2)**... This removes their only attacker." Fists of
  Ironwood deals ZERO damage — it makes two 1/1 Saprolings on ETB and grants the *enchanted*
  creature trample. qwen invented a kill that does not exist, and enchanted the OPPONENT's creature
  (gifting Ornithopter trample).
- **vs110 s5 (5.9k tax on a LAND DROP):** long derivation inventing that Fists gives "+1/+1", that
  Ornithopter is "already enchanted... a mistake in the log", Etched Champion pro-green math, etc.
- The draw engine is invisible to qwen: across the corpus it **cast enchantments BEFORE deploying
  Argothian Enchantress** (vs110: Fists s4/s6 before Argothian s12), wasting the card draw. It never
  once reasoned "cast Argothian first so my enchantments draw cards." Engine text present in 47
  records; qwen never sequenced around it.

## Aura targeting (load-bearing; engine offers both own AND opponent creatures)
- Legal-target lists offer the opponent's creatures alongside your own (e.g. vs135 "legal targets:
  Icehide Golem, Abominable Treefolk, Yavimaya Enchantress"). At the TARGET sub-menu qwen did
  correctly pick its OWN creature every observed time (vs110 s7 own Saproling; vs21 s6 own Birds;
  vs135 s11 own Yavimaya; vs44 s10/s14 own Canopy) — so it is not blindly enchanting opponents when
  it has a choice. The failure is at the CAST step with no own creature (vs110 s4: creatures:0,
  only legal target = Ornithopter -> forced gift). Fix: deploy a creature FIRST; don't fire a pump
  aura when your only legal target is the enemy.
- **Argothian Enchantress has SHROUD -> she can never be an aura target** (confirmed: she never
  appears in any "legal targets" list). Your Web/Blanchwood/Ancestral Mask/Fists go on Yavimaya, a
  dork, Canopy Spider, or a Saproling — never on Argothian. qwen did not trip on this but should be
  told so it doesn't waste reasoning hunting for it.

## The win line DOES emerge — recognize and force it
- **vs21 (win):** Birds of Paradise reached **9/10 trample+flying** (Ancestral Mask +2/+2 per other
  enchantment, Gaea's Anthem +1/+1, Primal Rage trample). This is lethal evasion. qwen ALMOST threw
  it away — s19 (10.4k tax) agonized over whether the [tapped] Birds could attack and mis-derived
  the aura math; s18 fully unparsed (11.0k). It recovered and won, but the position should have been
  a two-second "swing, this kills."
- **vs135:** built a **7/7 trample Yavimaya** into a tapped-out opponent (s20) and UNPARSED the
  attack (11.6k chars), surrendering the alpha strike to the heuristic. The correct declaration was
  trivially ATTACK: Yavimaya (the 4 Saprolings were summoning-sick, correctly excluded from the
  "creatures that can attack" list — qwen counted them anyway and drowned).

## Attack / deploy FLOOR is the second failure (durdle + combat tax)
- Massive tax and unparses concentrate on COMBAT and land drops: attackers unparsed at vs135 s20
  (11.6k), vs17 s19 (13.1k), vs44 s29 (11.9k); "no attackers" while losing (vs110 s19 at 1 life;
  vs21 s25). Both outright losses (vs110, vs17) were T10 races lost partly to passivity while the
  board sat home under auras.
- **Argothian Enchantress (0/1) and mana dorks (Birds 0/1, Druid 1/3) should not attack** — qwen
  repeatedly spent reasoning on whether to send a 0-power engine creature (vs135 s20). The "creatures
  that can attack" list is the authoritative eligible set; attack with the ones on it that have real
  power, keep 0-power engine/dorks and needed blockers home.

## Defensive tools matter vs the aggro/flyer decks (survival to set up)
Both losses were races. The deck's defensive kit is real and under-used: **Canopy Spider (1/3
reach)** and **Web (+0/+2 and reach)** let ground creatures block FLYERS (load-bearing vs deck17
Faeries, deck110 fliers); **Briar Patch** gives every attacker against you -1/-0; **Druid of the
Cowl** is a 1/3 wall-ish dork. Against a faster deck, block with reach creatures and stabilize
while the enchantress engine out-cards them.

## Reasoning-tax map (saliency ranking for teaching points)
1. Aura/enchantment mechanics (what they DO; draw-engine sequencing) — 5.9k-8.3k tax, hallucinated
   removal, caused the enchant-opponent gift and mis-sequencing. -> rule #1.
2. Combat/attack floor — 11-13k tax, multiple unparses, "no attackers" while behind. -> attack floor.
3. Aura-stack stat math on the pumped creature ("is it 9/10? +2/+2 per what?") — 10k tax even in the
   winning game. -> tell it to trust the printed [X/Y] on the battlefield line and just swing.
4. [tapped] confusion on its own creature at its own combat (vs21 s19) — a board-representation tax
   the guide can blunt ("your creatures untap on your turn; if the list offers it as an attacker it
   can attack").

## Engine/card-script items (routed to notes.md, NOT the guide)
- Fists of Ironwood enchanting the OPPONENT's creature grants THEM trample — verify vs Oracle (it is
  correct: "Enchant creature", any creature) — so the GUIDE, not the engine, must steer targeting.
- stale_echo/unparsed rate at this seat is high (see notes.md) driven by the combat-math tax.
