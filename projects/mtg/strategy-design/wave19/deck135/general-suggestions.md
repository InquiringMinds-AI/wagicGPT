# Deck-135 wave-19 — general suggestions (cross-deck / model / corpus items; NOT guide text)

Routed OFF the guide per layer-routing doctrine. deck135-observed but general in nature.

## #1 (MODEL, carried) — reflexive off-whitelist Force of Negation (2-for-1 on a low-impact noncreature).

5th corpus. This wave: d62 s19 Forced **Gaea's Anthem** (a +1/+1 team anthem) — NOT a wipe / mass-bounce
/ only-threat-kill (the guide whitelist). Same over-application as wave-18's Primal Rage. NON-FATAL.
The wave-18 finisher-eating half is now FIXED (the option names the auto-pitch pool; the engine pitched
Search for Azcanta, a dig, not the Treefolk finisher — see notes ENGINE-1d-followup). Remaining is only
the target-selection over-application, best handled by the incoming OFF-WHITELIST-TARGET option-line
annotation (owner ruling): flag when the counter's stack target is a pump/anthem/trample enchantment, a
single-target bounce, face burn, or discard (all "let it resolve"). Do NOT add a 4th Force line to the
guide (already-maximal; prior-wave ruling). Model-watch otherwise.

## #2 (MODEL / DECODE, DOMINANT residual, carried) — degenerate REPEAT-LOOP replies + salvageLoopedChoice gap.

Still the single most common failure shape at this seat: 4 of 9 fallbacks are decode-time repetition
spirals (d35 s16 mana-loop, s21 markdown-bullet spiral, s30 lethal-math loop; d110 s25 lethal-math at
1 life), all >12k chars, all on hard/hopeless spots, NONE flipped a winnable game. **salvageLoopedChoice
(new this wave) did NOT rescue any of the 9 fallbacks at this seat** — it keys on `CHOICE:`/`ATTACK:` and
misses two shapes the seat actually bled: (a) reveal `PUT:` lines (d35 s10, d27 s8 stated tentative picks
early — "pick 4 and 6" — before spiraling), and (b) BLOCKS lines (d62 s27, a fully-formed
`BLOCKS: Ice-Fang Coatl: Saproling (1/1) #1`). Extend the salvage scanner to recover the last well-formed
`PUT:` and `BLOCKS:` line too, and pair it with a repetition-penalty / max-token decode guard. This is
a model/decode item, NOT guide-fixable.

## #3 (ENGINE representation, NEW — cross-deck) — surface the eligibility filter on reveal/search choosers.

Generalizes beyond deck135: any deck with a reveal/search/tutor whose to-hand (or to-battlefield)
partition has a restriction. The wave-19 reveal seam offers the model cards it cannot legally move,
unmarked — Into the North offers the whole library for a "snow LAND" search; Glacial Revelation offers 6
without marking snow permanents; Azcanta activation offers 4 without marking noncreature-nonland. This
directly caused 2 giant-reply fallbacks and 1 zero-to-hand whiff at this seat. Fix at the option-line
representation: offer ONLY legal targets for the partition, or annotate each card's eligibility (same
family as the wave-18 "name the pitch card" win). -> notes ENGINE-R3/R4.

## #4 (CORPUS, carried) — GPU-latency LIFE-adjudication as tiebreak.

deck135 sits in 4 of the corpus's 5 adjudicated games. Adjudication itself is not a bug (brief owns),
but at this seat it CONFIRMED the deck's under-powered clock: vs35 (adj WIN, ahead) was deserved; vs62 /
vs49 / vs110 (adj losses, behind) were genuinely behind at the cap. The adj mechanism did not distort a
result here. Noted for completeness; not a deck135 item.
