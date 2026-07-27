# deck152 -> general (core) strategy prompt: **PASS** (26th consecutive)

No core-prompt change proposed from this seat this wave.

Reasoning, per the layer-routing discipline:

1. **The seat's decision quality was clean at the core-owned seams.** 0 fallbacks in 243
   decisions (wave-30: 1 unparsed). No off-list answers, no phantom options, no stale-plan
   commit, no parser contradictions. The core's existing floors ("every listed choice is legal
   AND payable", "never answer an unlisted number", trust-the-rendered-number) were obeyed
   throughout -- including at the two seams the wave-30 corpus broke them at.

2. **The one real piloting failure is deck-specific and already core-covered.** deck146 seq21
   t13 and seq26 t15 declined offered creatures on a chump-block fear ("Cast nothing right
   now" with four casts offered at 1 life). The core already forbids declining a listed action,
   and the skill's REJECTED appendix has turned down "tighten the phantom-lethal / plan-from-
   list core lines" for this exact family twice on single-seat evidence. The correct home is
   the deck guide, where it now lives as a hard deploy floor (Rule #3). If a second seat this
   wave reports a low-life creature-decline against an offered body, that is the promotion
   trigger -- I am not claiming it from here.

3. **The two findings with real magnitude are both below the prompt layer.** N-152c
   (castable-but-not-offered Sigarda: strict `potentialMana` credits one mana ability per card,
   so a two-ability dual land contributes only its first color) and N-152d (the `(printed X/Y)`
   delta reads the FRONT face of a transformed DFC) are an ENGINE defect and a REPRESENTATION
   defect. Neither is fixable by prompt text, and core prose written against a contradicting
   surface loses -- routed to notes.md with seq repros and code pointers.

4. **h4 and h5 both landed**, so the two representation items this seat carried into the wave
   are closed or reduced without any prompt-side compensation: the MDFC land annotation is
   truthful (48 occurrences of the new text; old text gone from all land lines) and
   display-toggle-only priority windows no longer reach the model (121 -> 0; priority asks
   148 -> 22).

One observation recorded WITHOUT a proposal, for the synthesis agent's convergence watch:
deck148 seq17 emitted `BLOCKS: B2:A1, B2:A2` as its first line -- one blocker on two attackers
-- three lines below the prompt's `Assign each blocker to AT MOST ONE attacker`, then
self-corrected in prose. First-wins parsing applied `B2:A1`, which is also what the corrected
reasoning chose, so the outcome was right. Single benign occurrence at this seat; the reply-
protocol answer-first/reason-after tension it illustrates is already a standing multi-seat
WATCH resolvable only by a harness A/B, never by prompt-body text. Not a proposal.
