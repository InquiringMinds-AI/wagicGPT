# Skill contribution — deck44, wave 11 (verification wave; single-seat, no core/guide edits)

The CURRENT skill is `strategy-design/wave10/strategy-writing-skill.md`. This wave the deck44 guide is
FROZEN byte-identical (8th wave), and per the convergence gate a single-seat verification wave cannot
add or cut a method/core line. What follows are diagnostic REFINEMENTS to existing rungs (offered to
synthesis; adopt only if a 2nd seat witnesses, or as a watch), plus the verification record for the
wave-10 engine batch.

## Method refinement 1 — score a STILL-UNFIXED engine defect by RECURRENCE, but report OUTCOME-COUPLING separately

The auto-tap color-stranding defect (ledger #2) is UNFIXED by design this corpus. Wave-10 it cost the
vs140 game (stranded blue -> couldn't counter Blightsteel). Wave-11 it RECURRED mechanically (vs131 s30:
Archmage {4}{u} tapped all 3 Islands with 3 Swamps available; vs131 s21: Sprite {1}{u} stranded a blue)
but was OUTCOME-NEUTRAL (no counter window followed before untap; the game was won). The refinement
(extends the confirmed-fired terminus to its mirror, the confirmed-STILL-BROKEN case): when re-verifying
a KNOWN-unfixed engine defect, report two things separately — (a) did the mechanical class RECUR (grep
the mana lines / the cast->next-mana delta), and (b) did it COUPLE to a decision cost this corpus. A
defect can be live-and-harmless in one corpus and live-and-fatal in another purely by trajectory; do NOT
downgrade the ledger item because a given corpus was harmless, and do NOT re-inflate it into a guide line
because it recurred. The harmless-recurrence corpus is USEFUL: it yields a clean, loss-decoupled repro
(vs131 s30) that is a better regression fixture for the eventual engine fix than a repro tangled with a
game loss. This is the "verify the fix FIRED" discipline applied to a fix that has NOT YET shipped:
confirm the DEFECT still fires, quantify its outcome-coupling, keep the routing, add the clean repro.

## Method refinement 2 — verifying a shipped TRUNCATION/PARSE guard: 0-scavenged-casts + all-fallbacks-heuristic is the signature

Wave-10 flagged a truncation-scavenge class (deck44 vs140 s68: a reply that blew the token cap with no
CHOICE line, whose parser scavenged a stray body digit and recorded a spurious cast). The wave-10 batch
shipped 4096 max_tokens + parse ramble guards. The verification signature at this seat (2 parts, both
must hold): (a) the own-targets annotation's mis-cast count is measured against the REASONING and the
recorded choice AGREES with it — 29/29 obeyed, 0 recorded self-casts (vs wave-10's 36/1); and (b) EVERY
fallback record carries `fallback=unparsed_reply|empty_reply` AND a HEURISTIC recorded action (choice 0
or -1), with NO fallback record showing a scavenged cast integer. When both hold, the truncation-scavenge
class is CLOSED. This is the parse-layer twin of the representation confirmed-fired terminus: the guard
fired if and only if a would-be-scavenged reply now shows the heuristic action instead of a body-digit cast.

## Watch (single-seat; promote only with a 2nd seat) — strict free-form block/attacker parser rejects reasonable declarations

The block/attacker declaration is free-form (no numbered menu); the expected format is `BLOCKS: B1:A2,
B2:none` / `ATTACK: <slot indices>`. This seat produced 3 rejects that fell to the heuristic:
`BLOCKS: B1:A2` (A2 out of range — a phantom-attacker ramble), `BLOCKS: Scion of Oona:none` (card NAME
instead of B-index), `ATTACK: Scion of Oona, Sleep-Cursed Faerie` (names instead of slot indices). All
three heuristic fallbacks matched the guide-correct or game-neutral action, so it cost nothing this
corpus, but it is a latent robustness gap distinct from the truncation class (these replies REACHED a
declaration line; the parser rejected its FORM). Route to the reply-protocol/parse layer, never the
guide. WATCH: promote to a harness item only if a 2nd seat shows a strict-parse reject flipping to a
game-costing heuristic (i.e., the heuristic disagreeing with a correct stated declaration).

## Convergence-gate note
All three items above are single-seat (deck44) and are DIAGNOSTIC/WATCH, not core or guide edits. The
core prompt and the deck44 guide are both FROZEN. Nothing here crosses the adopt-a-revision bar (a new
decision-level failure class); the wave's model-fixable content is entirely engine-ledger (auto-tap #2,
the standing top item; the free-form-parser watch #3).
