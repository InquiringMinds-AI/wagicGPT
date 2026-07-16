# General suggestions (wave 13) — deck110 seat

No new CORE prompt-body candidate from the deck110 seat (core PASS, 10th consecutive wave).
Guide FROZEN. The wave-13 items route to ENGINE/CARD-SCRIPT, MODEL-rules, and reply-protocol
layers. Each has its full repro in `notes.md`.

## Engine / card-script ledger (cross-deck relevance)

1. **[HIGH] Galvanic Blast metalcraft is broken — flat 2 damage in-engine, never 4.** The
   card's `auto=aslongas(artifact|myBattlefield) damage:2 >2` metalcraft line never resolves
   (it uses the STATIC-effect `aslongas` form for a ONE-SHOT damage and omits `oneshot`), so the
   Blast deals its base 2 even at 3+ artifacts. Confirmed corpus-wide (every Blast in 6 games
   dealt 2; clean repro deck133 s21 at surface `Artifacts in play: you 4`). This is deck110's
   core finisher/removal and the guide's LETHAL CHECK counts 4 per Blast — the engine
   systematically under-delivers the deck's reach. Cross-deck to ANY deck running Galvanic Blast
   or a similarly-scripted metalcraft one-shot. Owner: `bin/Res/sets/primitives/mtg.txt`; verify
   + fix via the wagic-cardscript skill (compare the `oneshot` idiom on Blade-Tribe Berserkers /
   Bleak Coven Vampires). **The guide is Oracle-correct; fixing the card restores it. Not a
   guide edit.**

2. **[MEDIUM] Equip re-offer no-op marker should be computed from OUTCOME DELTA, not target
   identity (E2, unfixed, ledger #4).** RECURRED milder this wave (deck131 s29 net-zero Plating
   relocation, non-outcome). One predicate — mark/suppress "Equip … targeting X" whenever the
   move would not raise X's resulting power above its current carrier's — catches both the
   equivalent-carrier bounce and the second-Plating-onto-an-equipped-creature shapes. Owner:
   `AIPlayerGPT.cpp` equip-option serialization.

3. **[LOW-MED] Add an attackers name→index reconcile arm (mirroring parseChoice/name-echo).**
   The `ATTACK:` protocol wants A-indices but the model sometimes emits creature NAMES
   (deck135 s9 `ATTACK: Ornithopter, Signal Pest` → unparsed) or MIXES them (deck131 s20
   `ATTACK: A1, A2, Memnite` → parsed). A name→A-index reconcile on the attackers path would
   recover the unparsed-by-name cases the same way name-echo recovers CHOICE conflicts.
   Cross-deck (matches the brief's noted `ATTACK: Hellrider` shape). Owner: attackers parse path.

## Model-layer items (not guide-fixable; cross-deck)

4. **Model rules gaps on burn-vs-toughness and triggered penalties (deck133 s21/s22).** The
   model (a) believes combat/burn damage permanently reduces toughness ("Blast reduces the 5/5
   to 1/5"), and (b) inverted Phyrexian Obliterator's "source's controller sacrifices" trigger,
   firing burn INTO it and force-sacrificing its own permanents. Cheap correctness win for any
   burn deck: the option-list target annotation could show the target's toughness alongside its
   power so a sub-lethal burn on a high-toughness creature is visible (the pilot already gets
   `(5/5)` — but reasoned past it). Primarily a MODEL knowledge gap; flag for the harness-side
   target annotation only if it recurs across seats.

5. **Blocker over-block while ahead (deck135 s17) — cross-seat SYNTHESIS question.** Now that
   the BLOCKS:none / blocker seam fires at scale, this seat over-blocked (10/2 equipped attacker
   into a 2/2, both die) DESPITE the prompt annotating the correct line verbatim ("block only
   where the trade favors you; taking damage while ahead is often correct"), under a "blocking is
   mandatory" misconception. Single-seat here — but ask synthesis whether the OTHER aggro seats
   also over-block on defense. If it's cross-seat, the right fix is a sharper prompt annotation
   (the block-value seam), NOT a per-deck guide line.

## Reply protocol (validated, no action)
6. **Name-echo shipped and works.** 177/177 CHOICE echoes present at this seat, 0 wrong remaps;
   the one echo-index conflict (deck131 s7) resolved correctly to index and absorbed a model
   target-subprompt confusion. Index-wins-on-conflict is the right design. Keep.

## Deck construction (not a guide task)
7. **deck110's loss column is owned by draw variance / matchup, not obedience.** vs133 (empty
   board into Obliterator+Liliana), vs109 (raced), vs140 (board-wiped, 2 attack windows in 24
   turns). Standing DC1; if ever tuned, trim ~0-power filler + Mox/land toward mid bodies +
   reach/evasion redundancy. Not a wave task.
