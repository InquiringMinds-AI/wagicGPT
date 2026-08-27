# deck146 - proposals for the strategy-writing skill (wave 48)

S-1. **A WORKED EXAMPLE IN A GUIDE IS A REPLY TEMPLATE, AND THE PILOT FILLS IT WITH NOTHING.**
Wave 47's 146-B added an accountability sentence with a worked example in quotes ("nothing right
now: the Command is entry 5, their board is empty and I control no creature ..."). This corpus
the pilot emitted that sentence verbatim on menus that had no Command row (`146 vs125` s83, s87).
Amendment 1 forbids quoting text you intend to override; this is its positive-side twin: any
sentence a guide asks the pilot to WRITE must be given as a shape with blanks the screen fills
(`entry <number> <card>; it did not match because <printed fact>`), never as a finished sentence.
A finished sentence is copied; a shape is instantiated. Cheap check: grep the corpus for any
sentence the guide contains in quotes; a verbatim hit in a reply on a screen where it is false is
the defect.

S-2. **THE THIRD-FAILURE ROUTING (#102) NEEDS A DIAGNOSTIC BEFORE THE REWRITE: WRITE DOWN THE FALSE
BELIEF THE REPLIES CONTAIN.** deck146's converter rule failed three corpora running with the
render complete. The replies were not ignoring the rule - they were computing it wrong in the
same way every time: "they gain 3 which Sanguine Bond drains from them, net 0". The pilot
believes the converter is aimed at its controller. No amount of "stay home" moves that; one
sentence that names the belief and refutes it in the fold's own words does. So the #102 step
"quote the exact wrong sentences" should be preceded by: read every violating reply for the
arithmetic it performed, and if all of them perform the same wrong arithmetic, the rule's real
defect is a missing fact, not a missing imperative.

S-3. **A "RUN THE CHECK" RULE FOR A REPEATABLE ACTIVATION NEEDS A TAPPED-STATE CHECK AND A
PAYMENT CHECK, NOT ONLY AN ALREADY-DONE CHECK.** The Hive rule had CHECK 0 (lethal), CHECK 1
(phase), CHECK 2 (already a creature) and went 3-for-3 on the Upkeep problem it was written for -
and then lost eight mana to a shape none of the checks covered: the source was tapped, and the
only mana to pay it was the other animated Hive. For any activated ability on a permanent, the
checklist enumerates the permanent's own printed state (`[tapped]`, `[summoning sick]`) and the
`Those sources` list, because both are rendered and the option row says nothing about either.

S-4. **"THERE IS ALWAYS A GOOD MODE" IS A CLAIM THE PILOT CAN FALSIFY FROM THE SCREEN, AND WILL.**
Two waves of restating that Silverquill Command is always castable produced 13 declines and then
7 Acererak-overs, and the s47 reply enumerates the modes and finds one live. A cast-list entry
for a modal card carries the modes' conditions as screen-readable facts (their line shows a
creature / your line shows a creature / the row names a returnable card), and the entry moves
in the order when none is true. Extends #57 (two menu paths need a verdict each) to the entry's
rank itself.

S-5 (measurement). **Count `answer_replaced` records per seat and read them: they are the
corpus's own record of the pilot arguing with a guide rule and losing or winning the argument.**
Nine on deck146 this corpus; six are Acererak-vs-Command, one is the Hive double-animation, one
is an ATTACK line the parser did not honour (E-1). They are the cheapest specimen set for #102's
"read the override argument" step.
