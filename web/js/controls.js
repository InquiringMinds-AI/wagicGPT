/* controls.js — the ACTION seam (stub).
 *
 * The mirror image of connection.js: connection brings BoardState IN, controls
 * send Action OUT. Disabled for now — the frontend is display-only until the
 * backend can receive actions — but the shape is fixed here so the renderer and
 * future wiring agree in advance.
 *
 * An Action is a small document describing a player's intent, engine-agnostic:
 *   { type: "declareAttackers", attackers: [cardId, ...] }
 *   { type: "castSpell", cardId, targets: [id...], x: 3 }
 *   { type: "declareBlockers", blocks: [{ blocker: id, attacker: id }] }
 *   { type: "chooseTarget", cardId }
 *   { type: "passPriority" }
 *   { type: "selectOption", index }
 *
 * NOTE: this maps cleanly to how the LLM opponent already thinks (bundled
 * attackers/blockers, X-announce, target choice) — the same decision surface,
 * just sourced from a human clicking instead of a model replying. When we wire
 * the backend, `sink` becomes the WsSource's send(); until then it logs.
 */
(function (root) {
  "use strict";

  const ControlBus = {
    enabled: false,          // flip on when a backend can receive actions
    sink: null,              // function(actionDoc) — set by app when live

    emit: function (action) {
      if (!this.enabled || !this.sink) {
        console.info("[controls] (disabled) would emit:", action);
        return false;
      }
      this.sink(action);
      return true;
    },

    // Convenience builders — the vocabulary the UI will call once interactive.
    declareAttackers: function (ids) { return this.emit({ type: "declareAttackers", attackers: ids }); },
    declareBlockers: function (blocks) { return this.emit({ type: "declareBlockers", blocks: blocks }); },
    castSpell: function (cardId, targets, x) { return this.emit({ type: "castSpell", cardId: cardId, targets: targets || [], x: x }); },
    chooseTarget: function (cardId) { return this.emit({ type: "chooseTarget", cardId: cardId }); },
    selectOption: function (index) { return this.emit({ type: "selectOption", index: index }); },
    passPriority: function () { return this.emit({ type: "passPriority" }); },
  };

  root.WagicControls = ControlBus;
})(window);
