# deck126 - render / core-prompt proposals (wave 50), layer-routed

Engine items are in `wave50/seats/seat-125-126.md` (HIGH #1 is this seat's); these are
render/prompt items.

G1 (HIGH, render/parse - companion to seat HIGH #1) - an in-range `CHOICE: n (<name>)` whose
name matches no row gets one `[RE-ASK]` that prints the mismatch, not a silent fallback. The three
records (`deck126 vs125` seq 14, `vs162` seq 16, `vs130` seq 13) show the model naming a HAND card
(Sanguine Bond / Exquisite Blood) that had no row because a pip was unpayable. Proposed re-ask
text, built from data the render already has: `Sanguine Bond is on your hand line but has no
"Cast" row this window: its {3}{b}{b} needs two {B} sources and your mana line counts {B} 1.
Answer with a numbered row.` The fallback label `stale_echo` is also wrong for seq 14 (the prior
reply was `Play Savannah`).

G2 (MED, render) - the edict row's victim naming (`<name> is sacrificed, you gain N`) should carry
the victim's tags when it has any: `Shield Sphere [defender] is sacrificed, you gain 6`. The guide
now keys a carve-out on that tag (F17) and the executor has to fetch it from the battlefield line;
the row already knows the card (`deck126 vs162` seq 6).

G3 (LOW, render) - the cleanup `discard` list prints bare names (`1. Tribute to Hunger` x3, `4.
Chromatic Lantern` ...) - same grammar as the bottoming ask (`(land: taps for ...)`, costs) would
let the order rules read off the list (`deck126 vs123` seq 7). Shared with deck125 G3.

G4 (INFO, prompt) - the `[no creature they control can block this attacker]` tag and the
`[NONE of your available blockers can block this attacker - it has flying ...]` A-line tag both
worked as printed (7/7 sends; the flier damage was on the A-lines at `vs152` seq 20 and the
reply still summed only the blockable attackers - a guide clause now, not a render item).
