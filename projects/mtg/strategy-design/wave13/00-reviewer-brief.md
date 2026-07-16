# Wave-13 reviewer brief (shared context for all 7 deck agents + synthesis)

Third cycle after the resume. Two intervening corpora (wave-12 `matchups-20260715-120540`,
wave-13a `matchups-20260715-151947`) were CONTAMINATED by Spark crashes and are engine evidence
only — this corpus (wave-13b) is the first clean validation of the name-echo protocol, the
Force round-2+3 fixes, the auto-tap two-pass fill, and BLOCKS:none at scale. Guides are frozen
(all 7 byte-identical since wave-11), so the review's center of gravity is (a) engine-fix
verification, (b) name-echo-era decision quality, (c) any class visible beneath the fixed noise.

## Corpus data

- **PRIMARY**: `~/.Wagic/ai/gpt/selfplay-runs/matchups-20260715-183228` — 21 games, round-robin
  over decks 44,109,110,131,133,135,140. Binary /tmp/wagic-a75e2625e (= eb5145082 Force round-2
  cost-target fallback + a75e2625e Force round-3 offer-legality/dead-end removal + d5d7b1f2e
  auto-tap two-pass + c3e6d95b2 name-echo protocol + 42e8bc180 BLOCKS:none + 240s timeout).
  Wave-11 guides (all frozen). Core prompt unchanged (8th PASS standing).
- Baseline for protocol comparison: wave-11 `matchups-20260715-072254` (pre-name-echo).
- Win table: 140:5/6, 109:5/6, 110:3/6, 44:3/6, 133:2/6, 131:2/6, 135:1/6. 0 raw timeouts,
  **13 life-adjudicated** (adj0/adj1 rows in results.tsv) — see latency note below.

## KNOWN CORPUS-WIDE (do not re-diagnose; account for in decision attribution)

- **Spark ran at GPU_MEM_UTIL=0.75** (crash-avoidance trial; it PASSED — no crash, memory flat).
  COST: decision latency med 11.2s / p90 87s / p99 185s (KV preemption under 3 concurrent
  games). That latency — not play quality — is why 13/21 games hit the 2400s cap and were
  life-adjudicated. Do NOT read adjudication count as a play-quality regression; DO evaluate
  the adjudicated positions themselves (was the seat ahead on life for the right reasons?).
- **empty_reply = 0** corpus-wide (the wave-12/13a empty-reply flood was Spark dying, class
  CLOSED). 49 unparsed_reply (3.7%): long reasoning rambles correctly discarded by the guard
  (heuristic answered). One NEW shape: `ATTACK: Hellrider` — a NAME where the protocol wants
  A-indices; unparsable today, ledger candidate (attackers name→index reconcile arm mirroring
  parseChoice). Note instances, don't re-diagnose.
- **Name-echo compliance 100.0%** (982/982 CHOICE lines carry "(name)"). Reviewers: look for
  echo-index CONFLICTS (echo naming a different option than the index — the reconcile arm
  remaps to the echo's unique match or falls back; any wrong remap is a HIGH finding).
- 4 defer records (first since the defer fix; check what they were).

## Engine-fix verification list (confirmed-fired-representation doctrine)

1. **Force of Negation (deck135 reviewer)**: round-3 shipped a75e2625e — alt-cast offers are
   now gated on a payable exile target and the standalone "exile a blue card" dead-end action
   is gone from the GPT menu. VERIFIED at corpus level: 0 dead-end picks, 1 legal offer
   (vs deck44 t8, Go for the Throat on stack) which the model DECLINED. Evaluate that decline
   as a decision (Force in hand, GFTT resolving — was holding correct?). Known residual, do
   not re-diagnose: an async response-window race can still eat answered casts on the
   opponent's turn (~1/3 completion measured pre-fix); if you see "casting Force of Negation
   (model's pick)" with no execution, tag it RESIDUAL-RACE with seq.
2. **Auto-tap color-stranding (deck44 reviewer)**: NOT FIXED — pre-analysis found 5/8
   generic+blue casts spending blue on generic while black sat untapped (e.g. {2}{u} from
   {u:3,b:1} spending u3). The d5d7b1f2e two-pass landed in ManaEngine::planPayment but the
   live tap path appears to be selectAutoTapProducers (unpatched). CAVEAT: verify against
   deck44's actual land set (duals would muddy pip accounting). Quantify game impact
   (counterspells unofferable after stranding?) — this is the top engine-batch item.
3. **BLOCKS: none** (all reviewers): 6 explicit all-decline firings, 2nd corpus. Confirm none
   of them were misparses of intended blocks.
4. **Name-echo remaps**: any out-of-range CHOICE repaired via echo? Any echo-vs-index conflict
   resolved wrong? (Index wins on conflict by design.)
5. **Auto-tap "Mana available" annotation trust**: with stranding live, did any seat's model
   REASON from a mana line that then didn't hold (cast declined that was affordable, or
   attempted that wasn't)?

## Doctrine (BINDING, unchanged)

Win-rate is context only. The unit of analysis is the DECISION with seq repros. Guides are at
their terminus on this pool: adopt-a-revision bar is HIGH (a new decision-level failure class,
not drift). The pause-aware demotion doctrine applies. Layer-routing enforced: engine items →
notes ledger, never guide prose. Single-seat items cannot add/cut core lines.

## Output contract (per deck agent, sequential, in strategy-design/wave13/deckN/)

findings.md -> strategy.txt (full guide, byte-identical if frozen) -> skill.md ->
general-suggestions.md -> notes.md. The CURRENT skill = strategy-design/wave11/
strategy-writing-skill.md. Read also your seat's wave-11 deckN continuity files.
