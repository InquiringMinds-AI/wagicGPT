# General (core-prompt) suggestions — deck131 reviewer, wave 4

Against the LIVE `bin/Res/ai/gpt/system_prompt.txt`. Evidence: 12 deck131 games across
`matchups-20260712-192832` + `matchups-20260712-230636` (2W/8L/2TO); details in
findings.md. Layer discipline applied per the brief: each item states CORE vs PER-DECK,
and the burden of proof is on CORE (every deck pays for each line).

## First: existing core lines observed WORKING in this corpus (don't touch)

- "Card draw is FUEL: cast it the turn you can afford it" — followed: 12 draw-spell
  casts across 12 games (~1.0/game vs 0.17 pre-wave3). The general line plus the
  positive deck-guide framing are jointly holding; no reinforcement needed.
- "If you cannot clearly tell who is the beatdown, be the aggressor by default" and the
  free-case attack floor — both wins (ep1783910629, ep1783922540) show turn-after-turn
  full attacks vs open boards, quoting the aggressor plan. The act-floor language works
  when the deck has action to take.
- The mulligan land-count default — no mulligan misplays observed in 12 games.

---

## G1 [CORE — rules-facts gap]. First strike breaks the "simultaneous damage" fact the prompt teaches

QUOTE (rules paragraph): "Combat damage is dealt simultaneously; a creature dies if
damage reaching it this turn is at least its toughness."

PROPOSAL (append to that sentence): "— except FIRST STRIKE: a creature with first
strike deals its combat damage first, and a blocker or blockee it kills deals NO damage
back. Blocking a first striker with a smaller creature kills your creature for zero
effect."

WHY: ep1783915599 T5 — a 1/1 Elemental token chump-blocked Ash Zealot (2/2 first strike,
`text=` verified) at 20 life: the token died before dealing damage, nothing was gained,
and the prompt's own "simultaneous" fact teaches the model that this trade deals 1 back.
The core prompt is the only layer that states combat rules facts, so a wrong fact there
poisons every deck's block math. One clause, universally true, fixes it.

## G2 [CORE — interface contract]. Bundled declarations: the reply is the COMPLETE list

GAP: the interface paragraph ("Every listed choice is legal AND PAYABLE...") covers cast
menus and lists-are-complete, but says nothing about how attacker/blocker REPLIES are
consumed. The strategy text says "attack with everyone" in two places, yet nothing tells
the model that an unnamed creature stays home.

PROPOSAL (add to the interface paragraph): "When a decision asks you to declare
attackers or blockers, your reply IS the complete declaration: every creature you do not
name stays home, and you will not be asked again this combat. Go through the listed
creatures one by one and name every one you mean to commit — replying with only your
best creature is a common way to attack with one when you meant all."

WHY: ep1783920139 T31 — board of 2 Young Pyromancers + Guttersnipe + Elemental tokens,
declaration "Guttersnipe" alone, repeatedly across turns. "Attack with everyone" was in
force at both the core and deck layer and was satisfied, in the model's reading, by one
attacker. This is a property of the ASK FORMAT, identical for every deck and both combat
seams — that makes it core-layer by construction (a per-deck fix would be seven copies
of the same sentence).

## G3 [CORE — combat guidance]. Give blocking a decision procedure; the current line assumes the trade-math the model doesn't run

QUOTE (combat bullet): "When blocking: block when the trade is favorable or free, and
chump to stay alive when an unblocked attack could kill you or take you dangerously low;
take the damage when your blockers are worth more alive than the life saved."

PROPOSAL (replace with): "When blocking: 'no blockers' is a normal and often correct
answer — taking damage is fine while your life comfortably exceeds the incoming total.
Block when you can name the specific gain: the block kills the attacker without losing
your creature (check the attacker's rules text first — first strike, deathtouch, or
destroy/damage riders turn a 'favorable' block into a loss), or it prevents damage that
threatens your life total. A 0-power attacker deals nothing — blocking it gains nothing.
Chump-block only when the unblocked attack would kill you or leave you at 5 or less, and
chump with your least valuable creature — a creature your strategy guide names as a win
condition blocks only to prevent lethal."

WHY: 4 of 12 games showed reflex-blocking the current line permits: Young Pyromancer
blocked Boreal Druid (0-power mana dork — zero damage prevented, ep1783908913 T6);
Guttersnipe blocked Ohran Viper (1/1 whose `text=` destroys the creature it damages —
the "favorable trade" heuristic without a rules-text check traded the deck's kill
condition for a utility snake, ep1783915602 T16). The current wording states outcomes
("favorable," "worth more alive") that require exactly the evaluation the executor
skips; the replacement converts them to checks it can run (read the attacker's text,
compare power to 0, life threshold at 5). LAYER ARGUMENT: the block-by-reflex DEFAULT
and the rules-text check are model behavior + rules facts — deck-agnostic, so CORE. The
POSTURE ("your creatures attack, they do their blocking by racing") is role-dependent —
a control deck should block — so it stays PER-DECK, and my deck131 guide already
carries it. Both layers are needed; neither substitutes for the other.

## G4 [STAYS PER-DECK — recorded so synthesis doesn't promote it]. "Creatures attack, don't block" is the wrong layer for core

The headline fix in my deck131 guide (attack with everything; default 'no blockers';
win condition never blocks) is a RACING deck's rule. Promoted to core it would misplay
every control and midrange deck in the pool — blocking is their correct plan. G3 above
is the deck-agnostic floor (don't block for zero gain, check rules text, protect named
win conditions); everything stronger is posture and belongs in guides.

## Considered and NOT proposed

- Reactive-spell durdle (casting utility artifacts while dying): observed, but traces to
  deck131's threatless-hand mode (findings.md #1) — deck-construction/deck-guide issue,
  not a core-prompt defect. The core prompt's hold-interaction language already carries
  the wave-3 release valves and was not the cause here.
- Priority-window pass rates: deck131 passed most of its many priority windows, and spot
  checks showed the passes were mostly correct (nothing useful to do). No evidence of a
  core-prompt defect; do not add text.
- Lifegain/timeout handling ("only race if you win the race" vs an opponent at 90 life):
  the model had no alternative line available — deck power, not prompt. No text.
