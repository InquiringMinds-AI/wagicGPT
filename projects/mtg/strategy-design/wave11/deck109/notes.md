# Dev notes — deck109 wave-11 (VERIFICATION wave; engine/harness/reply-protocol ledger with repros)

## Record: 5/6 (context only per doctrine) — matchup + draw-speed variance
Beat 135, 140, 110, 44, 131; LOST to 133. Matchups INVERTED vs wave-10 (then: lost 44 / beat 133;
now: beat 44 / lost 133) — n=6 draw-speed variance on the two structural-edge opponents. Guide
FROZEN byte-identical to the live guide (the wave-10 hybrid line is verified, not re-edited).

## Corpus / provenance
- Run: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-072254` (21 games round-robin, decks
  44/109/110/131/133/135/140). Binary `/tmp/wagic-54a5a9d01` (wave-10 batch + max_tokens 4096 +
  ramble guards + stack-target annotation fix + fetch pre-targeting keying). Answer-after-plan.
- deck109 seat files (opp): 1784118181 (135), 1784121838 (140), 1784122253 (110), 1784122965 (44),
  1784123208 (133), 1784123504 (131).

## ENGINE-FIX VERIFICATION (this seat)
- **Ramble guards / prose-hijack cast:** none at this seat. The one non-timeout fallback
  (vs135 s12) is an out-of-range `CHOICE: 5`, correctly returned choice `-1` + heuristic action —
  NOT a prose-scanned integer cast. The ramble guard behaved as designed (formless/illegal reply ->
  fallback record, not a scavenged cast).
- **Fetch pre-targeting keying:** N/A — deck109 runs 20 basic Mountains, 0 fetches. (No contribution
  to the corpus-wide 115 fetch windows.)
- **deck109 hybrid line:** VERIFIED (see LEDGER ITEM A below). 0 `{B/R}` declines, 0 spirals.
- **Auto-tap color-stranding / Force of Negation:** N/A at this seat (mono-red, no free-alt-cost,
  no color-stranding surface).
- **Legal-target names** (`- legal targets right now: ...`): rendered across burn windows; 0
  self-target burn picks (0/all target windows). Working.
- **adj0/adj1 rows:** none for deck109 (all 6 games decisive; results.tsv life/turn cross-checked
  against seat gameends — consistent).
- **empty_reply (corpus-wide 120s-timeout class):** 3 at this seat (vs135 s14, vs135 s24, vs131 s5),
  all reply_len=0, all in WON games, all resolved by heuristic. Do NOT re-diagnose (WAGIC_GPT_TIMEOUT
  240s shipped next corpus, commit 95cf9f5f9).

## LEDGER ITEM A (engine / representation, cross-deck) — annotate HYBRID mana costs on the option line
**STATUS: still UNSHIPPED; the deck-guide card note is VALIDATED as the interim carrier.**
Verified 0 `- hybrid:` strings in any deck109 prompt this corpus — the option line still renders bare
`Cast Rakdos Cackler {b/r} (1/1)`. The wave-10 guide card-note (Rakdos Cackler `{B/R}` = hybrid, one
red pays it) drove the wave-10 decline/spiral class (deck131 s18/19/21) fully to zero at this seat:
- 0 "lacking black mana" / "no black" declines (was 3 windows).
- 0 spiral markers (was 1 full-budget spiral -> unparsed -> `-1`).
- 2/2 offered `{b/r}` casts TAKEN (vs44 s3, vs131 s3); vs44 s3 reply echoed the guide fact verbatim.
**Proposed engine change (unchanged, still owed):** append a hybrid hint to any option whose cost
carries a `{X/Y}` symbol, e.g. `Cast Rakdos Cackler {b/r} - hybrid: pay with {R} or {B}` (and
`Boros Reckoner {r/w}... - hybrid: {R} or {W}`). Owner: the option-text builder in AIPlayerGPT.cpp
(same site that renders `{right now: ...}` and `- legal targets right now: ...`). This is the durable
cross-deck backstop; the deck-guide card note remains the interim and must NOT be demoted until this
ships (thin sample + sole carrier — see skill.md refinement).

## LEDGER ITEM A' (reply-protocol / representation) — UNLISTED-CARD FIXATION -> out-of-range index
**NEW this wave, 2nd witnessing seat for wave-10 deck110 P3.**
**Repro:** deck109 vs135 seq12 (T7, myL19 oppL17). Options (4): `Cast Pillar of Flame {r}` /
`Cast Searing Spear {1}{r}` / `Cast Krenko's Command {1}{r}` / `Cast nothing right now`. The PLAN
abandons its own previously-planned LISTED Krenko's Command ("My previous plan to cast Krenko's
Command is too slow") and fixates on **Hellrider — not offered / uncastable** ("I have enough mana to
cast the 3/3 Hellrider right now... I will cast Hellrider"), then emits `CHOICE: 5` (out of range for
4 options) -> parser returns `-1` -> heuristic fallback. The pilot appears to treat the imagined
unlisted card as an extra 5th option.
**Distinct from** the deck140 P2 answer-last trailing-index defect (there reasoning is CORRECT, only
the index is wrong). Here the reasoning reaches an ILLEGAL choice and the out-of-range index faithfully
encodes it.
**Proposed reply-protocol change (route here, NEVER the guide/core):** the A/B that reconciles the
trailing index against the plan's NAMED action should additionally REJECT-AND-REPAIR when the named
card is not among the listed options — re-ask, or fall back to the plan's best LISTED sub-conclusion
(here Krenko's Command). Single-seat-per-wave, now 2 seats / 2 waves -> flagged to synthesis
(general-suggestions #2) for promotion from WATCH.
**Impact:** none — vs135 WON; mild tempo only (the abandoned Krenko's Command was the correct develop).

## Blocker-seam verification (1 window, correct)
| file | seq | T | my/opp | surfaced line | choice | verdict |
|---|---|---|---|---|---|---|
| 1784118181 (135) | 7 | 4 | 20/19 | up to 2, at 18, NOT lethal | `BLOCKS: none` (0) | CORRECT (NOT-lethal branch OBSERVED OBEYED) |
- Only 1 blocker window all corpus (deck raced harder than wave-10's 2). LETHAL branch + exception-(a)
  had NO window -> BLOCKING block KEPT at full prominence.
- **Obliterator-block class: ZERO test windows AGAIN (3rd consecutive wave).** vs133 cast Phyrexian
  Obliterator (113 hits in the opp seat) but deck109 got 0 blocker-kind windows in that game — it
  raced and took the hits (correct), never faced a block decision. Guardrail neither fired nor
  violated -> KEEP at full prominence. Punisher-rider USE verification remains at the deck133 seat
  (engine ledger #3, still owed).

## strategy.txt freeze-check (FROZEN — byte-identical to the live guide)
`diff strategy.txt bin/Res/ai/baka/deck109_strategy.txt` = IDENTICAL. Also `diff` vs
wave10/deck109/strategy.txt = IDENTICAL (the live guide IS the wave-10 output). Every line held:
- Identity + #1 DEPLOY-AND-ATTACK + "declare EVERY creature that can attack" — 26/26 attacker windows.
- MP2 checklist / LETHAL recognition — wins closed on lethal (Hellrider-ping math correct); phantom-
  lethal watch CLEAN (4 "lethal" mentions all genuine/hedged, development taken in every case).
- BURN face-default + Pillar `{R}` corrective — 0 self-target, 0 false `{1}{R}` cost belief;
  token-maker (Young Pyromancer vs131 s7) + lifelink (Vault Skirge vs110 s11) exceptions fired.
- Rakdos Cackler `{B/R}` hybrid note — VERIFIED (Ledger Item A); frozen at full prominence.
- BLOCKING re-anchor + exceptions + NEVER-block-damage-punisher — NOT-lethal branch OBSERVED OBEYED
  1/1; LETHAL branch + exception-(a) + Obliterator absolute untested this corpus, KEPT.
- PHYREXIAN OBLITERATOR note — no block window vs Obliterator (raced), KEPT.
- EVASIVE/UNTOUCHABLE BOARDS note — load-bearing; vs44 WON this wave against this exact class.
- KEY CARDS / MULLIGAN / SITUATIONS — consistent with observed play.

## Stopgap ledger
| stopgap / rule | defect it patched | status this wave | action |
|---|---|---|---|
| Rakdos Cackler `{B/R}` hybrid note | hybrid misread -> offered creature not cast / spiral | VALIDATED (class -> 0; fact echoed verbatim) | KEEP at full prominence; retire only when Ledger Item A ships |
| BLOCKING chump re-anchored to surfaced LETHAL | too-eager chump fighting the math | NOT-lethal branch OBSERVED OBEYED 1/1 | KEEP (thin; other branches untested) |
| deck109 Obliterator "NEVER block" absolute | punisher rider unconfirmed on Obliterator | 0 test windows (3rd wave) | KEEP until engine ledger #3 confirms rider (deck133 seat) |
| Pillar `{R}` (not `{1}{R}`) corrective | false-cost belief + hold-mana | 0 false-cost instances | KEEP (validated, load-bearing) |

## Deck-construction observations (upstream, unchanged — do NOT relitigate)
No answer to evasion (flyers/shroud/pro-red) — beat 44 (untouchable flyers) on the race THIS wave,
lost it last wave; won vs110 same class both waves; the structural weakness stands but is racy.
No card advantage / mana sink — inherent to RDW; lost vs133 (devotion+Obliterator+Gray Merchant) on
draw speed (won it last wave by racing out T8). Do not change at 5/6.

## Resume handoff
1. Guide FROZEN byte-identical; the wave-10 hybrid line is verified end-to-end and shippable standalone.
2. Two durable items routed OUT of the guide: (A) engine option-line hybrid-cost annotation — STILL
   UNSHIPPED, keep the guide note until it lands; (A') NEW reply-protocol reject-and-repair for
   unlisted-card fixation (deck110 P3 2nd seat).
3. Convergences FLAGGED to synthesis (general-suggestions): #1 the hybrid/offered=payable family is
   VINDICATED at the representation rung (core PASS confirmed); #2 the unlisted-card-fixation
   reply-protocol sub-variant reached a 2nd seat (promote from WATCH).
4. Watches held (do NOT promote without further instances at OTHER seats): Obliterator-block class
   (guardrail kept, still awaiting a real test window across waves).
