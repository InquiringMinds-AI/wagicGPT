# Lane W — Vita frame-rate report (vpk12: "really bad frame rate", two wide-board games)

Owner reports, both tagged `other`, both on WIDE boards:
`psp-work/logs/20260902-vpk12/transcript-1788378791-player_deck3-vs-ai_baka_deck164.txt`
(Merkfolk + Drake tokens vs a 16-click Nazgul/Ring swarm, 12 turns) and
`...-1788381501-player_deck5-vs-ai_baka_deck95.txt` (Squirelly Wrath vs Troll Ascetic/Slith,
14 turns). Memlog for that session is flat (heap_free 26-28 MB), so this is CPU, not memory.
Lane S measured the hand-refresh and explicitly did NOT measure the render path; this lane
does, and ships the on-device instrument that will settle it.

---

## 1. TELEMETRY (shipped, alpha-gated) — what the next session's memlog will say

Same compile gate (`WAGIC_VITAMEMLOG`, defined only in the Vita `CMakeLists.txt`, marked
ALPHA-ONLY / REMOVE FOR RELEASE), same file (`ux0:data/Wagic/memlog.txt`), same one-line
ASCII shape. Nothing new compiles into a build without the define; the desktop is untouched.

The main loop (`JGE/src/Vitamain.cpp::MainLoop`) now reads `sceKernelGetProcessTimeWide()`
(microseconds) four times per frame and attributes the frame to **Update / Render / SwapBuffers**
separately. Swap is measured because a GPU-bound frame parks there, and telling
"the CPU is slow" from "the GPU is slow" is the first question a profile has to answer.

Two new line shapes, appended next to the existing `untap` memory line:

    t=<sec> frames turn=N n=F avg=M.T max=M.T avg_upd=M.T max_upd=M.T avg_rnd=M.T max_rnd=M.T \
            avg_swp=M.T max_swp=M.T p1perm=P1 p2perm=P2 hand=H abil=A

  one per turn (written from `GameObserver::nextGamePhase`'s untap case, right after
  `vitaMemProbe("untap", turn)`), then the accumulators reset. `M.T` = milliseconds with one
  decimal, produced by integer arithmetic (no float formatting pulled into the Vita build).

    t=<sec> frame turn=N phase=update|render|swap ms=M.T upd=M.T rnd=M.T swp=M.T \
            p1perm=P1 p2perm=P2 hand=H abil=A

  written whenever a SINGLE frame exceeds 100 ms, naming the phase that owned it. Capped at
  5 per turn (reset by the per-turn line) so a slow turn cannot flood the log.

Counts come from `GameObserver::Update`, republished every tick by
`vitaFrameCounts(turn, p1perm, p2perm, hand, abilities)`: `p1perm`/`p2perm` = each player's
`inPlay->nb_cards`, `hand` = the rendered player's hand size, `abil` = the action layer's
`mObjects.size()` (every live ability object). Four container sizes — no walk, no allocation.

Verification done here:
- the memlog block was extracted and compiled standalone against stub vitasdk/vitaGL
  declarations (`~/.gatelogs/w53W/vitastub.cpp`), `g++ -Wall -Wextra`, clean — this is the
  check that would have caught the vpk9 build break (an `extern "C"` inside a function body);
- the stub was then LINKED AND RUN to print the real lines, so the format is confirmed
  stable ASCII and the existing `untap` line is byte-for-byte unchanged:

      t=0 untap turn=1 heap_used=0 heap_free=0 ... ext_free=0
      t=0 frame turn=3 phase=update ms=184.0 upd=150.0 rnd=30.0 swp=4.0 p1perm=20 p2perm=18 hand=7 abil=140
      t=0 frames turn=3 n=1 avg=184.0 max=184.0 avg_upd=150.0 ... p1perm=20 p2perm=18 hand=7 abil=140

- the `GameObserver.cpp` hooks were compiled on the DESKTOP with the define forced on (a
  scratch copy with `defined(VITA) &&` stripped, `-DWAGIC_VITAMEMLOG`): clean.

---

## 2. DESKTOP PROFILE

**Instrument.** A throwaway env-gated profiler (`WAGIC_FRAMEPROF=<file>`) timed every
`GuiLayer`'s `Update` and `Render` inside `DuelLayers`, plus the AI `Act` and `CheckUserInput`
legs, bucketed by total permanents; a second level timed `GuiGameZone::Render`,
`GuiAvatar::Render`, `CardStack::RenderSpell` and `CardGui::Render`. **All of it was removed
before commit** (owner rule: no diagnostics in release builds); the patch scripts are in
`~/.gatelogs/w53W/` if it needs rebuilding.

**Two instrument corrections that changed the numbers** (both would have produced a confident
wrong answer):
1. The tracked desktop makefile builds at **-O0** (`CXXFLAGS` has no `-O`), the Vita CMake at
   **-O2**. An -O0 profile is dominated by un-inlined `std::string` accessors (`perf` put
   `basic_string::size()` at 8.5%) and mis-ranks the legs. Every number below is from a
   parallel **-O2** build (`Makefile.sdlO2` + `objs-o2`, both deleted afterwards).
2. `WAGIC_FASTCLOCK=0.1` inflates `dt`, which expires `AIPlayerBaka`'s 0.1 s decision timer
   EVERY tick — so `Act`'s share is an artefact at that setting. The final runs use
   `WAGIC_FASTCLOCK=0.0333`, i.e. a 30 fps `dt`, which is the console's cadence.

**Board source.** Replaying the owner's two transcripts did not reach a wide board:
`...deck164` diverges at action 94 (turn 5, `p2.no 1` vs `p2.choice 0`) and `...deck95`
aborts in `dumpAssert` at load. So the boards were grown instead by headless/offscreen
self-play of the reported Nazgul deck (`WAGIC_SELFPLAY_DECK0=164 DECK1=164`, `WAGIC_AI=baka`),
which reaches 20-29 permanents naturally. Rendering ran under SDL's **offscreen** video driver
(real EGL/GL, no window) so the owner's display was never touched.

### Per-frame cost, -O2, 30 fps dt, ~12-22 permanents

| leg | ms / frame | notes |
|---|---|---|
| **RENDER total** | **1.28** | |
| ` CardGui::Render` (all cards drawn that frame) | 0.81 | **0.061 ms per card**, ~13 cards — the dominant term, linear in board width |
| ` GuiGameZone::Render` × 10 zone widgets | 0.24 | constant, board-independent — **the fixable one** |
| ` CardStack::RenderSpell` scan (D28's O(n²)) | 0.13 | 0.007-0.010 ms per call, real but second-order |
| ` GuiAvatar::Render` × 2 | 0.08 | |
| ` GuiPhaseBar::Render` | 0.09 avg / **15.5 max** | spikes on phase change (texture//font), not steady state |
| **UPDATE total** | **2.02** | |
| ` [AI Act]` | 1.68 avg / 58.5 max | only on the AI's turn; see below |
| ` ActionLayer::Update` | 0.26 | walks every live ability object twice (testDestroy + Update) |
| ` everything else` | ~0.08 | `GuiHandSelf` 0.03 after lane S; the rest is noise |

### Scaling with board width (-O2, per tick / per frame)

| permanents | Update | Render |
|---|---|---|
| 0-3 | 0.35-0.38 | 0.65-0.70 |
| 8-11 | 1.08-1.25 | 0.71-0.80 |
| 16-19 | 3.43-3.97 | 1.09-1.16 |
| 20-23 | 3.2-7.5 | 1.28-1.36 |
| 24-27 | 10.0 | 1.60 |

Render is ~linear (+0.03 ms per permanent, i.e. one `CardGui::Render`). Update is
super-linear and is almost entirely `AI Act`: on the HUMAN's own turn `Act` is not called
(`isAI` is false for the rendered player), and Update-minus-Act on a 22-permanent board is
only **0.46 ms**.

### Ranked cost centres

1. **`CardGui::Render`, 0.061 ms per card on screen.** ~88% of it is the drawing itself
   (fake border, highlight border, quad, P/T and counter text); only ~12% is quad resolution
   (`RetrieveCard` + the `getCardByName` re-lookup for renamed cards). A wide board pays this
   N times a frame. Bounding it means drawing fewer or cheaper cards — i.e. Arena-style
   grouping, which is **D28 and an OWNER ITEM**. Measured, not redesigned.
2. **`AIPlayerBaka::Act` / `computeActions`, up to 10 ms per decision tick at 24-27
   permanents** (desktop, -O2). It is throttled to one call per 0.1 s, so at 30 fps it lands
   on every third frame — a periodic spike, not a constant tax, and it hits only during the
   opponent's turn. This is the largest single number in the profile and it is AI cost, not
   render cost; it is not touched here.
3. **`GuiGameZone::Render` × 10 invisible widgets, 0.24 ms per frame, constant** — fixed
   below.
4. **`CardStack::RenderSpell`, 0.13 ms/frame** — D28's named O(n²) scan. Real, and it grows
   with attached spells rather than with creatures, so a Nazgul swarm does not feed it.
   Left alone (D28 is an owner item).

---

## 3. THE FIX (bounded, no UI redesign)

`projects/mtg/src/GuiStatic.cpp`, `GuiGameZone::Render`:

1. **Skip a zone widget that cannot draw anything.** `GuiAvatars` builds TEN `GuiGameZone`
   objects (both graveyards, libraries, exiles, command zones, the sideboard, the opponent's
   hand) and sets every one of their alphas to 0 in its constructor; they only become visible
   while the cursor sits on the avatar row. Each still ran six texture-cache lookups, a
   `RenderQuad` and two `DrawString`s every frame to write nothing.
   Guard: `if (actA < 1.0f && !mHasFocus && !showCards && cards.empty()) return;`
   Why it is a provable no-op: every colour the body sets is `ARGB((int)(actA), ...)`, which
   truncates to a fully transparent 0 below `actA` 1.0, and the blend is
   SRC_ALPHA/ONE_MINUS_SRC_ALPHA; the focus rect is gated on `mHasFocus`, the expanded
   display on `showCards`, the animating card views on `cards`, and `PlayGuiObject::Render()`
   is empty. The threshold is "below one alpha step", NOT "exactly zero" — `Pos::Update`
   decays `actA` towards `alpha` asymptotically, so `actA <= 0.0f` almost never held. That
   was measured, not assumed: the first version of this guard used `<= 0.0f` and fired on
   **0 of 8690** zone renders, i.e. it was a no-op fix that A/B'd as noise.
2. **Fetch one icon, not six.** A zone wears exactly one of `iconcard/iconhand/iconlibrary/
   iconexile/iconcommandzone/iconsideboard`; all six were retrieved every frame, each
   building two `std::string`s and hashing a cache key. Now a `switch (type)`.

### Before → after (paired A/B, one binary, env kill-switch `W53W_NOSKIP`, median of 3 games)

| | before | after |
|---|---|---|
| zone renders actually executed | 10.0 / frame | 0.6 / frame (94% skipped) |
| `GuiGameZone::Render` total | 0.565 ms / frame | 0.133 ms / frame (**4.2×**) |
| `GuiAvatars` layer render | 0.673 ms / frame | 0.231 ms / frame (**2.9×**) |
| whole-frame Render | 3.03 ms | 1.86 ms |

The A/B ran while the master corpus loaded the machine, so the ABSOLUTE numbers there are
~2.5× the quiet-machine figures in the table above; the paired deltas are the claim. On a
quiet machine the same legs read 0.24 ms/frame before and ~0.06 ms/frame after — about
**18% of the whole render frame, removed, at every board size**.

---

## 4. Gate

Suite (single-threaded ground truth, `WAGIC_TESTSUITE_THREADS=1`): **1215 tests, 0 failed,
46 AI tests, 0 failed, 0 timeouts** — the baseline count, and the two known concurrency-only
failures (lifeline / merrow_reejerey) do not appear single-threaded, as in lane S.
PARSETEST: **2249 passed / 0 failed** — exactly the baseline; no parse-relevant string was
touched. No fixture added or removed.

## 5. What I did NOT verify

- **Nothing was measured on the console.** Every number is desktop. The whole point of §1 is
  that the owner's next session answers this; do not treat the desktop ranking as the Vita
  ranking, and in particular do not assume a fixed desktop→Vita factor (this lane deliberately
  quotes none).
- **The Vita main-loop code has never been through a Vita compiler.** It was syntax-checked
  against stubs on the desktop, which is the check that catches the vpk9 class of error, but
  `vitaGL`/`vitasdk` headers were not involved and no VPK was built (VPK builds are
  owner-initiated). The build is still the real test.
- **No suite pin.** The change is render-only and the suite is headless — there is no
  headless surface that can observe it. The correctness argument is the exhaustive read of
  `GuiGameZone::Render` above (every drawing statement is covered by one of the four guard
  terms), not a fixture. A screenshot A/B on the owner's display was deliberately not run: he
  was using the TV.
- **The two reported boards were not reproduced.** Both transcripts fail to replay (one
  diverges at turn 5, one aborts in `dumpAssert` at load — both worth their own item); the
  profile stands on the same Nazgul deck reaching a comparable width in self-play, not on his
  exact board or hand.
- **GPU cost is unmeasured.** The desktop ran offscreen EGL on an NVIDIA driver; vitaGL's
  submission path and the GXM wait in `vglSwapBuffers` have no desktop analogue. `avg_swp` in
  the new telemetry is the only instrument that will price them.
- `CardGui::Render`'s internals were split only into "quad resolve" vs "the rest"; which of
  the border/text/counter draws dominates the remaining 88% is not resolved.
- The AI `Act` number is per DECISION tick; I did not measure how often the timer expires in
  a real console frame sequence, only that its period is 0.1 s.
