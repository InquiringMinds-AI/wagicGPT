/* schema.js — the BoardState contract.
 *
 * This file is the DECOUPLING SEAM between the rules engine and any frontend.
 * The engine's only obligation is to emit a BoardState document; a frontend's
 * only obligation is to render one (and, later, to emit Action documents back).
 * Neither side knows anything about the other — only about this contract.
 *
 * The object literals below ARE the wire JSON: a BoardState fixture is the same
 * shape a live backend will send over HTTP/WebSocket. Keep it engine-agnostic —
 * it describes an MTG-style position in general terms, not Wagic internals.
 *
 * No dependencies, no build step: this is a classic <script>, exposing globals
 * so index.html loads from file:// with zero server.
 */
(function (root) {
  "use strict";

  // Canonical phase sequence. `meta.phase` should match one of these `id`s;
  // the renderer highlights the current one and greys the rest.
  const PHASES = [
    { id: "untap",     label: "Untap" },
    { id: "upkeep",    label: "Upkeep" },
    { id: "draw",      label: "Draw" },
    { id: "main1",     label: "Main 1" },
    { id: "combatBegin", label: "Begin Combat" },
    { id: "attackers", label: "Attackers" },
    { id: "blockers",  label: "Blockers" },
    { id: "damage",    label: "Damage" },
    { id: "combatEnd", label: "End Combat" },
    { id: "main2",     label: "Main 2" },
    { id: "end",       label: "End" },
    { id: "cleanup",   label: "Cleanup" },
  ];

  // Loose aliases so an engine emitting "Attackers" / "Combat begins" / "First
  // Main" still maps onto the canonical track. Match is case-insensitive and
  // substring-tolerant (see phaseIndex()).
  const PHASE_ALIASES = {
    "untap": "untap",
    "upkeep": "upkeep",
    "draw": "draw",
    "first main": "main1", "main 1": "main1", "main1": "main1", "precombat main": "main1",
    "combat begin": "combatBegin", "begin combat": "combatBegin", "combat begins": "combatBegin",
    "declare attackers": "attackers", "attackers": "attackers",
    "declare blockers": "blockers", "blockers": "blockers",
    "combat damage": "damage", "damage": "damage",
    "combat end": "combatEnd", "end combat": "combatEnd", "end of combat": "combatEnd",
    "second main": "main2", "main 2": "main2", "main2": "main2", "postcombat main": "main2",
    "end step": "end", "end": "end",
    "cleanup": "cleanup",
  };

  function phaseIndex(phaseId) {
    if (!phaseId) return -1;
    const key = String(phaseId).trim().toLowerCase();
    const canon = PHASE_ALIASES[key] || key;
    return PHASES.findIndex(function (p) { return p.id === canon; });
  }

  const MANA_COLORS = ["W", "U", "B", "R", "G", "C"];

  // A fresh, empty position — the factory a frontend renders before any data
  // has arrived, and the template that documents every field a card/player may
  // carry. All optional; renderer treats missing fields as absent, not zero.
  function emptyBoard() {
    return {
      meta: {
        turn: null,           // turn number (integer) or null if unknown
        activePlayer: 0,      // index into players[] whose turn it is
        priorityPlayer: 0,    // index into players[] who currently has priority
        phase: "main1",       // a PHASES id (or an alias the engine emits)
        step: null,           // optional finer step label
        viewpoint: 0,         // which player is "you" (rendered at the bottom)
      },
      // Spells/abilities waiting to resolve, top of stack LAST in the array.
      stack: [],              // [{ id, name, controller, cost, colors, text, targets:[] }]
      players: [ emptyPlayer(0), emptyPlayer(1) ],
    };
  }

  function emptyPlayer(id) {
    return {
      id: id,
      name: id === 0 ? "You" : "Opponent",
      life: 20,
      poison: 0,
      manaPool: { W: 0, U: 0, B: 0, R: 0, G: 0, C: 0 },
      library: 0,             // count only (hidden zone)
      handSize: 0,            // authoritative count; hand[] may be shorter if hidden
      hand: [],               // [Card]; for a hidden hand, leave [] and set handSize
      battlefield: [],        // [Card] — lands, creatures, enchantments, artifacts
      graveyard: [],          // [Card]
      exile: [],              // [Card]
    };
  }

  /* A Card is a plain object. Recognized fields (all optional except name):
   *   id            stable string id (needed for attachments / future controls)
   *   name          display name
   *   cost          mana cost string, e.g. "{2}{B}{B}"
   *   colors        ["B"], ["U","B"], [] for colorless — drives the color theme
   *   types         ["Creature"], ["Land"], ["Enchantment","Aura"], ...
   *   subtypes      ["Merfolk"], ["Swamp"], ...
   *   text          rules / oracle text (tooltip + detail later)
   *   power/toughness           CURRENT p/t (after buffs) for creatures
   *   printedPower/printedToughness   base p/t, shown when it differs from current
   *   tapped        bool
   *   summoningSick bool
   *   damage        marked damage this turn (int)
   *   counters      { "+1/+1": 2, ... }
   *   attachedTo    id of the permanent this aura/equipment is attached to
   *   attachments   [ids] attached to THIS permanent
   *   faceDown      bool — render as a card back
   *   canAttack     runtime hint the engine may provide (used by controls later)
   */

  // Light structural check — enough to catch a malformed document early without
  // being a strict validator. Returns { ok, errors: [] }.
  function validate(state) {
    const errors = [];
    if (!state || typeof state !== "object") { return { ok: false, errors: ["not an object"] }; }
    if (!Array.isArray(state.players) || state.players.length < 2) {
      errors.push("players must be an array of at least 2");
    }
    (state.players || []).forEach(function (p, i) {
      ["hand", "battlefield", "graveyard", "exile"].forEach(function (z) {
        if (p[z] != null && !Array.isArray(p[z])) errors.push("players[" + i + "]." + z + " must be an array");
      });
    });
    if (state.stack != null && !Array.isArray(state.stack)) errors.push("stack must be an array");
    return { ok: errors.length === 0, errors: errors };
  }

  // Parse "{2}{B}{B}" -> ["2","B","B"]. Tolerates double braces "{{b}}" and
  // lowercase, as the translog emits. Returns [] for empty/malformed.
  function parseCost(cost) {
    if (!cost) return [];
    const out = [];
    const re = /\{+([^{}]+)\}+/g;
    let m;
    while ((m = re.exec(cost)) !== null) out.push(m[1].trim().toUpperCase());
    return out;
  }

  // Derive a card's primary color class for theming. Prefers explicit colors[];
  // falls back to the mana cost; lands and empty-cost artifacts read colorless.
  function colorClass(card) {
    const isLand = (card.types || []).some(function (t) { return /land/i.test(t); });
    if (isLand) return "land";
    let colors = card.colors;
    if (!colors || !colors.length) {
      colors = parseCost(card.cost).filter(function (s) { return /^[WUBRG]$/.test(s); });
    }
    if (!colors.length) return "colorless";
    if (colors.length > 1) return "multicolor";
    return "c-" + colors[0].toLowerCase();
  }

  root.WagicSchema = {
    PHASES: PHASES,
    MANA_COLORS: MANA_COLORS,
    phaseIndex: phaseIndex,
    emptyBoard: emptyBoard,
    emptyPlayer: emptyPlayer,
    validate: validate,
    parseCost: parseCost,
    colorClass: colorClass,
  };
})(window);
