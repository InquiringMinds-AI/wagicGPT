# Proposed skill revisions — deck44 wave 7

Frozen-spine seat (5/6, top deck). One genuine method addition, two confirmations.

## P1 (ADD to Step 0) — the INTRA-GUIDE CONTRADICTION can fire at LOW life under survival pressure, not only at the top of a ranked list

Wave 6 added intra-guide-contradiction as a Step-0 mode (deck140: a #1 rule under-fires because a
sibling cast-nothing clause forbids its action). deck44 wave 7 shows a **second shape of the same
mechanism, at a decision SEAM**: a correct absolute is DISOBEYED not because a sibling names a
different action, but because the pilot enters a **loud MODE whose scope swallows the absolute's
exception**.

- deck44 F-P0: the guide's "Do NOT block Phyrexian Obliterator" lived as a sub-note INSIDE the
  "when your life is 8 or less, chump the biggest attacker with your least valuable body" paragraph.
  At L20 the pilot obeyed the prohibition and declined the block (`vs133 s9`). At L11 it entered
  chump-mode, Obliterator WAS the biggest attacker, and the chump rule vetoed the buried prohibition
  (`vs133 s18`) — feeding the sac punisher, losing the board, losing the game. The same pilot got it
  right 2 turns earlier at high life. The absolute did not fail from weakness; it failed from being
  **nested inside the very rule that contradicts it**, and the contradiction only bites once the
  low-life branch activates.

Proposed method addition (one bullet in Step 0's intra-guide-contradiction paragraph):
**A capability/target ABSOLUTE ("never block X", "never target Y") must never be authored as a
sub-clause of a MODE rule that will later command its opposite (a chump/desperation/all-in clause).**
When a #1-band absolute is obeyed at one life bracket and violated at another, the diagnosis is
POSITION, not prominence: pull the absolute OUT to its own standalone line ABOVE the mode rule, and
re-state it as the mode rule's explicit EXCEPTION. Concreteness-beats-prominence applies within a
paragraph too — the option the mode rule NAMES ("chump the biggest attacker") beats the caveat the
same paragraph buries. Litmus: for every "NEVER do Z" in the draft, check that no later rule's scope
("when low, do the aggressive/desperate thing") re-authorizes Z; if one does, hoist and except.

## P2 (CONFIRMATION, no change) — a shipped stopgap's underlying engine class MIGRATES; verify the class, not just your deck

deck44's GFTT no-target guide stopgap WORKED (0 `fails validation` for deck44, down from wave-6's 9).
But the deck-agnostic ENGINE defect it papered over did not go away — it surfaced on **Liliana of the
Veil (4x) and Fatal Push (2x)** in the opponents' seats this corpus. This validates wave-6's routing
(the real fix is engine-side target-existence checking) AND the "shipped-but-per-spell stopgap can
never be the general fix" reasoning. Method note for the synthesis: when a prior wave shipped a
per-deck stopgap for a deck-AGNOSTIC engine defect, the wave-N+1 sweep should grep ALL stderr (not
just the reviewer's deck) for the defect string to confirm the class migrated rather than resolved —
a stopgap that cures your deck while the class walks to the next targeted spell is a false "fixed."
This is the composition-seam rule (wave 6) applied to engine defects across SEATS, not just files.

## P3 (CONFIRMATION, no change) — the counter-value floor and the counter seam are corpus-proven

No method change. Recording for the synthesis: deck44's counter seam is now a THREE-corpus success
(wave-5 route -> wave-6 fire 13/14 -> wave-7 protected 4 of 5 wins), sweeper-first obeyed, and the
wave-6 counter-value floor held (0 cheap-cantrip counters). The one borderline (`vs133 s13`, Arcane
Denial on a 1-mana Inquisition against the pilot's OWN "let it resolve" plan) is a plan/choice
mismatch, NOT a value-floor failure — the floor's WORDING did not lose, the pilot mis-picked the
number. Keep the plan/choice-mismatch class (wave-6 called it "gone"; it recurs at low frequency on
attractive labeled counter options) on the watch list; single-seat, not a core edit.
