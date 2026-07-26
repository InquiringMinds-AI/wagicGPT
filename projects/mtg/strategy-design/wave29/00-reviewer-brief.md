# Wave-29 reviewer brief (shared context for all 7 deck agents + synthesis)

Fifteenth cycle. Pool: 137, 93, 136, 146 (holdovers) + **deck139 (Mutants — mutate,
campaign-first), deck148 (Kor Army — equipment), deck122 (Eternal Life — lifegain/Felidar
alt-win/Sorin loyalty) — Step-0 guideless debuts**. deck18+22 rotated out (canaries 18);
deck134 construction-terminal (separate list).

PROCESS RULE: NEVER kill any wagic process (pkill FORBIDDEN); kill only your own PIDs.

## The step-1 batch under live validation this corpus (binary 50eeed8be)

(f1) **March X-convoke FIXED, both routes** — {X} propagated onto the convoke shell +
X re-entry guard (also fixes latent double-X aborts for ~24 alt-cost X spells). deck137:
THE exit gate — trace every March cast end-to-end (X announced, payment, tokens).
(f2) **Dual-land oracle fix (N-146a/d root)** — potentialMana no longer collapses duals;
planPayment defers flexible sources; 'Mana available' renders true color reach. deck146:
verify the no-cast-window class is GONE (every turn with castables gets a cast ask) +
Mana-available lines correct. EVERYONE with duals: watch for newly-castable spells.
(f3) **Summoning-sick tag** on battlefield renders. deck93: exit gate — phantom-attacker
L2 dead? ATTACK lines match legal sets?
(f4) **Dungeon renders** — room-branch options carry effect text; venture selection
reframed with full paths + completion rewards. deck146: navigation quality now?
(f5) **Flip Side annotated**; (f6) **N-18e truncation safety (blockers)** — fired EXACTLY
ONCE this corpus (fallback kind=blockers/truncated_abandoned): whoever owns that record,
decompose it — did the safety correctly abandon a contradicted commitment?
Carry: e/d/c batches steady state.

## Corpus data

- **PRIMARY**: ~/.Wagic/ai/gpt/selfplay-runs/matchups-20260726-115018 — 21 games, binary
  50eeed8be, -T 3000, REPPENALTY=1.05.
- Win table: **136:5/6 (first corpus on the MV3-fixed guide)**, **93:5/6 (sick-tag
  corpus)**, 146:3/6, 137:3/6, 122:2/6 (Step-0), 139:2/6 (Step-0), 148:1/6 (Step-0).
  0 timeouts/draws, 7 adj (28 gameend logs = 42-14 — integrity clean).
- **Fallbacks: 11/1,316 (0.8%)** — 8 defers (halved from 16; decompose: still convoke? if
  the March fix works they should be a DIFFERENT shape or near-zero), 1 priority
  retracted_choice + 1 priority unparsed_reply (find + decompose), 1 blockers
  truncated_abandoned (the N-18e safety's first live fire — see f6). Retry 0.

## Per-seat focus

- **deck137 (3/6)**: f1 — THE EXIT GATE. March resolution table (X announced / tokens
  created / attempts) vs the 0/17+0/16 history; convoke steady state (Loxodon); defer
  decomposition (how many of the 8 are yours, what shape). If March validates and no new
  decision class: ROTATE OUT per the wave-28 named exit path.
- **deck93 (5/6)**: f3 — exit gate. Sick-tag renders + ATTACK-line phantom rate (was
  11/15); if L2 dead and guide frozen: ROTATE OUT (near-terminal since wave-28, held only
  by this fix).
- **deck136 (5/6)**: MV3-fix first corpus (Ritual of Soot correctly valued vs Rats now?
  the 5-turn impossible-plan class gone?); 2/6->5/6 decomposition; Rankle/Ugin surfaces;
  rotation projection.
- **deck146 (3/6)**: f2 + f4 — no-cast-window GONE? Mana-available correct? dungeon
  navigation with effect text (option-1 bias broken? Lost Mine picked?); Kaya exercised?
  dungeon completion reached (the 309.5b probe question)?
- **deck139 (Step-0, 2/6)**: FIRST READ — mutate is campaign-first: how do mutate casts/
  stacks render? does the model understand the merged creature? Initial guide draft.
- **deck148 (Step-0, 1/6)**: FIRST READ — equipment surfaces (equip costs, attach
  decisions, equipment-matters payoffs). 1/6 decomposition. Initial guide draft.
- **deck122 (Step-0, 2/6)**: FIRST READ — lifegain-matters + Felidar Sovereign alt-win
  (does the engine implement the upkeep win check? does the model see the win line?) +
  Sorin loyalty. Initial guide draft.
- **EVERYONE**: f-batch + steady state; the 2 priority fallbacks and 1 truncated_abandoned
  get decomposed by whoever owns them; parser contradictions verbatim.

## Layer-routing (unchanged)

Engine/harness -> notes.md with seq repros. Guide -> strategy.txt per
wave28/strategy-writing-skill.md (CURRENT skill — note the PERCEPTION-vs-STRATEGY routing
classifier and the verify-oracle HARD RAIL for every card fact a guide asserts). Skill ->
skill.md. Core -> general-suggestions.md (23 consecutive PASSes). Rotation: UNSENTIMENTAL.
Guideless roster 135 after this wave's three initial guides.

## Open ledger carried (cite if exercised)

March-fix live confirmation = f1 (this corpus IS the validation); N-146e dungeon
completion probe; N-134b flashback cost display; N-134d Mulldrifter double-ask; L2
cross-zone dup-name; L-18-1 planning tax (escalated); Rankle may-ask WATCH; illegal
gang-block WATCH; prose-reversal; untapped-tagging; I-4 wobbles; N8; Azcanta; undying;
commit-lock; ETB header card-name.
