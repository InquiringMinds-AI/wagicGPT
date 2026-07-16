# deck133 wave-15 — strategy-writing-skill proposals

Corpus: `matchups-20260716-040138` (first corpus under the aad32bc7f crash fix + d0f88326b
absent-echo staleness route). The wave-13 deck133 proposal (split echo-index conflicts into
Mechanism A / Mechanism B and route echo-names-no-option to the heuristic) was ADOPTED at synthesis
and SHIPPED as d0f88326b. This wave supplies its post-ship VALIDATION plus one small scoring-method
refinement. Single-seat, so nothing here adds or cuts a core method line.

## Confirmation (no change) — the shipped absent-echo route validated at this seat.
The wave-13 headline-1 completion is confirmed working: every echo-names-no-option event this corpus
(vs44 s25 "Cast Gray Merchant" at a fetch put-in-play menu; vs140 s11/s12/s36) routed to the
heuristic, 0 Mechanism-B index leaks on a full scan, and the wave-13 game-losing shape did not recur
as a loss. The Mechanism-A / Mechanism-B scoring split the skill adopted did its job — carry it
verbatim.

## Proposal 1 (WATCH — refines the absent-echo-route scoring rung with its SECOND error direction)

**A shipped absent-echo staleness route has TWO error directions, and a reviewer scoring it must
check both: (a) the leak direction — an echo naming NO option that still executes a raw index
(wave-13's concern; now closed) — and (b) the FALSE-POSITIVE direction — an echo that DID name an
offered option but was downgraded to the heuristic anyway, most often via a NAME-SUPERSTRING
mismatch (the model echoes the FULL card name while the option carries a SHORT name).**

Wave-13 established the Mechanism-A/B split and the "route absent echo to heuristic" rule. Wave-15's
one witness that the route can OVER-fire: deck133 vs140 seq37 — options `[Attack with Yawgmoth / Play
Verdant Catacombs / Cast Fatal Push / Pass]`, reply `CHOICE: 1 (Attack with Yawgmoth, Thran
Physician)`. The echo names offered option 1, but as a superstring (full card name ⊇ the option's
short name), so an equality/substring key match fails and the reply is downgraded to `stale_echo`
even though the model's intent was legal and present. Not deciding here (the heuristic's
decline-to-attack was defensible), but it is the mirror-image failure of the class the route was
built to fix, and it is invisible to a "route fired correctly N times" count — it only surfaces by
reading the echoed name against the option list at every stale_echo event and asking "did the echo
actually name an offered option?"

**Authoring consequence (adds the false-positive check to the absent-echo scoring rung):** when
scoring a shipped absent-echo route, do NOT credit every `stale_echo` as a correct catch — split them
into TRUE catches (echo names no offered option, incl. parent-action-at-sub-menu and just-cast-card
shifts) vs FALSE positives (echo names an offered option, lost to a superstring/formatting mismatch),
and count the false positives and their cost. The durable fix lives at the HARNESS (match when the
option name is a prefix/substring of the echoed full card name; notes #B), not at the guide.

Convergence status: **single seat (deck133), 1 firing / non-deciding, one corpus.** Per the
epistemics rule it does not add or cut a core method line; offered as a SHARPENING (add the
"false-positive direction: echo-names-an-option-but-downgraded, via name-superstring mismatch"
qualifier to the absent-echo scoring rung). Adopt into the method only if a second seat shows a
name-superstring false-positive downgrade this wave; otherwise WATCH.

## No other skill changes.
The freeze/demotion doctrine, the bare-N/shown==resolved terminus, the own-targets Fatal Push
representation item (recurred once, non-deciding, via a NEW card-presence-hallucination mechanism —
notes #A, an engine/representation item, not a skill-method item), and reply-protocol layer-routing
all held and required no revision from deck133's evidence.
