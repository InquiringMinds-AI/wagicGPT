# deck17 (UB Faeries) — REVISED-GUIDE VALIDATION findings (wave-17)

Corpus: `matchups-20260716-151559`, binary `/tmp/wagic-7cdcf9c73`. deck17 played all 6 games
under its **wave-16 REVISED guide** (walls-aren't-flyers root fix + race branch + steal-early
sharpen + trust-the-menu mana line + dead-section trims). **Record 5/6, up from 4/6.**

Acting-seat files (`ai_baka_deck17-*`):
- vs110 (affinity, Vault Skirge lifelink): **WON**, opp -8, t13. *This was a wave-16 LOSS.*
  Raced + stole Ornithopter t7 (s13), swung with attackers not walls, killed 20->3->dead.
- vs135 (snow midrange): **WON**, opp -3, t9. Cloud Sprite swarm, fast air kill.
- vs21 (Goblin/burn aggro): **LOST**, me -1, t11. *This was a wave-16 WIN.* Draw/matchup loss
  (see below) — decisions were sound, the race branch fired.
- vs49 (Dragons): **WON**, opp -2, t10. Stole Dragonmaster Outcast (the bomb) t8 (s14/s15).
- vs35 (Slivers): **WON**, opp -1, t10. Close race, Plumeveil wall-block at t9 (s19) bought the win.
- vs62 (Enchantresses): **WON**, opp 0, t10. Wide Wasp Lancer board, clean second-cast mana (s15/s16).

The 5 wins are the revised guide working. Critically, **the two wave-16-loss shapes both
reversed at the decision level**: vs110 (the lifelink matchup deck17 lost last wave) flipped to a
WIN via the new race branch + an on-curve steal, and NO over-walling occurred anywhere this corpus.

## STEP 0-ter — TAUGHT-CLASS DIED / PERSISTED / MUTATED

| # | Taught class | Verdict | Evidence (seq repros) |
|---|---|---|---|
| 1 | Hybrid mana = 1 pip each, all off Islands (rule #1) | **DIED** | 0/35 hybrid prompts produced an off-color misread ("need a Swamp/black/white"). 13 hybrid casts all at the correct annotated total (Wasp Lancer=3, Liege=4, Plumeveil=3, Gatewarden=1). |
| 2 | Stale within-turn mana / trust-the-menu (wave-16 MUTATION target) | **DIED / SHRANK** | New trust-line worked. vs62 s15->s16 (2nd same-turn cast Wasp Lancer then Briarberry): clean, NO recount, NO "why isn't X listed?" spiral. vs21 s15: one mild recount but **self-corrected** by reconciling the mana line with the board and casting right — none of wave-16's 6000-char cast-list-distrust spirals recurred. |
| 3 | Defenders never declared as attackers | **DIED (clean)** | No wall (Gatewarden/Plumeveil) ever declared an attacker. vs110 s24 prose over-claimed Gatewarden as a 5-dmg attacker, but the actual declaration excluded it — the class holds where it counts. |
| 4 | Walls-aren't-flyers at DEPLOYMENT (wave-16 NEW root fix — cause of both wave-16 losses) | **DIED as taught** | Every wall-vs-attacker development menu chose the attacker: vs135 s9 (Scion over Plumeveil), vs49 s7 (Wasp Lancer over Plumeveil), vs49 s10 (Scion over Plumeveil), vs35 s6 (Scion over Plumeveil). Plumeveil/Gatewarden cast ONLY as a needed blocker (vs35 s18->block s19; vs110 s4 wall-or-nothing). Zero over-walling. |
| 5 | Attack floor: swing full board every turn | **FIRED / PERSISTS** | Full-board attack declarations every combat in all wins (vs110 s19/s24/s27, vs135 s10/s12, vs62 s12/s17/s19). No under-attack-by-choice. |
| 6 | Steal early, don't hoard (Class 4 — wave-16 PRESENT-AND-DISOBEYED) | **PERSISTS + now OBEYED** | The wave-16 hoard-the-steal failure did NOT recur. vs49 s14/s15: Control Magic on **Dragonmaster Outcast** (3/3 haste bomb) the first turn it was legal (t8), over its own Scion. vs110 s13: Control Magic on Ornithopter t7 (only worth-taking opp target). Steals cast on curve, on the right target. |
| 7 | Race branch (wave-16 NEW) — when opp flies/gains life, race + steal early, don't wall | **FIRED** | vs110 (the wave-16 lifelink LOSS) reversed to a WIN: model explicitly raced, stole the flyer, swung attackers. Close races vs35/vs62 also won by keeping the clock up. |
| 8 | Cloud Sprite block restriction (wave-16 "died quiet") | **MUTATED** | Resurfaced as an unparsed RAMBLE, not a wrong choice. vs21 s10 (blockers): model re-derives "does Scion share Cloud Sprite's flyers-only restriction?" correctly but burns the whole token budget -> unparsed -> heuristic. Choice-level dead; ramble-level alive. No game impact (no-block was correct). |

## The single loss (vs21) is a DRAW/MATCHUP loss, not a taught-class regression
Opening hand: 3 Islands + 3x Scion of Oona + 1 Oona's Gatewarden vs a fast Goblin/burn deck
(Spark Elemental, Raging Goblin, Bloodmark Mentor, Boggart Ram-Gang, Volcanic Hammer, first
strike + haste). This is a land-light, redundant hand whose only sub-3-mana play is a single
wall — it cannot cast its first Scion until turn 3 and offers three copies of the same 1/1 lord.
The individual decisions were sound: it correctly DECLINED the wall on t0 when no blocker was
needed (s3/s4 — walls-aren't-flyers + race reasoning cited verbatim), cast Gatewarden t2 to block
(s6), and raced with Scions once the aggro clock was on (s9/s11/s14). deck21 neither flies nor
gains life, so the guide's race branch does not engage; the model correctly treated its flyers as
unblockable and raced — but a three-1/1-lord draw simply cannot out-race goblins-plus-burn from
behind. Per doctrine (single-seat, single-game, draw-driven), this does NOT justify a core-line
change. It is logged as a mulligan-heuristic observation in general-suggestions.md.

## Fallbacks at this seat (3 total, 0 game-flipping)
- vs135 s13 (unparsed): long-ramble at an ALREADY-LETHAL window — model correctly computes it has
  lethal on board (7 dmg vs 6 life) then over-recalculates and overflows. Heuristic caught; won
  that turn attacking. Model-incapacity/long-ramble class, not guide.
- vs21 s10 (unparsed): the Class-8 Cloud-Sprite block-restriction ramble (above).
- vs21 s15: not a fallback (self-corrected recount, cast Gatewarden). Logged under Class 2.

## Charge verdicts
- **Did the revised rules FIRE?** YES, decisively. Walls-aren't-flyers (Class 4) and the race
  branch (Class 7) both fired at the decision level, and together they reversed the exact matchup
  (vs110 lifelink) that lost under the first guide. The steal-early sharpen (Class 6) converted
  wave-16's disobedience into on-curve bomb-steals.
- **Hybrid annotation effect (35 exposures — the corpus's heaviest):** misreads/declines
  collapsed to ZERO. Wave-16 already had 0 off-color misreads at 4/6; wave-17 confirms it at the
  highest exposure in the corpus (35 of 44). Before: wave-15 signature leaks ("{u/b} needs a
  Swamp", `{u/b}{u/b}{u/b}` = 6 mana, "Scion over Wasp Lancer" under-deploy). After: 13/13 hybrid
  casts at the correct total, 0 declines from misread, 0 off-color beliefs. The annotation +
  guide rule #1 are mutually reinforcing and fully load-bearing.
- **Guide verdict:** FROZEN (byte-identical). The revision validated across the board; every
  remaining residual is corpus-wide model/representation (long-ramble, block-restriction ramble),
  routed to notes, and per doctrine single-seat items cannot add/cut core lines.
