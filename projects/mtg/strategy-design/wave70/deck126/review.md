# Wave-70 deck126 review — SANGUINE BLOOD (WGB wall prison, Exquisite Blood + Sanguine Bond)

Seat files (`...ai_baka_deck126-<ptr>-vs-...`): five in
`/home/magi/.Wagic/ai/gpt/selfplay-runs/matchups-20260906-224849`, the sixth (vs deck152) in
`.../matchups-20260907-065922`. Record 2/6. 310 decisions in this seat, 3 fallbacks, **0
`off_protocol_bytes` in every record of all six games**, 0 `action_before_plan`, 0 truncations, 0
`reasoning_chars==0`. Latency p50 61.3 s / p95 186.6 s; `reasoning_chars` p50 6,038 / p95 11,276 /
max 23,118. Every reply this seat wrote was exactly PLAN + action.

## 1. Game by game

| # | file (seat) | opp | result | deciding decision |
|---|---|---|---|---|
| 1 | 1788752955 | 162 | **WON** 33–0, t11 | Bond t9 (seq12), Blood t11 (seq15), 2 Vampires swing (seq17). Clean line, no misplay found. |
| 2 | 1788752975 | 123 | **WON** 39–0, t16 | Tutor→Blood t5 (seq8/9), Bond t9 (seq13), Blood t11 (seq14). Pair complete t11; seq17/19/21 the only cast row was a dead Tribute ("they control 0 creatures - at 0 this does nothing") so the holds were correct — the loop closed off the *opponent's* own life loss. |
| 3 | 1788752945 | 146 | LOST 21–(-1), t19 | **seq 8** (t8, life 11): tapped out for Chromatic Lantern instead of Tribute/wall into a 6-point crack-back with **zero creatures on board**. Life 11→5 next turn and never recovered. Detail below. |
| 4 | 1788752966 | 130 | LOST (-2)–12, t18 | Not a misplay game: two Rorix Bladewing (6/5 flying, haste); seat has no flier answer but Tribute, and it spent Tribute on Rorix #1 at seq15. By seq20 (t17) the crack-back header already read "would KILL you - 6 from 1 attacker nothing you control can legally block" and the only cast row was Staff of Nin. Lost to evasion, not to a decision. One guide-order departure at **seq 17** (see §3-E). |
| 5 | 1788752968 | 125 | LOST 31–175, t55 | Pair never closed: Sanguine Bond **countered twice** (Cancel at seq54, Dream Fracture at seq108) and Idyllic Tutor countered by Cancel at seq68. Matchup/variance, not guide. Two engine defects fell out of it (§2). |
| 6 | 1788782365 | 152 | LOST (-5)–20, t11 | Mull to 6 (seq2-4, bottomed Wall of Omens — correct per guide). Elite Spellbinder + Sigarda (both flying) for 10-11/turn from t9; three 0/4 defenders, none with reach. Blocks at seq15/seq18 were the best assignment the render allowed. Matchup. |

**Hung game (main dir, 1788752973 vs deck152).** This seat's log stops at **seq 17, t14, life 1**,
having just cast Exquisite Blood (seq15) and revealed Sanguine Bond off Idyllic Tutor (seq16/17) —
one resolution from the pair. The stall is on the deck152 seat (MDFC land-drop decline livelock);
already logged as **known-bugs L1/L2**, nothing new from this side. Worth recording only that the
hang destroyed a game the pair was about to close.

## 2. Engine / interface / card items

**HIGH-1 — `noop_row_zero_reask` fires FALSE on a Tribute row that then resolves and pays out.**
File `1788752968-ai_baka_deck126-...-vs-ai_baka_deck125.jsonl`, seq **131** and **133** (both of this
seat's 2 `noop_row_zero_reask` fallbacks; 2 of 2 false). Rendered row at seq 131:
`1. Cast Tribute to Hunger {2}{b} {right now: they control 1 creature - Emrakul, the Aeons Torn (17/15) [...] is sacrificed, you gain 15 - but an effect already on the stack is aimed at that same creature; if it is gone when this resolves they control 0 creatures and this does nothing}`
The guard reads the trailing conditional "this does nothing" as a zero verdict and refuses, *while
the same row is accepted one window later at seq 132 and pays 15 life.* Same predicate family as
**L3** (`rowSaysNoOp` substring-matching a conditional clause and ignoring the verdict clause on the
same row) — a third independent repro, and the first where the guard's verdict is contradicted by
the engine's own resolution in the next window. known-bugs R lists `noop_row_zero_reask` as KEEP on
6-of-9 firings; this seat's 2 of 2 were false, so the KEEP should be conditional on fixing the
predicate first.

**HIGH-2 — second repro of L6 (Tribute double payout off one creature), from the casting seat.**
Same file, seq **130 → 132**: two Tributes cast in the same window against one Emrakul, each
printing "is sacrificed, you gain 15"; life 10 → 25 → 38 across seq 134-136. I verified the card is
NOT at fault: `bin/Res/sets/primitives/mtg.txt:124073-124079` is byte-faithful to Scryfall's Oracle
(`{2}{B}` · "Target opponent sacrifices a creature of their choice. You gain life equal to that
creature's toughness."), so L6 is an engine resolution defect (the `dynamicability
toughnesslifegain` / `sacrifice` pair resolving twice) and not a card-script one — and the row
render is complicit: at seq 132 Emrakul is still listed on their line and the row still leads with
"is sacrificed, you gain 15" while this seat's own first Tribute is on the stack.

**HIGH-3 — the completed pair has NO header; the incomplete pair has a five-line one.**
When a half is missing the prompt prints `LOOP HALF PENDING: <half> is on YOUR battlefield and the
other half ... is in your graveyard ... the pair is NOT one resolution from closing` — 158 windows
in the vs125 game alone (a game where it could never close), 3 vs162, 2 vs152, 1 vs123. When **both**
halves are on the line — the state that wins the game — the only structured block is
`LIFE-TO-DAMAGE CONVERTER on the battlefield: yours - Sanguine Bond` (deck123 seq 21), which names
*one* half and never says the pair is complete or that any life event now chains to lethal.
Exquisite Blood appears only as a battlefield-line entry. Salience is exactly inverted: the engine
narrates the dead state at length and the winning state not at all. Repro: deck123 seq 17/19/21 vs
deck125 seq 130 (same seat, same fields, opposite completeness).

**MED-4 — the crack-back line is unreachable by the guide's own vocabulary at a casting window.**
deck146 seq 8, rendered header: `CRACK-BACK NEXT TURN: 2 of their creatures will be able to attack
(tapped ones untap first), for up to 6 - you would be at 5`. There is no `none of your creatures can
block` string at a main-phase casting seam (that phrasing belongs to the combat header), and the
seat controlled **zero** creatures, so nothing in the prompt says "you cannot block any of this".
A render fix ("of that, 6 from 2 attackers nothing you control can legally block", which the engine
*does* print in the deck130 seq 20 crack-back) applied at every casting seam would close it; I
patched the guide side too (§3-D).

**MED-5 — a taps-you-out row carries no survival annotation while a Tribute/wall row does.**
Same window: rows 1 and 3 (Tribute, Pride Guardian) each carry a `{crack-back cover: ...}` clause
computing where the player lands; row 2 (Chromatic Lantern) carries only `{taps you out}`. The row
that *worsens* the crack-back is the one with no crack-back arithmetic on it, so the cheapest
comparison the model can make is missing exactly where it matters. Add the cover clause (or an
explicit "adds no blocker and removes no attacker") to every row when a crack-back line is printed.

**LOW-6 — `LOOP HALF PENDING` is re-rendered verbatim 158 times in one game** (vs125) at full length
including the recursion paragraph, long after the board settled. A one-line form after the first
print would pay for itself. Related to WATCH in known-bugs (format ceremony as reasoning cost).

**Not recurring in this seat:** Idyllic Tutor payload stranding (L19/LOW-2) — the one Tutor with no
reveal record (vs125 seq 67) was **countered by Cancel** (seq 68 stack render confirms), not
stranded. No SEGV/abort; 5 `async_drops` (vs125) + 1 (vs130), 1 `http_error` fallback (vs125 seq 16,
HTTP 400, `max_tokens 400` — the BK retry-record shape).

## 3. Guide verdict: **EDIT** (`strategy.txt`, 19,971 B, unchanged from the live 19,971 B; net 0)

Every addition is paid for below; the added bytes were funded by trimming the mana-window section (whose
windows the engine now auto-skips: `mana_only_windows_skipped` 260 in vs125, 29 in vs152 — the model
never saw one), two restated WORKED SITUATIONS lines, and wordiness in HOLD/DISCARD/MULLIGAN.

**A. Rung-7 Chromatic Lantern escalation — narrowed. Paid by deck146 seq 8 (the game's losing decision).**
before: `IT MOVES ABOVE ENTRY 4 IN EXACTLY ONE SHAPE: an enchantment half or Idyllic Tutor on your hand line AND "Mana available:" at 4 or less`
after: `IT MOVES ABOVE ENTRY 4 IN EXACTLY ONE SHAPE: a half or Idyllic Tutor on your HAND line that your untapped SOURCES cannot pay the coloured pips of - count the {b}/{w} sources under the mana line first. A low "Mana available:" is not that shape: if the pips are already there the Lantern buys nothing and the wall is right.`
The model's own reasoning at seq 8 shows the clause doing the damage, twice over: it computed the
right play and then overrode it — *"If I cast Lantern, I tap out. Next turn they attack with two
3/2s. I have no walls. I would take 6 damage and go to 5. ... blocking is safer. However, the guide
explicitly overrides this with the Lantern rule."* and *"I can make {B}{B} easily without Lantern.
... So Lantern is probably not needed for mana. But the guide says it moves up ... I'll follow the
guide."* Both trigger conditions were satisfied literally (Exquisite Blood was on the hand line;
Mana available 3) and both were meaningless: Blood needed 5 mana and the missing half was in the
graveyard, and its three lands already made {B}{B}. Life 11→5; the seat never cast an enchantment
again and lost at t19.

**B. The crack-back binds below lethal — new sentence in entry 0. Paid by deck146 seq 8.**
after (added): `THE CRACK-BACK BINDS BELOW LETHAL TOO: while it prints any number and you control NO untapped blocker, a row that neither adds a blocker nor removes an attacker is not worth "casting this taps you out" - the wall or the Tribute is.`
The old entry 0 keys only on `that would KILL you`, so a 6-of-11 crack-back with an empty board
bound nothing; the model checked that string, found it absent, and dropped straight to the rung list.

**C. Tribute's second-window rule now quotes the string that is actually rendered. Paid by deck125 seq 132.**
before: `Read "ON THE STACK" first: a Tribute of yours there means that creature is already dead - "Cast nothing right now".`
after: `THE ROW SAYS SO ITSELF: "but an effect already on the stack is aimed at that same creature; if it is gone when this resolves they control 0 creatures and this does nothing" OUTRANKS the "is sacrificed, you gain N" in front of it - take the hold row, never a second copy.`
The `ON THE STACK` block *is* present at seq 132, but the fact that decides the window lives in the
row's brace clause, and the model read the leading "is sacrificed, you gain 15" instead — its PLAN
line says *"sacrifice Emrakul and gain 30 life total"*. (This is the strategy half of HIGH-2; the
engine half stands whatever the guide says.)

**D. The low-life Tribute escalation now keys on the crack-back line too. Paid by deck146 seq 8/10.**
before: `The header prints damage you cannot block ("none of your creatures can block") and your life is 12 or less: ANY live Tribute row ... outranks another wall.`
after: `The header prints damage you cannot block ("none of your creatures can block", or a "CRACK-BACK NEXT TURN" total with no untapped blocker of yours) and your life is 12 or less: ANY live Tribute row ... outranks another wall, a Lantern and a Staff.`
At seq 8 the seat was at 11 (≤12) with no blocker and a live Tribute row on the menu; the clause
should have fired and could not, because its trigger string only exists in the combat header.

**E. Sorin over Staff of Nin while Sanguine Bond is alone. Paid by deck130 seq 17.**
before: ` 5. Staff of Nin.  6. Sorin.`
after: ` 5. Staff of Nin.  6. Sorin - but with SANGUINE BOND ALONE on your line SORIN COMES FIRST: his Vampire's lifelink gain is life YOU gain, so the Bond converts it; the Staff's damage is not.`
The model departed from the printed order here and was **right** — its reasoning: *"Staff pings 1.
Sorin makes a 1/1 lifelinker. If I attack with the 1/1, I gain 1, they lose 1. ... Sorin is
definitely superior."* Bond converts life *you gain*, and Staff damage is not a life gain, so a
lifelink Vampire is 2 points a turn under Bond-alone against the Staff's 1. The guide's fixed 5/6
order was wrong in that regime; the sibling sentence in `=== STAFF OF NIN AND SORIN ===` was
corrected the same way ("the Bond alone does not convert it").

**F. No header announces a complete pair — new lead line under THE WIN BUTTON. Paid by HIGH-3.**
after (added): `Read your battlefield line at every decision - NO HEADER ANNOUNCES A COMPLETE PAIR. The "LOOP HALF PENDING" block prints only while a half is MISSING, so its absence is not "no loop".`
Decision-time only; it tells the model what the prompt will and will not do for it.

**What KEPT and earned it:** the LAND DROP rule (6/6 games, every land row taken); the dead-Tribute
"ZERO STOP" (deck123 seq 17/19/21, deck130 seq 11 — four correct holds on `at 0 this does nothing`);
the HOLD ROW rule (`hold_windows_skipped` 720/102/39 with no wrong hold found); the "second copy does
not complete the pair" rule (deck125 seq 199 correctly read the `[second copy]` tag); MULLIGAN and
BOTTOMING (the one mulligan, rerun seq 2-4, was by the book).

## 4. Not checked
The opponent seat logs were read only where this seat's window needed context (deck125 counterspell
timing, deck152 rerun attackers). I did not re-derive L1's livelock from the deck152 seat, did not
audit the `.stderr` files, did not census `hold_windows_skipped` window-by-window for a wrong skip
(only for a wrong *take*), and did not verify Emrakul, Rorix, Sigarda or Elite Spellbinder against
their primitives — no decision in this seat turned on their printed text.
