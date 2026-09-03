# Wave 56 — lane X (hotfix): Light-Paws lets the tutored Aura land on any creature

## The report

Owner Vita play report, 2026-09-03, vpk15, tag `bug`, his words:

> "found another bug with light paws. it lets me put the tutored aura onto other creatures."

Transcript `psp-work/logs/20260903-vpk15/transcript-1788460925-player_deck4-vs-ai_baka_deck27.txt`
~line 146: `p1.battlefield[2] 1light-paws, emperor's voice` / `p1.battlefield[4] 1starfield mystic`
— the tutored Aura had been attached to Starfield Mystic.

## Oracle

> Whenever an Aura enters under your control, if you cast it, you may search your library for an
> Aura card with mana value less than or equal to that Aura and with a different name than each
> Aura you control, **put that card onto the battlefield attached to Light-Paws, Emperor's
> Voice**, then shuffle.

The attachment is not a choice. The player gets to choose *which* Aura is fetched, never *where*
it goes.

## The confirmed mechanism (reproduced first, not assumed)

Fixture `light_paws_attaches_to_lightpaws.txt` reproduces the report on the seeded base binary
(a829763b7): Light-Paws + Grizzly Bears in play, Pacifism cast on the Bears to fire the trigger,
Holy Strength (+1/+2) fetched — the script clicks **Grizzly Bears** at the attach prompt and the
Bears came out 3/4 while Light-Paws stayed 2/2. RED, exactly the owner's shape.

Three probe runs and one round of env-gated `fprintf` instrumentation established what the base
binary actually does:

1. **There is exactly one prompt after the fetch, and it is unrestricted.** A variant that
   clicked Grizzly Bears and *then* Light-Paws showed the second click was a no-op — one prompt,
   consumed by the first click, wherever it pointed. A variant with no click at all left the
   fetched Aura sitting in the library, so that prompt is the Aura's *own* attach chooser,
   opened while the move is still in flight.
2. **`AAMover::resolve` enters an Aura on the battlefield through an `AACastCard`** (a real
   cast, hence the free "Enchant creature" choice) that is `oneShot = false`: it *arms* here and
   resolves on a later tick.
3. **The `and!( ... )!` payload was being dropped on the floor.** The old code tried to apply it
   immediately, gated on `_target->next` — the instance the deferred cast has not created yet.
   The probe printed it plainly:

   ```
   PROBE AAMover aura branch: card=Holy Strength andAbility=0x55c812087b00 next=(nil)
   ```

   `andAbility` present, `next` NULL, payload discarded. `AANewTarget::resolve` never fired at
   all — so the script's whole "attach it to Light-Paws" leg (and its `shuffle`) had never once
   executed in any game. The player's click was the only thing deciding the attachment.

## Fix: card script first, then the engine leg that made it impossible

**(1) Card script** — `Res/sets/primitives/borderline.txt`, `name=Light-Paws, Emperor's Voice`:

```
- newability[name(Attach aura) retarget target(Light-Paws^ Emperor's Voice|mybattlefield)]
+ newability[name(Attach aura) all(creature[Light-Paws^ Emperor's Voice]|mybattlefield) rehook]
```

Two faults in one line. `retarget` is the wrong member of the family: it pushes the Aura back
through exile and re-resolves it, where `rehook` re-attaches it in place (this is Runed Crown's
established idiom, and since w54 `rebindAuraAbilitiesToHost` makes an in-place re-attach carry
the Aura's granted effects to the new host). And the `target(...)` form makes the grant a
`GenericTargetAbility` — an **activated** ability, `oneShot == false`, which
`ATransformer::resolve` parks in the card's menu instead of resolving. `all(...)` is one-shot
and resolves at once, so the grant fires the moment it lands:
`ATransformer` sets the granted ability's `source` to the transformed card (the fetched Aura)
and `all(...)` supplies Light-Paws as its target, which is exactly the pair `rehook` needs
(`_target = source` = the fetched Aura, `source = target` = Light-Paws, then
`_target->target = source`).

**(2) Engine** — `src/AllAbilities.cpp`, `AAMover::resolve`, the Aura-to-battlefield branch.
Hand the `and!()!` payload to the `AACastCard` (which already owns an `andAbility` slot and
applies it in `resolveSpell()` against `copy`, the instance the cast actually produced) instead
of applying it immediately against a `_target->next` that is always NULL:

```cpp
if(andAbility)
    a->andAbility = andAbility->clone();
a->addToGame();
```

The clone is owned and freed by the `AACastCard` (its dtor `SAFE_DELETE`s `andAbility`, and its
`clone()` deep-copies it). The old read of `_target->next` was unsafe in its own right: a card
carrying a **stale** `->next` from an earlier zone move would have had the payload applied to a
dead instance.

### Why the engine had to move (counterfactual, run — not argued)

The card-script change was stashed against the *unfixed* engine and rebuilt: with the new script
and the old `AAMover`, `light_paws_attaches_to_lightpaws.txt` is still **RED** (the payload is
dropped before the script can matter) while the negative control stays green. The engine leg is
load-bearing. There is no card-script-only expression of "enters attached to X" while the
payload never runs.

The change is deliberately narrow and general: it does not alter *whether* an Aura moved to the
battlefield asks for a target (Arachnus Web, Bitterheart Witch and friends legitimately say
"attached to target creature"), only that the script's `and!()!` payload is delivered to the
instance that entered, at the moment it entered.

## Fixtures

| fixture | asserts | base a829763b7 | after |
|---|---|---|---|
| `light_paws_attaches_to_lightpaws.txt` (new) | the script clicks **Grizzly Bears** at the attach prompt; Light-Paws must still come out 3/4 and the Bears 2/2 | **RED** (2/2 and 3/4 — the report) | **GREEN** |
| `light_paws_normal_aura_choice_free.txt` (new, negative) | Holy Strength **cast** on the Bears with Light-Paws in play and the search declined: Bears 3/4, Light-Paws 2/2 | GREEN | GREEN |
| `light_paws_fetch_positive.txt` | existing | GREEN | GREEN |
| `light_paws_different_name.txt` | existing | GREEN | GREEN |
| `light_paws_bestow_fetch.txt` | existing | GREEN | GREEN |
| `light_paws_bestow_mv_bound.txt` | existing | GREEN | GREEN |
| `light_paws_bound_by_entering_aura.txt` | existing | GREEN | GREEN |

The attachment is asserted through `assertpt`: Holy Strength is +1/+2, so which creature grew
*is* which creature the Aura attached to. No new harness assertion was needed — an
`assertattachedto` would be more direct but adds engine surface for something the existing
P/T assertion already sees.

## Gate (worktree, THREADS=1)

| gate | baseline | this branch |
|---|---|---|
| suite | 1234 / 0 failed | **1236 / 0 failed** (+2 new fixtures) |
| AI tests | 54 / 0 | **54 / 0** |
| timeouts | 0 | **0** |
| PARSETEST | 2736 / 0 | **2736 / 0** (unchanged) |

## Other cards on the same defect (NOT fixed here — docket for wave 57)

Every "put it onto the battlefield attached to X" card is scripted as
`moveto(mybattlefield) + and!( transforms((,newability[... rehook target(X) ...])) oneshot )!`,
so all of them lost both legs of the payload. The engine fix restores payload *delivery* for
all of them; the `target(...)`-instead-of-`all(...)` dormancy is per-card and still stands:

- **Runed Crown** (`borderline.txt`) — `rehook target(equipment[Runed Crown]|mybattlefield)`.
  Same one-line conversion applies (`all(equipment[Runed Crown]|mybattlefield) rehook`), and its
  `newability[shuffle]` now runs. Not changed here because verifying it needs an
  Equipment-plus-Rune fixture whose `[INIT]` placement fires the search trigger at setup, which
  is a fixture-craft problem of its own; shipping the edit unverified would be worse than
  leaving it.
- **Mantle of the Ancients** (`borderline.txt`) — `rehook target(creature|mybattlefield)` for
  "attached to enchanted creature". Genuinely harder: the grant is hosted on the *returned* Aura,
  so no chooser on that card can name "the creature Mantle enchants". Needs a construct that
  does not exist yet. Its Oracle is also violated today in a second way (the chooser is every
  creature, not the enchanted one).

Both were previously inert; after this change their grants become clickable menu entries rather
than silently absent, which is a strict improvement but is not the Oracle.

## What was NOT verified

- **No Vita/PSP build, no console run.** Desktop SDL binary only.
- **No GUI drive.** The repro and the fix were established headless through the suite harness
  and instrumented probes; the owner's exact board (Starfield Mystic) was not reconstructed.
- **No selfplay corpus.** The AI seat's Light-Paws was not exercised end-to-end; the fix is in
  the resolution path (not a decision seam) and is seat-agnostic by construction — the forced
  attach happens during ability resolution, below any chooser either seat would answer — but
  that is reasoning, not an observation.
- **Runed Crown and Mantle of the Ancients were not re-tested** after the engine change beyond
  the full suite staying green; neither has a fixture.
