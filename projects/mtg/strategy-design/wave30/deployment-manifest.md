# Wave-30 deployment manifest

Deploy target: `projects/mtg/bin/Res/ai/baka/deckN_strategy.txt` (gitignored -> `git add -f`).
Source of truth for each proposed guide: `strategy-design/wave30/deckN/strategy.txt`.
The ORCHESTRATOR performs the deploy/commit/push -- this manifest documents WHAT to deploy and the
cmp/md5 evidence. Verified against the live tree this session (binary 6e168aa05).

## Actions

| Deck | Action | Deployed md5 | Proposed md5 | Bytes (dep -> prop) | Evidence |
|------|--------|--------------|--------------|---------------------|----------|
| 152 | **NEW initial deploy** | (none) | fd70432e53c2d4bdcbcce98968868a59 | -- -> 6654 | no deployed file; Step-0 debut guide |
| 139 | **REVISION** | 8e7131f4046a8490d50a2577d062feab | 8f4aebe09e1b3458f3c32a7def91e2f0 | 8320 -> 8439 | stopgap retirement (render-trap prose -> shipped-render "READING THE MUTATE FLOW"; decide-combat-fast strengthened with adjudication-clock rationale) -- diff-verified |
| 122 | BYTE-FROZEN | 537a20f16acac0c33eec21a35c679a76 | 537a20f16acac0c33eec21a35c679a76 | 6575 (identical) | cmp+md5 identical; near-terminal, no redeploy |
| 136 | BYTE-FROZEN | f96f6e77bffae8caba7ddde2bd2d0d46 | f96f6e77bffae8caba7ddde2bd2d0d46 | 10367 (identical) | cmp+md5 identical; rotating OUT, no redeploy |
| 137 | BYTE-FROZEN | 413133bda07d1804a3e2a7a1c12b3718 | 413133bda07d1804a3e2a7a1c12b3718 | 9522 (identical) | cmp+md5 identical; convoke gate not met, guide unchanged |
| 146 | BYTE-FROZEN | 636e29cfbc5c7f5eb65c240d6d906f1d | 636e29cfbc5c7f5eb65c240d6d906f1d | 8482 (identical) | cmp+md5 identical; every wave-29 fact-correction validated live |
| 148 | BYTE-FROZEN | d2086fceb9145f766b52546a7bb4a719 | d2086fceb9145f766b52546a7bb4a719 | 5709 (identical) | cmp+md5 identical; construction-ceilinged, freeze + hand-off |

Cross-wave check: deployed md5 for 122/136/137/146/148 == wave-29 PROPOSED md5 (the wave-29 deploy
is live and byte-frozen). deck139 deployed == wave-29 proposed (8e7131f...); the wave-30 proposed
differs (revision). deck152 has no deployed file (Step-0 debut). All wave-30 seat strategy.txt
files pass `U+FFFD == 0` (verified: deck139 = 0, deck152 = 0; frozen files unchanged from wave-29
which passed at author time).

## deck152 (NEW -- Bant Midrange GWU, Step-0 initial guide)

First guide for this seat. Content anchored to primitives (engine matched Scryfall for all 20+
cards, no divergence). Rule #1 = TRUST-THE-MANA-LINE / DO-NOT-RE-DERIVE (the deck's DOMINANT cost
this corpus was reasoning-tax + one mana-perception fabrication on a derived 3-color manabase);
ATTACK FLOOR keyed to board-readable facts, names the flyers (Elite Spellbinder, Sigarda) --
written despite wins looking like the deck already attacks, because the losing games
under-developed; TRUST-THE-RENDERED-[P/T] (Briarbridge's +2/+0 and [counters] render live). The
Rule #1 tail carries the N-152b interim ("if your only listed action is a cosmetic Flip Side
toggle, answer 0/pass -- do not invent a Cast number") and the N-152a interim (lead on Deserted
Beach or flip a Pathway before playing when you want Teferi). Deploy as a new file, `git add -f`.

## deck139 (REVISION -- diff-verified stopgap retirement, ~17 changed lines)

Two changes, both principled (the pipeline never exits a deck the wave its guide changed):
(1) STOPGAP RETIREMENT -- the wave-29 "RENDER TRAPS" block described renders the g3 fix ELIMINATED
("TARGET CHOICE for Pollywog listing your HAND"; "TWO adjacent lines" pile; "alternative cost"
synonym). Replaced with "READING THE MUTATE FLOW" that follows the SHIPPED render strings in order
("DISCARD ONE OF YOUR OWN CARDS"; over/under step; "[mutated pile - ONE merged creature]" one
line; sole "mutate cost" label). A byte-copy would have shipped prose contradicting the live
render -- a latent misteach even though no teach demonstrably misfired this corpus.
(2) DECIDE-COMBAT-FAST strengthened with the adjudication-clock rationale (the latency tail
relocated to combat math; games are time-capped and a slow seat gets adjudicated behind). Deck-fact
claims unchanged (verify-oracle: N-139e mutate-only, base-cost-from-`other=` all held). Deploy as a
revision, `git add -f`.

## Post-deploy verification (orchestrator)

- [ ] deck152 new file present; `git add -f` staged; cmp vs wave30/deck152/strategy.txt == identical.
- [ ] deck139 redeployed; cmp vs wave30/deck139/strategy.txt == identical; md5 == 8f4aebe...; diff
      vs the deployed 8e7131f... shows ONLY the stopgap-retirement + decide-fast lines.
- [ ] deck122/136/137/146/148 UNTOUCHED (md5 unchanged, cmp identical, no redeploy, no restage) --
      all byte-frozen; deck136 rotates OUT to the canary list with its guide frozen.
- [ ] `git diff | /usr/bin/grep -c U+FFFD == 0` on every edited/new Res guide (all pass at author time).
- [ ] No AI attribution in the commit (verify-oracle: all deck-fact numbers grounded before deploy).
