# deck135 wave-6 — development notes (engine / harness / model / deck-construction)

## ENGINE LEDGER — fetch re-offer loop (STILL LIVE; updated repro pointers)
Persisting since wave-3 (F2) / wave-4 (E1). The engine re-offers a fetchland's crack activation at
EVERY priority window until the land is sacrificed, and (a) a HELD fetch re-prompts its crack every
window, (b) a CHOSEN crack is re-offered on the next tick so the pilot re-picks it. One crack costs
multiple model calls; a held fetch taxes every window.
- Fresh repro (run matchups-20260713-190011):
  - vs44 (1783989289) s111→s112→s113: identical option string
    "Put in Play with Windswept Heath targeting Snow-Covered Plains [cost: Tap, Life, Sacrifice]"
    chosen 3 consecutive decisions (Main1→Main2→Upkeep), then s114 cracks Prismatic Vista; life
    paid 20→18. Dominant contributor to that game's decision-count TIMEOUT at T22.
  - vs110 (1783991383) s2→s3 and s13→s14: same crack option chosen on consecutive ticks (~2 model
    calls / crack).
  - Held-fetch re-offer runs (pilot correctly passes tapped-out but re-reads the option every
    window): vs110 s25–37 (13), vs109 s41–53 (13), vs140 s19–29 (11), vs44 s68–76 (9).
- Detection is now trivial with options_text: grep for the verbatim crack string recurring across
  consecutive seqs.
- FIX (engine): consume/withdraw the crack activation once chosen for the tick; do not re-offer a
  held fetch's crack at every priority window (offer it on the owner's main phases only, or once
  per window-cluster). This is the single highest-value engine cleanup for this deck's timeouts.
- Guide stopgap shipped this wave (strategy.txt): "Crack a fetch the first time its option appears,
  then MOVE ON... do not re-pick a fetch-crack you already took." Removal condition: delete when the
  engine stops re-offering consumed/held crack activations.

## MODEL RESIDUAL — own-board hallucination / phantom lethal
vs131 (1783995005) s52–s60: pilot's PLAN asserts "Abominable Treefolk 5/5" on its battlefield and a
7-damage lethal; Treefolk was offered 0 / cast 0 all game; battlefield line shows only Icehide
Golem (2/2). Pilot declined Ice-Fang Coatl twice on the false lethal and lost. The deciding fact is
fully surfaced and the "count only listed creatures / trust the board" instruction is present and
lost → not guide/core-fixable. Litmus record for the model-experiments track (stale-plan /
own-board hallucination class). Guide reinforcement added as a cheap stopgap only.

## DECK-CONSTRUCTION FLAGS (route upstream to deck design, not the guide)
1. MANABASE SKEW — the deck's wincons are GOLD/2-color but the base is green-heavy and thin on
   blue/white:
   - Ice-Fang Coatl {G}{U} sat uncastable in hand T0–T8 in vs109 (color screw) while RB aggro
     killed it (opp at 20 all game). Blue = only 2 Snow-Covered Islands + Misty/Flooded/Prismatic
     fetches + Astrolabe.
   - Diamond Faerie {2}{G}{W}{U} offered 0 / cast 0 for the SECOND straight wave — a GWU 5-drop off
     a 2-Plains base is effectively uncastable. Consider cutting it from the maindeck (it is already
     a 1-of + 1 in SB) or adding a white/blue snow source; it is dead weight the guide can only
     footnote.
   - Suggestion for the deck builder: more Arcum's Astrolabe is already 4x (good); the real lever is
     a couple more dual/snow-dual sources or trimming the double-pip creatures for the fast matchups.
2. SLOW CLOCK — the maindeck's bodies are tiny (Icehide 2/2, Coatl 1/1, Viper 1/3) and only
   Abominable Treefolk (5–7 power) closes. Games where Treefolk doesn't land early grind to the
   decision/latency cap even with Rule #1 obeyed (vs140 draw 16-16 @T18; vs44 win-but-slow @T22).
   The deck wants Treefolk to arrive faster or a second real finisher; this is a construction limit,
   not a piloting one.

## HARNESS / INFRA
- Latency: this corpus carried the flagged 60–80s Spark spikes (−j3). vs44 had 15 decisions >20s
  (max 74.9s); vs110 timed out at only T7/48 decisions purely from shared-server serialization.
  For decision-QUALITY corpora on this deck (slow clock → many decisions → long games), run −j1/−j2
  or raise WAGIC_GPT_TIMEOUT, or these deck135 games will keep timing out regardless of play.
- Reply parser CLEAN this corpus: 0 head-integer-vs-choice desyncs, 0 numeric-offset menu traps,
  0 degenerate blocker replies.

## GUIDE STOPGAPS shipped this wave (triple: line / defect+citation / removal condition)
- "do not re-pick a fetch-crack you already took" / fetch re-offer loop (vs44 s111-113) / delete
  when engine stops re-offering consumed crack activations.
- "if a creature isn't printed on your battlefield line it is NOT in play" / phantom-lethal
  hallucination (vs131 s54/s59) / delete if the model stops hallucinating own board state (model
  residual — may never clear at guide layer).
