# Deck 44 (UB Faerie Tempo) — Wave 16 review (CORRECTED READ)

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-084532/` (21-game round-robin over
decks 44,110,133,135,17,21,62). Binary `/tmp/wagic-69793930a` (free zero-mana alt-casts +
stale_echo option-subset fallback + hybrid-pip affordability annotation). deck44 = **deck0 in all
6 games** (`results.tsv` lists 44 first every row; `my_deck=ai_baka_deck44`).
**Record 4W-2L.** 118 ask decisions + 30 attacker + 14 blocker asks; 7 fallbacks; name-echo 116/116.
Win column is context only (doctrine).

game -> result (`results.tsv`, deck0=44):
- `1784209534` vs21 **W** (kill 4/-4 T12) . `1784215280` vs135 **W** (kill 17/-4 T16)
- `1784218657` vs133 **W** (kill 18/-4 T12) . `1784218523` vs17 **W** (kill 6/-4 T20)
- `1784215445` vs62 **L-adj** (2/9 T18) . `1784217277` vs110 **L** (kill -10/25 T11)
Citations: `vsNNN sN`.

## POSTURE / DISPOSITION

**Guide: FREEZE (byte-identical, md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines, 12084 B —
unchanged since wave-13; live == wave-15).** The wave-15 reviewer's central finding ("combat
asks removed corpus-wide") was **REJECTED** by synthesis (orchestrator-verified per-seat counts).
This corrected read CONFIRMS the rejection: **deck44 has 30 attacker asks + 14 blocker asks this
corpus** (the LLM declares combat normally). The wave-15 loss-attribution built on the false
premise ("all-in auto-attacker left no crackback blocker") is **not reproducible and not adopted.**

The record is 4/6, up from wave-15's 1/6 — driven by roster (vs131-style guaranteed wins replaced,
better matchup spread) plus clean pilot execution, NOT any guide change. The two losses decompose
into **construction/draw variance** (vs110 reactive-heavy no-clock draw; vs62 hard BLACK color-screw)
— neither is a pilot-decision or guide regression. The pilot's reachable decisions (mulligan, land,
casts, counters, removal, combat declarations) were high-quality. ONE genuine misplay recurred
(vs135 s21 self-GFTT) but it is an **engine/representation** item the guide already addresses in
text and the model disobeyed — the durable fix is structural suppression, not more prose.

---

## F1 (CORRECTED READ — the headline) Combat asks are PRESENT; the wave-15 "removed" premise is false

Phase/kind enumeration of the deck44 seat this corpus:
- **attacker-declaration asks (`kind=attackers`): 30** (vs21:6, vs135:6, vs62:5, vs110:0, vs17:9, vs133:4).
- **blocker-declaration asks (`kind=blockers`): 14** (vs21:2, vs135:1, vs62:4, vs110:1, vs17:5, vs133:1).
- Phases seen include `Attackers` and `Blockers` at every seat that had creatures.

The LLM declares attackers and blockers normally. Concrete combat decisions the pilot made and OWNED:
- vs21 s30: `Oona's Gatewarden blocks Goblin King` — correct wither block (defender into a real body).
- vs62 s16: `Oona's Gatewarden blocks Canopy Spider` — correct wither block.
- vs135 s25: `Shadow Puppeteers blocks Ohran Viper; Faerie rogue blocks Icehide Golem`.
- vs17 s22: attacks `Sleep-Cursed Faerie, Obyra` — held a clean flying clock across the game.
- vs133 s20: attacks `Archmage x3` for the kill.

**The wave-15 F2/notes-#1 "combat auto-resolved, guide's block/lethal prose inert" narrative is
FALSE and the block-prose-trim method built on it is void.** The guide's BLOCKING and LETHAL-CHECK
sections are LIVE and executed. No corpus-wide block-prose trim is warranted from this seat.

---

## F2 (HIGH — ENGINE/REPRESENTATION, the one genuine misplay) self-GFTT recurred despite the friendly-only annotation

**vs135 s21 (T10, opponent's turn):** option `[1] Cast Go for the Throat {1}{b} - the only legal
targets are YOUR OWN right now` / `[2] Cast nothing`. The model chose **c=1 (cast GFTT)** and the
narration in the s22 prompt confirms the result:
```
- Opponent's Ohran Viper: hand -> stack
- Your Go for the Throat: hand -> stack
- Your Faerie Miscreant: battlefield -> graveyard   <-- deck44 destroyed its OWN creature
- Your Go for the Throat: stack -> graveyard
- Opponent's Ohran Viper: stack -> battlefield        <-- and the Viper resolved anyway
```
The model's PLAN reveals a **RULES error**: it believed GFTT could kill Ohran Viper *while the Viper
was on the stack* ("This is a nonartifact creature, making it a valid target"). GFTT targets a
creature already **on the battlefield**; the only battlefield nonartifact creature was deck44's own
Faerie Miscreant. The `only legal targets are YOUR OWN right now` annotation was PRESENT and the
model read it, then cast anyway.

**This contradicts wave-15's F3/synthesis-#7 "self-destroy class CLOSED" claim at this seat.** The
absent-echo/stale_echo route did NOT catch it because the echo NAMED an offered option (GFTT was
option 1, model chose 1 — index and echo agree, no staleness). This is EXACTLY wave-15 synthesis
ledger #7's scenario (a friendly-only removal that punches through instruction), and it recurred.
**Severity: LOW-outcome (deck44 won 17/-4; cost was 1 Faerie Miscreant in a game already won),
HIGH-signal** (the structural hole is real).

**Route: ENGINE/representation ledger, NOT a guide edit.** The guide already covers this exhaustively
(lines 117-120, 164-166) and the model disobeyed — the "present-and-disobeyed" pattern. Durable fix =
suppress/hard-deprioritize a top-level targeted-removal cast whose ONLY legal targets are friendly
(as 601.2c already suppresses no-legal-target spells), making the misplay structurally impossible.
Reaffirms wave-15 synthesis #7. Secondary model-watch: the stack-vs-battlefield targeting confusion.

---

## F3 (HIGH — construction/draw variance, the two losses) neither loss is a pilot or guide fault

**vs110 (L, hard kill -10/25 T11) = reactive-heavy draw with NO clock.** Over 11 turns deck44
deployed only Oona's Gatewarden (defender) + one Surveilling Sprite (1/1) as offense — ~1 power of
real clock — while drawing all interaction: Arcane Denial (s5 Cranial Plating), Arcane Denial (s8
Vault Skirge), Counterspell (s13 2nd Cranial Plating). It countered three affinity cards competently
but had no threats to race; the opponent went to 25 life (Vault Skirge lifelink + Plating) and killed
it. This is the same reactive-heavy loss signature flagged waves 5-15 — a construction/draw property,
not a misplay. (Minor note: the s8 Arcane Denial on a 1/1 Vault Skirge draws them 2 cards; defensible
under the affinity "counter the artifact engine" line since Plating makes Skirge lethal, but it is the
loosest counter of the corpus.)

**vs62 (L-adj 2/9 T18) = hard BLACK color-screw.** deck44 held `Obyra {u}{b}`, `Go for the Throat
{1}{b}`, `Faerie Bladecrafter {2}{b}` — all needing black — while its board was **all Islands**
(vs62 s27: `Mana available: {u}{u}{u}{u}{u} from 5 untapped sources`, zero black). Those three cards
were **dead in hand for the whole game.** The model repeatedly PLANNED to cast Obyra (s27, s28) but
the engine never offered it (uncastable) and the harness correctly refused via stale_echo. deck44's
only clock was Faerie Miscreant + a slowly-woken Sleep-Cursed Faerie; it stalled at 18 life, then got
ground out by trampling Enchantress bodies. **This is the MIRROR of wave-15's construction flag
(that game was BLUE-screw — couldn't pay `{U}{U}` off a Swamp-heavy board).** The deck has a
**two-sided color-consistency problem**: 16 Island / 10 Swamp must support BOTH double-blue counters
(`{U}{U}` Counterspell) AND black creatures/removal (`{u}{b}` Obyra, `{2}{b}` Bladecrafter, `{1}{b}`
GFTT), so either color can be the pinch depending on the draw. Route: construction notes (F6).

---

## F4 (hybrid-pip annotation effect — the brief's measurement charge) reinforces; deck44 was never a hybrid-failure seat

The `[hybrid: each {u/b} pays with U or B - total N mana]` annotation was offered on **7 deck44
decisions**, all on Oona's Gatewarden `{u/b}`:
- vs21 s24, vs62 s5/s28, vs110 s3, vs17 s9, vs133 s19 — **6/6 that reached the model cast the
  Gatewarden correctly.** The 7th (vs62 s27) was a stale_echo (the model was trying to cast Obyra,
  not Gatewarden; harmless — Gatewarden cast one seq later at s28).
- **0 hybrid misreads, 0 wrong declines, 0 hallucinated `{1}{x}` costs** at this seat.

**Before/after:** the wave-15 corpus (binary `aad32bc7f`, NO annotation) offered Gatewarden 4x at
deck44 and cast it 3x — the one non-cast (vs140 s16) chose Shadow Puppeteers, a legitimate
bigger-threat pick, **not a hybrid misread.** So deck44 was NOT one of the hybrid-decline seats the
wave-15 synthesis (#2) found (those were 135/17/21). **Verdict at deck44: the annotation is
CONFIRMED CLEAN and does no harm; its rescue value showed elsewhere.** ONE genuine local win: in the
vs62 black-screw game the hybrid pip let Gatewarden cast off pure Islands (s5, s28) — the only creature
deck44 could deploy those turns — which the model did without hesitation. Net: the annotation removes
the *risk* of a hybrid decline at this seat without changing the already-correct behavior.

---

## F5 (counter discipline) mostly correct; a few context-defensible loose counters

- **Correct/strong:** vs135 s6 Counterspell -> Icehide Golem (artifact 4/4, cat #2, can't GFTT);
  vs135 s11 Arcane Denial -> Ice-Fang Coatl (flash flying deathtouch, threatens the air war);
  vs21 s29 Arcane Denial -> Boartusk Liege (anthem lord, cat #3); vs110 s5/s13 Cranial Plating +
  s8 Vault Skirge (affinity engine, cat #2); vs17 s23 Arcane Denial -> Scion of Oona (opposing
  faerie lord); vs17 s40 Counterspell -> Oona's Gatewarden (mirror).
- **Context-defensible loose:** vs62 s9/s12/s15 spent three counters on Enchantress auras (Fists of
  Ironwood x2, Primal Rage) at ~19 life — none is a sweeper/unanswerable-artifact per the strict
  list, so on paper these are loose; but against Enchantresses each countered enchantment also denies
  their card-advantage trigger, so the calls are reasonable in context. Not flagged as misplays.
- **vs21 s21 Counterspell -> Volcanic Hammer at 6 life:** near-lethal-range burn (6 -> 3 vs an
  aggressive goblin board); consistent with the guide's "counter burn only when in its kill range."

## PILOT DECISION QUALITY — clean (the seat's decisions are not the problem)
- **Mulligans (6):** all reasonable keeps.
- **GFTT discipline:** vs21 s27->s28 hit opp Scuzzback Scrapper; vs17 s15->s16 hit opp Wasp Lancer;
  correctly identified `only YOUR OWN` cases in reasoning at vs62 s18 (declined) — the ONE failure is
  vs135 s21 (F2), a rules-confusion self-target in a won game.
- **Blocking:** designated-blocker discipline good (Gatewarden into Goblin King / Canopy Spider). Two
  minor deviations at high life (vs17 s6 Obyra blocks a 1/1 Cloud Sprite at 20 — Obyra survives,
  removes an evasive body; vs17 s36 chumps two Archmages + a Miscreant at 16/7 against deck17's bigger
  fliers — arguably over-defensive while ahead on the clock). Both LOW severity; game won. See notes.
- **Fallbacks (7):** 4 unparsed_reply (rambles, guard working) + 3 stale_echo, all correct catches;
  0 wrongful downgrades verified (vs62 s27's "dropped Obyra" was an UNCASTABLE black spell on an
  all-Islands board — the refusal was CORRECT). **Name-echo 116/116.**
