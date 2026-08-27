# deck126 - render / core-prompt proposals (wave 49), layer-routed

Engine bugs (the Tribute-with-no-sacrifice event, the mana-only leak) are in
`wave49/seats/seat-125-126.md`; these are render/prompt items.

G1 (MED, render) - the owner-side LOOP clause names the victim's fate, not the owner's trigger.
The lane-U sentence on the deck126 prompts reads "Both halves of a life LOOP are on YOUR
battlefield (Sanguine Bond + Exquisite Blood): Exquisite Blood turns every life the other player
loses back into life for you, which Sanguine Bond turns into life ..." (`...vs-ai_baka_deck152.jsonl`
seq 24). On the deck125 side the same clause ends "so ANY nonzero payment on a tag above is
fatal". The owner-side clause could end the same way from the owner's chair: "any row above
that gains you life, or costs them life, ends the game" - the seat that held both halves for
three windows with `Cast Tribute to Hunger {right now: they control 7 creatures}` on the menu
(seq 24-27) waited for a combat. Guide F10 carries it meanwhile.

G2 (LOW, render) - `{paying this taps: <creatures> - they cannot attack this turn}` never fired
on the deck126 seats (0/6 games) although Overgrown Battlement was a mana source in most
casts; either the payment order never reached the Battlement (no `Overgrown Battlement [tapped]`
was found on any opponent-turn battlefield line of the six logs) or the clause is suppressed for
defenders. If the latter: a tapped wall cannot BLOCK, which for this deck is the whole cost -
propose the clause read "they cannot attack or block until your next untap" and fire for
defenders too. UNTESTED which of the two it is.

G3 (LOW, render) - a cast row for a permanent whose same name is already on the caster's
battlefield line could say so: `Cast Exquisite Blood {4}{b} {you already control 1 - a second copy
adds nothing}` for non-stacking enchantments/artifacts (Exquisite Blood, Sanguine Bond, Chromatic
Lantern). `...vs-ai_baka_deck162.jsonl` seq 12 (second Blood read as "both enchantments") and the
four second-Lantern casts (`vs130` seq 34; `vs125` seq 73, 78, 100) are the evidence. The engine
knows the stacking answer only per card, so the annotation could be just the count.

G4 (INFO, narration) - reveal-zone residue lines: "Your revealed Savannah went to your hand" x2
printed at the resolution of a Tribute to Hunger one turn after the opponent's Pelakka Predation
(`...vs-ai_baka_deck146.jsonl` seq 14 events). This is the visible symptom of seat-file HIGH #1
(the stale reveal zone), not a separate narration item.
