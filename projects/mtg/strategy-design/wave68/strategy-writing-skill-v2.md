# Skill: writing a per-deck strategy guide (v2, 2026-09-06)

## 1. Purpose and reader

You are a REVIEW SEAT: an agent that reads one deck's games from a corpus and then edits that deck's
strategy guide (`projects/mtg/bin/Res/ai/baka/deck<N>_strategy.txt`). Read this file once before
editing; it is the whole contract.

The guide's reader is not you. It is the playing model (a 35B pilot) that reads the guide fresh at
EVERY decision, pasted under a prompt that prints the board, the hand, the mana line and a numbered
menu of legal rows. It has no memory of any earlier decision, game, corpus or wave. It cannot count
what is not printed, cannot remember what it did last window, and does not know what "this corpus"
means. Everything the guide says must be usable by that reader, at that moment, from that screen.

Two documents come out of a seat's work and they hold different things:

- the GUIDE holds decision-time instruction only (section 2);
- `review.md` (your seat file) holds the evidence — seqs, counts, verdicts, predictions, what was
  kept and why. The guide states the rule; the review states the proof.

## 2. The guide contract

### 2.1 Amendment 332 (owner ruling, 2026-09-06) — verbatim

**Owner, verbatim, after reading deck125_strategy.txt at 71,499 bytes:** "The guide is too long. It contains
many details that are not relevant to the llm playing the game. It even uses concepts that the llm has no
context to understand. 'This corpus' 'previous games' these are useful to the agent writing the guides maybe,
but they dont belong in the guide."

**The rule.** The playing model reads the guide fresh at every decision with no memory of any game, corpus,
wave, or seat. Therefore a guide may contain ONLY: (a) decision rules, stated as conditions on what the prompt
prints; (b) the deck's engine — what it is trying to do and in what order; (c) deck-specific card facts the
prompt does not already print; (d) worked examples phrased as SITUATIONS ("at 3 life with an untapped Hive
and no creatures on their side, ..."), never as past games. It may NOT contain: corpus, wave, seat, seq, game
or record citations; "previous games", "last wave", "this corpus", "you declined N of M windows", counts of
past behaviour, or any sentence whose subject is the model's own history. That evidence belongs in the
reviewing seat's review.md, where the seq that paid for a rule is recorded — the guide states the rule, the
review states the proof.

**Size.** The 41,000-71,500 byte band is retired as a target. A guide is as long as its rules need and no
longer; the rest of the pool (5-18 KB) is the reference range. A guide that cannot say its rules in 20,000
bytes is carrying something that is not a rule.

**Verification before install.** `grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this
wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}'` on the guide must print 0.

**Supersedes** every earlier amendment insofar as it asked for seq citations, "WHAT THIS COST" ledgers, or
per-corpus counts INSIDE the guide (those amendments' rules still hold for review.md).

### 2.2 Guide shape (from the wave-68 rewrite brief)

1. The deck in one paragraph: what it is trying to do, the engine, the win condition, the kill turn.
2. The engine's order of operations: what to cast/hold/activate first, and the printed conditions
   that change it.
3. Rules, each as CONDITION (what the prompt shows) -> ACTION, one or two sentences, grouped by
   decision kind: land drop; what to cast; holds and passes; attackers; blockers; targets; X;
   discard; mulligan.
4. Card facts the prompt does not print: interactions, what a card is FOR in this deck, traps.
5. A short worked-situations section (5-10 items): a situation and the right answer, no game
   references.

Plain prose and short lists. No headers naming waves, seats or corpora; no "you did"; no counts of
past windows; no "newest only" ledgers; no meta-commentary about the guide itself.

**Size**: hard ceiling 20,000 bytes; expect 8,000-16,000. If a rule does not fit, it was not a rule.

**Verification** (both, numbers printed in your final message and in review.md):
- `stat -c %s` on the guide is <= 20000.
- `/usr/bin/grep -ciE 'corpus|wave[- ]?[0-9]|\bseq\b|previous game|last wave|this wave|census|paid (for )?by|[0-9]{3}v[0-9]{3}|you (declined|took|cast|passed) [0-9]'` prints 0.

Standing owner doctrine that governs every rule you write: the reviewer owns the deck guide; hard
caps on the model's legal choices are rejected; every guide rule is reasoned and odds-based, never
a code-like floor; the engine never lies to the pilot and the guide never teaches doubt of a printed
surface (the trust doctrine).

## 3. The method — how a seat turns a corpus into a guide edit

**Read the games, not the summaries.** Read every reply at the windows that mattered. A record's
`choice` can differ from the reply's conclusion; the reasoning between first and last coded line is
the mechanism trace. [A116]

**Classify PERCEPTION vs STRATEGY before writing anything.** Was the fact the pilot needed PRINTED
on the row it answered? If not (missing render, truncated target list, parse fault, an engine
decision the pilot was never asked) it routes to the engine/render ledger, never the guide — a
guide cannot fix a target shown as illegal or an answer the parser could not read. [A44, A127,
A152, A197, A236, A273] If it was printed and the pilot reasoned past it, that is strategy — but
first check for a wrong BELIEF (the same wrong arithmetic every time; a mana objection to a `{T}`
cost): a belief is corrected by one fact in the render's own words, not a louder imperative. [A106,
A218]

**Diagnose a break in this order**, stopping at the first that fits: (1) a missing fact — write it;
(2) a contradiction elsewhere in the guide (a card note against a procedure, two co-occurring
absolutes, an imperative one paragraph below the stop) — fix the ORDER or the collision, not the
wording; (3) every "break" was a correct play — the rule is mis-scoped: carve the shape out, never
tighten; (4) a third identical failure with the fact printed — restatement is exhausted: shrink the
rule, move the check into the pilot's PLAN line, file the render item. Never a fourth paraphrase.
[A53, A65, A85, A96, A97, A102, A118, A158, A256, A288, A309] When a key measures something the pilot cannot
act on, re-key it, do not sharpen it; when the surviving breaks share a printed fact the rule did
not name, that fact is the missing condition; when a carve-out was taken with its condition false,
the defect is reading (move the condition into the imperative's clause), not wording. [A157, A204,
A316, A321] For a recurring break under a correctly keyed rule the allowed edit is a recognise->do
entry naming the WRONG answer and its cost; before filing a render item on a repeated identical
answer, grep the guide for the imperative that mandates it. [A225, A261]

**KEEP is a success verdict.** A rule that held is left alone; a wave whose predictions all passed
is the wave to write the fewest lines. "No open lane" and "the ceiling is the deck" are legitimate
terminal verdicts. [A220, A267, A325]

**Evidence for an edit** is a window count on the edit's own literal — offers, takes, and the split
`raw / live / consequential` (live = a better row existed on that screen). Never a win rate, a rate
whose denominator the engine moved, or one favourable sample. Report offers per card: three
restrictions and a 0% take is a card that never gets cast. "Violated-unpunished" is its own verdict
and never loosens a rule; a rule broken 40% of its firings across three corpora is one the pilot
does not have. A PASS with no window in which the rule could have broken is UNTESTED, with its
cause (window never arose / arose and the string did not print / deck cannot produce it). Count
where the pilot CITED the rule in its own words, and list the decisions the guide has no rule for —
correct ones included — and write the rule that sanctions them. A routing rule (which row) and a
reporting ritual (write this sentence) are scored differently; a rung's exit test is two-sided —
exhausted when its artefact is never produced OR produced with no change in the decision. [A21,
A45, A63, A64, A87, A93, A109, A118, A181, A186, A212, A216, A242, A275, A283, A292, A303]

**Before writing a new rule**: walk it against the render's vocabulary and find its falsifier by
reading; run a coverage diff against the other guides — losses usually trace to sections a sibling
has and this guide lacks; where one guide holds a shared literal at zero breaks and another breaks
on it, the passing sentence is the fix and the port is a copy; when a lane ships a count or tag on
a row, re-key every rule that derived that quantity in the same wave, by the QUESTION the old
clause answered. [A59, A81, A121, A144, A154, A169, A187, A203, A279]

**Run the literal audit first.** For every string the guide quotes as something the pilot sees:
count renders on the rows the rule governs (from the rendered `prompt`, never `options_text`,
whitespace-normalised), count old-string renders (expect 0), grep the guide for sentences that
assume the old string. A zero-hit literal, a marker on a new seam, or a sentence describing a
screen the engine no longer prints is a defect whether or not the rule held. Check every removal
condition recorded in review.md that has come due and retire what the render now answers; count
takes of every row the guide NAMES — a named row at 0 takes is ignored or superseded. [A90, A103,
A128, A168, A188, A209, A210, A239, A268, A289, A311, A318, A320]

**Where evidence goes.** review.md carries the seq that paid for each rule, the per-rule counts,
each edit's prediction (window class, literal, refuting number, "a hit is a break only if"), every
trim's prediction ("rung X stays at N of M; a rise restores the text"), each rule's removal
condition in its class (information-supplying: one corpus with the render carrying the fact;
belief-correcting: zero breaks across two corpora containing the triggering shape; or "none"), and
the byte count against a named baseline. None of it goes in the guide. [A15, A20, A61, A201, A270,
A285, A288, A328]

**How to write a rule.** CONDITION on what the prompt PRINTS -> ACTION as the line the reply emits.
The condition is a number, tag or quoted literal on the row being answered; the action is the row
to take or the exact answer (`BLOCKS: none`, `CHOICE: 0 (pass)`). Exit or null answer FIRST, as one
comparison against a printed number; then the positive branch; then the stop, in the same sentence
as the imperative. Several branches: numbered, decision order, highest-cost first, "stop at the
first that answers." [A11, A27, A60, A79, A126]

**How to write a worked situation.** A board in the prompt's vocabulary — life totals, what the
lines print, the rows on the menu — then the answer and the one printed fact that decides it. A
shape with blanks the screen fills, never a finished sentence the pilot can copy, never a past
game; the seq stays in review.md. [A105, A126]

**What earns bytes.** The engine order, rules keyed to printed strings, deck facts the prompt does
not print, the exclusion lists that close absolutes. Cut, in order: worked examples on rules that
hold; derivations the render now performs ("answer the marked row" keeps one sentence);
restatements; anecdotes about hands that never come; anything whose subject is history. A rung
surviving only on its example is the rung to rewrite. Before any cut ask whether the remaining
rules are jointly SUFFICIENT to answer the decision, not merely to forbid answers. [A20, A35,
A200, A284, A302, A307]

## 4. The durable rules

Each rule is stated once; bracketed tags name the amendments it was harvested from.

### 4.1 Rule phrasing — what the pilot can execute

- Every rule input is a number or tag the prompt prints: no derived counts ("count your hand"), no
  running tallies across windows, no mood words ("when desperate"). If the input is not rendered,
  pick a constant or a printed proxy and say so; never tell the pilot to count harder. [A33, A72,
  A155, A237]
- Take the quantity from the surface nearest the choice — the option row first, then the dedicated
  line, then the header; a substring inside a composite line fails by crowding. Name the UNIT the
  rule reads (row, line, menu header) or an aggregate eats it. A price rule quotes the row's printed
  number, never the formula; a counting literal names what is outside the count (`kills 0` = 0
  CREATURES). [A89, A173, A189, A193, A235, A301]
- One operation on one printed number: fold corrections into the constant (`8 - handline`),
  collapse case analyses to one number, and give one boundary one threshold — two thresholds for
  one boundary is a contradiction the pilot resolves by whichever it read last. [A48, A50, A73]
- A threshold is a constant against a printed finished number, never a relation between two moving
  totals — EXCEPT where the danger IS the relation between two printed numbers (life vs incoming):
  then write "if <printed A> is at or above <printed B>"; a constant cannot narrow either — a
  second printed literal sorts the cases. [A192, A214, A255]
- A constant floor is a proxy: name what it stands for and, where the render prints the proxy's own
  inputs, write the card's own test instead (`N >= the printed toughness of a counted creature`). A
  constant at 100% obedience is the one to audit. [A142, A331]
- A cap lives in the sentence of the verb it caps; the menu-reading step points at a number
  computed BEFORE looking at the menu, never a property of the menu, and never pushes the same way
  as the row's own headline. [A23, A86]
- Restate the stop wherever the imperative lives — an imperative said three times outvotes a stop
  said once; every take-it / send-all instruction carries its exception inline; a card note that can
  collide with a procedure names which wins; when a rule gains an exception every line summarising
  it gains it in the same edit. [A6, A11, A24, A51]
- Absolutes split on the edge case, edge case FIRST ("if X is the ONLY creature listed: none — stop
  reading"); audit a class absolute by constructing its least-harmful member. [A12, A135]
- Every absolute has a recorded falsifier — the board on which it is false — as a named exception or
  a lethal carve-out placed ABOVE it as a numbered check; ask "can obeying this decline a win this
  turn?" An absolute a rendered price can switch off names its SUBSTITUTE in the same sentence.
  [A30, A71, A123, A308, A315]
- Every "any row" / "never" absolute carries its exclusion list inline, as a LIST drawn from every
  other stop, closed by "when every row is one of these, the answer is <decline>"; a rule offering
  alternatives says what to do when none is on screen, with the do-nothing row promoted. [A185,
  A234, A277]
- Three or more branches: numbered checks in decision order, highest-cost first, "stop at the first
  that answers" — never a paragraph of exceptions (the pilot resolves prose by narrative fit). [A27]
- An exception lives ABOVE the imperative it excepts; an unconditional rung deletes every
  conditioned rung beneath it; a conditioned exception to an ordered list is a numbered entry at
  the rank where it wins, and a floor that must beat a list is numbered into it. [A26, A70, A108,
  A165]
- A first-match list is a total function: every castable card matched by an entry, a named
  catch-all, "cast nothing" ranked; every conditional stated elsewhere gets a rank; the list ships
  a READ instruction ("read entry 1 first, every window"); the game-winning step is step 0 and
  overrides the rest, latches included. [A62, A76, A92, A199, A313]
- An entry that can match two rows names its winner; a rule elevating a set writes the tiebreak for
  two members on one menu. [A95, A241]
- Two imperatives that can fire in one window state which runs first, at the site of the later one;
  two rules quantifying over the same card set are read against each other. [A97, A228, A304]
- A posture rule selects among options the price rules allow and never adds options they refuse;
  it names the price rule it defers to, and its trigger is a literal on this prompt, not an
  archetype name. [A78, A114, A251]
- Every hold / brake names its RELEASE as a printed number or says none exists; a one-way latch is
  rewritten as a comparison of two on-screen rates; a prohibition names what the card is held FOR.
  An exclusion rule keys on the rendered clause that identifies members, category name as gloss
  only; both the braked and the exempt branch get imperatives. [A16, A156, A179, A206, A227,
  A253, A280, A281]
- A brake is an operation run LAST: (1) the literal byte-for-byte; (2) where to look — the row about
  to be named; (3) the releases, enumerated and closed; (4) a verdict on the answer already chosen
  ("if present and no release applies, that row is not your answer; name the next"). An anti-anchor
  clause names the LOSING row too. Leaks are counted in the exclusion, not the trigger. [A290,
  A322, A326]
- Naming a rationalization does not suppress it; only a test on a printed string does ("read the
  `{right now:}` brace; if it names 0 of anything, `Cast nothing right now`"). A banned plan
  sentence is a paraphrase class: state the structural test; refute the PRINCIPLE, not the instance.
  [A7, A28, A77, A254, A317]
- Never invite a mid-reply change of mind; every check is a PRE-condition on the single CHOICE line.
  [A314]
- A rule lives on the window that can still refuse: when the losing choice is made on a forced ask
  (target menu with no pass row; the X menu after `Cast Card Normally`), the rule sits on the row
  that OPENED it, quoting the last screen where "cast nothing" is still an answer. A marker answers
  WHICH, never WHETHER. [A140, A170, A244]
- A multi-ask sequence (mulligan -> keep -> bottom; cast -> target -> mode -> X) is one governed
  window: each section covers every ask or hands the next to another section by name. [A37]
- Every decision kind has a named owner — land drop, mulligan, bottoming, cast, target, mode, X,
  activation, attackers, blockers, forced sacrifice, reveal, cleanup discard; the free action
  needs one most. [A46, A136]
- A rule keyed to a rendered token branches on its ABSENCE and names every present form (collapsed
  and enumerated); a conditional annotation gets its untagged branch; a substring printed once per
  list member carries the list's cardinality; a re-key is audited for coverage over the rule's own
  windows. [A82, A111, A202, A221, A252, A269]
- Counts read off a line quote the collapsed handle once (`Island #1-#5 x5` = five) with the
  arithmetic; never lean on an enumerating clause for a count unless it states how many. [A91,
  A178, A249]
- Where the render marks the answer, line one is "answer the mark" and the derivation stays only
  for the unmarked case; where a marker prices a TRADE, supply the arithmetic ("count names after
  THEIRS and after YOURS"). [A110, A319]
- A rendered count is not a predicate: beside the number write the sentence it proves and its tense
  ("`of which 1` = their ONLY creature, NOW"); key countables to an always-present surface with
  the header as fallback; say what does NOT reset at turn start. [A5, A13, A107]
- Superlatives name the rendered field they range over; each procedure step names the string that
  satisfies it; a send-list has a tiered head and a sequenced tail. [A215, A224, A248, A257]
- Where one number governs, the section OPENS with "the first line of your answer is <number>";
  the gate is the first clause of the dictated PLAN sentence, the action after it under "only";
  computed numbers get one blank per input (`L <>, C <>, stop <L+C+3>`), thresholds one line with
  one blank each, filled BEFORE the CHOICE line; one PLAN ritual per window class, naming its class,
  checked against the parser's reversal tests. [A79, A115, A120, A133, A172, A211, A245, A330]
- The PLAN line is written AFTER the choice, names only objects in this window's text, as forward
  intent; a fresh-computed stop outranks the carried plan, said once at the stop; an answer that
  contradicts its own plan is wrong on the answer. [A10, A94, A176, A196, A282]
- A rule must be true at the window it FIRES in, not where it was learned (an announce-X rule with
  a cast-menu premise is false by construction). [A306]
- One rule, one place, at its firing seam; elsewhere a pointer. Class facts live in one named block;
  the card section points, never restates; the situations list is derived from the rules, every
  branch present, diffed with each edit. [A96, A177, A190, A286, A307]
- Emit grammars come with the pass answer beside them (`CHOICE: 0 (pass)`, never `x0`) and list
  asks with the list's content rule in emit form. [A104, A134, A143]
- Tokens and cards are different nouns in a combat stop. A role is a printed count first ("if your
  untapped creatures number one"), cards as examples after. Class words are verified against the
  primitive's filter. A class rule names one plausible NON-member. [A39, A83, A145, A195, A293]

### 4.2 Prompt-surface literacy — what the pilot can and cannot see

- Read one late-turn prompt verbatim before editing (battlefield lines and header, hand line with
  count, `Mana available:`, the menu's per-row `{right now:}` verdicts and `{X pricing:}` lists,
  the `[repeat:]` / `[legendary:]` / `[second copy:]` / `at 0 this does nothing` tags, attack and
  block tags and headers, the carried `YOUR PLAN`). Key rules to what it prints, in its words.
- The trust doctrine: never teach doubt of a printed value. Its converse: a printed row is the
  engine's assertion that the action is legal and payable NOW; the plan, the log and last turn's
  memory are not evidence against it. A hand card with no `Cast` row is not castable this window;
  a printed row is payable. [A99, A151, A159, A250]
- A legal route-around uses only true surfaces as inputs ("read the header's land number, then add
  one for each <card> on the hand line"); "the number may be wrong" is never written. [A287]
- The carried `YOUR PLAN` block is NOT a render; a guide may tell the pilot to prefer a current
  `{right now:}` clause over it. [A207]
- The pilot reads verdicts about the BOARD; the stack is not the board. A rule about a card that
  can already be on the stack reads the `ON THE STACK` section by name. [A278]
- A per-row verdict is a fact about a row FAMILY; where a sibling family prints no verdict, say so
  as a card fact ("these rows print no result — read the target's toughness yourself"). [A139]
- Never quote prompt text you intend to override; describe it, assert the override. The pilot
  latches onto quoted text as guide advice. Never state what the render does NOT print — the claim
  goes false the day the field ships, and licenses doubt of the render meanwhile. [A1, A99, A238]
- Quote engine literals verbatim, in quotes, on their own — never dissolved into the sentence's
  grammar — because the audit that keeps them true is a grep. A literal that gains a clause is a new
  literal; a marker that moves seams makes every sentence about its location stale; when a lane
  adds a NUMBER to a clause a rule treats as a flag, rewrite the rule as arithmetic over it. A dead
  literal is an audit debt, not by itself a behaviour failure. [A209, A210, A226, A258, A264, A318]
- Anything printed inside a fatal parenthesis is INSTEAD OF, not as well as: `(your attacker dies,
  their blocker lives (lifelink: you gain 3))` pays the gain with the attacker. For every stop keyed
  to an outcome, list the keywords that can appear inside its brackets (lifelink, deathtouch,
  trample, first strike) and say the stop survives them. [A30, A55]
- Any rule keyed to a tag with a collapsed form keys to the PRESENCE of one instance plus the
  collapse's own count word, never to how many you can see. [A91]
- The pilot restates facts it can already read; rendering a fact does not shorten its reply. Do not
  write guide text whose goal is a shorter reply. [A100]

### 4.3 Card facts and Oracle verification

- Every card claim — cost, MV, P/T, loyalty, colours a land makes, targeting restriction, zone,
  return path, mechanism — is written from the primitive (`Res/sets/primitives/*.txt`) at draft
  time and re-checked every wave, never from memory. Retrieve the real Oracle text first; where
  Oracle and primitive disagree, the guide states the fact the ENGINE delivers and the seat routes
  the script fix. [A43, A145, A164]
- Print the integer beside every mana symbol the guide names (`{2}{u} (three mana)`). [A47]
- A by-name property list ("these lands make black") is a static claim in a game where permanents
  rewrite each other; prefer the render's per-colour count and say so. [A75]
- "The only answer" / "removal" / "the only way" are card-fact claims: check the primitive for a
  return path (`canplayfromexile`, `moveto`, `transforms`) and write the effect actually delivered.
  [A164]
- Once per deck, run the decklist against the primitives for structural traps: legendary duplicates
  (a dead second copy), non-stacking effects, cards with no way to act on a named condition (a hold
  "until their enchantment is gone" in a deck with no enchantment removal is an unconditional rule
  wearing a condition), and instants/flash (a deck with none says so: "if it were castable this
  window it would be a row"). [A68, A229, A247]
- A card whose conditional text the pilot could read backwards ("whenever an OPPONENT loses
  life") gets an explicit false sentence in the always-false voice. State a printed evasion or
  type-identity fact twice — as a capability and as an attack/mulligan order — because a fact stated
  for one phase does not reach the phase where it decides. [A174; wave-26 lineage]
- Printed-power floors are blind to engines: exempt creatures whose text contains `{T}:`,
  "Whenever ... attacks", "At the beginning of", or that act every turn. A counting rule names the
  class that defeats the count, in render vocabulary: block-triggered lifegain (Perimeter Captain,
  Pride Guardian) and life-to-damage converters (Sanguine Bond, Exquisite Blood). A brake keyed to
  a count counts every converter of the resource it brakes; a lethal check enumerates one-shot
  sources with their timing windows, not only per-turn ones; an exception narrowed by opponent
  behaviour is checked against every pool deck's list before it ships. [A17, A29, A149, A150, A161]
- Combo halves are rarely symmetric: for each half alone ask whether the deck's own engine already
  produces the input it consumes; if so, teach it as a standalone plan. A named pair is stated as
  two DIFFERENT names; the duplicate form (`<A> #1-#2 x2`) is named as NOT the pair. Every combo
  guide carries a no-starter branch for when the win-button's inputs are absent. [A42, A125, A175]
- A "which half do I have" pick reads the battlefield line AND the hand line before the PUT line.
  [A160]
- For each ability that can target its controller's own permanents, state the owner tag and the
  pass rule — not only for the one that bit last. [A124]
- When evidence overturns a card teach, the correction names the card, says why, and gives the
  observable proof shape. A teach keyed to an annotation covers every field and every form it
  prints, and is applied on every surface where the property renders (stack, battlefield, combat,
  target menu). Where a guide rule and an emitted clause can disagree on one board, the rule says
  which wins and why — after checking the primitive and the trigger timing. [A22, A31, A32, A213]

### 4.4 Decision-kind conventions

**Mulligan / bottom.** The rule for the first ask goes at the TOP of the file. Step 1 is the hard
floor as an imperative the pilot performs and stops on ("count the lands on the header line; at two
or more, answer KEEP and read no further"), keyed to the engine's land count and coverage line — the
only property the pilot may ship on; every other property (colour, curve) is a bottom-ask question.
The keep list is marked complete or "not a test". Mark every regime ("You will be left with an
N-card hand"): at `(keeping 5)` or lower, keep any hand with a land; the chain stops at the second
look. Bottoming needs a SPELL floor as well as a land floor, stated as the ordered PUT list with the
one class that never enters it at the current N. A floor is never moved on one window in either
direction. Check self-consistency by constructing the hands the sentences could disagree on
(0 / 1 land without coverage / 1 with coverage / all lands, at keeping 7, 6, 4). [A4, A38, A66,
A67, A69, A80, A119, A131, A143, A153, A171]

**Land drop.** Always has an owner line: "a `Play <land>` row is taken every window it appears
unless <printed condition>". [A46] Fetch lands, Treasure and rocks the engine offers as a row are
taught as a cost, not a choice: the prompt's own mana count first, the first window second, the
colour third. [A166] A fetch rule keyed to `(you cannot make {W} right now)` says what to do when
the tag does not print. [A269]

**What to cast.** One ordered cast list per guide; card sections hang off its numbered entries.
Rank is also order in time: an X spell or mana-hungry top entry goes first in the turn. A deploy
floor ("if a creature is offered, cast one") enumerates the engine-marked dead forms it does not
reach (`[legendary:` second copy, `at 0 this does nothing`, own-target-only rows, `[finds only ...
none left]`). One sentence per guide refuses "nothing else on the menu was castable" by name: a
menu on which nothing is worth casting is answered with the decline row. A small menu-level section
covers facts about the list rather than any card: two rows naming the same card (prefer the
cheaper), a row that is a strict subset of another, collapsed runs. [A62, A130, A184, A194, A246,
A266]

**Holds, passes, activations.** Every hold names its release. A repeatable activation carries an
already-on check pointed at a rendered field ("if the battlefield line already reads (3/3), pass")
and a checklist that reads the permanent's own printed state (`[tapped]`) and the payment's source
line. Every permanent with an activated ability the emitter can offer in upkeep gets a section
keyed to the row's present forms (`[Upkeep offer:`, `[repeat:`) before its first corpus; decks with
upkeep mana sinks carry a "your upkeep" block that reads the hand and land count and names what the
upkeep must leave untapped. A stop condition is testable only against a number the action itself
moves ("keep making tokens until N" fails if the tokens do not change N). [A40, A88, A113, A129,
A147]

**Attackers.** Every combat section carries an affirmative rung: name the rendered string that means
GO (`(you kill it, your attacker lives)` -> SEND; no blocker tag at all -> SEND). Team rules decide
one A-line at a time, with the sole-attacker branch (the only creature is also the only blocker) and
per-creature stops. Outnumbering their blockers does not make a send safe — THEY choose which
attacker to block, and they pick the one their block wins; an attacking creature does not block.
Prefer total-power gates to single-biggest-power gates. For a deck whose mana comes from creatures
or whose activations tap attackers, the lethal count is the first check of the turn, above the
deploy floor, and the survival gate lives at the ask that SPENDS the resource. A creature singled
out on defence is singled out on offence in the same paragraph. [A8, A9, A19, A36, A54, A56, A77,
A112, A163, A182, A205]

**Blockers.** The survival gate sits ABOVE the ladder: at a LETHAL header, first assign enough
"deals N" to bring the remainder below printed life, checking the remaining total after each
assignment; only then rank trades. Step 1 of the block section says which body is spent (tokens
first — a token row prints no mana cost), as a numbered step before any trade evaluation. The
ladder, render-keyed, shared across guides with one deck constant: rung 0 `(neither dies)` always;
rung 1 `(you kill it, your blocker lives)` always; rung 2 `(both die)` unless theirs is worth less;
rung 3 `(your blocker dies, attacker lives)` only when the header's "you would be at N" is at or
below the deck's floor — with the trample carve-out (`N tramples to your face` means the block does
not stop the damage) and the menace carve-out (one blocker on a menace attacker stops nothing).
`BLOCKS: none` permission is the FIRST line, as one comparison. A per-card exception is keyed to
the B-row parenthesis and the header N, never the attacker's identity. A damage rule says a kill the
header prices is a different question. [A2, A3, A25, A58, A60, A101, A141, A148, A271; wave-40 HL3]

**Targets and X.** Gate on the rendered tag (`[defender]`, `[flying]`), never on a numeric proxy
(printed power, a life threshold); where there is no tag, name members of the class. Two menu paths
that dispose of the same card need a verdict each by their rendered row names, including the
degenerate path (X=0 is not cycling). Every size rule for an X spell carries its life-critical
branch keyed to printed life and the header's creature count. A survival carve-out names the
post-action quantity it bets on and walks the subtraction on the row's own lists (THEIRS minus the
kill list against your life). Where the guide names a literal row text as the answer, also state
the answer in range terms, because the X menu collapses (`2-5. X = 5 down to X = 2`). Buff
placement asks a connection question before a size question. [A41, A49, A57, A74, A122, A162, A274]

**Discard (cleanup) and list answers.** Every list-answer ask has an ordered numbered list with
"stop at the first name you find", an anti-anchor clause ("do not answer from the top of the list;
never `PUT: 1` because it is first"), a printed string per step, and a comparison step for any
superlative. [A136, A143, A215, A248]

**Matchups.** One paragraph per pool mechanic that has beaten this deck, keyed to printed facts
(their line, their untapped count, the dedicated `LIFE-TO-DAMAGE CONVERTER` / `DRAW PUNISHERS`
lines), naming the mechanism's escalated form first in render vocabulary. Read the opponent's
battlefield line whenever a tag there changes the price of one of your rows (`[named: <card>]`).
Every imperative in a matchup rule is audited against what the row says the action does on that
board — where the row prices it fatal, the sentence is cut. Both branches of a two-branch rule
(catastrophic / ordinary) get their own literal and imperative. [A114, A146, A180, A183, A219]

**Library state.** A deck that draws extra keys a rule to `Your library: N`, sized against the
offer rate of the action, not the danger point. [A18, A34]

### 4.5 Things that never belong in a guide

- Any sentence whose subject is history: waves, seats, corpora, seqs, records, "you declined N of
  M", "last wave", scoreboards, "WHAT THIS COST", "newest only" ledgers, `perception status` notes,
  `REMOVE THIS RULE WHEN` lines, baselines dated by wave. [A332]
- Counts stated as facts about the render ("this marker appeared zero times"), and negative-
  existence claims about what the prompt prints. [A99, A238]
- Descriptions of engine behaviour ("the engine discards for you") — write the card fact instead.
  [A127, A136]
- Frequency and rarity adjectives the corpus did not exercise: `rarely|often|usually|regularly|
  typically|seldom|expect|most of the time|by default|almost never`. [wave-31 HL6, wave-40 HL9]
- Equivalences ("both are correct") where an imperative is needed; a stated equivalence changes
  nothing. [A233]
- A mechanism the render refutes, even under a correct conclusion. [A324]
- Rebukes: where a rung holds, the register is standard, not "you took it late". [A265]
- Hard caps on the model's legal choices; code-like floors with no odds reasoning. [owner doctrine]
- Cross-guide contradictions: two seated guides teaching different things about the same rendered
  string; diff them and install one shared ladder with one deck constant. [wave-40 HL3, A32, A52,
  A137, A203]

## 5. What not to write — anti-patterns

- **History in the guide** (banned by A332): seq citations, corpus counts, wave numbers, seat
  names, "this corpus", "previous games", "you did X N times", WHAT THIS COST paragraphs,
  control/experiment notes, size-band numbers. A332 supersedes, as guide content: A61's "one-line
  why" when phrased as wave history; A194's "cite the seat's own number as proof"; A238's "a guide
  may cite a count as evidence"; A272's "N of N on boards with no punisher"; A291 and A323's dated
  WHAT THIS COST paragraphs; A305's "since wave NN" baseline sentence; A310's `perception status:
  CLOSED, wave NN`; A285(c)/A320's `REMOVE THIS RULE WHEN` lines; A232's size band and per-wave
  byte budget. All of those still hold — for review.md.
- **Quoting prompt text to argue with it**; finished example sentences the pilot can copy. [A1, A105]
- **Prohibition-only sections** and rules stated only as their exception. [A36, A98]
- **Restatement as repair** — a fourth paraphrase of a rule that failed three times with the fact
  printed. [A102, A138, A273]
- **Story-keyed rules**: card-named roles, anecdotes as keys, the guide's own worked example as the
  failing window. [A83, A126, A148]
- **Unexampled classes** and **unconditional numeric triggers** that never read the opponent's line.
  [A14, A74]
- **Gates on the wrong surface**: converted counts, hand sizes, life thresholds standing in for tags.
  [A49, A72, A237]
- **Unreachable conditions**: a release this deck cannot act on, a self-anti-correlated precondition,
  a hold with no release, a carve-out whose allowed actions cannot satisfy the rule above it. [A8,
  A68, A191, A280]
- **Opponent-modelling** without verifying the opponent's actions are narrated on this seat's
  prompt. [wave-40 HL2]
- **Text for a window class this deck never acts in.** [A243]

## 6. Verification checklist — run before your final message

1. `stat -c %s` on the guide <= 20000; number recorded.
2. The A332 grep (section 2.2) prints 0; pattern pasted beside the result. [wave-40 HL9]
3. Frequency-adjective grep (section 4.5) prints 0, or every hit is a mechanic the corpus exercised.
4. Every literal the guide quotes was counted on the rows its rule governs, whitespace-normalised,
   from the rendered prompt; zero-hit literals repaired or deleted; old-string sentences gone.
   [A128, A188, A294]
5. Every card number and mechanism was checked against the primitive this wave. [A43, A145]
6. Every decision kind has an owner line; the land drop and the mulligan floor are present and the
   mulligan rule is at the top of the file. [A46, A69]
7. Every absolute has its exclusion list closed and its lethal/survival carve-out above it; every
   hold has its release; every brake has its four parts. [A71, A277, A280, A322]
8. No two rules state one threshold twice; no rule appears in more than one place except as a
   pointer; the situations list agrees with the rules it summarises, branch for branch. [A24, A50,
   A177, A307]
9. Every edit has, in review.md: the seq that paid for it, its prediction with denominator and
   "a hit is a break only if", its removal condition in its class, and — for a trim — the rung it
   bets on. [A15, A270, A285, A288]
10. Cross-guide: every render string this guide quotes as an anchor was diffed against what other
    seated guides teach about it; any difference is one deck constant in a shared rule.
    [wave-40 HL3]
11. Final message <= 6 lines: byte count, grep count, rules kept / dropped, anything unsure.
