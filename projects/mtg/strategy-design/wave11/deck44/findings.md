# Deck 44 (UB Faerie Tempo) — Wave 11 review (2nd corpus after the resume)

Corpus: PRIMARY `matchups-20260715-072254/` (binary `/tmp/wagic-54a5a9d01` = wave-10 engine batch +
max_tokens 4096 + parse ramble guards + stack-target annotation fix + fetch pre-targeting keying;
wave-10 guides; core prompt unchanged, 7th PASS). Round-robin, 21 games. deck44 = **deck0 in all 6
games** (confirmed by `gamestart` `my_deck=ai_baka_deck44`). Reply protocol: answer-after-plan
(scratch -> PLAN: -> CHOICE:/ATTACK:/BLOCKS: final line).

**Record 3W-3L.** WON vs133 (20/-7 T12), vs135 (18/-7 T18), vs131 (20/0 T14). LOST vs109
(-5/13 T11, outright), vs110 (adj1 1/17 T16), vs140 (adj1 17/25 T32). Waves 4->11: 3/6 -> 2/6 ->
3/6 -> 5/6 -> 5/6 -> 4/6 -> 4/6 -> **3/6**. Win column is context only (doctrine).

game->file map (via `gamestart` opp_deck + results.tsv):
`1784119227`=vs133 **W** . `1784122411`=vs110 **L(adj)** . `1784122965`=vs109 **L** .
`1784124541`=vs135 **W** . `1784124811`=vs140 **L(adj)** . `1784126729`=vs131 **W**.
Citations: `vsNNN sN`.

Posture: **FROZEN-SPINE, 8th wave running.** The spine (hold {U}{U}, sweeper/engine-first counters,
cast-a-faerie-every-turn, attack-every-turn, own-targets annotation obedience) is intact and obeyed.
This is the 2nd corpus under answer-after-plan and the FIRST with the wave-10 engine batch (4096 cap,
ramble guards, stack-annotation fix) deployed at this seat — and every wave-10 fix VERIFIES CLEAN.
**No new decision-level failure class. Guide disposition: FROZEN (byte-identical, md5
`bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines).** The three losses are construction/variance (a
creature-light reactive draw that cannot out-clock a fast board — the waves-5-11 residual). The
wave's whole model-fixable content is engine-ledger (notes.md), led by the STILL-UNFIXED auto-tap
color-stranding (ledger #2), which recurred mechanically this corpus but was outcome-neutral.

---

## FOCUS 1 (assigned) — AUTO-TAP COLOR-STRANDING (ledger #2, UNFIXED): recurred, but outcome-neutral

**The wave-10 class RECURRED mechanically. It did NOT cost a game this corpus** (unlike wave-10 vs140,
where a stranded Archmage cast denied the Counterspell for Blightsteel and lost the game). The engine
still auto-taps COLORED (blue) sources for the GENERIC portion of a cost when off-color (black) sources
are available, stranding the pilot's ability to hold {U}{U}.

Confirmed repros (mana line: `Mana available: <symbols> from N untapped sources (tapped
automatically when you cast)`):
- **vs131 s30 (T12 MP1):** cast `Archmage of Echoes {4}{u}` from `{u}{u}{u}{b}{b}{b}` (3 Island +
  3 Swamp). Archmage = 1 blue + 4 generic. After the cast, s31 shows `{b}` — **all 3 Islands were
  tapped** (1 for {u}, 2 for the {4}) plus 2 Swamps, leaving a single Swamp. Blue-preserving tapping
  (1 Island for {u}, then Swamps for generic) would have left `{u}` untapped. The greedy-blue auto-tap
  fired again.
- **vs131 s21 (T8):** cast `Surveilling Sprite {1}{u}` from `{u}{u}{u}{b}{b}` -> after (s22) `{u}{b}{b}`.
  Tapped 2 blue (1 for {u}, 1 for the {1} generic) when a Swamp was available for the generic; a blue
  source was stranded that need not have been.

**Why it was harmless this corpus:** at BOTH events the following turn had NO counter window
requiring the stranded blue — vs131 s30 was on a turn the opponent had 0 creatures and no stack, and
the pilot untapped to 7 sources next turn and won T14. **Every counter the pilot actually wanted this
corpus was cast successfully** — all 8 counter windows (below) resolved with sufficient blue, and the
late-game grind (vs140) had 10-13 untapped sources where stranding is moot. So: the ENGINE DEFECT
PERSISTS and is re-confirmed live via vs131, but 0 counters were denied by it this corpus.

**Routing UNCHANGED: engine ledger #2 (notes.md), NOT guide text.** The guide already says "LEAVE two
blue sources untapped," which auto-tap makes UNEXECUTABLE (the pilot cannot choose which lands tap).
This remains the single clearest game-changing mechanical item across waves 10-11 and is next cycle's
engine work. This corpus adds a clean, low-noise repro (vs131 s30) that is decoupled from a game loss,
which is useful for the fix's regression test.

---

## FOCUS 2 (assigned) — COUNTER SPINE: steady; correct/defensible with two minor blemishes

8 counter windows (counter cast) + 4 correct let-resolves + 2 GFTT windows. No new failure class;
comparable to wave-10's 15/15-with-2-blemishes. Full audit:

| window | stack spell | action | verdict |
|---|---|---|---|
| vs135 s14 | Ohran Viper | COUNTER (Counterspell) | CORRECT — growing value creature (deathtouch/card-draw), cat #3 |
| vs135 s17 | Ice-Fang Coatl | COUNTER (Arcane Denial) | CORRECT — growing value flyer, cat #3 |
| vs140 s21 | Lightmine Field | COUNTER (Counterspell) | CORRECT — attacker-hosing enchantment, cat #3 |
| vs131 s29 | Young Pyromancer | COUNTER (Arcane Denial) | CORRECT — growing engine, cat #3 |
| vs140 s6 | Cruel Edict (#1) | COUNTER (Counterspell) | DEFENSIBLE — would sac the pilot's ONLY creature (Gatewarden) |
| vs140 s12 | Cruel Edict (#2) | COUNTER (Arcane Denial) | DEFENSIBLE-BLEMISH — pilot held 2 creatures; the edict would only cost the defender, so an Arcane Denial (hands opp 2 cards) to save a 2/1 defender is a mild over-counter. Low stakes (T7, grind). |
| vs109 s15 | Boros Reckoner | COUNTER (Counterspell) | DEFENSIBLE — a 3/3 that punishes blocks/combat vs mono-red swarm at L16; the one card that makes combat math miserable |
| vs133 s12 | Bloodghast | COUNTER (Counterspell) | BLEMISH — Bloodghast is on the guide's EXPLICIT let-resolve list ("recursive small bodies"); it recurs on landfall so the counter is a mild waste. Low stakes: won 20/-7, counter-surplus hand, mono-black opp with few threats. Single instance. |

Correct let-resolves (declined counter):
- vs110 s11 Galvanic Blast (face burn, L19) — CORRECT (let burn resolve at high life).
- vs110 s23 Mox Opal (mana rock) — CORRECT (named let-resolve item).
- vs110 s9 Signal Pest (1/1 artifact) — DEFENSIBLE (not an engine; not worth an early Arcane Denial).
- vs110 s28 Vault Skirge (1/1 lifelink, L11) — DEFENSIBLE cat-#4 OVERRIDE. Category #4 ("counter any
  creature at <=12 life") is a broad safety net; the pilot correctly reasoned a 1/1 is not worth a
  counter when it has no board and needs the mana to deploy a faerie. Mirrors wave-10's vs109 s17
  Legion Loyalist defensible override. GFTT option here rendered the own-targets annotation correctly
  and was NOT picked.

GFTT windows (both CORRECT, both hit the OPPONENT via the named-target form):
- vs135 s24/s25: `legal targets right now: Scion of Oona, Ice-Fang...` -> sub-pick s25 hit Ohran Viper.
- vs131 s13/s14: `legal targets right now: Scion of Oona, Young Py...` -> sub-pick s14 hit Young Pyromancer.

**Verdict:** spine STEADY. 6/8 counters clearly correct/cat-aligned, 2 minor blemishes (Bloodghast
over-counter; 2nd Cruel Edict over-counter), neither a class, neither cost a game. No revision trigger.

---

## FOCUS 3 (assigned) — LEGAL-TARGET NAMES + own-targets steady; the stack-contradiction is GONE

The wave-10 ledger #1 defect (counter options rendering `- NO legal target right now` over an
OCCUPIED stack, contradicting the same option's `can target on the stack: <name>` clause) is **GONE
at this seat**: a grep of all 6 deck44 seat files for `NO legal target` returns ZERO hits (matches the
brief's corpus-wide "0 prompts"). Counter options now render cleanly:
`Cast Counterspell {u}{u} - can target on the stack: <name>` / `Cast Arcane Denial {1}{u} - can
target on the stack: <name>`. **The stack-annotation fix shipped and validated.** No contradiction
prompt, no self-negotiation loop, appears anywhere this corpus.

GFTT annotations — both forms render and are obeyed:
- **own-targets** (`the only legal targets are YOUR OWN right now`): **29 windows, 0 recorded
  self-casts.** (Wave-10 had 36 windows / 1 truncation-artifact self-cast; this corpus has ZERO — see
  Focus 4.)
- **named-target** (`legal targets right now: <names>`): 2 windows, both correctly hit an opponent
  creature (vs135 s25 Ohran Viper, vs131 s14 Young Pyromancer). The wave-9 fabricated-no-legal-target
  self-negotiation ("is it an artifact? Wait...") appears NOWHERE.

---

## FOCUS 4 (assigned) — RAMBLE-GUARD: VALIDATED; no prose-scanned casts

The wave-10 vs140 s68 class (a truncated reply with no `CHOICE:` line whose parser SCAVENGED a stray
body digit and recorded a spurious `Cast Go for the Throat` self-target) **did NOT recur.** Evidence:
- **0 truncation self-casts** — own-targets GFTT was 0/29 self-cast, and there is no fallback record
  anywhere with a scavenged cast integer.
- **All 4 fallback records show fallback + the HEURISTIC action, never a prose-scanned cast:**
  - vs110 **s10** (blockers) `fallback=unparsed_reply, choice=0` (no blocks). The reply was a
    PHANTOM-ATTACKER ramble — it hallucinated "Signal Pest is 4/1" attacking when the ACTUAL attacker
    list held only `A1. Memnite (1/1)`, and emitted `BLOCKS: B1:A2` (A2 out of range). Parser correctly
    rejected -> heuristic no-block, which is the GUIDE-CORRECT play (don't chump a lone 1/1 with the
    designated blocker at L19). The ramble was neutralized, not int-grabbed.
  - vs110 **s15** (blockers) `unparsed_reply, choice=0`. Reply reasoned "I will not block" and emitted
    `BLOCKS: Scion of Oona:none` (card NAME instead of `B#`). Parser rejected -> heuristic no-block =
    matched intent.
  - vs110 **s29** (blockers) `empty_reply, choice=0`. Empty reply = the known 120s HTTP-timeout class
    (240s shipped next corpus). Heuristic no-block, game-neutral.
  - vs135 **s37** (attackers) `unparsed_reply, choice=-1`. Reply reasoned a lethal swing and emitted
    `ATTACK: Scion of Oona, Sleep-Cursed Faerie` (card NAMES, not slot indices). Parser rejected ->
    choice -1 (heuristic). The lethal swing landed one window later (s41, opp 2 -> -7). Game won.

**The ramble guard is HOLDING at this seat:** every fallback resolved to the heuristic (0 or -1), none
scavenged a cast. Sub-note (harness, NOT guide): the free-form block/attacker parser is strict and
rejects the model's NAME-based or invalid-index declarations (`B1:A2` out-of-range, `Name:none`,
`ATTACK: <names>`). This corpus it cost nothing (every heuristic fallback matched the guide-correct or
game-neutral action), but it is a latent robustness gap — route to the reply-protocol/parse layer
(notes.md #3), never the guide.

---

## THE THREE LOSSES — ownership (all construction/variance, none guide-fixable)

- **vs109 (L, -5/13 T11):** at its last decision the pilot had 2 creatures vs the opponent's 5
  (mono-red Boros/Rakdos swarm: Goblins, Stromkirk Noble, Boros Reckoner). The pilot countered the
  Reckoner (defensible), deployed on every castable window, and attacked every turn — but a
  creature-light reactive draw (Surveilling Sprite + Sleep-Cursed Faerie as the only bodies) cannot
  out-race a wide aggro board. Construction/variance.
- **vs110 (adj-L, 1/17 T16):** the recurring vs110 signature — a creature-light draw where the ONLY
  recurring attacker was Surveilling Sprite (1 power/turn). Counter windows this game were all small
  non-engine cards (Signal Pest 1/1, Galvanic Blast face, Mox Opal, Vault Skirge 1/1) correctly or
  defensibly let-resolved; NO artifact ENGINE (Steel Overseer / Master of Etherium / Cranial Plating)
  reached the stack for the pilot to counter. Lost the affinity race on clock, not on a counter
  misplay. Construction/variance.
- **vs140 (adj-L, 17/25 T32):** a long grind; the pilot stayed alive at 17 (no Blightsteel this game),
  countered Cruel Edict x2 + Lightmine Field, and deployed a slow board — but fell behind on life
  adjudication against deck140's higher total. Grind/variance; the auto-tap defect did NOT bite here
  (late-game mana pools were 10-13 sources).

The loss ROTATES by matchup/draw (wave 7 vs133; 8 vs109; 9 vs109+vs110; 10 vs110+vs140; 11
vs109+vs110+vs140) — a variance signature on the slow reactive half of the deck, not a guide
regression.

---

## RE-CONFIRMED / FROZEN (all held; no change)
- **Counter spine on sweepers/engines/growers:** correct/defensible at 8/8 counter windows (2 minor
  blemishes) + 4 correct let-resolves. Sweeper-first (#1) UNTESTED this corpus (no Wrath/Pyroclasm hit
  the pilot's stack). #3 growers VALIDATED (Ohran Viper, Ice-Fang Coatl, Lightmine Field, Young
  Pyromancer). Freeze.
- **own-targets annotation: 29 windows, 29/29 obeyed, 0 self-casts.** Named-target: 2/2 hit opponent.
- **Stack-annotation fix (wave-10 ledger #1): shipped & validated** — 0 "NO legal target" contradictions.
- **Cast-a-creature-every-turn spine:** deployed on curve at essentially every castable window; the
  "Cast nothing" priority windows were correct holds (no creature castable / holding counter mana /
  opp at low life with lethal already assembled). Obeyed.
- **Attack-every-turn + lethal check:** obeyed; vs135 s41 swung for exact lethal (opp 2 -> -7); vs133
  s19 lethal swing (opp 3 -> -7). One `attackers: no attackers` (vs140 s33) was a correct hold on a
  turn with only a summoning-sick body.
- **BLOCKING block** (Obliterator never-block absolute; Gatewarden designated-blocker + growing/killable
  + wither-through-first-strike; 9+ no-block; <=8 chump): largely UNTESTED. Clean blocker windows: the
  vs110 phantom-ramble s10 (correct no-block at L19) and vs109 s10 (Gatewarden blocks Rakdos Cackler =
  correct designated-block). No Obliterator/first-striker/chump window arose. Pause-aware FREEZE. VERBATIM.
- **GFTT block** (target Obliterator; no-target fallback; can't-hit list): own+named annotations
  validated; GFTT-hold-for-Obliterator UNTESTED (vs133 mono-black did not present Obliterator as a
  combat factor). No-target fallback KEEP (untested this corpus). VERBATIM.
- **Face-burn value-floor carve-out:** partially TESTED and obeyed — vs110 s11 let Galvanic Blast
  resolve at L19. Held.
- **Mulligan; all card notes; SITUATIONS recap:** obeyed/untested. VERBATIM.

## HARNESS-DEFECT SWEEP (run first, per skill)
- DESYNC / head-choice mismatch: 0 genuine (answer-after-plan; CHOICE emitted last).
- ENGINE NO-OP: none (casts spend mana + tap sources). VALIDATION-DROP: none in the deck44 seat.
- NUMERIC-OFFSET / X-menu: N/A.
- Fallbacks: 4 total — 3 `unparsed_reply` (block/attacker free-form parse rejects, all heuristic-correct
  or game-neutral) + 1 `empty_reply` (vs110 s29, the 120s timeout class). Timeouts (raw): 0.
- **Truncation-scavenge class (wave-10 vs140 s68): GONE** (4096 cap; 0 scavenged casts). Ramble guard holds.
- **Stack mis-annotation (wave-10 ledger #1): GONE** (0 "NO legal target" over an occupied stack).
- **NEW-observed (low priority): block/attacker free-form parser rejects NAME-based / out-of-range
  declarations** -> heuristic. Cost nothing this corpus; latent. Engine/harness, notes.md #3.

## RESIDUAL (construction, not guide) — persists waves 5-11
16 Island / 10 Swamp + 6 reactive (2 Counterspell, 2 Arcane Denial, 2 GFTT). Counters earn their
slots (protected all 3 wins). The all-reactive/creature-light slow keep is the loss signature (all
three losses this wave). "Trim the counters" is the WRONG lever. Sharper levers: black-source density
vs the black spell load ({u}{b}/{2}{b}/{1}{b}), and/or 1-2 more cheap flyers over the 3rd/4th reactive
card to raise the aggro floor. Route to the user (notes.md).

## FREEZE-CHECK (guide is BYTE-IDENTICAL to the live wave-9/10 guide)
Live guide md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines. Every load-bearing line was OBSERVED
OBEYED or UNTESTED this corpus; NONE was observed-obeyed-AND-double-covered in a way that licenses
demotion under the pause-aware doctrine. **CHANGES: NONE.** No new decision-level failure class; the
wave-10 engine fixes (4096 cap, ramble guards, stack annotation) all verify clean; the auto-tap defect
(ledger #2) persists but is engine-layer, not guide. Displacement audit: 0 lines added, 0 removed.

## One-line handoff
Top deck, spine frozen 8th wave, 3W-3L (all 3 losses = creature-light reactive draws out-raced,
construction/variance). Wave-10 engine batch VERIFIES CLEAN at this seat: stack "NO legal target"
contradiction GONE (0), ramble guard HOLDS (0 scavenged casts; 4 fallbacks all heuristic), own-targets
29/29 obeyed 0 self-casts, named-target 2/2 hit opponent. Counter spine steady (8 windows, 6 correct +
2 minor blemishes). The ONE persistent model-relevant item is the STILL-UNFIXED auto-tap
color-stranding (ledger #2) — recurred at vs131 s30 (Archmage {4}{u} tapped all 3 Islands) but
outcome-neutral this corpus; next cycle's top engine item. Guide byte-identical FROZEN.
