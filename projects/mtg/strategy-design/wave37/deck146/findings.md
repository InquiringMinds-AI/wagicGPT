# deck146 "Orzhov Dungeons" — wave-37 guide revision (mandate: wave36/validation-146-139.md)

Constrained diff over the deployed wave-35 guide (verified byte-identical baseline). Mandate:
the three combat-value teaches misfired where armed; validated text stays verbatim. Evidence
corpus: `matchups-20260822-155508`, deck146-vs-deck152 log, seqs re-read directly (s23-s25,
s26, s29). Frozen doctrine (Lost Mine, Verse, Kaya tree, Acererak, Soul Shatter, mulligan +
bottoming, Agadeem, planeswalkers, MDFC) untouched — confirmed by diff: only the three edit
sites and their DECIDING SITUATIONS mirror lines changed.

## Edit 1 — hold-back trigger reframed as a HARD GATE at the attackers ask

Evidence: s29 (t16, 6 life) — board renders Katilda (3/3) and Wolf (4/4) BOTH marked
`[tapped - cannot attack or block this turn]`; the pilot computed 7 power, noted "but tapped",
and attacked. The old rule mentioned "the tapped ones too, they untap" but read as blocking
advice ("HOLD BACK BLOCKERS") and never framed tapped-now as irrelevant.

Changes (HL2-conformant):
- Retitled and re-scoped: "HOLD-BACK GATE - RUN THIS CHECK AT EVERY 'declare ALL attackers'
  ASK, BEFORE PICKING ATTACKERS. It is a hard gate, not advice."
- The tapped exclusion is now an explicit inclusion anchored to the render's own marker string
  (`[tapped - cannot attack or block this turn]` — verbatim from s29's prompt) plus the direct
  refutation: "'Their creatures are tapped' is NEVER a reason your attack is safe - the attack
  that kills you is NEXT turn's, and nothing of theirs is tapped then."
- Content name per HL2: the sum is THEIR TOTAL POWER (no bare variable); anchored to "the
  first number in each '(P/T)'".
- **Deliberate deviation (correctness fix): "printed power" -> "current power".** The old
  sentence said sum PRINTED power; at s29 printed is 1+2=3 (both creatures counter-pumped)
  and the rule as written would NOT have fired at 6 life — only the CURRENT sum (7) does. The
  pilot in fact summed current power. "Current" also agrees with the frozen "THE BOARD RENDER
  ALREADY DID YOUR COMBAT MATH" teach two paragraphs down.
- Added the mechanism ("a creature that attacks now is tapped through that next-turn swing and
  blocks nothing") so the gate and the attack cost are one causal story.

## Edit 2 — chump teach anchored to the rendered forecast number

Evidence: s26 (10 life, render: "Unblocked, these attackers deal up to 4 - you would be at 6")
— declined the Silencer chump on "Block: I take 4 / No block: I take 4. So blocking is
strictly worse." The old teach stated the fact but never attacked the self-derived
both-ways damage model.

Changes (HL2: name the number by its render substring):
- The forecast is named: "the number after 'you would be at' is YOUR LIFE AFTER if you block
  with NOBODY" — the no-block price, not an invariant.
- The false model is named and forbidden verbatim: "NEVER write 'I take the damage either
  way': against a non-trampler that is false, and it is the exact reasoning that loses these
  games."
- The action trigger keeps its validated form (forecast <= 10 -> chump the biggest
  non-trampler with the cheapest creature), now reading the same anchored number.

## Edit 3 — Command TIMING: the main-2 anti-pattern named

Evidence: s23-s25 (t14, MAIN 2, Silencer summoning-sick): cast Command, chose pump+sacrifice,
plan "Pump Silencer to 6/5 ... I will attack with Silencer next turn" — the old sentence only
banned "casts in second main and then attacks" (same-turn), which this plan sidestepped by
deferring the attack a turn.

Changes:
- Duration made load-bearing: "the +3/+3 lasts only UNTIL END OF THIS TURN."
- The anti-pattern named per mandate wording: "A MAIN-2 PUMP BUFFS NOBODY: there is no combat
  left this turn, and the attack such a plan imagines is NEXT turn - by then the pump is gone
  and the creature is back to its printed size." Both plan shapes banned ("pump now ... attack
  next turn" and "second main and then attack").
- Constructive alternative added (the mode menu forces choosing TWO; s24's menu had option 8):
  after combat / no attacker available, skip the pump mode and pair sacrifice with draw. The
  s23 play's real value WAS the sac 2-for-1; this converts the wasted half instead of banning
  the cast.

## Mirror updates (DECIDING SITUATIONS)

Three recognize->do lines updated to match: total-power line gains "(count EVERY creature -
the tapped ones untap for their turn)" + "Their creatures being tapped never makes attacking
safe"; chump line re-anchored to "The forecast's 'you would be at' number" + "you never 'take
it either way'"; Command main-1 line gains "A 'pump now, attack next turn' plan is always
wrong: the +3/+3 ends this turn."

## Not changed

Validation's defensible divergences (s42 lethal push, s35 partial hold-back, s40 Hive-only
race) fit the revised gate without carve-out text: lethal pushes only pass the arithmetic
when blocking cannot save the pilot anyway, and the animated Hive is not a next-turn blocker.
The unexercised mulligan/bottoming and Agadeem teaches stay verbatim per mandate. The "at any
X" wording defect on Silverquill's returns-NOTHING annotation is engine-lane (validation item
2), not guide text.

NOT DEPLOYED — revision lives here pending wave-37 step-4 apply.
