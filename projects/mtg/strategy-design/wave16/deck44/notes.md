# Notes for wagicGPT development — deck44 wave 16 (self-contained; full repros)

Run: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-084532/`. deck44 = deck0 in all 6 games.
Binary `/tmp/wagic-69793930a`. **Record 4W-2L** (W vs21, vs135, vs133, vs17; L vs62-adj, vs110-kill).
118 ask decisions + 30 attacker + 14 blocker asks; 7 fallbacks (4 unparsed_reply, 3 stale_echo, all
correct); name-echo 116/116. Win column is context only (doctrine).

game -> result (`results.tsv`, deck0=44):
- `1784209534` vs21 W (kill 4/-4 T12) | `1784215280` vs135 W (kill 17/-4 T16)
- `1784218657` vs133 W (kill 18/-4 T12) | `1784218523` vs17 W (kill 6/-4 T20)
- `1784215445` vs62 L-adj (2/9 T18) | `1784217277` vs110 L (kill -10/25 T11)

## WAVE-15 CORRECTION APPLIED (the binding charge)
Wave-15 deck44 F2/notes-#1 ("combat asks — attacker+blocker declaration — removed corpus-wide; 0 at
any seat") is **FALSE and rejected** (synthesis boundary check; per-seat counts). Corrected read this
corpus: **deck44 = 30 attacker asks + 14 blocker asks** (`kind` enumeration across the 6 seat logs).
The LLM declares combat normally; the guide's BLOCKING/LETHAL sections are LIVE. The wave-15
loss-attribution ("all-in auto-attacker left no crackback blocker") and the block-prose-trim method
are void and NOT carried forward.

## Guide change this wave: NONE (frozen byte-identical)
Live guide md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines, 12084 B; `cmp` vs wave16 strategy.txt
IDENTICAL. 12th consecutive frozen wave. Losses are construction/variance/engine, none guide-authorable.

---

## ENGINE / HARNESS LEDGER (layer-routed here, NOT prompt text)

### #1 (HIGH · reaffirms wave-15 synthesis #7) suppress a top-level targeted-removal cast whose ONLY legal targets are friendly
**Repro:** vs62... no — vs135 `1784215280` **s21** (T10, opp turn). Options `[1] Cast Go for the
Throat {1}{b} - the only legal targets are YOUR OWN right now / [2] Cast nothing`. Model chose c=1.
Result (from s22 prompt narration): `Your Go for the Throat: hand -> stack` ; `Your Faerie Miscreant:
battlefield -> graveyard` ; `Opponent's Ohran Viper: stack -> battlefield` — deck44 **destroyed its
own Faerie Miscreant** and the opponent's creature resolved anyway. The friendly-only annotation was
PRESENT and read (the model's PLAN even quotes the target reasoning). Echo NAMED option 1 and index=1,
so NO staleness arm caught it. **This is wave-15 synthesis ledger #7 recurring — the self-destroy
class is NOT fully closed.** Contradicts wave-15 deck44 F3's "class CLOSED at this seat."
**Fix:** make a top-level cast of a mandatory-target removal spell whose only legal targets are the
caster's own permanents structurally unavailable (extend the 601.2c no-legal-target suppression to
"no legal OPPONENT/hostile target for a removal spell"). MED severity (low outcome cost this instance
— game already won; high signal). Owner: cast-option serialization / 601.2c-style filter. Secondary
model-watch: the caster believed GFTT could target a creature on the STACK (rules confusion —
removal answers the battlefield, counters answer the stack). Model-side; not guide-fixable.

### #2 (translog nit — carried from wave-15 #12, re-confirmed) `options` field logged as an INT COUNT
Every deck44 record this corpus has `options` = an integer (1, 2, 3...) not the option LIST — the
option text had to be recovered from the `prompt` CHOICES block for every decision. This made the
vs62 s27 "was Obyra offered?" question require prompt-scraping (answer: no — uncastable, all Islands).
**Fix (cheap):** serialize `options` as the list. Also still no `resolved:` field (deltas scraped from
`events`). Owner: translog serialization (c2 fields). LOW, reviewer-facing.

### #3 (VERIFIED WORKING — hybrid-pip annotation) no defect; positive control
The `[hybrid: each {u/b} pays with U or B - total N mana]` annotation fired on 7 deck44 Oona's
Gatewarden decisions; 6/6 that reached the model cast correctly, 0 misreads (F4). One useful rescue:
vs62 s5/s28 — on an all-Islands (black-screwed) board it correctly cast the {u/b} Gatewarden off
Islands. No fix needed; logged as the annotation working as designed at a high-hybrid seat. Owner: n/a.

### #4 (harness — stale_echo, all correct this corpus) 3 catches, 0 wrongful downgrades
- vs62 `1784215445` s18 (opp creatureless; model declined GFTT, echo didn't match an offered option) — correct.
- vs62 s27 (model planned to cast Obyra {u}{b} on an ALL-ISLANDS board — **uncastable**, engine never
  offered it) -> stale_echo, choice=-1 -> heuristic. **Correct refusal** (NOT a wrongful downgrade;
  the "dropped" cast was color-screwed and illegal). Verified against the s27 prompt `Mana available:
  {u}{u}{u}{u}{u}`.
- vs17 `1784218523` s34 (attacker-declaration ramble) -> stale_echo, harmless.
The wave-15 superstring FP fix appears effective at this seat — no wrongful downgrade found. Owner: n/a
(behaving correctly).

### #5 (unparsed_reply guard — 4 fired, all harmless) reasoning rambles, guard working
vs21 s23, vs135 s18, vs17 s14 + s18 — replies rambled without a clean CHOICE line -> heuristic
resolved. No cost. Owner: reply parser (behaving correctly).

---

## MODEL WATCHES (route to model-experiments, not any guide)
- **Uncastable-cast hallucination under color-screw** (vs62): the model repeatedly PLANNED to cast a
  black spell (Obyra {u}{b}) while `Mana available` showed only `{u}` sources. The harness caught every
  instance, but the model wasted decisions not reading its own mana line. Latent risk only. Mitigation
  if it ever bites: none needed while the harness refuses illegal casts. Cross-check whether other
  color-committed seats show the same tic.
- **Over-blocking at healthy life** (vs17 s36, 16/7): chumped two Archmages + a Faerie Miscreant into
  deck17's bigger fliers while ahead on the clock (opp at 7). Guide says at 9+ life keep threats
  attacking. Traded key threats defensively; game won regardless. LOW; single instance; watch whether
  it recurs at other high-life seats before treating as a class.
- **Stack-vs-battlefield removal targeting** (vs135 s21, see ledger #1): believed GFTT could answer a
  spell on the stack. Rules-model gap, same family as the corpus-wide damage-vs-toughness /
  summoning-sickness model watches.

## DECK-CONSTRUCTION FLAGS (for the user / decklist owner) — persists waves 5-16, now TWO-SIDED
16 Island / 10 Swamp + 6 reactive (2 Counterspell {U}{U}, 2 Arcane Denial {1}{U}, 2 GFTT {1}{B}).
The mana base must serve BOTH double-blue counters AND black creatures/removal (Obyra {u}{b},
Bladecrafter {2}{b}, GFTT {1}{b}), so **either color can be the pinch:**
- **wave-15:** BLUE-screw — couldn't pay {U}{U} Counterspell off a Swamp-heavy board (vs140 s22).
- **wave-16:** BLACK-screw — Obyra/Bladecrafter/GFTT all dead in hand on an all-Islands board (vs62,
  a LOSS). deck44's only castable creature those turns was the {u/b} Gatewarden (hybrid saved it).
The counters EARN their slots (carried multiple wins). Real construction levers (NOT "trim counters"):
(a) more dual/hybrid fixing to un-pinch both colors — Oona's Gatewarden's {u/b} was the only card
that stayed castable under screw, arguing for more flexible-cost cards or fixing lands; (b) 1-2 more
cheap flyers / a low-pip threat to raise the aggro floor and stop losing the close races (vs110 died
with ~1 power of clock; the reactive-heavy no-threat keep is the recurring loss signature). Route:
construction notes (owner's call — decklist, not guide).

## ROTATION VERDICT (explicit, per brief)
**Guide: FREEZE (12th wave). Seat: ROTATE OUT after this cycle.**
Rationale: (1) the wave-15 corrected-read charge is DISCHARGED — combat asks confirmed present
(30/14), the false loss-attribution is refuted, and the seat now posts a clean 4/6. (2) The guide
took no modification and none is warranted (its failures are engine/construction/variance, all
routed off-seat). (3) The one new decision-quality signal (self-GFTT, ledger #1) is a REAFFIRMATION
of an already-open synthesis item (#7), not a NEW seat-specific work item — it lives in the shared
engine ledger and does not depend on deck44 staying in the active pool. (4) The hybrid-annotation
measurement charge is DISCHARGED (F4: clean, positive). Both rotation conditions are met
(no-guide-mod AND no NEW seat-only work signal). deck44 is a proven, frozen, well-understood seat;
keeping the FROZEN guide live while rotating the seat out of active re-review is the correct move.

## One-line handoff
Corrected read: combat asks PRESENT (30 attacker + 14 blocker) — wave-15's "removed" finding refuted;
guide FROZEN 12th wave, 4W-2L. Losses are construction/variance (vs110 reactive no-clock; vs62 hard
BLACK color-screw with Obyra/GFTT/Bladecrafter dead in hand). Hybrid-pip annotation clean at this seat
(6/6 correct, one black-screw rescue). One genuine misplay — vs135 s21 self-GFTT into own Faerie
Miscreant despite the friendly-only annotation (rules confusion, won game) — reaffirms engine ledger
#7 (own-target-only removal suppression); NOT a guide fix. Rotate the seat out; keep the frozen guide.
