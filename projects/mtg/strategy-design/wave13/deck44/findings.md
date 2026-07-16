# Deck 44 (UB Faerie Tempo) — Wave 13b review (name-echo era, first CLEAN validation corpus)

Corpus: PRIMARY `matchups-20260715-183228/` (21 games round-robin; binary `/tmp/wagic-a75e2625e` =
Force r2/r3 + d5d7b1f2e auto-tap two-pass + c3e6d95b2 name-echo + 42e8bc180 BLOCKS:none + 240s timeout;
wave-11 guides frozen; core 8th PASS). deck44 = **deck0 in all 6 games** (confirmed via `gamestart`
`my_deck=ai_baka_deck44`). **Record 3W-3L — the SAME matchup split as wave 11** (W vs135 adj0 20/8 T14,
vs131 0 19/-2 T14, vs133 adj0 9/4 T15; L vs109 1 -1/9 T9, vs110 adj1 7/18 T14, vs140 adj1 12/16 T32).
Win column is context only (doctrine). 160 decisions logged, **0 empty replies, 0 fallback records,
0 choice=-1, 0 defers** at this seat (the corpus-wide 49 unparsed / 4 defers are all other seats).

game->file map (via `gamestart` opp_deck + results.tsv):
`1784162895`=vs109 **L** . `1784165345`=vs135 **W(adj)** . `1784166093`=vs110 **L(adj)** .
`1784167378`=vs131 **W** . `1784169213`=vs140 **L(adj)** . `1784171613`=vs133 **W(adj)**.
Citations: `vsNNN sN`.

## POSTURE / DISPOSITION
**Guide: FREEZE (byte-identical, md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines).** The counter/attack/
block spine is intact and obeyed. BUT this corpus is NOT clean at the play level: the newly-shipped
name-echo protocol + specific same-turn multi-cast sequences produced a **NEW decision-level failure
class — a self-inflicted removal (own creature destroyed by own Go for the Throat)** plus a
guide-violating removal waste. Both route to the **reply-protocol reconcile arm + forced-sub-menu
representation**, NOT to guide prose (the guide's own-target rule was obeyed at 20+ OWN-ONLY windows;
the failures happened at NAMED-target sub-menus the guide can't reach and the pilot wasn't reading).
So the guide freezes, but the wave's model-fixable content is NOT purely the standing auto-tap ledger
item this time — F1/F2 below are new, HIGH, and engine/representation-layer.

---

## F1 (HIGH — NEW CLASS) reply-protocol + representation: absent-echo + stale/shifted index → unwanted removal cast → forced target sub-menu → SELF-DESTROY / guide-violating waste

The name-echo reconcile arm's **"index wins on conflict"** default fires an UNWANTED cast when the
trailing `CHOICE:` index encodes a CAST the plan explicitly refused AND the echoed name is ABSENT from
the current options (a stale card or the parent action). When that cast is a targeted removal, a
downstream forced target sub-menu — which offers no "cast nothing" — compounds it. **Two repros, one
catastrophic:**

**Repro A — vs135 s23→s24→s25 (SELF-DESTROYED own Sleep-Cursed Faerie 3/3 flyer):**
- s23 (T8): options `[1] Cast GFTT {1}{b} - legal targets: Sleep-Cursed Faerie, Faerie Bladecrafter,
  Boreal Druid` / `[2] Cast nothing`. Model commits to cast GFTT on the opponent's Boreal Druid
  (borderline — premium removal on a 1/1 mana-dork, mild waste — but a legitimate opponent target).
  `CHOICE: 1`. Correct-ish.
- s24 (T8): the GFTT **target sub-menu** appears — `[1] Sleep-Cursed Faerie [your battlefield]`,
  `[2] Faerie Bladecrafter [your battlefield]`, `[3] Boreal Druid [opponent's battlefield]`. The model
  **re-litigates the whole cast** ("I will NOT cast Go for the Throat... my guide says don't spend
  removal on small creatures... I will pass the turn") and emits `CHOICE: 1 (Cast Go for the Throat
  {1}{b})`. The echo names the PARENT action (absent from the 3 target options); no unique remap →
  **index wins → index 1 = its OWN Sleep-Cursed Faerie.**
- s25 events: `Your Go for the Throat: hand -> stack | Your Sleep-Cursed Faerie: battlefield ->
  graveyard`. **The pilot destroyed its own 3/3 flyer with its own removal.** (Game still won on adj,
  20/8 — outcome-decoupled, but a clear play-quality catastrophe and the exact event the own-target
  annotation exists to prevent.)

**Repro B — vs133 s13→s14→s15 (index-drift after same-turn cast → cast a removal it refused → wasted on Bloodghast):**
- s13 (T8): options `[1] GFTT` / `[2] Cast Surveilling Sprite` / `[3] nothing`. Model casts Sprite,
  `CHOICE: 2`. Correct.
- s14 (T8, Sprite now removed from the list): options `[1] GFTT` / `[2] Cast nothing`. Prose:
  "I will NOT cast Go for the Throat as there is no priority target (Bloodghast low priority, save for
  Obliterator)" — intent = HOLD = option 2. But it emits `CHOICE: 1 (Cast Surveilling Sprite)` — echo
  names Sprite (already cast, ABSENT), and index 1 (which the model thought was "the creature") is now
  GFTT because the list shifted. Echo unmatchable → **index wins → GFTT cast against the model's own
  stated intent.**
- s15: forced target sub-menu; model reasons "the prompt is likely a glitch... I am not casting GFTT...
  but the format requires a CHOICE, I pick the least valuable" → picks Bloodghast (index 4, opponent).
  **Result: GFTT spent on Bloodghast — the guide's explicit anti-pattern ("do NOT spend it on
  Bloodghast... killing it only delays it one turn") AND the model's own refusal — because the reply
  protocol forced the cast.** (Won adj 9/4 — no game cost, clear decision defect.)

**Contrast — the CLEAN case (vs131 s20):** OWN-ONLY GFTT window; model reasons "GFTT has no valid
target, pass," emits `CHOICE: 2 (Cast nothing)`, echo matches index 2 = Cast nothing → correct. When
the model gets the index right, it works. The failure is specifically **index encodes a cast while the
plan wants to hold + echo names something absent**.

**Attribution:** reply-protocol reconcile arm (index-wins-over-**absent** echo is the wrong default —
an echo that matches NO current option is a STALENESS SIGNAL; the safe route is the Cast-nothing option,
not the raw index) + representation (option indices SHIFT after a same-turn cast, so the model's index
intent goes stale; and the forced target sub-menu offers no cancel/framing). Directly re-witnesses
wave-11 skill headline 1 ("reconcile-named-action arm must route absent-named-action to the best-available
LISTED option / Cast-nothing, never an arbitrary listed cast") and adds the name-echo-era twist: **the
100%-compliant echo does NOT rescue the absent-echo case — index-wins overrides it into an unwanted cast.**
Route: reply-protocol ledger (notes #1). NOT guide.

---

## F2 (MEDIUM — representation) forced-choice sub-menus are bare/unframed → model calls them a "glitch," re-litigates, picks poorly

Every forced sub-menu this corpus (spell target, sacrifice target) is a bare creature list with no
statement of the committing spell/effect and no cancel. The model repeatedly treats them as errors:
- vs135 s24: "I will pass the turn" (re-litigates a committed GFTT → self-destroy, F1-A).
- vs133 s15: "The prompt is likely a glitch. There is no active effect requiring... I should not cast."
- vs133 s19 (T11): opponent's **Liliana of the Veil +1 forced sacrifice** — sub-menu lists the pilot's
  4 creatures. Model: "The prompt is likely a glitch... if I must pick, I pick the least valuable —
  Scion of Oona." It sacked **Scion of Oona** (its anthem lord: other Faeries +1/+1 and shroud), when
  **Surveilling Sprite (draws a card on death) is the textbook sacrifice fodder** and Scion's loss
  shrank the whole team and stripped shroud. Suboptimal, mild, and misread as a glitch.

**Attribution:** representation/core — targeted-spell and forced-sacrifice sub-menus need (i) a header
echoing the committing spell/effect ("Choose the target for Go for the Throat you are casting" /
"Liliana of the Veil forces you to sacrifice a creature — this is not optional"), (ii) an own-vs-opponent
danger annotation on target menus ("a `[your battlefield]` target destroys your OWN creature"), and
(iii) opponent targets listed first / a safe default. Overlaps F1 at vs135 s24 & vs133 s15 but is a
DISTINCT layer (F1 = how the wrong option got selected; F2 = why the model was confused and couldn't
escape). Route: engine/representation ledger (notes #2). NOT guide.

---

## F3 (engine ledger #2, STANDING TOP ITEM — STILL UNFIXED) auto-tap greedy-blue-for-generic strands blue; the two-pass fix did NOT reach the live tap path

I own this item. **CONFIRMED: the d5d7b1f2e two-pass did NOT change live behavior** — the engine still
auto-taps COLORED (blue) sources for the GENERIC portion of a cost while off-color (black) sources sit
untapped, exactly as in waves 10-11. This matches the brief's hypothesis (two-pass landed in
`ManaEngine::planPayment`; live path is `selectAutoTapProducers`, unpatched).

**Dual-land caveat RESOLVED:** deck44's land base is **16 Islands + 10 Swamps, ALL basics, NO duals**
(decklist verified: Island ELD/FDN/LRW/WOE ×4 each, Swamp ELD ×2 + LRW/WOE ×4). Islands make only {U},
Swamps only {B}, so pip accounting is unambiguous and the stranding is real, not a dual artifact.

**Cleanest loss-decoupled repro — vs140 s39 (T26):** cast `Archmage of Echoes {4}{u}` (1 blue + 4
generic) from `{u}{u}{u}{u}{b}{b}{b}{b}` (4 Isl + 4 Swamp) → next mana line `{b}{b}{b}` (dU=4, dB=1).
It tapped **all 4 Islands** (1 for {u}, 3 for the {4}) + 1 Swamp — spending 4 blue on a 1-blue cost.
Blue-preserving (1 Island {u} + 4 Swamps {4}) would leave `{u}{u}{u}`. Zero blue retained vs three.
Other same-turn confirmations: vs140 s19 Scion {2}{u} `u4b1→u1b1` (tapped 3 blue, dU=3), vs140 s25
Sprite {1}{u} `u4b4→u2b4` (stranded 1 blue), vs133 s13 Sprite {1}{u} `u3b1→u1b1`.

**Outcome-coupling THIS corpus: ZERO.** All 9 counters the pilot wanted were cast successfully (F4
spine), and every counter DECLINE had adequate blue (e.g. vs135 s14 declined at `{u}{u}` in hand — a
judgment call, not a mana denial). In every stranding event the pilot was either deliberately tapping
out for creatures or retained ≥2 blue or had ≤1 blue to begin with (forced). Late losses (vs140 T32,
vs110) had 8-13 sources where stranding is moot. So: **DEFECT PERSISTS, re-verified, live-and-harmless
this corpus** — same status as wave 11. vs140 s39 is a cleaner regression fixture than any prior
loss-tangled repro. Route: engine ledger #2 (notes). The guide's "LEAVE two blue sources untapped" is
UNEXECUTABLE (the pilot cannot pick which lands tap) — do NOT re-word it; the fix is the tap path.

---

## F4 (LOW — harness/parse ledger; the brief's ATTACK-name→index candidate) mixed index+name declarations: parser takes A-indices, silently drops name-form entries — HARMLESS at deck44 because every dropped name was INELIGIBLE

The attacker parser does a PARTIAL parse of mixed-form lines: it accepts the leading `A#` indices and
silently drops trailing card-NAME entries. Repros (deck44 seat): vs140 s6 `ATTACK: A1, Obyra` , s14
`A1, A2, Faerie Bladecrafter`; vs131 s16 `A1, Faerie Bladecrafter`, s29 `A1,A2,A3, Faerie Bladecrafter
×2`; vs133 s8, s22. **In EVERY case the dropped-by-name creature was NOT in that turn's attacker-slot
list (summoning-sick — just cast that turn), so the parser correctly attacked with all ELIGIBLE indexed
attackers and dropping the names cost NOTHING.** The model habitually over-names creatures it wishes it
could attack with; it does not reliably track summoning sickness / the eligibility list, but the slot
list protects it. This is the brief's `ATTACK: <name>` shape (mirrors the CHOICE index-vs-echo problem
on the attacker side); confirmed **latent, 0 cost** at deck44. Latent risk: if the model ever names an
ELIGIBLE attacker (present as an A-index) by name instead of index, the partial parse would drop a valid
attacker. A name→index reconcile arm for attackers must ALSO respect eligibility. Route: harness/parse
ledger (notes #3). NOT guide.

---

## VERIFICATION OF SHIPPED FIXES (this seat)

- **Name-echo compliance: 152/152 CHOICE lines carry `(name)` = 100%.** But the reconcile arm's
  index-wins-over-**absent**-echo default is the mechanism behind F1's two bad casts — compliance is
  perfect, the reconcile POLICY has the gap. No echo-vs-index conflict was REMAPPED WRONG (the one
  genuine out-of-range remap opportunity, vs131 s20, resolved to the correct `Cast nothing` because the
  model's FINAL CHOICE line was already correct — the harness took the last line). choice-field vs
  chosen_text: 160/160 consistent.
- **BLOCKS:none (42e8bc180): VERIFIED CLEAN.** 3 bare `BLOCKS: none`/`None` firings (vs109 s10, vs110
  s7, vs133 s20) all parsed to choice=0 (no blockers) and all THREE reasonings intended no-block
  ("first strike, don't trade", "don't trade my 2/2 for their 1/1", "take the 5 damage"). **0 misparses
  of intended blocks.** The wave-11 bare-none→unparsed bug is closed at this seat.
- **Force r2/r3: N/A at deck44** (deck44 has no Force of Negation). The vs135 t8 GFTT-on-stack Force
  offer the brief cites is the deck135 SEAT's decision, not deck44's.
- **240s timeout: 0 empty replies, 0 fallbacks, 0 choice=-1.** Latency displacement invisible at this
  seat; the truncation-scavenge class stays closed (0 scavenged casts).
- **Defers: 0 at deck44 seat.**

## COUNTER SPINE — STEADY (9 counters cast + 7 declines; no new counter class)
CAST (all correct/defensible): vs110 s9 Cranial Plating (affinity equip, guide-named, cat #2 — best
counter of the corpus), vs140 s17 Staff of Nin (unanswerable artifact ping/draw engine, cat #2/3),
vs140 s30 Lightmine Field (guide-named hoser), vs131 s8 Young Pyromancer (grower), vs135 s26 Ice-Fang
Coatl + s30 Ohran Viper (deathtouch grower-flyers, cat #3), vs133 s11 Geralf's Messenger (undying
draining value creature), vs140 s11 Cruel Edict (would sac the sole creature), vs109 s21 Stromkirk Noble
(grower at L10, cat #4). Two MILD: vs133 s9 Counterspell→Fatal Push (spent a hard counter on 1-mana
removal to save the sole creature; both counters gone by T7, leaving Liliana T12 unanswered — defensible
in the moment, mild misallocation in hindsight); vs135 s14/s26 declined the FIRST Ice-Fang then
countered the SECOND (backwards — the first deathtouch flyer-blocker is the better counter; low stakes,
won). DECLINES (correct let-resolves): vs109 s6 Krenko's Command, vs109 s14/s15 Gore-House Chainwalker
(L14>12), vs135 s29 Search for Azcanta, vs110 s4 Vault Skirge (1/1, L20). No sweeper (Wrath/Pyroclasm/
Damnation) hit the stack — cat #1 UNTESTED. Spine holds; no revision trigger.

## MINOR block-adherence blemish (no class)
vs131 s17 (T9, L19): **Faerie Bladecrafter blocked a 1/1 Elemental** — a soft violation of the guide's
"9+ life = only Oona's Gatewarden blocks; every other flyer stays on offense." Model reasoned "favorable
trade, no damage" (mechanically true) but the guide's doctrine is take-the-1 and keep swinging. Cost
~one turn of a flyer's clock; won 19/-2. Single instance, no game cost. Log only.

## THE THREE LOSSES — construction/variance (identical matchups to wave 11)
vs109 (L -1/9 T9): creature-light reactive draw out-raced by a wide mono-red swarm; countered Stromkirk,
attacked every turn, no misplay of note. vs110 (adj-L 7/18 T14): the recurring affinity race lost on
clock — the ONLY recurring attacker was small; no artifact ENGINE reached the stack to counter (Cranial
Plating WAS countered s9). vs140 (adj-L 12/16 T32): long grind, behind on life adjudication by 4; the
auto-tap defect did NOT bite (10-13 late sources). Same rotation as waves 5-11 — a variance signature on
the slow reactive half of the deck, not a guide regression. Note: F1/F2 fired in WON games (vs135, vs133),
so the new self-destroy class did not cost a game this corpus — but it is play-quality-real and severity-HIGH.
