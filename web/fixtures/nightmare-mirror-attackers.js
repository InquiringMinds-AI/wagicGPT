/* fixtures/nightmare-mirror-attackers.js
 *
 * A REAL position, lifted verbatim from the self-play corpus
 * (~/.Wagic/ai/gpt/selfplay-runs/corpus-20260705-212611): deck1 "Nightmare"
 * mirror, the active player's Attackers step. This is the decision where the
 * AI (viewpoint player, life 15) had only Lord of Atlantis able to attack into
 * an untapped 2/2 and replied "none".
 *
 * The object below is the BoardState WIRE FORMAT — the exact shape a live
 * backend will send. P/T values are as the engine reported them (current with
 * printed shown where they differ), not recomputed.
 */
WagicFixtures.register("nightmare-mirror-attackers", {
  meta: {
    turn: 7,
    activePlayer: 0,
    priorityPlayer: 0,
    phase: "attackers",
    step: "Declare attackers",
    viewpoint: 0,
  },
  stack: [],
  players: [
    {
      id: 0,
      name: "You (AI)",
      life: 15,
      poison: 0,
      manaPool: { W: 0, U: 0, B: 0, R: 0, G: 0, C: 0 },
      library: 47,
      handSize: 3,
      hand: [
        { id: "y_h1", name: "Unholy Strength", cost: "{B}", colors: ["B"], types: ["Enchantment", "Aura"], text: "Enchant creature. Enchanted creature gets +2/+1." },
        { id: "y_h2", name: "Fear", cost: "{B}{B}", colors: ["B"], types: ["Enchantment", "Aura"], text: "Enchant creature. Enchanted creature has fear." },
        { id: "y_h3", name: "Cursed Land", cost: "{2}{B}{B}", colors: ["B"], types: ["Enchantment", "Aura"], text: "Enchant land. Enchanted land is a Swamp." },
      ],
      graveyard: [],
      exile: [],
      battlefield: [
        { id: "y_l1", name: "Swamp", types: ["Land"], subtypes: ["Swamp"], tapped: false },
        { id: "y_l2", name: "Swamp", types: ["Land"], subtypes: ["Swamp"], tapped: false },
        { id: "y_e1", name: "Bad Moon", cost: "{1}{B}", colors: ["B"], types: ["Enchantment"], text: "Black creatures get +1/+1." },
        { id: "y_l3", name: "Island", types: ["Land"], subtypes: ["Island"], tapped: true },
        { id: "y_e2", name: "Bad Moon", cost: "{1}{B}", colors: ["B"], types: ["Enchantment"], text: "Black creatures get +1/+1." },
        { id: "y_l4", name: "Swamp", types: ["Land"], subtypes: ["Swamp"], tapped: true },
        { id: "y_l5", name: "Island", types: ["Land"], subtypes: ["Island"], tapped: true },
        { id: "y_c1", name: "Lord of Atlantis", cost: "{U}{U}", colors: ["U"], types: ["Creature"], subtypes: ["Merfolk"],
          power: 2, toughness: 2, printedPower: 2, printedToughness: 2, tapped: false, summoningSick: false, canAttack: true,
          text: "Other Merfolk get +1/+1 and are unblockable." },
        { id: "y_c2", name: "Wall of Bone", cost: "{2}{B}", colors: ["B"], types: ["Creature"], subtypes: ["Wall", "Skeleton"],
          power: 4, toughness: 7, printedPower: 1, printedToughness: 4, tapped: false, summoningSick: false, canAttack: false,
          text: "Defender. {B}: Regenerate Wall of Bone." },
      ],
    },
    {
      id: 1,
      name: "Opponent (AI)",
      life: 20,
      poison: 0,
      manaPool: { W: 0, U: 0, B: 0, R: 0, G: 0, C: 0 },
      library: 47,
      handSize: 2,
      hand: [],   // hidden — handSize drives the card-back count
      graveyard: [],
      exile: [],
      battlefield: [
        { id: "o_l1", name: "Swamp", types: ["Land"], subtypes: ["Swamp"], tapped: false },
        { id: "o_l2", name: "Island", types: ["Land"], subtypes: ["Island"], tapped: false },
        { id: "o_l3", name: "Swamp", types: ["Land"], subtypes: ["Swamp"], tapped: false },
        { id: "o_e1", name: "Bad Moon", cost: "{1}{B}", colors: ["B"], types: ["Enchantment"], text: "Black creatures get +1/+1." },
        { id: "o_l4", name: "Swamp", types: ["Land"], subtypes: ["Swamp"], tapped: false },
        { id: "o_c1", name: "Merfolk of the Pearl Trident", cost: "{U}", colors: ["U"], types: ["Creature"], subtypes: ["Merfolk"],
          power: 3, toughness: 3, printedPower: 1, printedToughness: 1, tapped: true, summoningSick: false,
          attachments: ["o_a1", "o_a2"], text: "A 1/1 Merfolk." },
        { id: "o_a1", name: "Animate Dead", cost: "{1}{B}", colors: ["B"], types: ["Enchantment", "Aura"], attachedTo: "o_c1",
          text: "Reanimate a creature; it gets -1/0." },
        { id: "o_a2", name: "Unholy Strength", cost: "{B}", colors: ["B"], types: ["Enchantment", "Aura"], attachedTo: "o_c1",
          text: "Enchanted creature gets +2/+1." },
        { id: "o_l5", name: "Island", types: ["Land"], subtypes: ["Island"], tapped: false },
        { id: "o_l6", name: "Island", types: ["Land"], subtypes: ["Island"], tapped: true },
        { id: "o_c2", name: "Merfolk of the Pearl Trident", cost: "{U}", colors: ["U"], types: ["Creature"], subtypes: ["Merfolk"],
          power: 2, toughness: 2, printedPower: 1, printedToughness: 1, tapped: false, summoningSick: false,
          text: "A 1/1 Merfolk." },
      ],
    },
  ],
});
