# deck14 (Deep Blue) — notes.md — engine/harness/model items + rotation verdict

## ENGINE / HARNESS / REPRESENTATION LEDGER (layer-routed here, never core prose)

### E1 (HIGH; 4-repro incl 1 costly misplay, DECIDING) — BOUNCE-ON-THE-STACK: model targets a spell on the
### stack with Unsummon/Boomerang; engine offers the bounce as a "respond now" answer and then forces a
### battlefield-only target menu the model thrashes over.
Signature: the model casts Unsummon/Boomerang intending to "counter"/"stop" a card the opponent is CASTING,
then either (a) self-bounces its own creature at the forced target menu, or (b) thrashes to a truncated
fallback hunting for the stack card that is not in the (battlefield-permanent) list. Instances:
- **vs62 s18 (parsed MISPLAY, cost a turn):** cast Unsummon to hit the on-stack Canopy Spider; the warning
  annotation fired ("only legal targets are YOUR OWN"); model cast anyway; s19 events show it bounced its
  OWN Lord of Atlantis, the Spider resolved, and it recast the Lord. Wasted tempo in a LOST game.
- **vs35 s27 (stale_echo):** Boomerang target menu, Fury Sliver on stack, 15.1k thrash, no CHOICE.
- **vs27 s11 (unparsed):** Boomerang target menu, Lord of the Undead on stack, 15.6k truncated.
- **vs102 s9 (unparsed):** Boomerang target menu, Shadowspear (equipment) on stack, 16.3k truncated.
DURABLE FIX (representation/engine, in priority order): (1) do NOT surface Unsummon/Boomerang as a castable
"respond now" option whose ONLY conceivable value is an illegal stack target — if there is no legal
battlefield target the bounce wants, either don't offer it or annotate the CAST option "[cannot target the
spell on the stack — only battlefield permanents]"; (2) when the forced TARGET CHOICE menu appears, prepend
a one-line header "these are battlefield permanents only; the spell resolving on the stack is NOT here";
(3) the interim GUIDE trap shipped in strategy.txt (hoisted #1-MISFIRE bullet). This is the seat's #1
model-fixable leak and the single largest fallback+misplay cluster. seqs above.

### E2 (MEDIUM; 3-repro, 2 blocker-decisive) — TOKEN-CAP TRUNCATION of combat/target replies (reply never
### reaches ANY final line). Salvage extension cannot reach it; needs decode-side or prose-intent salvage.
Distinct from the wave-19 format-DRIFT class (which terminated). Here the reply exhausts the token cap
mid-derivation and emits no coded line, so salvageLoopedChoice (BLOCKS/ATTACK/PUT) has nothing to salvage.
- **vs62 s28 (blockers, 8k+):** exhaustive A-F block enumeration, PROGRESSING, truncated; board was
  genuinely lethal (model's own math), outcome-neutral.
- **vs27 s47 (blockers, 12.2k):** truncated but the PROSE TAIL states a decision ("block A3 (Lord of
  Atlantis)") — a prose-intent salvage COULD have recovered `BLOCKS: B1:A3`. Outcome-neutral (adj win 6-3).
- **vs27 s42 (ask, 12.9k):** a 2-option `Cast Mahamoti / Cast nothing` durdle that truncated.
Fixes: (a) decode-time max-token / repetition guard so combat replies terminate; (b) an ANSWER-FIRST reply
protocol for combat (emit the coded line THEN reason) so a truncation still leaves a parseable line; (c) a
PROSE-INTENT salvage that extracts the last stated "block X"/"attack X" from the reasoning when no coded
line exists. Triggers are (i) impossible-target confusion (see E1) and (ii) hopeless/complex boards.

### E3 (LOW; observability/coverage) — #N ordinal disambiguation NOT observed at this seat.
No `#N` token rendered on any option list across all 6 games, including duplicate-name TARGET menus (vs62
Control-Magic "Yavimaya Enchantress, Yavimaya Enchantress"; vs35 Persuasion "Gemhide Sliver, Gemhide
Sliver"; vs27 Unsummon "Lord of the Undead"; vs49 duplicate lands). No duplicate-name pick was fumbled
this corpus, so no harm — but the fix's would-matter surface (an actual SELECTION between two same-named
creatures in DIFFERENT states, e.g. the 7/9 vs 7/7 Yavimaya) did not occur as a decision. Target menus DO
render per-line P/T, which functionally disambiguates different-stat duplicates without #N. Cannot confirm
the fix fired at this seat; flag as a coverage hole for synthesis (some combat seat with a real duplicate-
state target selection is needed to validate #N). NOTE: combat A#/B# codes already disambiguate by
position, so #N is only load-bearing on NON-combat target menus with same-named, different-state creatures.

### E4 (CLOSED / VALIDATED) — combat `options_text` observability gap (wave-19 E3) is FIXED.
Wave-19 every deck14 combat record logged empty `options_text`. Wave-20: 40/40 combat records carry full
options_text WITH the trade annotations ("your blocker dies, attacker lives, 3 tramples to your face").
Combat-decision review is now possible at this seat. Close.

### E5 (LOW; model, WATCH) — invented board object recurred (hallucination class).
vs27 s47: the reply invents a "Vodalian Zombie (7/7)" and "8 attackers" when the menu lists 6 (max 5/5);
the Vodalian Zombie entered only the NEXT turn (t24). Fires inside the E2 truncated-ramble environment;
outcome-neutral. Correlated with reasoning-tax truncation, not with clean short replies — fixing E2
likely reduces E5. Watch; do not write a guide line for it (it is a decode/model artifact, not a strategy
gap, and it did not change an outcome).

## MODEL-BEHAVIOR notes (guide-side, tracked)
- **Rule #1B LANDED** — finisher deployment 1/6 -> 4/6, all three wins are finisher games, finisher cast the
  turn it is available. The wave-19 slow-clock soft-durdle is substantially cured; residual non-deployment
  (vs135) is draw/mana STARVATION (displacement), not a rule failure.
- **Steal-their-best held** (Thunderbreak Regent vs49, Fury Sliver vs35); no 1/1 steals.
- **Bounce aimed at real threats** when not confused by the stack (Lord of the Undead, Sanguine Guard,
  Ohran Viper); the wave-19 re-bounce-a-1/1 anti-pattern did not meaningfully recur.
- **The one guide diff this wave** targets E1 (bounce-on-stack) as a hoisted trap; re-validate next round.

## CARD-SCRIPT DIVERGENCES (carried, still to Oracle-verify by owner — unchanged from wave-19)
- **Gravelgill Axeshark**: engine {4}{u/b} 3/3 [islandwalk, persist]; paper is {4}{U} 4/3 Merfolk Soldier,
  no islandwalk/persist. Likely a mis-mapped primitive — flag for Oracle verify. Guide teaches engine reality.
- **Counsel of the Soratami**: engine {2}{u} (3 mana); paper 10E {3}{U} (4 mana). Minor.
- Air Elemental 4/4 flying, Mahamoti Djinn 5/6 flying, Lord of Atlantis 2/2 lord, Coral Merfolk 2/1 — match
  paper. Islandwalk near-dead in this pool; guide already scopes it correctly.

## ENGINE ITEMS COUNT
**5 open** (E1 HIGH bounce-on-stack; E2 MEDIUM token-cap truncation; E3 LOW #N-not-observed; E5 LOW
hallucination watch) + **1 closed/validated** (E4 combat options_text). E1+E2 are the wave's high-value pair
and are partially coupled (E1 confusion is one of E2's two truncation triggers).

## ROTATION VERDICT
**NOT a rotation candidate — deck14 STAYS for a third guided round.** The per-deck rotation test (no-guide-mod
AND no-new-work-signal = rotation candidate) fails BOTH exit conditions: there IS a warranted guide-mod (the
constrained bounce-on-stack trap diff) AND strong new-work signal (E1 bounce-on-stack now the #1 leak with a
costly misplay + 3 fallbacks; E2 truncation-before-line as a distinct-from-format-drift class needing a
decode/answer-first/prose-salvage fix). The wave-19 revision (Rule #1B) demonstrably LANDED (finisher 1/6->4/6,
2/6->3/6), which is turnaround-mode SUCCESS that warrants a light constrained diff + one more validation
round, not rotation. Re-evaluate after wave-21: if the bounce-on-stack trap lands (the misfire count drops)
AND E1/E2 get an engine/representation fix, and no new class emerges, deck14 becomes a rotation candidate then.
