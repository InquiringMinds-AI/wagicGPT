# Deck 44 (UB Faerie Tempo) — Wave 15 review

Corpus: PRIMARY `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260716-040138/` (21-game round-robin over
decks 44,110,133,135,140,17,21). Binary `/tmp/wagic-aad32bc7f` (target-cancel use-after-free fix +
absent-echo staleness route + attackers-name reconcile + Galvanic metalcraft fix; all wave-14 validated).
deck44 = **deck0 in all 6 games** (`results.tsv` lists 44 first every row; `gamestart my_deck=ai_baka_deck44`).
**Record 1W-5L.** 126 decisions logged, 3 fallbacks (all correct guard), name-echo 125/126.
Win column is context only (doctrine).

game -> result (`results.tsv`, deck0=44):
- `1784195382` vs110 **L** (kill, -2/1 T11) . `1784197337` vs21 **L** (kill, -2/1 T11)
- `1784198247` vs133 **L** (kill, -4/7 T15) . `1784200032` vs17 **L** (kill, -4/9 T21)
- `1784202539` vs140 **L-adj** (15/19 T22) . `1784204084` vs135 **W** (20/0 T14)
Citations: `vsNNN sN`.

## POSTURE / DISPOSITION

**Guide: FREEZE (byte-identical, md5 `bd097b0c4cf8524fb6cc30b75312cabe`, 173 lines; live == wave-13, diff empty).**
The 1/6 drop is **NOT a guide regression and NOT a pilot-decision regression.** It decomposes cleanly into
roster variance + draw variance + a persistent construction/matchup signature + a **confirmed harness change
(combat asks removed) that made the guide's block/lethal apparatus inert and handed the losing races to an
all-in auto-attacker.** The pilot's reachable decisions (mulligan, land, casts, counters, removal targets)
were high-quality all corpus. The two wave-13 open items I owned both RESOLVED in deck44's favor this cycle:
the reconcile-arm self-destroy class is **CLOSED** by the wave-14 absent-echo route (verified below), and the
forced-sub-menu representation item is **downgraded to cosmetic** (4/4 target sub-menus resolved correctly,
0 self-destroys). The dominant NEW signal is engine/harness, not guide.

---

## F1 (HEADLINE — the 1/6 investigation) The drop is variance + matchup + a harness change; NOT a regression

**Same-opponent comparison (the 4 decks in both wave-13 and wave-15 corpora):**
- vs135: W -> **W** (consistent — deck44 ran away at 20 life; clean win)
- vs110: L -> **L** (consistent — the recurring affinity clock race)
- vs140: L-adj -> **L-adj** (consistent — the control/Wrath grind, life-adjudicated)
- vs133: W -> **L** (the ONE flip)

**Roster change:** deck131 (was a reliable **W**) and deck109 (was an **L**) rotated OUT; deck17 and deck21
entered GUIDELESS and both **beat** deck44. So the 3/6 -> 1/6 move = lost the guaranteed vs131 win to
rotation + the vs133 flip + two genuinely hard new matchups. No same-opponent result *worsened* except vs133.

**vs133 flip = DRAW VARIANCE, not misplay.** Wave-13's vs133 win came from a hand with counters (it countered
Geralf's Messenger). This corpus deck44 kept a **counter-light** hand (3 Islands, Faerie Miscreant, 2 GFTT —
zero Counterspell/Arcane Denial, s1) and was out-raced by **double Gray Merchant of Asphodel** drain (a
life-swing engine GFTT can only partly answer and counters can't reach without being in hand). The pilot
played its hand correctly (held GFTT off Bloodghast per guide, kept its flying clock). Different opening,
no interaction for the drain — textbook variance, not a guide or decision fault.

**vs17 (Faeries mirror) and vs21 (Goblin swarm) = matchup + the auto-combat item (F2).** In both, deck44 was
AHEAD on the pilot-controlled axis (vs17: countered 2x Glen Elendra Liege + Control Magic, GFTT'd the 5/4
Wasp Lancer; vs21: developed a clean flyer clock, correctly declined GFTT on 1/1 goblins) yet **lost the
race**. vs17 lost the AIR WAR to strictly bigger flyers (Wasp Lancer 5/4, Glen Elendra Lieges as anthem
lords, Sentinels 4/5) — a structural size disadvantage, not a misplay. vs21 and vs110 lost the CRACKBACK
(see F2): deck44 got the opponent to exactly **1 life** and then died at 9 -> -2 on the return swing.

**Adjudication is NOT inflating the loss count.** Only vs140 is adjudicated; the other **4 losses are hard
board kills** (life went negative). deck44 legitimately died on board in 4/5 losses. The vs140 adj is a real
"behind on life in a control grind" (deck44 countered Wrath but was ground by Pyroclasm/Damnation/Staff of
Nin and one missed Pyroclasm counter — see F4), not a latency artifact. **Verdict: no regression; variance +
matchup + a harness-driven engine item.**

---

## F2 (HIGH — ENGINE/HARNESS, NEW, deck-agnostic) Combat asks were REMOVED; the LLM no longer declares attackers or blockers

**Confirmed harness change between corpora.** deck44's ask records this corpus are **only** Main phase 1 (110)
and Main phase 2 (16) — phases across the ENTIRE corpus are `{Main phase 1, Main phase 2, Upkeep, Combat
damage}`; there are **0 "Blockers"-phase asks and 0 attacker/blocker-declaration asks at any seat**. The
wave-13 corpus (`matchups-20260715-183228`) deck44 seat had a **`Blockers` phase (3 asks)** plus in-main
`ATTACK:`-format attacker declarations. So combat is now **fully auto-resolved by the engine**; the pilot's
only combat-adjacent choice left is a removal **target** sub-menu.

**This is the mechanism behind the vs21 and vs110 race-losses (identical shape):**
- vs110 s26 (T10 MP2): opp at **1**, deck44 at **9** with two 5/5 Bladecrafters + Obyra. -> T11 opp untaps,
  swings, deck44 **9 -> -2**, dead with opp at 1.
- vs21 s22 (T10 MP2): opp at **1**, deck44 at **9** with 7 power of flyers. -> T11 goblin crackback, deck44
  **9 -> -2**, dead with opp at 1.
- In BOTH, deck44 had **no untapped blocker** on the crackback because the **auto-attacker declared ALL
  creatures** every combat (tapping them), and the auto-blocker then had nothing. There was **no ask** at
  which the pilot could hold a flyer back or chump — so the guide's entire BLOCKING section
  ("only Gatewarden blocks", "chump the biggest at 8 or less", wither/first-strike targeting) and its
  LETHAL-CHECK / attack-selection prose are **INERT**: ~40% of the 173-line guide addresses decisions the
  pilot is no longer offered.

**Why this matters for the guide (but is NOT a single-seat guide edit):** the "8-or-less chump to stay out of
lethal range" survival line — the guide's own answer to exactly the vs21/vs110 death — is unexecutable when
combat is auto. The block-prose trim would be a **corpus-wide core/method change** (deck-agnostic, every seat
affected), not a deck44 edit, and must not be made on a single seat. Route: **engine/harness ledger + synthesis.**
Open question for the owner: **is auto-combat intended?** If yes, guides corpus-wide should eventually shed
their block/attack-selection sections (they can only mislead reasoning now); if no (harness regression),
restoring combat asks re-arms deck44's existing chump line. Either way this is the cycle's dominant signal.

---

## F3 (VERIFICATION — wave-13 F1 self-destroy class is CLOSED) absent-echo staleness route works at this seat

The wave-13 catastrophe (reconcile arm's "index wins over ABSENT echo" default self-destroyed deck44's own
Sleep-Cursed Faerie via its own GFTT, vs135 s24) is **fixed and verified**. Two stale_echo fallbacks fired
this corpus and **both correctly refused the index-1 cast** instead of self-targeting:
- **vs140 s22 (T17):** options `[1] GFTT (own-only) / [2] nothing`; pilot echoed `CHOICE: 1 (Cast Counterspell)`
  (wanted to counter Pyroclasm) — Counterspell not offered -> **absent echo -> stale_echo -> choice=-1**, did
  NOT cast the own-only GFTT. Correct catch (a wave-13-style self-destroy averted).
- **vs135 s48 (T14):** options `[1] GFTT (targets incl. opp Treefolk) / [2] nothing`; pilot echoed the prior
  land decision `CHOICE: 1 (Play Island)` -> **absent echo -> stale_echo -> choice=-1**. Heuristic then cast
  GFTT and the pilot picked the OPPONENT's Treefolk at the s49 sub-menu (harmless waste; game already won).
- Third fallback: **vs133 s22 (T14)** `unparsed_reply` on a land drop (reply rambled, no CHOICE line) — the
  "reasoning rambles, guard working" class; harmless (deck44 at 2 life, dying regardless).

**Self-destroy signature grep: 0 hits.** Every GFTT that resolved hit an OPPONENT creature (vs17 Scion +
Wasp Lancer, vs135 Ohran Viper + Treefolk). **Target sub-menus: 4/4 resolved to `[opponent's battlefield]`.**
The wave-13 F1 (reconcile self-destroy) and F2 (sub-menu "glitch" self-harm) failure modes did **not recur**.

---

## F4 (forced-sub-menu representation item — DOWNGRADED to cosmetic) target menus handled correctly

The item I own from wave-13 (bare, unframed forced sub-menus read as "glitches"). This corpus had **4 removal
target sub-menus** (vs17 s10, s18; vs135 s24, s49) and the pilot picked the correct `[opponent's battlefield]`
creature every time, reading the side annotations without re-litigating into self-harm. No forced-sacrifice
sub-menu appeared (no Liliana this corpus). The remaining representation gap — no committing-spell header, no
cancel — is now **cosmetic at this seat**: it produced exactly one harmless artifact (vs135 s48->s50, the
stale_echo downgrade let the heuristic cast a GFTT that killed a 1-life-opponent's Treefolk, zero cost).
**Severity: LOW/log-only.** The deck-agnostic representation improvement (header + own-vs-opp annotation)
still stands as a cross-deck nicety for synthesis, but it is NOT load-bearing for deck44 anymore. Route:
representation ledger, log-only.

---

## F5 (construction, persistent — for the decklist owner) blue-source density vs the blue spell load

vs140 s22: deck44 **wanted to counter Pyroclasm** but had only `{b}{b}{b}{b}` untapped (4 Swamps, 0 Islands)
and could not pay Counterspell `{U}{U}`; it lost its only creature to the sweeper — a contributing factor in
the adj loss. This is **NOT the retired auto-tap misdiagnosis**: deck44 legitimately spent its blue on its own
T16 creatures (Oona's Gatewarden `{u/b}` + Surveilling Sprite `{1}{u}`) and its board was Swamp-heavy, so no
Island remained. Same construction tension the wave-13 notes flagged: 16 Island / 10 Swamp with a **double-blue
Counterspell + `{1}{u}` Arcane Denial** load means blue is the pinch color exactly when a sweeper appears.
Sharper lever than "trim counters" (the counters earn their slots): a touch more blue-source density, or a
cheap black-castable interaction, to keep `{U}{U}` available on the opponent's turn. Route: construction notes.

---

## PILOT DECISION QUALITY — clean (the seat's decisions are not the problem)
- **Mulligans (6):** all reasonable keeps (2-4 lands + a castable creature or removal each time).
- **Counter spine (all correct/defensible):** vs110 s9 + s22 Cranial Plating (affinity equip, cat #2),
  vs140 s14 Wrath of God (sweeper, cat #1 — first sweeper counter TESTED and CORRECT) + s25 Staff of Nin
  (unanswerable artifact, cat #2), vs135 s14 Ohran Viper + s42 Abominable Treefolk (growers, cat #3),
  vs17 s21 Control Magic + s28 2nd Glen Elendra Liege. Declines correct: Arcum's Astrolabe (mana rock),
  Into the North (land search), cheap goblins.
- **GFTT discipline:** held off Bloodghast (vs133), fired on the right growers (opp Scion, Wasp Lancer, Ohran
  Viper, Treefolk); 0 self-targets.
- **Fallbacks:** 3, all correct guard (F3). **Name-echo:** 125/126 (the one miss = the s22 unparsed ramble).
- Note **cat #1 (sweeper) counter is now TESTED and CORRECT** — vs140 s14 Wrath (guide gap from wave-13 closed).
