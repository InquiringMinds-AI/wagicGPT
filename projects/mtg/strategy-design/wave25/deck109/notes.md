# Dev notes — deck109 wave-25 (engine/harness ledger with repros) + ROTATION VERDICT

## Record: 5/6 (context only) — beat 131/27/137/133/102, lost 59. The one loss = the persist/wither
grind ceiling (WB deck59, Safehold Elite + Rendclaw Trow), no misplay, out of burn against a walled
ground. 0 fallbacks at seat.

## Corpus / provenance
- Run `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260725-035605` (PRIMARY). Binary
  `/tmp/wagic-62d8783a5` (rules-fix batch f0500ad66 + step-1 batch 62d8783a5), -T 3000, REPPENALTY=1.05.
- deck109 acting-seat files (opp): 0x55ae6c74b200 (131), 0x558a8acc6600 (27), 0x5562224db0d0 (137),
  0x56055ee20090 (59), 0x55a3a5626190 (133), 0x55693828fae0 (102).

## TWO NEW HARNESS ITEMS TO OPEN (both vs131, both OUTCOME-NEUTRAL — won 8/0 T8 — both blocked a
## guide-mandated on-sight kill; cross-cutting, not deck109-specific).

### LEDGER ITEM 1 (RENDERING): truncated burn-target PREVIEW hides on-sight-kill engines.
The cast-option line's inline target preview truncates and mis-prioritizes. Observed shape:
`Cast Searing Spear {1}{r} - legal targets right now: Stromkirk Noble, Goblin, Goblin, you (+2 more)`
— it lists the caster's OWN creatures first (never valid RDW burn targets) and truncates the
opponent's Young Pyromancer (the guide's named on-sight-kill target) into "(+2 more)".
- **REPRO vs131 s11** (opponent's turn, Guttersnipe on stack, Young Pyromancer on board): the model
  reasoned verbatim *"the option list explicitly excludes Pyromancer from the target list for Searing
  Spear. Therefore, I cannot target Pyromancer with the current option... I will pass."* It DECLINED
  the mandated kill because it treated the truncated preview as the complete legal set. Recorded:
  Cast nothing / pass.
- **REPRO vs131 s8** (main phase 1, same board): preview `...you (+2 more)` again; the model never
  considered Pyromancer a burn target and passed to combat.
- Effect: Young Pyromancer + Guttersnipe each pinged the model for 2 (events confirm 4 damage the
  model explicitly wanted to prevent). Won anyway from a healthy total.
- FIX DIRECTION (harness option-rendering, `AIPlayerGPT` option assembly / the "legal targets right
  now" preview): when truncating, PRIORITIZE opponent permanents over the caster's own; or don't
  truncate the preview (the downstream chooseTarget window already shows the full list); or annotate
  "(+N more, includes opponent permanents)". The current order (own creatures first, opponent's
  engine truncated away) is exactly backwards for burn-target decision quality.

### LEDGER ITEM 2 (PARSER, b2 edge): prose-only answer reversal recorded the wrong action.
- **REPRO vs131 s10** (main phase 2, Young Pyromancer on board, mana {r}{r}{r}): reply opens
  `CHOICE: 3 (Cast nothing right now)`, reasons ~5.7k chars, concludes *"So I will cast Searing Spear
  at Young Pyromancer... So Choice 1"* with a PLAN naming Searing Spear -> Young Pyromancer. It never
  re-emits a coded `CHOICE: 1` line. The parser took the only coded line (`CHOICE: 3`) and recorded
  Cast nothing — the OPPOSITE of the model's actual final decision.
- **b2 is NOT buggy.** Counter-repro vs59 s13: the model re-emitted coded lines (`CHOICE:` = [2,1,1])
  and b2 correctly recorded 1 (the last coded line). b2's "last coded line" precedence works whenever
  the reversal is re-CODED; it is blind only to a prose-only reversal.
- ROUTING: the parser side (should a naturally-terminated reply whose PLAN contradicts its sole coded
  line be re-examined?) is a HARNESS question logged here; the cleaner fix is a core-prompt protocol
  clause ("a reversed answer MUST be re-emitted as a new coded CHOICE line") — routed to
  general-suggestions.md as a single-instance convergence candidate (do NOT adopt on one instance).

## VALIDATED SURFACES — held clean, nothing to open:
- **Pillar cost line (guide, deployed lines 37-39) — 2-CORPORA CLOCK CLOSED.** 4 exact {R}=1-red
  trigger windows (vs131 s3, vs137 s3, vs133 s3, vs102 s3), all costed Pillar correctly as {R}=1;
  0 conflations corpus-wide. NOT an engine item (option line always rendered `{r}` correctly) —
  confirmed for the second corpus. Do NOT open an engine ticket for Pillar cost.
- **a4 pw-redirect removal — CLEAN at seat AND corpus-wide.** `grep -il redirect *.jsonl` = 0 files.
  All 11 burn-target windows offered "The opponent (player, life N)" directly; no redirect menu.
  No behavioral shift in burn lines (deck109 never faces a planeswalker; a4 confirmed absent+harmless).
- **a1/a2 turn-1 upkeep — CLEAN.** No turn-1 upkeep action attempts. The 6 "Untap"-phase records are
  the pregame mulligan (cosmetic label). Only 2 real mid-game Upkeep priority windows (vs59 s26,
  vs102 s11 = Boros first-strike), both correctly passed. No spurious untap/cleanup priority (a2 OK).
- **Attack-hijack class (fix (d)) — STAYS CLOSED (25/25).** First coded ATTACK line authoritative;
  0 eligible dropped; 7 over-declarations all named INELIGIBLE tokens, correctly dropped. No prose
  "Attack: Deal N" hijack line appeared. No deck109 attention needed.
- **Decode-collapse retry — fired 0x, correct.** Longest reply 6.1k chars (vs131 s10), coherent long
  spiral, not decode garbage. 0 fallbacks; answer-first 133/133 + 25/25.
- **Pregame mulligan/bottom (b2 pregame path) — bottom path UNEXERCISED** (0 mulligans this corpus,
  all 6 hands trivial 3-land keeps). The bottom-N-at-keep translog record remains unverified at this
  seat for a second corpus. Flag to synthesis: needs a seat that actually mulligans.

## N/A at this seat: transform/DFC (mono-red 20 Mountains), echo/reveal, forced self-sac,
## shockland/pathway ETB (b4), wither-FS (b5, deck109 has no wither), persist narration (b6, deck109
## is not the persist deck — it FACES persist in vs59, where the persist wall was the loss cause).

## ROTATION VERDICT — ROTATE. Deck-guide layer converged; Pillar 2-corpora clock CLOSED clean.
Applying the test unsentimentally (no-guide-mod AND no-new-work-signal at the DECK-GUIDE layer =
rotate), and answering wave-24's explicit condition directly:

- **Wave-24's stated rotation condition IS MET.** It said: "If corpus #2 again shows 0 Pillar
  mis-costings with the 1-red-mana trigger present AND the mulligan phase produces no anomaly, THEN
  rotate deck109 out as a fully converged canary." Corpus #2 shows exactly 0 Pillar mis-costings with
  4 trigger windows present (incl. the vs137 regressing matchup), and the mulligan phase produced no
  anomaly (6 clean keeps, no double-ask, no wrong-N). Both clauses satisfied.
- **No guide mod this wave:** strategy.txt byte-copied from deployed, cmp-verified identical (md5
  a8f163f7f2bf1ecce5c9f4f13997ccf1). The guide already mandates the correct behavior everywhere the
  model was tested; burn/attackers/deploy/mulligan-keep all clean; the restored Pillar line validated.
- **The two NEW findings do NOT hold deck109 in the canary pool.** Both are HARNESS/core items
  (target-preview truncation, prose-reversal parse), both cross-cutting — they would recur at ANY
  seat facing engine creatures behind a truncated target list or emitting a prose-only reversal. They
  belong in the engine/harness ledger (above) and a core-convergence check (general-suggestions.md),
  NOT in a deck109-specific canary hold. Keeping deck109 to watch them would be watching a
  general-engine issue through a deck-specific lens.
- **VERDICT: ROTATE deck109 out.** Fully converged at the deck-guide layer; Pillar crutch settled
  across both required corpora; all live-validated engine surfaces (a1/a2/a4/fix-(d)) clean at this
  seat. The loop's endgame is real here — this seat has no remaining deck-guide work.
- **One NON-blocking pool-composition note for synthesis (not a hold):** if the pool wants a
  regression seat to confirm LEDGER ITEM 1's fix once shipped, deck109 is a natural choice (its
  vs131 matchup reliably presents Young Pyromancer/Guttersnipe behind a truncated preview). That is
  the synthesis agent's pool call, not a reason to withhold the converged rotation verdict.
