# deck158 — engine/harness/render notes (wave-35 → wave-36 step-1 docket)

Seq repros use the corpus dirs from the reviewer brief (A = matchups-20260820-192210,
B = matchups-20260821-033000, C = matchups-20260822-111102), file pattern `*ai_baka_deck158*`.

## P6 — Bowmasters/Foray target lists: print the toughness VERDICT, not just (P/T)  [render, HIGH]
The FP8 pilot transposed (3/1) into "toughness 3" twice at the same seam (C-152 seq10 T7 and
seq14 T9 — Elite Spellbinder, a legal toughness-1 kill, shot to the face both times) and read
"6 damage" as killing an 8/8 (C-36 seq12 T6, Foray → Master of Etherium; engine log confirms
survival). Candidate: on Bowmasters target lines, append a computed verdict per creature —
"toughness 1 — dies to the shot" / "toughness 3 — survives"; on Foray target lines, "damage 6 —
kills it" / "damage 6 — survives (toughness 8)". This is the wave-33 HL7 annotation-truth
programme applied to the target menu: the pilot trusts printed verdicts above its own arithmetic,
which is exactly what we want here.
REMOVAL CONDITION for the guide stopgaps it retires: when
`grep 'dies to the shot' <corpus deck158 prompts>` (or the shipped verdict string) is non-empty,
demote the guide's READING A CREATURE'S NUMBERS section and the Foray/Bowmasters second-number
clauses to one line each.

## R1 — blockers header: state the 0-life boundary  [render/core, LOW]
C-36 seq18 T8: block trace computed "4 - 4 = 0 life. I survive!" (outcome-neutral — every line
was lethal). If the blockers header's lethality line does not already print an explicit verdict
at exactly-0 ("you would be at 0 — LETHAL"), add it; the guide now carries the boundary clause as
a stopgap (remove when the header prints the verdict).

## F4 — payability guarantee overridden by a mana recount  [watch, guide-hardened]
C-36 seq16-17 T8: "Cast Olog-hai Crusher" listed; trace re-derived mana, miscounted Treasure
(3 lands + Treasure counted to "{R}{B}{R}, still not {3}{R}") and declined, overriding the
prompt's "every listed choice is legal AND PAYABLE". Guide now carries the never-recount
falsity-fact. If the class recurs guide-hardened, consider the render printing the tap plan
("pays: Swamp+Swamp+Mountain+Treasure") on cast lines that need a Treasure crack.

## P3 — Ichorclaw Myr B-line annotation vs becomes-blocked trigger  [engine verify, carried]
A-105 seq8 T5 (tier review): B-line said "you kill it, your blocker lives" for blocking Ichorclaw
Myr; Oracle gives it +2/+2 when blocked (2/2 → would kill the 2/2 Army). Either the card script
lacks the trigger or the annotation ignores it (/wagic-cardscript check). The guide's
"answer from the B-lines and nothing else" doctrine depends on these annotations being true —
this outranks any deck158 guide finding if the annotation is wrong. Unexercised at arm C.

## P2 — opponent-forced sacrifice labeled "your own effect"  [render, carried]
B-116 seq12-17 T11: Emrakul Annihilator-6 sacrifice prompts headed "SACRIFICE ONE OF YOUR OWN
CARDS: your own effect (Sacrifice)...". The effect is the opponent's. The 122B parsed through it;
the standing 35B-FP8 pilot has not been exercised on this prompt shape.

## P4/P5 — minor, carried from the tier review
P4: hand list contained an already-played Mountain same turn (B-105 seq6) — hand snapshot timing.
P5: floating-mana note ("Already in pool... SEPARATE from...") cost a long disambiguation detour
(B-36 seq12) — say what produced the floating mana.

## Assault on Osgiliath — third consecutive unexercised corpus
Zero offers again at arm C (2 copies / 60 cards). Wave-33 HL12 already retired it from the corpus
ledger (probe-or-close). No new evidence this wave; do not re-open as a corpus item.
