# deck140 wave-13b — development notes (engine/harness/model; layer-routed, with repros)

Corpus: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228`, 6 deck140 seat files
`*ai_baka_deck140-0x*-vs-*.jsonl`. Binary `/tmp/wagic-a75e2625e`. Fields: `prompt, reply, choice,
chosen_text, options_text, options, my_life, opp_life, latency_ms, turn, seq, kind, phase, events,
model, fallback`. `choice` 1-based; `choice -1` = heuristic fallback; `CHOICE: 0` = pass. Opponent
creature count = `Opponent battlefield (creatures: N)`; `Mana available: {..} from N untapped sources`.

## ENGINE (C++; route to engine batch, never guide prose)
1. **[HIGH] Rakdos's Return X auto-underpaid; X-menu not surfaced on an intentional cast.** vs109 seq43
   t30, life22/opp9, `Mana available {r}{b}{b}{b}{w}{w}{w}{w}{w}{w}{w}` (11 sources). Model cast Rakdos
   intending "X=9... lethal... win the game". NO X-menu decision was logged (seqs are contiguous 42->43->44;
   contrast vs44 s41 where `[X=8..X=0]` appeared as its own `ask`). seq44 shows `Already in pool:
   {w}{w}{w}{w}{w}{w}` (6 unspent) and events `Damage: 3 dealt by Rakdos's Return` -> engine paid **X=3**,
   tapping 5 of 11 sources. Opp 9->6; game dragged to a T32 adjudication that should have ended T30.
   ACTION: verify the X-choice prompt fires on every X-cast (Rakdos/BSZ) and that auto-tap maximizes X in
   lethal range. REPRO: read s42(fallback)->s43(cast reply)->s44(pool+events).
2. **[brief #5 CONFIRMED] "Mana available" line hides dual-producible colors; contradicts the offered-legal
   set.** vs109 seq9 t8: line `Mana available: {r}{w}{w}{w}{w}` (no black) while the SAME prompt offers
   `Cast Cruel Edict {1}{b} - legal targets right now: the opponent`. Model reasoned "I lack Black mana",
   declined, missed a mandated life-16 C=2 sweep. Black WAS available (s11 t10 casts Cruel Edict from the
   same manabase). The line renders one defaulted tap-plan (duals -> white); the model reasons from the
   line, not the offer. ACTION: generate the affordability annotation from the same engine as the legal-move
   set (or show per-source color options) so "offered" and "Mana available" cannot disagree. REPRO: s9
   prompt (mana line vs options) then s11.
3. **[brief #2 candidate, corroborates deck44 reviewer] Auto-tap stranding at the LEGAL-MOVE layer.** vs133
   seq11 t8: only `Life with Elixir` offered though Plateau(R/W)+Chapel(W/B) were untapped, from which
   Pyroclasm {1}{r} should be castable (Chapel->B pays generic, Plateau->R). The legal-move generator
   appears to use the same defaulted `{w}{w}` plan and excluded it. UNCERTAIN (Pyroclasm may not have been
   in hand — can't confirm from the prompt). Flag only; do not assert. REPRO: s11 prompt.
4. **[REPRESENTATION — WATCH, needs 2nd witness] Forced-discard menu renders like a cast menu.** vs133 seq12
   t9: Liliana of the Veil +1 forced deck140 to discard (loyalty 1->2 across s11->s12); the prompt listed
   hand sweepers as `Black Sun's Zenith [sorcery] [your hand] - "text"` etc. with no discard framing. Model
   read it as a stack response ("let the ability resolve, discard if forced") and emitted CHOICE: 0 (Pass)
   -> unparsed_reply -> heuristic discarded. Fast latency (14.5s), short reply (825 chars) — a parse/label
   issue, not the latency class. Low impact. Candidate: label forced-discard/sacrifice menus explicitly.
5. **[carried, corpus-wide, NOT a deck140 decision] Attackers name->index reconcile arm.** The brief's
   `ATTACK: Hellrider` unparsable is at the deck109 OPPONENT seat seq33 (deck109 attacking with its own
   Hellrider -> choice -1 unparsed_reply -> heuristic). deck140 seat emits ZERO ATTACK:/BLOCKS: lines all
   corpus (no creature ever deployed as an attacker; nothing to block). The attack-phase prompt carries
   `options_text: None` (free-form ATTACK: line, no index list) so a name reply cannot be reconciled — the
   mirror of parseChoice for the attackers list is the real fix. Confirmed needed, but does not touch any
   deck140 decision.

## MODEL / REPLY-PROTOCOL (route to the name-echo reconcile A/B; do NOT touch the guide)
6. **UNLISTED/UNAFFORDABLE-CARD FIXATION recurs under name-echo; name-echo does NOT close it.** The echo
   only helps when the echoed name matches a listed option; when the fixated card is absent, index-wins is
   the only behavior and it is wrong. 3 genuine mis-maps this corpus, all vs44 s40-42 (the "Play Badlands"
   after-land-drop fixation): s40 echo "Play Badlands" -> Cast Rakdos's Return (dumped kill shot); s41 ->
   X=8; s42 -> Cast Wrath into C=0. Plus the under-cast direction at vs109 s9/s10 (declined offered payable
   Cruel Edict, wanted unaffordable Damnation). Model reliably NAMES its intent in prose -> the absent-
   named-action arm is tractable (route to intent-consistent best-available / pass, never the raw index).
   deck140 is now the highest-impact witness (a wasted Rakdos, not a wasted Pyroclasm). REPRO: read the PLAN
   tail + options_text + Mana available for each seq. See general-suggestions #1, skill Proposal 1.
7. **Name-echo compliance 100% (204/204 final CHOICE lines carry "(name)")**; NO echo-vs-index conflict
   resolved wrong (index wins by design everywhere). Benign conflicts (index won, right outcome): vs109 s31
   (Staff-upkeep-trigger menu; echoed "Cast Cruel Edict", index 1 = Staff ping at a player = correct).
   Protocol nits (harmless): vs135 s7 / vs133 s15 BSZ X-menus echoed the spell name instead of "X = N".
   Compliance != protocol success — the damaging cases (item 6) are invisible to a compliance grep.

## HARNESS / INFRA (confirmed; account for, do not re-diagnose)
8. **9 unparsed_reply fallbacks at the deck140 seat; 0 empty_reply.** 8 are long-latency rambles (150-199s,
   KV preemption at GPU_MEM_UTIL=0.75) correctly discarded by the label guard -> heuristic; 1 is the discard
   misread (item 4, fast). NONE cost a game. The only critical-window fallback — vs110 s13 t6 life16 C=1,
   payable Cruel Edict offered — SELF-HEALED (heuristic cast Cruel Edict; s14 events show the Vault Skirge
   dying). No fallback at C>=1 + payable sweeper + life<=16 with a bad heuristic outcome. Latency, not pilot
   quality. REPRO: filter `fallback` and join to results.tsv.
9. **adjudicate-by-life-at-cap confirmed again.** All 6 deck140 games adjudicated. Rows exact on
   vs109(21-5 T32)/vs133(15-14 T14)/vs131(29-31 T34)/vs110(18-14 T24); vs135 and vs44 show the known
   life-snapshot-timing nit (deck140 gained 1 after its last logged decision — 29 vs logged, 16 vs logged
   17). Verdicts unambiguous. Carried from wave-11 INFRA #8.

## DECK-CONSTRUCTION (user owns; flag only, restated not relitigated)
10. **Reach-starvation is the entire loss column (unchanged).** vs131 (control mirror): out-survived at 30
    life but no late reach (Rakdos only early/non-lethal, Staff resolved T34, Blightsteel never affordable)
    -> lost the adjudication by 2. Priorities unchanged from wave 9/10/11: (a) a cheaper counter-resilient
    recurring clock; (b) a 1-2 mana early lifegain/blocker vs aggro; (c) more untapped RED for a reliable
    turn-2/3 Pyroclasm. 14 sweepers is the right density — needs TEETH, not text.
