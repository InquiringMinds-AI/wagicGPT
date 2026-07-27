# deck152 wave-30 -- core (general prompt) routing: PASS (25th consecutive)

Verdict: the general prompt routes to PASS at this seat -- the 25th consecutive PASS (wave-26=21st,
27=22nd, 28=23rd, 29=24th, 30=25th). No deck-agnostic defect at deck152 that the core should own;
every leak found is either deck-specific (goes to the guide) or representation (N-152a/b in notes).

Reasoning, per the layer-routing discipline:
- The one hard fallback (vs136 seq27) is a PERCEPTION leak -- the model re-derived mana and
  OVERRODE the accurate "Mana available: {w} from 2 untapped sources" line, fabricating an untapped
  Forest and treating a hand card as a source. The corrective FACT (trust the rendered mana line;
  a land in hand is not a source; if it is not in your Cast options you cannot cast it) is already
  the established TRUST-THE-RENDERED-NUMBER rule and belongs in the DECK guide (Rule #1) for a
  derived-3-color-mana deck; it is not a new core sentence. Single-seat perception evidence does
  not cut or add a core line.
- The bare-priority Flip-Side-only window that TRIGGERED the fabrication is a representation seam
  (N-152b), not a core-prompt matter -- the general prompt already says pick a number from the list
  and 0 to pass; the fix is to stop presenting a cosmetic-only "action", not to add prose.
- Coven reasoning tax is deck-specific (a named mechanic on 3 of this deck's cards); pre-answered
  in the guide, not core.
- No cross-deck agreement observed this wave that would justify touching core text. Do NOT mint a
  core line off this single seat.

Nothing to promote upstream from deck152 this corpus. Core PASS.
