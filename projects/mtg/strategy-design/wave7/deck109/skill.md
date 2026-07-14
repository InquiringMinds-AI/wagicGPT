# Proposed revisions to the strategy-writing skill (from deck109 wave-7)

**Net: no changes proposed. The skill's machinery held up; this seat is again a validation.**
One small pattern is worth flagging for the synthesis agent's attention, not a method rewrite.

- **OBEYED-BUT-LOSING + ceiling-sizing drove the right call on a RECORD DROP.** deck109 fell 5/6->4/6,
  exactly the "falling record on n<=6 against a harder slate" the skill warns is variance, not a broken
  guide. Both losses attributed to matchup + variance (obedience metrics all healthy: 25/25 attacker
  commitment, all burn face, 0 hoard, 0 durdle beyond one marginal pick). The one model-fixable lever
  (a single decline-a-listed-body misplay) had a near-zero ceiling (the game was lost to unblockable
  flyers regardless), so per "size the residual lever's ceiling before the #1 slot" it became a folded
  reinforcement of an existing line, not a rewrite. The skill steered this correctly end to end.

- **The stopgap-DELETION regression check is a real, distinct audit and should be named as such.** Wave 6
  DELETED deck109's fallback rule (after the engine fix). Wave 7's mandated job was to confirm the
  deletion caused no regression -- specifically that the deleted phantom reasoning did not return. This is
  the mirror of the skill's existing "verify a shipped fix FIRED" rule: after you DELETE a stopgap, grep
  the next corpus for the STRING of the reasoning it used to produce (here: "missed cast" / phantom
  cast-happened framing) and confirm it stays at zero. It did (0 across 6 games). Consider adding a one-
  line companion to the stopgap-lifecycle triple: *"a deleted stopgap gets a next-wave regression grep
  for the misreasoning it patched, same as a shipped fix gets a fired-check."* Minor; the discipline was
  already inferable, but naming it would make the deletion audit non-optional.

- **A new matchup class worth a named skill example: the UNTOUCHABLE BOARD** (opponent's whole threat
  base is unblockable by AND untargetable by your deck -- flyers+shroud vs a ground/burn deck; pro-red
  artifact creatures vs mono-red). It reads like a play problem but is a deck-construction/meta fact: the
  guide has no lever except "race harder," which it already does. This is the evasion cousin of the
  Phyrexian Obliterator named-matchup note. Not a method change -- just a good exemplar for the "route to
  deck construction, don't write louder prose" bucket.

No method line was found wanting; nothing to cut. The two additions above are optional sharpenings for
the synthesis pass, not corrections.
