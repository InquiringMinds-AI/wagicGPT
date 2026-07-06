/* app.js — wiring. Chooses a data source, populates the dev toolbar, renders.
 *
 * Today the source is the bundled FixtureSource. To point at a live backend
 * later, construct an HttpSource/WsSource instead and call the same three
 * methods — nothing else changes.
 */
(function (root) {
  "use strict";

  function boot() {
    const boardEl = document.getElementById("board");
    const picker = document.getElementById("fixture-picker");
    const status = document.getElementById("status");

    const source = new root.WagicConnection.FixtureSource();

    function show(name) {
      source.load(name).then(function (state) {
        if (!state) { status.textContent = "no fixture: " + name; return; }
        root.WagicRender.renderBoard(state, boardEl);
        status.textContent = "showing: " + name;
        root.__lastState = state; // handy for console poking
      });
    }

    const names = source.list();
    names.forEach(function (n) {
      const opt = document.createElement("option");
      opt.value = n; opt.textContent = n;
      picker.appendChild(opt);
    });
    picker.addEventListener("change", function () { show(picker.value); });

    document.getElementById("reload").addEventListener("click", function () { show(picker.value); });

    if (names.length) show(names[0]);
    else status.textContent = "no fixtures registered";
  }

  if (document.readyState === "loading") document.addEventListener("DOMContentLoaded", boot);
  else boot();
})(window);
