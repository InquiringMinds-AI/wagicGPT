# Core-prompt / render proposals - deck125 seat (wave 47)

Layer-routed per the brief: nothing here is an engine bug (those are in the seat file's ranked
list). These are prompt-text and render-shape proposals.

## G1 - The stack line and the counter-target clause should carry the same keyword tags the
## battlefield line carries

The battlefield line prints `Perimeter Captain {w} (0/4) [defender]`. The stack line for the SAME
card prints `1 (top): opponent's Perimeter Captain {w} (creature 0/4) [spell]`, and the counter
option prints `- can target on the stack: Perimeter Captain {w} (creature 0/4) {target text:
"Defender -- ..."}`. Neither carries `[defender]`, `[flying]`, `[deathtouch]` or any other keyword
tag. The whole pool's counter and removal rules are written against battlefield vocabulary, so a
rule keyed to a tag is unsatisfiable at exactly the moment it matters. Repro:
`1787786522-ai_baka_deck125-0x5555ed1493c0-vs-ai_baka_deck126.jsonl` seq 66.
Cost is the tag string the emitter already builds for the battlefield line. This is the same defect
class as the own-hand count (#W46-1 / 1c71d5301): the fact is at the call site and the pilot is asked
to re-derive it from prose.

## G2 - `{leaves N ...}` should also appear on ACTIVATED-ability rows, not only cast rows

R14b put `{leaves N of your M untapped mana sources untapped}` on cast rows and it is the single
most useful field on this seat's menus. It is absent from activated-ability rows that cost mana -
`Life with Elixir of Immortality [cost: {2}, Tap]` prints its cost but not what it leaves. deck125's
tap-out gate explicitly covers "an Elixir of Immortality cast OR activated", so the rule can now be
read off the row in one half of its scope and must be computed in the other. 15 Elixir activation
windows this corpus.

## G3 - `X pricing` is excellent; consider the same treatment for the CAST row of an X spell

`{X pricing: max affordable X=9 (12 mana total); each point of X gains you 1 life and draws you 1
card}` on the cast row, then a per-value menu where every row states its own effect
(`X = 3 {X pricing: X=3 - you gain 3 life and draw 3 cards}`, and the X=0 row saying outright that
the cast does nothing) - this is the cleanest new surface in the corpus and it took the deck125 X
lane from 4/17 conforming to 12/12 with no guide change to the rule itself. Noted as a shape to
reuse, not a request. The one thing the cast row does NOT print is `{leaves N ...}` (it carries the
X clause instead), which is what makes G2's gap visible on this seat's most expensive spells.

## G4 - Degenerate decode: an 800 s reply of repeated tokens counted as one `unparsed_reply`

`1787786524-ai_baka_deck125-0x555dc22a73c0-vs-ai_baka_deck123.jsonl` seq 39 (turn 22, 4 options,
25,138-char prompt): the reply is 16,271 characters of `ai茧ai...` with no CHOICE line, latency
798,216 ms. The seat's other max is 800,769 ms (deck126 vs162 seq 15), which suggests an ~800 s
ceiling somewhere in the client. This is an inference-layer event, not a prompt defect, and one in
5,644 decisions - but it is worth a named counter distinct from `unparsed_reply`, because a reply
that is 16 KB of one repeated token is diagnostically different from a reply that merely did not
parse, and the current record cannot tell an operator which happened without reading the text.
