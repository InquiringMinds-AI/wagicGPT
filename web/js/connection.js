/* connection.js — the data-source abstraction.
 *
 * This is the ONE file that changes when we go from static fixtures to a live
 * backend. Everything upstream (render.js) only ever sees a BoardState object;
 * it neither knows nor cares whether that object came from a bundled fixture,
 * an HTTP poll, or a WebSocket push. Swapping the source is swapping the class
 * used here — the renderer is untouched.
 *
 * Sources expose a common shape:
 *   .list()                 -> [names]         (what positions are available)
 *   .load(name) -> Promise<BoardState>
 *   .subscribe(cb)          -> unsub fn        (cb(state) on every update; live
 *                                               sources push, static ones no-op)
 */
(function (root) {
  "use strict";

  // Fixtures register themselves into this global (see fixtures/*.js). Using a
  // registry instead of fetch() is deliberate: fetch('*.json') is blocked under
  // the file:// origin, so bundling fixtures as classic scripts keeps the app
  // working when index.html is opened directly, with no server.
  const REGISTRY = {};
  function register(name, state) { REGISTRY[name] = state; }

  // Static source backed by the bundled fixtures. Default for now.
  function FixtureSource() {}
  FixtureSource.prototype.list = function () { return Object.keys(REGISTRY); };
  FixtureSource.prototype.load = function (name) {
    const s = REGISTRY[name] || REGISTRY[this.list()[0]];
    return Promise.resolve(s ? JSON.parse(JSON.stringify(s)) : null); // clone: renderer must not mutate the fixture
  };
  FixtureSource.prototype.subscribe = function () { return function () {}; };

  /* ---- Stubs for when the backend exists. Not wired up yet; kept here so the
   * seam is visible and the shape is agreed in advance. ----
   *
   * HttpSource(baseUrl): GET {baseUrl}/state -> BoardState. `subscribe` would
   * poll on an interval. A future engine build exposes such an endpoint (the
   * same BoardState it would hand the SDL renderer).
   *
   * WsSource(url): connect, receive BoardState frames, push each to subscribers.
   * This is the target for live play — the engine emits a new BoardState after
   * every state change, the frontend re-renders. Controls (controls.js) send
   * Action documents back over the same socket.
   */
  function HttpSource(baseUrl) { this.baseUrl = baseUrl; }
  HttpSource.prototype.list = function () { return ["live"]; };
  HttpSource.prototype.load = function () {
    return fetch(this.baseUrl.replace(/\/$/, "") + "/state").then(function (r) { return r.json(); });
  };
  HttpSource.prototype.subscribe = function (cb) {
    const self = this;
    const t = setInterval(function () { self.load().then(cb).catch(function () {}); }, 1000);
    return function () { clearInterval(t); };
  };

  function WsSource(url) { this.url = url; this.ws = null; this.subs = []; }
  WsSource.prototype.list = function () { return ["live"]; };
  WsSource.prototype._ensure = function () {
    if (this.ws) return;
    const self = this;
    this.ws = new WebSocket(this.url);
    this.ws.onmessage = function (ev) {
      let state; try { state = JSON.parse(ev.data); } catch (e) { return; }
      self.subs.forEach(function (cb) { cb(state); });
    };
  };
  WsSource.prototype.load = function () {
    // On a push source, "load" resolves on the next frame.
    const self = this; self._ensure();
    return new Promise(function (resolve) { self.subs.push(function once(s) {
      self.subs = self.subs.filter(function (c) { return c !== once; }); resolve(s);
    }); });
  };
  WsSource.prototype.subscribe = function (cb) {
    const self = this; self._ensure(); this.subs.push(cb);
    return function () { self.subs = self.subs.filter(function (c) { return c !== cb; }); };
  };

  root.WagicConnection = {
    register: register,
    FixtureSource: FixtureSource,
    HttpSource: HttpSource,
    WsSource: WsSource,
    _registry: REGISTRY,
  };
  // Fixtures call this global directly for brevity.
  root.WagicFixtures = { register: register };
})(window);
