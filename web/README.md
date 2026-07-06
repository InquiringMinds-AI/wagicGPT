# wagicGPT web frontend

An engine-agnostic web frontend for viewing (and, later, playing) wagicGPT
games in a browser. It exists for two reasons:

1. **Right now** — a fast way to *see* a board state while evaluating the LLM
   opponent's decisions, without fighting the SDL GUI.
2. **Long term** — the first concrete step toward **decoupling the rules engine
   from the frontend**, so different frontends (this web one, the native SDL
   one, a TUI, …) can drive the same engine.

## The decoupling seam: `BoardState`

The whole design turns on one idea: a frontend and an engine agree on a single
**data contract** and nothing else.

```
  engine  ──emits──▶  BoardState (JSON)  ──renders──▶  frontend
  engine  ◀─receives──   Action (JSON)   ◀──emits───   frontend
```

- **`BoardState`** fully describes a position: both players (life, mana pool,
  library count, hand, battlefield, graveyard, exile), every card (name, cost,
  colors, types, current-vs-printed P/T, tapped, counters, attachments,
  face-down, …), the stack, and game meta (turn, phase, active/priority player).
  Defined and documented in [`js/schema.js`](js/schema.js).
- **`Action`** describes a player's intent (declare attackers, cast with targets
  and X, declare blockers, choose target, pass priority). Defined in
  [`js/controls.js`](js/controls.js). This vocabulary intentionally mirrors the
  LLM opponent's existing bundled decision surface — a human clicking and the
  model replying produce the *same* Action shape.

Freeze that contract and the frontends become interchangeable, because none of
them knows anything about the others — only about the contract.

## Files

| File | Role |
|---|---|
| `index.html` | Shell. Loads the scripts in order; no bundler, no server. |
| `css/board.css` | Layout + dark royal-purple theme. Knows nothing about Wagic — styles whatever the renderer emits. |
| `js/schema.js` | **The `BoardState` contract**: phase model, validator, an `emptyBoard()` factory, helpers. |
| `js/render.js` | **Pure renderer**: `renderBoard(state, rootEl)`. No engine coupling, no network. This purity is what keeps the frontend swappable — change the data source, never this file. |
| `js/connection.js` | **The data-source seam**: `FixtureSource` (now) + `HttpSource` / `WsSource` stubs (later). The one file that changes when a live backend appears. |
| `js/controls.js` | **The action seam** (stubbed/disabled). Where clicks become `Action` docs once the backend can receive them. |
| `js/app.js` | Wiring: pick a source, populate the dev toolbar, render. |
| `fixtures/*.js` | Bundled `BoardState` positions that self-register (see below). |

## Running it

Just open `index.html` in a browser — **no server needed**:

```
firefox web/index.html
```

Fixtures are bundled as classic scripts that self-register into a registry,
rather than being `fetch()`ed, precisely so the `file://` origin works with zero
setup (`fetch('*.json')` is blocked under `file://`). Pick a position from the
toolbar dropdown.

### Adding a fixture

Create `fixtures/<name>.js`:

```js
WagicFixtures.register("<name>", { /* a BoardState document */ });
```

…and add a `<script src="fixtures/<name>.js">` line to `index.html` before
`app.js`. The object you register is the exact wire JSON a live backend will
send. `fixtures/nightmare-mirror-attackers.js` is a real position lifted from
the self-play corpus.

## Status / roadmap

- [x] `BoardState` contract + pure renderer + dark theme
- [x] Real fixture from the self-play corpus
- [x] Connection / control seams stubbed with the agreed shapes
- [ ] Backend: an engine endpoint that emits `BoardState` (the SDL renderer and
      this one would consume the same document)
- [ ] Live source (`HttpSource`/`WsSource`) wired in `app.js`
- [ ] Interactive controls: enable `controls.js`, make cards/zones clickable,
      emit `Action` docs back to the engine
- [ ] Richer fixtures (a position with a non-empty stack, combat with blocks)

## Design constraints kept in mind

- **Renderer stays pure** — the moment it reaches for the network or engine
  internals, the decoupling is lost. Data in, DOM out.
- **No build step / no dependencies** — classic scripts, opens from `file://`.
  A bundler/dev-server can come later *if* a live backend needs it; it is not
  required to view a board.
