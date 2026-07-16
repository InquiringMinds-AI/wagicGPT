# Deck-110 wave-17 — cross-seat / core-prompt observations

(Single-seat items cannot add/cut core-prompt lines; these are routed for the synthesis pass to
weigh against other seats' evidence.)

## 1. CROSS-DECK ENGINE FIX (highest leverage remaining): suppress no-op mana-ability activations from the GPT priority menu.
The E6 item is not deck110-specific — it is any mana rock (Mox Opal, and by extension any
`{T}: add mana` permanent) at any seat. The engine currently surfaces a standalone mana-ability
activation as a top-level priority action labeled `Cast Card Normally with <source> {source's own
rules text}`. With no spell being paid for, choosing it is a no-op (the prompt already auto-taps for
casts). When it is the SOLE option, it forces the model to either mis-echo an off-menu card or
ramble ("This is a puzzle") — 5 of deck110's fallbacks this wave. **Recommended engine change
(owner `AIPlayerGPT.cpp` option serialization):** do not list bare mana-source taps as GPT options
when no cast is in progress; if they must appear, relabel as `Tap <source>: add one mana of <color>`
and never put the source's reminder text in the `{card text:}` (card-being-cast) slot. This is
behavior-neutral (it removes only no-ops) and would cut fallbacks at every seat that runs mana
rocks. Repros: deck110 vs deck17 s24/s25/s28/s29, vs deck49 s34.

## 2. VALIDATED cross-deck WIN — the PLAN-carry stale-intent caveat.
The wave-16 caveat ("...the actions your plan names are no longer among the options available right
now...") is doing its job at the seat that produced the wave-16 self-blast repro. At deck110 it
appears on ~33% of decisions and the model re-derives to a valid on-menu option ~93% of the time;
the self-blast and the 14k-char contradiction-spiral subclass both vanished. The corpus-wide unparsed
halving (40→18 per the brief) is consistent with this. Its one non-rescue mode is when the sole
remaining option is itself unreadable (E6) — i.e. the caveat can't fix a menu bug, which is
suggestion #1's job. No caveat text change suggested; flagged as a confirmed keeper for whoever owns
the prompt-assembly core.

## 3. Observation (no action): the model repeatedly and correctly DIAGNOSES the E6 mislabel in-prose but cannot act on it.
At deck17 s24 the reply reasons "Maybe the option 'Cast Card Normally with Mox Opal' is actually
'Activate Mox Opal' but the game is mislabeling it?" — a correct read — then still fails to emit a
usable choice because there is nothing sane to map to. This is evidence the failure is purely
representational (fixable in the engine), not a reasoning or knowledge gap, and that no amount of
guide text will close it. Reinforces routing E6 to the engine, not any prompt/guide surface.
