# deck123 — strategy-writing-skill proposals (wave 58)
Current edition read: `wave57/strategy-writing-skill.md` (amendments 1-329).

## S1 — TWO UNSCOPED PLAN-LINE RITUALS IN ONE GUIDE COLLIDE, AND THE ONE THAT LOSES IS THE ONE WHOSE RULE THEN NEVER FIRES. SCOPE EVERY RITUAL TO A NAMED WINDOW CLASS, IN THE RITUAL ITSELF.

Amendment **#181** already says a ritual is a different instrument from a routing rule and must be
gated on an observable that can change an answer. This is the next failure past that gate: two
rituals that individually satisfy #181, in the same file, competing for the same one line of the
reply — and neither one saying which windows are its own.

deck123's guide mandates both:

- Rule 1: `PLAN: L <opponent life>, C <their creature count>, stop <L+C+3>; M is <M> now;
  this window: <x<N> / pass>` — introduced with "carry ALL FOUR numbers in your PLAN line".
- Rule 3: `PLAN: Damnation: N <their number>, K <able to attack>, M <mine>; <cast / hold>` —
  introduced with "**Whenever** 'Cast Damnation' is on the menu".

Neither says which ask it belongs to. What the pilot did (wave-58 corpus,
`...-ai_baka_deck123-...-vs-ai_baka_deck146.jsonl`): at **s22**, turn 14, own life 9, zero
creatures of its own, on a casting menu with **no token row at all**, facing
`Cast Damnation {2}{b}{b} {right now: destroys 4 of their creatures (4 without a restriction
against attacking), 0 of yours}`, it wrote

    PLAN: L=22, C=3, stop=26; M=0 now; this window: cast Idyllic Tutor

— Rule 1's ritual, the one it had been writing for windows on end, on a window Rule 3 owned. It
therefore never performed Rule 3's count, and the guide's own top cast-priority entry (CL0:
"0 are creatures + K ≥ 2 → Damnation, **nothing else first**") never got evaluated. It repeated
this at **s23** (life 7, `destroys 5 ... 0 of yours`, cast a Thraben Doomsayer) and was dead at
s26. The same non-evaluation at `...vs-ai_baka_deck152.jsonl` **s12** cost the vs152 game at
5 life with two Damnations in hand.

The tell that this is ritual displacement and not a routing failure: at s22 the pilot **wrote
`M=0`** — the exact number Rule 3's rule consumes, under the wrong ritual's label, and drew
nothing from it. The number was on the page; the rule that reads it was never entered.

**Rule to add.** A guide may carry more than one mandatory PLAN ritual only if each ritual states,
in its own text, the window class that owns it *and* what to write instead on the others ("this
shape belongs to the token-activation window; a casting menu takes Rule 3's line"). Prefer ONE
ritual per guide; where a second is unavoidable, make the rarer, higher-stakes one
(here: the sweeper count, which is checked once or twice a game and decides the game) explicitly
displace the frequent one — the frequent ritual is the one carried forward by habit, and habit
wins any collision the text leaves open.

**Diagnostic for future waves.** Grep a deck's replies for its ritual labels and cross-tab against
window class. A ritual appearing on windows its rule does not govern is not harmless verbosity: it
is the receipt that the OTHER rule was skipped, and it is visible in the translog before the
game is lost.
