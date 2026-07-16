# Deck-135 wave-15 — development notes (engine / harness / model / construction) + ROTATION VERDICT

Run: PRIMARY `matchups-20260716-040138`. Binary `/tmp/wagic-aad32bc7f` (target-cancel use-after-free
fix + absent-echo staleness route + attackers-name reconcile + Galvanic metalcraft fix). deck135
jsonl epochs + opponents in findings.md game→file map. Doctrine: win column context-only;
layer-routing enforced (engine items live HERE with repros, never in prompt/guide text). Each item
self-contained with seq repros.

## ENGINE LEDGER

- **ENGINE #1 (was HIGH-PRIORITY CARRIED; NOW PARTIALLY RESOLVED + NEW SUB-BUG) — Force of
  Negation's resolution and its free alt-cast execution.** Two developments this corpus, the first
  live Force casts ever recorded at this seat (wave-13 had 0):
  - **#1a — RESOLUTION CONFIRMED WORKING (the long-open fix landed).** deck140 seq18 (free exile
    alt-cast) → seq19 events `Force: hand -> stack | Black Sun's Zenith: stack -> EXILE | Force:
    stack -> graveyard`; deck140 seq30 (hardcast {1}{u}{u}) → seq31 `Lightmine Field: stack ->
    EXILE`. Both targets went stack→EXILE (COUNTERED), not stack→battlefield (resolved). **This is
    the FIRST completed live Force counter — and seq18 is the FIRST completed live free ALT-CAST
    counter — captured in the project. It won the vs140 game (the countered Black Sun's Zenith would
    have wiped Treefolk+Golem).** ENGINE #1's resolution layer moves from "unvalidated end-to-end"
    to "VALIDATED; watch for regression." DETECT REGRESSION: a Force cast whose target EVENTS show
    `stack -> graveyard/battlefield` (resolved) instead of `stack -> exile`.
  - **#1b (NEW, HIGH VALUE) — the free alt-cast SILENTLY FAILS TO EXECUTE when the caster controls
    NO untapped mana sources** (the sharp new form of RESIDUAL-RACE, owned ledger #4). REPROS: deck21
    seq27 (target Volcanic Hammer, "(no untapped sources)" → Hammer resolved, 0 Force-hand->stack
    events), deck21 seq29 (Volcanic Hammer #2, no untapped → Hammer resolved, Treefolk died to
    3+3=6), deck110 seq4 (Cranial Plating, no untapped → Plating resolved to battlefield). CONTRAST:
    the only alt-cast that EXECUTED (deck140 seq18) had `{w}` from 1 untapped source. NOT latency:
    deck140 seq30 executed at 92,954 ms while deck21 seq27 fizzled at 8,127 ms — untapped-mana
    presence is the clean separator, reply time is not. The free alternative cost requires ZERO mana,
    so gating its execution on untapped sources is wrong. The round-3 offer-legality gate checks the
    exile target (a blue card in hand) but not untapped mana, so the offer is legal and the execution
    fizzles. This disables Force in its intended window (tapped out on the opponent's turn after
    deploying). CONFOUND: deck133 seq24 had `{g}{u}{w}` untapped but the game ended the next seq, so
    it neither confirms nor refutes. FIX DIRECTION: the alternative-cost (exile-a-blue) cast path must
    put Force on the stack without routing through / gating on the mana-payment or untapped-source
    machinery — an empty mana cost must be payable with a fully tapped board. DETECT: an `ask` window
    where Force alt-cast is chosen (choice selects the "with its exile a blue card" option) followed
    by a target-spell `stack -> graveyard/battlefield` and ZERO `Force of Negation: hand -> stack`
    events, WITH "(no untapped sources)" on the caster's board that window.

- **ENGINE #2 (CARRIED from wave-13; freeze half UNVALIDATED, [attacking] half REPRODUCED) — stale
  `[attacking]` tag + inverted/absent Treefolk-freeze annotation on the battlefield line.** This
  corpus:
  - **Stale `[attacking]` — REPRODUCED, widespread, benign here.** ~20+ battlefield lines render a
    tapped post-combat creature as `[tapped - untaps and can attack next turn] [attacking]` during
    ASK / priority / main-phase windows with no active combat (deck17 seq33–45 on Ohran Viper /
    Icehide Golem; deck140 seq22–23 on Treefolk+Golem; opponent creatures at deck21 s11/s19,
    deck110 s9, deck133 s9). A creature is "attacking" only during its declared combat; the lingering
    tag is stale. Did NOT drive a wrong decision this corpus (resolved windows / the won vs17), but
    it is the identical defect and near-caused a blocker misread at deck133 seq9 (see below). FIX
    (unchanged): clear `[attacking]` on any creature NOT in the current declared-attackers set.
  - **Inverted freeze — NOT REPRODUCED (opportunity absent).** The only Treefolk ETB freeze at this
    seat (deck44 seq26, froze Faerie Bladecrafter) was followed by gameend at seq28, so the frozen
    creature was never re-rendered at a later combat window. The wave-13 defect (a frozen creature
    annotated `untaps and can attack next turn`) is neither confirmed fixed nor reproduced — carry as
    UNVALIDATED. VALIDATION TODO: a non-terminal Treefolk-freeze window where the frozen creature is
    re-rendered; confirm it shows `frozen`/no `[attacking]` and suppresses `untaps and can attack
    next turn`.
  - **Adjacent misread — deck133 seq9 (blockers, no cost).** Bloodghast carried `[attacking]`,
    Geralf's Messenger did not; the model misread Messenger as an attacker and emitted illegal
    `BLOCKS: B1:A2` → unparsed_reply → heuristic no-block (choice=0), which was the correct play
    anyway. Shows the two near-identical `untaps…` tags (one with, one without `[attacking]`) are a
    parse hazard even when the tag is correct. Low priority.

- **ENGINE #3 (minor, CARRIED from wave-11/13) — fetch chosen-but-unresolved re-ask + duplicate
  identical option lines.** Not re-audited in depth (no batch targeted it). Persists as decision-count
  inflation only, zero resource loss. Carried unchanged; low priority.

## HARNESS

- **HARNESS #1 (LATENT, CARRIED — brief's earlier flagged candidate) — attackers name→index parse
  gap.** Model emits mixed `ATTACK: A1, <Name>`; parser takes the A-index, drops the name. INSTANCES:
  deck21 s22/s33 `A1, Abominable Treefolk`; deck140 s17 `A1, Abominable Treefolk`; deck17 s30 `A1,
  Ice-Fang Coatl, Ohran Viper, Icehide Golem`. EVERY dropped name was an ILLEGAL attacker (a same-turn
  summoning-sick creature; at deck17 s30 only A1 Boreal Druid was offered, the three named creatures
  were cast s27–s29 that turn). Clamp correct every time; 0 outcome cost. Still no outcome-coupled
  instance. FIX (unchanged): a name→index reconcile arm mirroring parseChoice's name-echo (map the
  name to its A-index; drop explicitly if not a legal attacker). DETECT: an `ATTACK:` line mixing
  `A<n>` indices with bare creature names.

- **HARNESS #2 — fallback class is `unparsed_reply` (long-reasoning-ramble guard); `empty_reply` = 0
  (3rd corpus).** 12 unparsed + 1 stale_echo this seat. 11/12 unparsed are latency TIMEOUTS
  (155k–197k ms); the 12th (deck140 s23) emitted `CHOICE: None`; deck133 s9 was the illegal-block
  drop. The 1 stale_echo (deck21 s26) is a CORRECT absent-echo catch (reply named "Snow-Covered
  Forest" into an Island/Plains list). ALL self-healed; NONE changed a game. Wave-12/13a `empty_reply`
  flood stays closed.

- **HARNESS #3 (CARRIED) — GPU_MEM_UTIL=0.75 latency → LIFE-adjudication mis-scores board positions.**
  Seat latency med 12.0 s / p90 120.6 s / max 196.9 s (30 windows >60 s, 20 >120 s). This corpus the
  adjudication coin landed FOR deck135 (vs17 WIN, ahead 13/4) — the mirror of wave-13's vs110 robbery.
  Read adjudicated games as board states, not results. Corpus-wide item (brief owns it).

## MODEL (route to model-experiments; NOT guide text)

- **NEW — Force reflexive-cast swing.** deck135 went from wave-13's perfect discipline (1 offer,
  correctly declined) to casting ALL 6 offers this corpus. Correct on the two WIPES (deck140 s18/s30,
  guide-cited), WRONG on Cranial Plating on an empty board (deck110 s4, not on the sanctioned list),
  and on a false damage premise vs Volcanic Hammer (deck21 s27, thought 3 kills a 6/6). No
  face-burn/discard Force offers this corpus, so the decline spine went untested. Model-side; the
  guide already scopes Force correctly (routed up to general-suggestions #3).

- **NEW — two Rule #1 creature-skips against verbatim frozen guide lines (both in LOSSES).**
  (1) deck110 seq3: chose Arcum's Astrolabe over an OFFERED `Cast Icehide Golem … {1}` on a
  hallucinated `{1}{g}` requirement — an ACTUAL offered-payable creature decline. This FIRES the
  wave-13 promotion trigger for the colored-pays-generic / trust-the-option-list CORE candidate
  (routed to general-suggestions #2). (2) deck133 seq18: chose Glacial Revelation (a dig) over an
  OFFERED Diamond Faerie (3/3 flyer) — disobeying the guide's verbatim "a 3/3 flyer beats a dig."
  Both are model-adherence against maximally-explicit frozen text; a 3rd guide restatement is the
  sanctioned-against escalation, so the guide stays frozen.

- **CARRIED — damage-vs-toughness rules gap** (deck21 s27, 3 dmg "kills" a 6/6). Same family as
  deck110's known gap. Model-side, cross-seat (general-suggestions #4).

- **CARRIED — summoning-sickness blindness in attack declaration** (the HARNESS #1 instances). The
  engine's attack-eligible list correctly excludes same-turn creatures and the model trusts it at
  genuine multi-attacker windows; harmless representation tic.

## DECK-CONSTRUCTION FLAGS (route upstream; user owns the decklist — restate, don't relitigate)

- **Manabase strands the deck's own gold / double-pip win creatures — STANDS (unchanged from
  wave-13).** Ice-Fang Coatl {G}{U}, Ohran Viper {1}{G}{G}, Abominable Treefolk {2}{G}{U}, Diamond
  Faerie {2}{G}{W}{U} off a base with only 2 Snow-Covered Island + 2 Snow-Covered Plains + fetches.
  This corpus: vs44 (real loss, dead at 1) is a near-identical repeat of wave-13's vs44 — no answer to
  an evasive snowballing 6/6 flyer. Direction (user's call): raise effective double-pip sources, or
  shave a pip off the creature suite.
- **Slow clock is structural.** Abominable Treefolk is the only real closer and lands T6–T13; small
  bodies can't race aggro (vs21/vs110) or a 5/5-trample Obliterator (vs133). Construction, not guide.
  This corpus the ONE game where the closer stuck AND a board wipe was countered (vs140) was a win —
  which is exactly the deck's ceiling working as designed when Force protects the finisher.

## VALIDATION TODO (for the next run)
1. ENGINE #1b: confirm the fix (when it ships) — a Force alt-cast chosen with "(no untapped sources)"
   PUTS Force on the stack and counters the target (no silent fizzle). The counter itself is now
   proven to work (ENGINE #1a) — this is purely the tapped-out execution path.
2. ENGINE #1a regression watch: any Force cast whose target resolves (stack→graveyard/battlefield)
   instead of stack→exile.
3. ENGINE #2 (freeze half): a non-terminal Treefolk-freeze window where the frozen creature is
   re-rendered — confirm `frozen`/no `[attacking]`/no `untaps and can attack next turn`. UNVALIDATED
   this corpus (the one freeze ended the game).
4. HARNESS #1: an `ATTACK: A1, <Name>` where <Name> IS a legal A2+ attacker (first outcome-coupled
   instance) → promotes the name→index reconcile arm.
5. colored-pays-generic CORE candidate: it PROMOTED this corpus (deck110 s3 actual decline). Watch
   for a 2nd cross-seat offered-payable decline to confirm before any core edit.

## PER-DECK ROTATION VERDICT: **DO NOT ROTATE deck135 out. KEEP IN.**

The rotation rule retires a veteran whose guide takes NO modification AND whose seat surfaces NO new
work signal this cycle. deck135's guide took no modification (FROZEN, byte-identical) — but the seat
surfaced the corpus's single HIGHEST-value new work signal: the first ever completed live Force
counter (ENGINE #1a validated) AND a new, sharply-characterized execution bug (ENGINE #1b, the
no-untapped-sources alt-cast fizzle) on the ledger item deck135 OWNS. It also fired the wave-13 CORE
promotion trigger (deck110 s3) and carries UNVALIDATED work (ENGINE #2 freeze half, still
un-reproduced). This seat is the designated owner of the Force seam and that seam is now the most
active engine surface in the project. Rotating it out this cycle would orphan the exact validation
loop (ENGINE #1b fix → confirm) that just opened. KEEP deck135 in through at least the ENGINE #1b
fix + confirmation.
