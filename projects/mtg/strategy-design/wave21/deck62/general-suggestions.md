# deck62 wave-21 — general (cross-seat) suggestions

## 1. (cross-seat, HARNESS, MEDIUM) The `retracted_choice` route OVER-FIRES: it discards a correct, uncontested, first-line coded answer when self-correction language ("Wait" / "Correction:") appears in the DOWNSTREAM reasoning rather than as a retraction OF the choice.
deck62 owns 3 of the corpus's 5 retracted_choice, and all three are false positives (vs27 s10, vs135 s5, vs102 s3):
each emitted the correct `CHOICE: 1 (Play Forest)` on line 1 (answer-first), never retracted it, and only
self-corrected a FUTURE-PLAN detail (Fists targeting a Shroud creature, mana arithmetic) in the reply body. Each
reply has exactly ONE `^CHOICE:` line and no contradictory second one. The detector routes to heuristic anyway.
Harm was zero here only because all three were land drops (the heuristic plays the land), but on a CAST/TARGET ask
the same over-fire would throw away a correct answer for a blind heuristic pick. FIX: fire retracted_choice ONLY
when a coded CHOICE is explicitly disavowed AND no valid CHOICE remains, OR a CONTRADICTORY second CHOICE is emitted
(the latter is already handled by "last well-formed wins"). If exactly one well-formed, in-range CHOICE stands with
no contradictory successor, TAKE IT regardless of prose "Wait/Correction" in the reasoning body. This must preserve
the genuine revise-and-replace case (a SECOND coded line -> last-wins), which is distinguishable by the second coded
line. Cross-seat: any deck whose model settles an answer then self-corrects downstream reasoning (a very common LLM
shape) is exposed. deck62 is the ideal validator. MEDIUM: recurred 3x at one seat, latent-consequential elsewhere.

## 2. (cross-seat, WATCH — answer-first residual) For a reasons-then-revises model, answer-first's snap first answer is often the WORSE option; "last well-formed wins" preserves quality, and the residual risk is a TRUNCATION between the snap answer and the correction.
deck62 vs14 s26 (snap `CHOICE: 4` 2nd-Yavimaya -> reasoned `CHOICE: 2` Fists) and vs135 s21 (snap `BLOCKS: B2:A1`
deathtouch-Coatl -> reasoned `BLOCKS: B2:A2` 13/13-Treefolk, saving 8 life) both had the parser correctly take the
reasoned last line. No regression THIS corpus (0 truncations). But the exposure is structural: if a reply truncates
after the snap answer and before the correction, the worse snap answer executes with no rescue. WATCH: on any seat,
flag a truncated reply whose parsed (snap) answer diverges from where its cut-off reasoning was heading — that is
the answer-first quality-regression signature (distinct from a parse regression). The mitigations already in the
batch (answer-first + last-wins) handle the non-truncated case; only a decode-time length/repetition guard closes
the truncation-between-snap-and-correction gap. Low urgency (unobserved), named for cross-seat vigilance.

## 3. (cross-seat, HARNESS, LOW) A `CHOICE: N (spell-name + target-name)` in a "answer with the TARGET's name, not <spell>" menu falls to stale_echo when N is correct and the target name is present but PREFIXED with the spell name.
deck62 vs14 s29: Web target menu, model wrote `CHOICE: 4 (Web targeting Yavimaya Enchantress)` — N=4 is the correct
in-range target (Yavimaya), but the "Web targeting" prefix broke the option-4 name match -> stale_echo -> heuristic
Web'd a 1/1 Saproling instead of the 4/4 Yavimaya (marginal harm; won anyway). TOLERANCE: when a `CHOICE: N (...)`
has an in-range N whose option-N name appears as a SUBSTRING of the parenthetical, prefer N (strip the leading
spell-name qualifier) rather than falling to stale_echo. Cross-seat by construction (the "not <spell>" target-menu
instruction is shared). LOW: single instance, marginal harm. Sibling of the wave-20 N7-template leak (both are the
model injecting extra tokens into the CHOICE parenthetical that the shared prompt format invites).

## 4. (observation, not a change) deck62's record remains a matchup-shape floor with pool-luck swing.
4/6 this wave (up from 3/6), but the +1 is pure pool/draw variance on a guide that changed only edit D: vs35 and
vs49 (both wave-20 "matchup floors") flipped to WINS while vs27 (a wave-20 win) flipped to a LOSS. The four wins
all fired the taught attack-every-turn/go-wide beatdown; the two losses are matchup floors (deck135 out-sizes with
a 13/13 by T10; deck27 walls the go-wide plan with protection-from-green + unblockable + flyers and denies power).
It is a turn-8-13 ground/go-wide deck; the guide cannot fix a clock deficit — route the record to the roster owner,
not the guide. (Same standing verdict as waves 18-20; win column is context-only per Step 0.)
