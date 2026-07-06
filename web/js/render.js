/* render.js — pure rendering of a BoardState into the DOM.
 *
 * The ONLY input is a BoardState document (see schema.js). No engine coupling,
 * no network, no globals mutated. renderBoard(state, rootEl) wipes rootEl and
 * rebuilds it. That purity is what makes the frontend swappable: change the
 * data source (connection.js), never this file.
 *
 * Layout is a standard MTG table seen from `meta.viewpoint`'s seat:
 *   opponent at the top (creatures near the centre line), the stack in the
 *   middle, you at the bottom. Hands: yours face-up, theirs as backs.
 */
(function (root) {
  "use strict";
  const S = root.WagicSchema;

  function el(tag, cls, text) {
    const e = document.createElement(tag);
    if (cls) e.className = cls;
    if (text != null) e.textContent = text;
    return e;
  }

  // A single mana pip. sym is like "B", "2", "U".
  function manaPip(sym) {
    const isColor = /^[WUBRG]$/.test(sym);
    const p = el("span", "pip " + (isColor ? "pip-" + sym.toLowerCase() : "pip-generic"), isColor ? "" : sym);
    p.title = sym;
    if (isColor) p.dataset.sym = sym;
    return p;
  }

  function manaCost(costStr) {
    const wrap = el("span", "manacost");
    S.parseCost(costStr).forEach(function (s) { wrap.appendChild(manaPip(s)); });
    return wrap;
  }

  function isCreature(card) {
    return (card.types || []).some(function (t) { return /creature/i.test(t); });
  }

  // One card face. `mode` = "battlefield" | "hand" | "stack" | "small".
  function cardEl(card, mode) {
    if (card.faceDown) {
      const b = el("div", "card cardback " + (mode || ""));
      b.title = "(hidden)";
      return b;
    }
    const c = el("div", "card " + S.colorClass(card) + " " + (mode || ""));
    if (card.tapped) c.classList.add("tapped");
    if (card.summoningSick) c.classList.add("sick");
    if (card.id) c.dataset.id = card.id;

    const head = el("div", "card-head");
    head.appendChild(el("span", "card-name", card.name || "?"));
    if (card.cost) head.appendChild(manaCost(card.cost));
    c.appendChild(head);

    const typeline = [].concat(card.types || [], (card.subtypes && card.subtypes.length) ? ["—"].concat(card.subtypes) : []).join(" ");
    if (typeline) c.appendChild(el("div", "card-type", typeline));

    // Attachment / counter badges.
    const badges = el("div", "card-badges");
    if (card.attachedTo) badges.appendChild(el("span", "badge badge-aura", "▸ attached"));
    if (card.attachments && card.attachments.length) badges.appendChild(el("span", "badge badge-host", "+" + card.attachments.length + " aura"));
    if (card.counters) {
      Object.keys(card.counters).forEach(function (k) {
        if (card.counters[k]) badges.appendChild(el("span", "badge badge-counter", card.counters[k] + "×" + k));
      });
    }
    if (badges.childNodes.length) c.appendChild(badges);

    // Power / toughness for creatures, current with printed when they differ.
    if (isCreature(card) && (card.power != null || card.toughness != null)) {
      const pt = el("div", "card-pt");
      const cur = (card.power != null ? card.power : "?") + "/" + (card.toughness != null ? card.toughness : "?");
      pt.appendChild(el("span", "pt-cur", cur));
      const hasPrinted = card.printedPower != null || card.printedToughness != null;
      const printed = (card.printedPower != null ? card.printedPower : "?") + "/" + (card.printedToughness != null ? card.printedToughness : "?");
      if (hasPrinted && printed !== cur) pt.appendChild(el("span", "pt-base", "(" + printed + ")"));
      if (card.damage) pt.appendChild(el("span", "pt-dmg", "-" + card.damage));
      c.appendChild(pt);
    }

    const tip = [card.name, typeline, card.text].filter(Boolean).join("\n");
    if (tip) c.title = tip;
    return c;
  }

  // Split a battlefield into visual rows: lands/other near the player edge,
  // creatures near the centre. Auras/equipment already attached are folded onto
  // their host (shown as a badge) and not rendered as loose permanents.
  function splitBattlefield(cards) {
    const attachedIds = {};
    cards.forEach(function (c) { if (c.attachedTo) attachedIds[c.id] = true; });
    const loose = cards.filter(function (c) { return !c.attachedTo; });
    const creatures = loose.filter(isCreature);
    const other = loose.filter(function (c) { return !isCreature(c); });
    return { creatures: creatures, other: other };
  }

  function battlefieldRows(player, opts) {
    const wrap = el("div", "battlefield");
    const split = splitBattlefield(player.battlefield || []);
    const creatureRow = el("div", "row creatures");
    const otherRow = el("div", "row lands");
    split.creatures.forEach(function (c) { creatureRow.appendChild(cardEl(c, "battlefield")); });
    split.other.forEach(function (c) { otherRow.appendChild(cardEl(c, "battlefield")); });
    if (!split.creatures.length) creatureRow.appendChild(el("div", "empty-hint", "no creatures"));
    if (!split.other.length) otherRow.appendChild(el("div", "empty-hint", "no lands / other"));
    // For the top (opponent) seat, lands sit ABOVE creatures so the centre line
    // stays between the two armies.
    if (opts.top) { wrap.appendChild(otherRow); wrap.appendChild(creatureRow); }
    else { wrap.appendChild(creatureRow); wrap.appendChild(otherRow); }
    return wrap;
  }

  function manaPoolEl(pool) {
    const wrap = el("div", "manapool");
    wrap.appendChild(el("span", "manapool-label", "pool"));
    let any = false;
    S.MANA_COLORS.forEach(function (col) {
      const n = pool ? pool[col] : 0;
      if (n) { any = true; for (var i = 0; i < n; i++) wrap.appendChild(manaPip(col === "C" ? "1" : col)); }
    });
    if (!any) wrap.appendChild(el("span", "empty-hint", "empty"));
    return wrap;
  }

  function playerBar(player, opts) {
    const bar = el("div", "playerbar");
    const nameEl = el("span", "pb-name", player.name || ("Player " + player.id));
    if (opts.active) nameEl.appendChild(el("span", "turn-dot", "●"));
    bar.appendChild(nameEl);
    bar.appendChild(el("span", "pb-life", "♥ " + player.life));
    bar.appendChild(el("span", "pb-stat", "🂠 lib " + (player.library || 0)));
    bar.appendChild(el("span", "pb-stat", "✋ hand " + (player.handSize != null ? player.handSize : (player.hand || []).length)));
    bar.appendChild(el("span", "pb-stat", "⚰ gy " + ((player.graveyard || []).length)));
    if (player.poison) bar.appendChild(el("span", "pb-stat pb-poison", "☠ " + player.poison));
    if (opts.priority) bar.appendChild(el("span", "pb-prio", "priority"));
    bar.appendChild(manaPoolEl(player.manaPool));
    return bar;
  }

  function handEl(player, faceUp) {
    const wrap = el("div", "hand");
    if (faceUp) {
      (player.hand || []).forEach(function (c) { wrap.appendChild(cardEl(c, "hand")); });
      if (!(player.hand || []).length) wrap.appendChild(el("div", "empty-hint", "empty hand"));
    } else {
      const n = player.handSize != null ? player.handSize : (player.hand || []).length;
      for (var i = 0; i < n; i++) wrap.appendChild(cardEl({ faceDown: true }, "hand"));
      if (!n) wrap.appendChild(el("div", "empty-hint", "empty hand"));
    }
    return wrap;
  }

  function phaseTrack(state) {
    const track = el("div", "phasetrack");
    const cur = S.phaseIndex(state.meta && state.meta.phase);
    S.PHASES.forEach(function (p, i) {
      const seg = el("span", "phase-seg" + (i === cur ? " active" : ""), p.label);
      track.appendChild(seg);
    });
    return track;
  }

  function stackEl(state) {
    const wrap = el("div", "stack" + ((state.stack && state.stack.length) ? " nonempty" : ""));
    wrap.appendChild(el("div", "stack-label", "Stack"));
    if (!state.stack || !state.stack.length) {
      wrap.appendChild(el("div", "empty-hint", "(empty)"));
      return wrap;
    }
    // Top of stack resolves first — show it first (visually on top).
    state.stack.slice().reverse().forEach(function (item, idx) {
      const it = el("div", "stack-item");
      it.appendChild(el("span", "stack-order", (idx === 0 ? "▲ top" : "")));
      it.appendChild(el("span", "stack-name", item.name || "?"));
      if (item.cost) it.appendChild(manaCost(item.cost));
      if (item.targets && item.targets.length) it.appendChild(el("span", "stack-targets", "→ " + item.targets.join(", ")));
      wrap.appendChild(it);
    });
    return wrap;
  }

  function seat(state, playerIndex, top) {
    const p = state.players[playerIndex];
    const wrap = el("div", "seat " + (top ? "seat-opponent" : "seat-you"));
    const active = state.meta && state.meta.activePlayer === playerIndex;
    const priority = state.meta && state.meta.priorityPlayer === playerIndex;
    const bar = playerBar(p, { active: active, priority: priority, top: top });
    const bf = battlefieldRows(p, { top: top });
    const hand = handEl(p, /*faceUp*/ playerIndex === (state.meta ? state.meta.viewpoint : 0));
    hand.classList.add(top ? "hand-top" : "hand-bottom");
    if (top) { wrap.appendChild(hand); wrap.appendChild(bar); wrap.appendChild(bf); }
    else { wrap.appendChild(bf); wrap.appendChild(bar); wrap.appendChild(hand); }
    return wrap;
  }

  function renderBoard(state, rootEl) {
    const check = S.validate(state);
    rootEl.innerHTML = "";
    if (!check.ok) {
      const err = el("div", "board-error", "Invalid BoardState: " + check.errors.join("; "));
      rootEl.appendChild(err);
      return;
    }
    const vp = state.meta ? (state.meta.viewpoint || 0) : 0;
    const opp = vp === 0 ? 1 : 0;

    const meta = el("div", "meta-bar");
    meta.appendChild(el("span", "meta-turn", "Turn " + (state.meta && state.meta.turn != null ? state.meta.turn : "—")));
    const activeName = state.players[state.meta ? state.meta.activePlayer : 0];
    meta.appendChild(el("span", "meta-active", (activeName ? activeName.name : "?") + "'s turn"));
    if (state.meta && state.meta.step) meta.appendChild(el("span", "meta-step", state.meta.step));
    meta.appendChild(phaseTrack(state));
    rootEl.appendChild(meta);

    rootEl.appendChild(seat(state, opp, /*top*/ true));
    rootEl.appendChild(stackEl(state));
    rootEl.appendChild(seat(state, vp, /*top*/ false));
  }

  root.WagicRender = { renderBoard: renderBoard, cardEl: cardEl };
})(window);
