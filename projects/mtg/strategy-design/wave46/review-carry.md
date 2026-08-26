# Wave-46 review carry-list (assembled 2026-08-26 at step-1 merge)

Corpus: third fair-hand corpus. Binary = master 486c83189 (lanes F/G/H/I + wave-45 lane E;
PARSETEST 1332/0). Serve FP8-35B @ spark:8084. Same pool/recipe. Guides = wave-45 reviewer
revisions + the boundary pass (3ca0b2622) — first corpus on them.

## Discipline
- Owner doctrine (2026-08-26): reviewers OWN guide changes; synthesis owns skill + general
  strategy; the CORE LOOP (engine mechanics, LLM UX, bugs) is the purpose. Deck-content
  questions are decided by the reviewer from evidence — never routed to the owner.
- Adjudicate render predictions by the emitter's actual string (read the code / PARSETEST).
- Baselines: fallbacks 0.10% (w45); 21/21 natural x6.

## Prediction set A — wave-45 guide edits (per wave45/deck*/edit-texts.md + boundary pass)
Headliners: land-drop RULE 0 at 123/125/126; the 'your attacker dies' STOP re-formed three
ways (146/152/162); GANG BLOCK keyed rules; deck123 cap 12->20 (WHY recorded; wave-47 sunset
test); mulligan-chain teach at 123/125/152; deck130 Starstorm quick-reference qualifier.

## Prediction set B — wave-46 engine lanes
- #W46-1 (F, 421bd5f62): per-colour source counts on `Mana available:`; coloured-pip
  overcommits 0/N; `Add N green` taken with no cast 0.
- #W46-2 (F): copies-left-behind clause on zone-change lines from shown zones; no
  'contradiction' reasoning about the hand list; p99 latency on 2-option asks < 120s.
- #W46-3 (G, bcea9f63c): `{effect:}` on opponent non-creature permanents — textless 0/N;
  converter line on a non-attackers window in every game with a converter on board.
- #W46-5 (H, 2003520a1): blockers-screen GANG BLOCK mirror — hand-derived group arithmetic in
  blockers replies 0/N; all-`(your blocker dies)` windows with a lethal legal group and no
  price 0/N. Measure token cost on wide boards (no collapse threshold on that screen).
- #W46-9 (H): lifelink tail bound to its branch — replies adding keyword benefit to face
  damage 0/N.
- #W46-6 (I, 81cefd06e): `A choice is required` headers 0/N (was 9; storedSourceCard unwrap).
- #W46-7 (I): mana-only priority windows reaching the model 0/N; `mana_only_windows_skipped`
  in gameend records; take-rate on surviving windows unchanged.
- #W46-8 (I): X casts above a collapsed equal-kill row 0/N.
- Lane E (#W45-20, 607ca6bb7, first corpus carrying it): unprefixed card-SUBJECT history lines
  0/N; damage OBJECT still bare (known).

## Carried (wave45/engine-ledger.md MED/LOW not shipped)
#W46-4 non-damage X preview + X=0 null-cast callout; #W46-10 activated-ability state on the
option row (Hive re-animation); #W46-11 stale-plan note repeats on identical plan text (49%);
#W46-12 plan-vs-affordability clause; LOW batch; Baka menace gang-block probe (red pin).
