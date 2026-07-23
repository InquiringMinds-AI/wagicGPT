# deck14 (Deep Blue) — notes.md — engine/harness/model items + rotation verdict (wave-21)

Layer-routed here, never core prose. seqs are deck14 owner-seat, corpus matchups-20260723-084938,
binary 3be7f5a67.

## ENGINE / HARNESS / REPRESENTATION LEDGER

### E1 (CLOSED / VALIDATED) — BOUNCE-ON-THE-STACK: the wave-20 #1 leak is fixed by item (f).
Wave-20 this was HIGH (1 costly self-bounce misplay + 3 truncation fallbacks). Wave-21 the shipped
representation fix — CAST-option annotation `[this cannot target the spell on the stack - battlefield
permanents only]` + TARGET-menu header `these targets are battlefield permanents only - the spell being
cast on the stack is NOT a legal target...` — is CONFIRMED FIRED and OBEYED:
- Annotation rendered on **26 cast records** across all 6 games (grep string confirmed on-decision).
- Model chose "Cast nothing" on **25/26**; the 1 cast (vs62 s18) was CORRECT (bounce the aura's target
  permanent so Web fizzles), NOT a stack-target misfire.
- The header rendered on the **1 live bounce-target MENU** this corpus (vs62 s19); model picked the
  opponent's 9/9 Yavimaya Enchantress, NOT its own creature, NOT a land. Wave-20's same-seat same-matchup
  s18 self-bounced the Lord of Atlantis — clean A/B, opposite outcome.
- **0 self-bounces, 0 fallbacks in this class, 0 truncation-thrash at a bounce menu.**
This is the marker-string terminus (confirmed fired + obeyed). It TRIGGERS DEMOTION of the interim guide
trap — done in wave-21 strategy.txt (the hoisted `#1 MISFIRE` procedure compressed to 3 lines keeping the
corrective fact + marker string + never-self-bounce). Close E1. (Caveat for synthesis: the target-MENU
subclass is validated on n=1 window — robustly obeyed on the n=26 CAST side; neither subclass is
zero-window, so the PAUSE-AWARE demotion precondition "observed obeyed, not merely absent" is met.)

### E2 (CLOSED / VALIDATED) — token-cap truncation-before-line is fixed by item (b) answer-first.
Wave-20 MEDIUM (3 truncation-before-line fallbacks, 2 blocker-decisive). Wave-21: long deliberations still
occur (p95 replies 11k-15k chars) but the coded line is emitted FIRST, so all parse. Verified on every
p95+ reply at the seat: vs135 s29 (15,373) `CHOICE:` first; vs27 s28 (14,655) `CHOICE:` first; vs35 s19/
s23/s22 (13,005/11,882/11,342) `CHOICE:` first; vs49 s6 (13,598) `CHOICE:` first; vs62 s21 (10,374,
attackers) `ATTACK: A1` first; vs27 s42 (9,376, blockers) `BLOCKS:` first. **0 truncation fallbacks, 0
prose salvages needed.** No answer-quality regression from answer-first (first-line matches the reasoning
that follows). Close E2.

### H1 (NEW, LOW; harness/parser) — spell-name echo on a TARGET-CHOICE menu -> stale_echo.
The seat's ONLY fallback this corpus (vs27 s29). A normal Unsummon target menu (no stack spell); the model
picked the correct target in prose (Inkfathom Infiltrator, option 1) but wrote its coded line as
`CHOICE: 1 (Unsummon targeting Inkfathom Infiltrator)` — echoing the SPELL name ("Unsummon", in no
option) ahead of the target. Parser Mechanism-B (echo names a card in no option) -> stale_echo ->
heuristic (which resolved an Unsummon; non-fatal). The header already says "answer with the chosen
TARGET's name (not 'Unsummon')" and the same seat formats it right elsewhere (vs62 s19:
`CHOICE: 10 (Yavimaya Enchantress)`). This is a parser-robustness gap, not a guide gap: the parser could
match the "targeting <OPTION>" tail — the echo DID contain the exact option name "Inkfathom Infiltrator"
as a substring. Suggested harness fix: on a target menu, if the CHOICE parenthetical contains
"<spell> targeting <X>" and X matches an option, resolve to X (the index also pointed at option 1 here,
so an index-wins tiebreak would ALSO have recovered it — the parser routed to stale_echo instead of
honoring the in-range index). 1 instance, non-deciding. WATCH; adopt into the parser only if a second
seat shows the "<spell> targeting <target>" echo-format downgrade (cross-seat by construction — any
bounce/steal/removal deck hits target menus).

### M1 (LOW; model, WATCH) — defensive bounce-slide under a recursive unblockable clock (vs27).
vs27 (LOSS): from t8->t15 deck14 dealt ~0 damage (opp stuck at 16) while sliding 18->-11, spending the
midgame Unsummon-looping the opponent's unblockable Inkfathom Infiltrator (s24/s28/s31) instead of
racing. This LOOKS like the wave-19 defensive-durdle, but three confounds: (a) deck27 improved sharply
this corpus (revised guide, 3/6->5/6) — a genuinely stronger opponent; (b) the Infiltrator is UNBLOCKABLE
so bouncing it is the only interaction, semi-forced; (c) deck14 drew its Air Elemental only t14, far too
late to convert. The guide already carries both relevant lines (attack floor Rule #1 + "don't re-bounce
the same cheap creature every turn on defense"). n=1, opponent-improvement + late-draw dominated — do NOT
write a guide line; a fourth wave with deck27 at 5/6-strength would tell whether this is a real
matchup-specific slide or variance. WATCH.

### E5 (CLOSED for now) — invented board object (wave-20 phantom Vodalian Zombie) did NOT recur.
Wave-20's hallucinated 7/7 fired inside the E2 truncated-ramble environment. With E2 closed (answer-first),
no invented-object hallucination was observed altering a decision this corpus. Consistent with wave-20's
prediction that fixing E2 reduces E5. Close pending recurrence.

## CARD-SCRIPT DIVERGENCES (carried, still to Oracle-verify by roster owner — unchanged)
- **Gravelgill Axeshark**: engine {4}{u/b} 3/3 [islandwalk, persist]; paper {4}{U} 4/3 Merfolk Soldier,
  no islandwalk/persist. Likely mis-mapped primitive — flag for Oracle verify. Guide teaches engine reality.
- **Counsel of the Soratami**: engine {2}{u} (3 mana); paper 10E {3}{U} (4 mana). Minor.
- Air Elemental 4/4 flying, Mahamoti 5/6 flying, Lord of Atlantis 2/2 lord, Coral Merfolk 2/1 match paper.
  Islandwalk near-dead in this pool (no opponent Islands seen); guide already scopes it correctly.

## MODEL-BEHAVIOR notes (guide-side, tracked)
- **Rule #1B HELD** — finisher deployed 3/3 when drawn, the turn it became castable; the 3/6->... vs 4/6
  is fewer finishers DRAWN (displacement), not fewer deployed. No sit-on-flyer.
- **Steal-their-best held** (Sheoldred vs102, closed the win); no 1/1 steals.
- **No self-bounce** anywhere; bounce aimed at real threats when not stack-confused.
- The ONE guide diff this wave is a DEMOTION of the bounce-on-stack trap (E1 fix shipped+obeyed).

## ENGINE ITEMS COUNT
**2 CLOSED/VALIDATED this wave** (E1 bounce-on-stack representation fix; E2 answer-first truncation fix) +
**1 NEW LOW** (H1 spell-name-echo target-menu stale_echo, parser-owned) + **2 WATCH** (M1 defensive slide;
E5 hallucination, currently quiet). Both of wave-20's high-value items (E1, E2) are now closed by the
shipped engine batch.

## ROTATION VERDICT — deck14 IS a rotation candidate. All three standing conditions met.
The wave-20 synthesis set the test: deck14 becomes a rotation candidate if **the bounce trap lands** AND
**E1/E2 got engine fixes** AND **no new class emerges**. Status:
1. **Bounce trap landed: YES** — 25/26 declines, the 1 menu played correctly, 0 self-bounces, 0 fallbacks
   in-class. The seat's historically dominant leak is solved.
2. **E1 + E2 engine fixes shipped and CONFIRMED: YES** — item (f) header/annotation (E1) and item (b)
   answer-first (E2), both fired and obeyed this corpus.
3. **No new STRATEGY class emerged: effectively YES** — the only new item (H1 spell-name-echo stale_echo)
   is a harness/PARSER robustness gap, not a strategy/guide class; M1 is a WATCH entangled with opponent
   improvement, and the guide already covers it. Nothing new needs guide work.

**Recommendation: deck14 rotates OUT as a converged seat; its guide (with this wave's demotion) stays
deployed as the canary.** The single wave-21 diff is a terminal-lifecycle DEMOTION (compressing a solved
leak whose engine fix landed), not new leak-work — so it does not re-open the "no-guide-mod" arm in a way
that argues for another active round. If synthesis prefers one more validation round instead, the demotion
is also the correct turnaround-mode diff and the only open watch (M1) would be its focus; but the evidence
says converged. One caveat for the synthesis owner: H1 (parser) should be handed to the harness, and the
Gravelgill Axeshark / Counsel card-script divergences remain open for the roster/Oracle owner.
