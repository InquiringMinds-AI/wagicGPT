# Skill contribution — deck135 (Modern Snow / Snow-Force control) seat, wave 18

Per-seat proposals for the synthesis step to fold into `strategy-writing-skill.md`. deck135 is a
FROZEN veteran (guide unchanged since wave-11; frozen again this wave, `cmp`-verified). No guide
change proposed. The value this seat adds is METHOD: one framing validation, one annotation-design
input, one new reviewer instrument. All are measurement/observation tools (not guide-content
changes), so acceptable as single-seat contributions per existing doctrine.

## CONFIRMS (promote from watch): the TARGET CHOICE sub-menu framing is a CONTROL-DECK UNLOCKER, and it CLOSED the target-sub-menu seam deck49 raised in wave-17.

Wave-17's deck49 watch named the TARGET SUB-MENU as the dominant idiosyncrasy of any deck whose
plays target something, and the wave-17 binary shipped the framing ("this spell/ability is already
on the stack and needs a target — it is NOT a cast or phase step; answer with the TARGET's name").
deck135 is the seat that most exercises this seam (fetches + ETB taps + Gelid Shackles all route
through it). Wave-18 result AT THIS SEAT: **8 target decisions, 0 stale_echo, 0 wrong pick** — vs
2 stale_echo on the identical Into-the-North sub-menu last wave. The framing did not merely stop
the fallbacks; it produced the RIGHT high-value picks that WON games (vs110 adj: Treefolk-ETB-tap
-> Signal Pest, Gelid Shackles -> Master of Etherium 12/6, the two biggest threats, correctly
chosen). Method note for Step 0-ter / the target-sub-menu watch: for a control/removal deck, the
framing is now a CONFIRMED seam-closer — a guide's target-choice teaching can be lighter where the
engine framing is present, and reviewers should score target-decision QUALITY (right target chosen),
not just fallback absence, when validating it.

## NEW annotation-design input (per the owner ruling replacing beneficial suppressions with option-line warnings): warn on OFF-WHITELIST counter targets, and NAME the pitch card.

The reflexive-Force class (a weak pilot Forces a low-impact noncreature at a 2-for-1) is 4 corpora
durable and mutated this wave to a pump/trample ENCHANTMENT (Primal Rage) and a single-target BOUNCE
(Unsummon) — neither on the guide's NEVER-list. Two design inputs for the incoming warning
annotation:
1. **Warn when the counter's target is off the deck's whitelist** (for a free-counter deck: a pump/
   anthem/trample enchantment, a single-target bounce that returns a recastable creature, face burn,
   discard — all "let it resolve"). This catches exactly the bad-but-legal casts the model
   rationalizes; the guide prose alone has not (the model quotes the general rule and Forces anyway).
2. **Name/warn WHICH card the free alt-cost will exile**, especially when it is the caster's ONLY
   card of the pitch color or its finisher (vs62 s12: the free "exile a blue card" pitch silently ate
   Abominable Treefolk — the deck's only real closer — because it was the only blue card, while the
   model believed it was pitching a green Into the North). The option line currently hides the true
   cost; the model commits to the counter without knowing it loses its finisher.

General lesson for pitch-cost / choose-and-discard options: surface (or annotate) the CARD the cost
will consume when the engine auto-selects it — the model cannot weigh a trade whose price it cannot
see.

## NEW reviewer instrument (single-seat, measurement-only): the REPEAT-LOOP unparsed signature, distinct from the PLAN-caveat stale-plan spiral.

The seat's residual `unparsed_reply` fallbacks are no longer plan-drift; they are decode-time
REPETITION LOOPS — the model repeats one or two sentences dozens-to-hundreds of times until the
token cap truncates the reply, leaving no CHOICE line. Cheap, countable signature for reviewers:
- **Detect:** an `unparsed_reply` whose text contains a short phrase repeated many times (e.g.
  "I must cast X... No, I need Y. I don't have one." x100; "So I need to pass... But they will." x60).
- **Distinguish from the PLAN-caveat class:** a repeat-loop carries NO stale prior-turn PLAN — it is
  a fresh spiral, so the caveat's stale-intent fix does not apply and its ABSENCE from the caveat
  class is the expected, healthy signal (confirmed this seat: 0 stale-plan spirals).
- **Trigger profile:** LOW-LIFE or ambiguous-board spots on a phase-restricted / binary / trigger
  menu where the model recognizes a hopeless or confusing situation (all 3 this seat: 5-life vs
  lethal with no creature; opp-upkeep with only a fetch offered while facing a scary board; a combat
  log whose "life -1 (now 2)" duplicate rendering confused a Draw/Decline trigger).
- **Severity gate:** score by whether the loop landed in a WINNABLE game. This seat's 3 all landed
  in already-lost or post-adjudication spots — 0 flipped a win — so the class is a cosmetic/tax item,
  not a result-changer, and routes to model/decode watch (repetition-penalty / max-token guard), NOT
  the guide. A parsed-but-degenerate loop (choice still emitted after a 15x repeat) is the same
  pathology below the truncation threshold — count it too; it is the broader tax.

Sibling to deck35's 0-attacker durdle instrument: a per-game COUNT (loop-fallbacks, and their
life/board context at fire time) is cheaper and more objective than re-reading the prose.
