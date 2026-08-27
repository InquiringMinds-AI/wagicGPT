# deck146 - wave-49 core-prompt / render proposals (layer-routed; engine bugs -> seat file engine list)

Lane T/S/U render items my deck146 seats confirmed WORKING (keep, do not re-open):
- `[this land is TAPPED: animated, it still cannot attack this turn]` + `{paying this taps: Hive of
  the Eye Tyrant - it cannot attack this turn}` on the `becomes beholder` row for a tapped Hive
  (`146 vs152` s59): rendered and OBEYED (passed). This retires the wave-48 E-2 render half.
- lane S `named_row_reask`: `146 vs126` s9 off-menu name -> one `[RE-ASK]` -> s10 recovered. Keep.
- lane S ATTACK/BLOCKS last-line-taken: `146 vs123` s11 `answer_replaced` executed the LAST coded
  line (`ATTACK: A1` over `ATTACK: none`). Retires wave-48 E-1's ATTACK-first bug.

New/carried (routed to the seat file's engine list, summarised here):
- **R20 degenerate_decode counter (carried, one new instance).** `146 vs125` s82 `unparsed_reply`
  is a 286-char garbage decode (mojibake, no coded line), distinct from an off-menu-name fallback;
  a `degenerate_decode` counter would separate the two classes. LOW.
- **R36 back-face marker (carried).** 12 `{card text:}` blocks on my seats end on a bare `// <name>`
  with no `(text omitted)` marker while the same prompts render the marker 148 times elsewhere. LOW.
