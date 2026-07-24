# Wave-23 deployment manifest (for the integrator — do NOT deploy from the synthesis seat)

Corpus `matchups-20260724-013710`, binary `/tmp/wagic-0e6361732`. Guides live at
`projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (`bin/Res` is gitignored → new guides need `git add -f`).
Core prompt `bin/Res/ai/gpt/system_prompt.txt`: **NO CHANGE** (18th consecutive PASS). The wave artifact
`strategy-design/wave23/strategy-writing-skill.md` is the reviewer instrument, not a Res asset — it is not deployed.

## cmp-verification of the seat strategy.txt files vs the LIVE deployed guides

Verified with `cmp -s bin/Res/ai/baka/deckN_strategy.txt strategy-design/wave23/deckN/strategy.txt`:

| deck | claim | cmp result | live bytes | rev bytes | disposition |
|------|-------|-----------|-----------|-----------|-------------|
| 27  | BYTE-COPY (guide frozen) | **BYTE-IDENTICAL** ✓ | 7497 | 7497 | NO DEPLOY (already live; the deployed wave-22 free-kills-first guide) |
| 102 | 4 edits | DIFFERS | 8201 | 9192 | DEPLOY (revision) |
| 109 | Pillar {R} cost restoration | DIFFERS | 8430 | 8729 | DEPLOY (revision) |
| 133 | 2 ADDs | DIFFERS | 8960 | 9689 | DEPLOY (revision) |
| 131 | Guttersnipe attack guard | DIFFERS | 9036 | 10262 | DEPLOY (revision) |
| 140 | crutch DEMOTE + 3 refinements | DIFFERS | 8567 | 8865 | DEPLOY (revision) — deploy BEFORE rotating 140 out |
| 137 | NEW initial guide | NO LIVE FILE (newcomer) | — | (see file) | DEPLOY NEW (`git add -f deck137_strategy.txt`) |

The deck27 byte-copy claim is CONFIRMED (`cmp` exit 0). No other seat's strategy.txt is a byte-copy.

## Deploy actions (integrator, step 4 of the wave loop — apply to live Res, then rebuild-free since guides are data)

1. `cp strategy-design/wave23/deck102/strategy.txt bin/Res/ai/baka/deck102_strategy.txt` — DFC flip-thrash
   crutch DEMOTE + Lantern oracle text trimmed to "cheaper face" (pending R-DFC-BACKFACE-RESOLVES-FRONT) +
   explicit steal-take instruction + Smallpox fire-gate re-scope + keep-your-bombs-on-forced-discard.
2. `cp strategy-design/wave23/deck109/strategy.txt bin/Res/ai/baka/deck109_strategy.txt` — RESTORE the lean
   Pillar {R} cost line naming the Pillar-{R}-vs-Searing-Spear-{1}{R} conflation (belief-correcting crutch
   regressed round-2; hybrid-mana retirement stays retired).
3. `cp strategy-design/wave23/deck133/strategy.txt bin/Res/ai/baka/deck133_strategy.txt` — ADD Bloodghast
   forced-loss-fodder line (recurs on next land) + Liliana of the Veil +1 caution paragraph. Everything else
   BYTE-FROZEN from wave-11.
4. `cp strategy-design/wave23/deck131/strategy.txt bin/Res/ai/baka/deck131_strategy.txt` — ADD the
   Guttersnipe attack-side guard with a PER-CREATURE [tapped]-read (never attack the lone reach source into a
   possible trade; a creature without [tapped] can block even if a sibling on the same line is tapped).
5. `cp strategy-design/wave23/deck140/strategy.txt bin/Res/ai/baka/deck140_strategy.txt` — DEMOTE the interim
   forced-discard crutch to a marker-anchored nod deferring to the engine framing (keep pitch-order + named
   finishers) + 3 observed-friction refinements. **Deploy this BEFORE rotating deck140 out** (guide stays
   deployed byte-identical as a canary control thereafter).
6. `cp strategy-design/wave23/deck137/strategy.txt bin/Res/ai/baka/deck137_strategy.txt && git add -f
   bin/Res/ai/baka/deck137_strategy.txt` — NEW GW Adventures initial guide (Step-0-bis; capability table +
   "(P/T) on the alt-cost option is the creature you get LATER" corrective + convoke-trust line + attack-floor).
7. deck27: **NO ACTION** — live guide is already the deployed wave-22 revision (`cmp`-confirmed byte-identical).

## Post-deploy verification (integrator)
- Re-`cmp` each deployed guide against its seat strategy.txt (exit 0).
- Confirm `git status` shows deck137_strategy.txt tracked (force-added) and the 5 revised guides modified;
  `bin/Res` is gitignored so ALL need `-f` if committed. NO AI attribution on any commit (user's own repo).
- No binary rebuild needed (guides are runtime data). Core prompt untouched → no rebuild for it either.

## Between-wave engine/harness work (step 4 ledger — see synthesis-notes.md WAVE-24 ENGINE LEDGER, ranked)
Not part of guide deployment; worked separately before the wave-24 corpus. Highest: #1 Yawgmoth unpayable-sac
legality-filter gap (legality-oracle invariant violation), #2 R-ADV-FACE adventure option mislabel, #3
R-DFC-BACKFACE-RESOLVES-FRONT, #4 decode-collapse sampler mitigation (rep_penalty=1.05 insufficient).

## DO NOT DEPLOY from this seat.
The synthesis agent produces the manifest; the integrator applies it (development-loop step 4). Deployment,
the engine-ledger work, and the wave-24 pool swap (rotate 140 → canary, rotate deck59 in) happen in step 4.
