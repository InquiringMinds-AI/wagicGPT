# deck116 — development notes (engine / harness / render / construction)

Layer-routed per the brief. Nothing here is guide prose.

---

## N-116a (RENDER, HIGH, cross-seat) — library tutors render the WHOLE LIBRARY, not the eligible set

**Class:** the wave-31 reveal-chooser rule ("asks must be eligible-count or no-ask") is violated on
the `revealzone(mylibrary)` search path. Same path as the KNOWN-OPEN **N-136a** (Mausoleum
Secrets); this is a fresh, high-volume witness on ordinary TUTORS, so the wave-30 g2 PUT-predicate
filter still has not reached this emitter.

**Repro (14/14 reveals in the corpus, all six games):**
- `matchups-20260727-115847/1785174946-ai_baka_deck116-...-vs-ai_baka_deck152.jsonl` **seq 15**,
  turn 11, kind=`reveal`: Time of Need. `options`=**45**. Eligible set = 5 legendary creatures
  (Jin-Gitaxias, Sheoldred, Kozilek, Ulamog, Emrakul). Prompt header:
  `Reveal: you looked at the top 45 cards of your library. Choose the ONE card that goes to
  "choose card"; every other card goes to "shuffle".` then
  `ELIGIBILITY: only a legendary creature may go to "choose card" - the rest do not qualify and go
  to "shuffle" regardless. Pick ONLY from the cards marked [eligible] below.`
  and 40 of the 45 lines end `[does NOT qualify - goes to "shuffle"]`, each carrying full card text.
- Fabricate (eligible set = 4 Quicksilver Amulets): 49 / 51 / 47 / 51 / 49 options
  (deck139 s8, deck152 s8, deck105 s13, deck146 s8, deck146 s11).
- Farseek (eligible set = 7): 51 / 44 / 51 / 48 / 45 options.

**Severity:** correctness is currently intact (9/9 `<1>` picks legal; see N-116d for the other
framing) — the cost is TAX. The corpus's single largest reply is one of these:
`deck36 s10`, a 48-option Farseek reveal, **13,796 chars / 220,606 ms** to choose a land. Three of
the seat's six largest replies are reveals. **Repro quality: excellent** (14 instances, two
different `auto=` shapes, one seat).

**Fix direction:** filter the reveal option list to the eligible set at the `revealzone(mylibrary)`
emitter (the labelling machinery already computes eligibility per line — the predicate exists, it
is only being used to annotate rather than to filter), and no-ask when the eligible set is empty
or singleton.

---

## N-116b (RENDER, HIGH, cross-seat with deck105) — POISON COUNTERS ARE NEVER RENDERED

**deck116 lost `vs105` at 23 life** with the opponent on 20. The loss condition was poison, and
**no poison count appears anywhere in the prompt** — not in CURRENT SITUATION, not in the game
narration, not on any option line.

**Repro:** `1785176893-ai_baka_deck116-...-vs-ai_baka_deck105.jsonl`, **seq 17** (last decision
before gameend). CURRENT SITUATION reads in full:
```
Your life: 23 | Opponent life: 20
```
The opponent's board carries four `[infect]` creatures with keyword text, and the combat events
narrate `Damage: 1 dealt by Necropede to you` with no poison-counter line — indistinguishable from
life damage in the log. A grep of all six seat logs for `poison` outside the `[infect]` keyword
blurb returns **4 hits, all of them inside the pilot's own PLAN prose**, where it is manually
tallying counters it cannot read back (`"Wait, the opponent has 1 poison counter."`).

**Consequence at this seat:** the pilot chump-blocked with Elvish Piper twice (s8, s11) to stop
poison, destroying the deck's engine card, and still died to the alt-win it could not count.

**Fix direction:** a first-class CURRENT SITUATION field whenever either player has >0 poison, e.g.
`Poison counters: you 7 / opponent 0 (10 = you lose)`, plus a `Poison +N (now M)` narration line
beside the damage event. This is the deck122 alt-win-visibility shape at the representation layer;
deck105's own seat is the other witness — **route as one item, not two.**

---

## N-116c (RENDER, LOW) — colourless-producing lands render as bare `(land)`

`Glimmerpost` renders `Glimmerpost (land)` on hand lines while every coloured land renders
`Forest (land: taps for {G})` / `Tropical Island (land: taps for {U}{G})`. Its primitive is
`auto={T}:Add{1}`, so the taps-for clause has nothing coloured to print.

**Repro:** `...vs-ai_baka_deck158.jsonl` seq 1. Hand = `Glimmerpost (land); Tropical Island (land:
taps for {U}{G}); Island (land: taps for {U}); Farseek; Farseek; Time of Need; Kitchen Finks`.
Reply: *"This hand has zero lands and only one source of mana (Glimmerpost, which enters tapped
and produces only one mana)"* — two false claims, one of which (enters tapped) is not in the
render at all. Suggest `Glimmerpost (land: taps for {1} colourless)` for parity.

Severity LOW: this is a contributing surface, not the cause — the same reply called a hand with a
`(land: taps for {G})` Forest "zero lands" (see the guide's rule #6, which owns the belief).

---

## N-116d (RENDER, MEDIUM) — pick-EXACTLY-ONE search rendered with SUBSET framing when the `auto=` carries no `<N>` token

Second-seat reproduction of the deck102 pick-cardinality rung, now on a LIBRARY tutor.

- Time of Need / Fabricate: `target(<1>creature[legendary]|reveal)` / `target(<1>artifact|reveal)`
  -> header **"Choose the ONE card that goes to 'choose card'"**. Pilot answers one index. 9/9 clean.
- Farseek: `target(plains,island,swamp,mountain|reveal)` — **no `<1>`** -> header **"Decide, in ONE
  reply, which of them go to 'choose card'"** -> the pilot over-selects every time.

**Repro:** `...vs-ai_baka_deck152.jsonl` seq 5, reply `PUT: 14, 23, 25, 26, 29, 36` +
*"I will select all available Islands/Tropical Islands from the revealed list"*; `chosen_text` =
six card names. **Arrival trace: exactly ONE land landed** — next record events end
`- Your Tropical Island: zone -> battlefield`. All 5 Farseeks over-selected (6, 7, 4, 7, 6 picks).

**Severity:** outcome-neutral in this deck (every over-selected card was an Island or a Tropical
Island, so the engine's finalize choice is interchangeable) but LATENT: the pilot is not choosing
*which* card it gets, and a deck whose eligible set is heterogeneous will silently get the wrong
one. Farseek's Oracle text is "Search your library for a Plains, Island, Swamp, or Mountain card"
— singular — so the `auto=` is arguably under-specified; the render should derive singular framing
from the effect, or the primitive should carry `<1>`. Route the render fix; do NOT edit the guide
beyond the one-line "Pick ONE card" instruction already present.

---

## N-116e (CONSTRUCTION / roster lane, LOW — informational, do NOT act this wave)

Recorded because the reviewer brief named the 1-of Eldrazi as an absent-engine-card risk. **The
risk did not materialise** — see findings (c): the payoffs were reached in 4 of 6 games and Time
of Need hit 4/4. Two construction observations, neither of which caused a loss:

1. `Sheoldred, Whispering One {5}{B}{B}` is HARD-uncastable — the deck's 24 lands (Tropical Island,
   Hinterland Harbor, Glimmerpost, Forest, Island) produce no black. She is a cheat-only card by
   construction. Consistent with the shell's intent; stated in the guide so the pilot stops
   planning to cast her.
2. The curve has no play below three mana (cheapest creature Kitchen Finks at 3), and the three
   games that ended on turn 8 all ended with the pilot on 4-6 lands. That is a real clock deficit,
   but the decomposition puts it BEHIND the deploy-floor failure: in two of those three games a
   castable body was OFFERED AND DECLINED. Re-assess only after the guide's first validation
   corpus, per the ceilinged/terminal rung (do not route to the roster owner now).

---

## N-116f (AI-ARCHITECTURE, MEDIUM — proposal, not a defect)

The deck file `Res/ai/baka/deck116.txt` carries author HINT lines that encode, exactly, the rule
this seat violated twice and lost a game to:
```
#HINT:dontattackwith(Elvish Piper)
#HINT:dontblockwith(Elvish Piper)
#HINT:combo hold(Quicksilver Amulet|myhand)^cast(Quicksilver Amulet|myhand)^...^totalmananeeded({4})
```
These feed `AIHints` and are consumed only by the heuristic Baka path. **Verified absent from the
GPT prompt** (grep of a full assembled prompt: no `HINT`, no `dontattackwith`). So the deck author's
own strategic annotations — a hand-curated, per-deck, machine-readable source of exactly the
teaches the guide loop spends a wave discovering — are invisible to the LLM pilot.

Two options, in preference order: (a) mine the HINT lines when AUTHORING a guide (free, zero
engine risk, and it would have produced rule #4 before the corpus did); (b) surface a rendered
translation of the deck's hints in the per-duel system prompt. Flagging (a) as a **skill** proposal
too — see `skill.md` S-116a.
