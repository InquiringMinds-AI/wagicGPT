# Audit 2026-09-02 — owner decisions (ruled 2026-09-03, recorded verbatim where quoted)

Rulings on the O1-O11 list in `09-synthesis.md` §3. Each line: ruling, action, when.

- **O1 heap 224 MB** — "lets try it." APPLIED on master for vpk13 (`JGE/src/Vitamain.cpp`,
  `_newlib_heap_size_user`); untested on hardware until vpk13's memlog reads (`ram_free` should
  drop ~96 MB at boot; `heap_arena` flat with lane N's decode scratch).
- **O2 PARSETEST out of handheld builds** — "sure." APPLIED on master: `runParseSelfTest` body
  gated on `WAGIC_GPT_PARSETEST_BUILD`, defined in `wagic-SDL.pro` (desktop) only; stub path
  syntax-checked; desktop PARSETEST unchanged at 2475/0.
- **O3 dead modules / NETWORK_SUPPORT** — dead modules: delete (own commit, AFTER the wave-54
  corpus). NETWORK_SUPPORT and the network scaffolding: KEEP. His words: "networking is high on my
  desired features list. multiplayer makes wagic a much more interesting project. that said, i
  dont anticipate working on it soon. i want to solve all of the issues i can find first before i
  look to make the equation more complicated."
- **O4 kMaxOptSources** — "sure." Measure-then-decide: a lane times `refineForOptions` at 16/20/24
  producers on desktop, scales by the Vita factor, then picks the cap (or a smarter search).
  Queued, not started.
- **O5 RenderSpell / Arena grouping** — remains his UI redesign. Owner datum: "i've done an
  infinite loop to generate many squirrel tokens that didnt result in the game performance
  tanking" — consistent with the audit's finding that the scan grows with attached spells, not
  creature count.
- **O6 WAGIC_FIZZLELOG in Vita builds** — KEEP. His framing: "the builds we're doing now are not
  release builds. we're making diagnostic development builds at the moment. my playtesting on the
  vita is to produce another set of data for the core loop to make use of." The
  diagnostics-out-of-release rule applies to RELEASE builds; the Vita alphas are instruments.
- **O7 Vita gnu++14** — "we should try it, but maybe not in conjunction with many other changes.
  we dont want to confound diagnosing potential issues." NOT in vpk13; its own later build.
- **O8 second RNG stream for the heuristic AI** — "yes." Lane after the wave-54 corpus (breaks
  replay of pre-change transcripts; re-baseline AI-draw-dependent fixtures). Note the separate
  equip-autotap replay blocker (lane AA) — this fix alone does not make replay whole.
- **O9 AIStats** — keep as is (lane K's I/O fix shipped; feature untouched).
- **O10 NOSTARSDYNAMIC on the Vita** — KEEP the mana stars. His words: "this is fun eye candy, and
  its how the player tapping their own mana knows how much mana they have floated."
- **O11 30 fps cap expression** — wait for vpk13's `avg_swp`/`avg_upd`/`avg_rnd` reading (lane
  N's analysis in `wave54/lane-N.md` §1 decides which regime the lag games are in).
