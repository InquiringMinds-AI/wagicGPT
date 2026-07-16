# general-suggestions.md — deck49 (Dragons) core-prompt proposals (layer-routed)

## G1 (CORE PROMPT candidate) — explain the TARGET SUB-MENU once, in the core, not per-deck.
The target-sub-menu confusion (findings C2) is NOT deck49-specific — it fires for ANY deck that casts
targeted spells (burn, auras, removal, pump), which is most decks. deck49 merely concentrates it
because nearly all its plays target something. All 7 of deck49's stale_echoes and both unparsed loops
live here. This is the correct layer for a one-time interface explanation rather than a line duplicated
into every guide. Proposed core addition (wording for the synthesis agent to calibrate):
  "Some spells need a target. When you choose to CAST such a spell, the NEXT menu you are shown lists
   the possible TARGETS (a player 'The opponent (player, life N)', or a creature). You have already
   committed to the cast — on the target menu, pick the target you want by its number. There is no
   'cast nothing' on a target menu, and you should not re-state the spell's name; just choose the target."
Route: CORE PROMPT. Rationale per layer-routing: cross-deck interface fact, belongs above the guides.
Keep the per-deck guide's rule-2 as reinforcement for spell-heavy decks until this ships and is verified
fired (grep the next corpus for a drop in target-sub-menu stale_echoes / tax).

## G2 (already-known corpus-wide, restate as convergence not a new ask) — the mana false-belief.
deck49 adds a third mono-color seat to the "I have 0 mana / my land is tapped" false-belief (findings
C1). The representation lever ("Mana available: {r}{r} from N untapped sources") is already shipped and
present in the prompts; the belief still spikes (deck135 s4, 11.7k tax, fabricated a mana-lock straight
over the surfaced line). No new core ask — flag for the synthesis agent that the surfaced-mana line is
being reasoned-AROUND on mono-color decks, so the per-deck rule-1 trust-line is still load-bearing and
must NOT be demoted yet (the representation is present but not yet winning against the false belief).

No other core proposals — the core already races reasonably (burn goes face by default in this corpus)
and handles attacker eligibility correctly. Everything else is deck-knowledge (the guide) or engine
(notes.md).
