# deck102 dev notes - wave 21 (engine/harness/representation ledger + rotation verdict)

## CLOSED items (wave-20 ledger, validated this corpus)

### E1 (wave-20) -> item (e) reveal framing: CLOSED / VALIDATED at deck102.
The wave-20 E1 (targeted-discard reveal misrepresenting a pick-ONE effect) shipped as wave-21 item (e).
Both of this seat's reveal records confirm the fix end to end:
- Framing now names whose hand and pick-EXACTLY-ONE: "The opponent revealed their hand (N cards). Choose
  the ONE card to send to 'choose discards' ... every other card stays in their hand." + "PUT: followed by
  the ONE card number you choose". The wave-20 "top N cards of your library" mislabel is GONE.
- Over-selection -> 0/2 (wave-20 was 2/3). vs14 s7 is the SAME hand that over-picked both Boomerangs in
  wave-20 and now picks exactly one (Boomerang).
- Zone-outcome read clean: vs27 s10 events `Opponent's Glen Elendra Liege: hand -> graveyard`; vs14 s7
  events `Opponent's Boomerang: hand -> graveyard`. Named card leaves the hand exactly once; the rest
  return to hand. No wrong-zone drop.
Repros: vs27 s10, vs14 s7. Recommend closing E1 in the master ledger.

### E2 (wave-20) -> item 8 residual (c) cast-stall loop: NOT FIRING at deck102.
Scanned all 6 games: no repeat-pick of the same cast across consecutive asks, no floating-mana
("Already in pool") coincident with a re-offer. vs62 (wave-20's Dreadhorde-Invasion-x3 game) casts it
ONCE (s6) this corpus. The item-8 counting fix (a) + plan-abort (b) are confirmed at this seat; residual
(c) (the vs62-Myr acting-player / stale-pointer path) did not surface. Deck102 does not, by itself,
discriminate (c)'s mechanism - it just did not occur. Keep (c) open for the probe, not for this seat.

## NEW representation item

### R-PAINLAND (MEDIUM, cross-deck) - self-damaging mana sources hide their life cost.
Ancient Tomb and City of Traitors deal 2 damage to their controller on every tap for mana, but the
decision-facing "Mana available: {b}{b} from N untapped sources (tapped automatically when you cast)" line
and the cast-option cost render show NO self-damage. The model cannot price the tap, and it self-killed:
- vs27: `Damage: 2 dealt by Ancient Tomb to you` fires at s14 (L12->10) and again at s21-22 (L3->1) - the
  model cast Damnation via Ancient Tomb at 3 life, dropped to 1, and died at -1 the same turn.
- vs14: 5 painland self-damage events; vs35: 2. (vs49/62/135: 0 - variance in which lands were drawn.)
Proposed durable fix (representation): annotate self-damaging mana producers in the mana line and/or the
cast option cost - e.g. "Mana available: {b}{b} from 2 untapped sources (Ancient Tomb deals 2 to you when
tapped)". Generalizes to any painland/painful mana source (City of Traitors, Grim Monolith-style, filter
lands with life costs). Interim guide crutch shipped this wave (strategy.txt "MIND YOUR OWN LIFE FROM YOUR
LANDS"). Repros: vs27 s21-22 events; vs14 s19 events.

## Corpus-construction / deck-content observations (not engine bugs)

- Tergrid steal engine STILL unexercised (0/12 games across two corpora). vs27 drew Tergrid on turn 14 but
  the game had ended turn 13 - drawn, never cast. The Tergrid discard/sac STEAL trigger has never fired in
  a real game. Recommend a probe deck (Tergrid + Hymn/Thoughtseize/Smallpox/edicts + an opponent deck with
  cheap permanents) per the skill's probe-deck pattern to validate the steal seam directly, rather than
  waiting on the 1-of. Routed to whoever owns engine-seam validation.
- deck102 mana base runs non-Swamp utility lands (Geier Reach Sanitarium, Strip Mine, Ancient Tomb, City
  of Traitors) that slow or damage the black manabase; vs62's fast loss correlates with a
  Sanitarium+Strip-Mine opener. Deck-construction observation only (routed to the roster owner) - the
  guide already tells the pilot to keep 2-4 Swamps at mulligan.

## Model-behavior observation (context for synthesis, not an engine item)

- Answer-first protocol (item b) VALIDATED here: 0 fallbacks vs wave-20's 6, and all p95+ replies
  (14,310 / 14,243 / 13,867 chars) lead with a complete CHOICE: line before truncation. The behind/stalled
  giant-reply TAX persists (long deliberations still occur at the same spots) but no longer converts to a
  parse failure. No answer-quality regression from committing before reasoning.

## ROTATION VERDICT
**NOT a rotation candidate.** deck102 is on its FIRST GUIDED corpus (Step 0-ter) - a second-pass seat with
a fresh guide, which the standing test says is rarely a candidate. It also generated real targeted guide
edits (three: retire the stale Thoughtseize crutch, sharpen the symmetric-edict clause, painland-life
crutch) = a new-work signal by construction, and its keystone engine payoff (Tergrid steal) has never once
been exercised, so the guide's central engine block is un-live-tested. Re-validate next wave (Step 0- quater):
does edit 1 keep pick-ONE clean now that the guide no longer describes the old mislabel; does edit 2 cut
the marginal symmetric-edict-into-evasion cast; does edit 3 reduce painland self-death; and (probe-deck
permitting) does the Tergrid steal actually fire and get prioritized.
