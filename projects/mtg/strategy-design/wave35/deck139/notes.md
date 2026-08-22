# deck139 wave-35 dev notes (engine/harness items with seq repros)

All repros from arm C (`~/.Wagic/ai/gpt/selfplay-runs/matchups-20260822-111102`) unless noted.

## N1 — Grazer-put shockland enters UNTAPPED on payment (rules question, MEDIUM)

Repro: `1787415065-ai_baka_deck139-...-vs-ai_baka_deck116.jsonl` seq8 (t2); narration visible in
seq10's prompt:

> You targeted Steam Vents ... with Arboreal Grazer's Put in Play ability
> You played Steam Vents
> It entered untapped (you paid 2 life)

Arboreal Grazer's Oracle text puts the land onto the battlefield **tapped**. A shockland put
this way still gets its "as ~ enters, you may pay 2 life" replacement, but under real rules
paying does not undo Grazer's tapped instruction — the land enters tapped either way and the
payment is a pure loss (CR 614.6-ish interaction: both replacements apply; Grazer's effect
taps it regardless of the shock choice). The engine instead granted an UNTAPPED entry for the
2 life. Two defects in one seam: (a) rules fidelity (land should enter tapped, pay or not);
(b) offering the pay ask at all in this context invites a strictly-bad payment (the arm-C
pilot paid — 2 life for nothing under real rules, and even in-engine it had no same-turn use).
Verify against the current binary; if confirmed, either suppress the pay ask on
put-onto-battlefield-tapped entries or make the payment ineffective per Oracle.

## N2 — ALL THREE arm-C fallbacks at this seat landed on the shockland pay/tap ETB ask (harness/prompt, MEDIUM)

- vs158 seq9 (t4): `unparsed_reply`. Reasoning shows the pilot derailed on the CURRENT
  SITUATION's "Land drop: you have no land you could play right now" line rendered while the
  Temple Garden ETB choice was pending — it litigated whether a land drop was legal instead of
  answering the two-option pay/tap menu, and never emitted a parseable CHOICE.
- vs152 seq8 (t0) and seq27 (t10): `stale_echo`. Same ask shape; the echo failed to match the
  option short-name (the options are the bare labels "pay 2 life" / "tap" while the pilot
  reasons about the LAND's name — its echo tends to say "(Temple Garden)" / "(Steam Vents)").
Suggestion: put the entering land's name INTO the option short-name ("pay 2 life - Steam Vents
enters UNTAPPED" / "tap - Steam Vents enters TAPPED") so the natural echo has a matchable
anchor, and consider suppressing/adjusting the "Land drop: none playable" line while an ETB
choice for a just-played land is pending (it reads as a contradiction of the event the pilot
just took). Rate: 3/195 decisions, all in this one ask class; 0 fallbacks anywhere else at the
seat.

## N3 — over/under is asked BEFORE the host target (design observation, LOW; guide now compensates)

Repro: vs105 seq38 (over/under) -> seq39 (TARGET CHOICE host list). The pilot must commit
over/under without the engine telling it which host it will be offered, and the option lines
carry no consequence preview. The wave-35 G1 misplay (3/4 Greathorn "over" onto the 6/6
Brokkos pile; pile rendered 3/4 at seq42) rode partly on this ordering. The revised guide
teaches "decide your host first, biggest printed P/T on top", which closes it at the guide
layer; a render-layer assist would be annotating the two options with the resulting pile P/T
once a host is known — i.e. asking host FIRST, then over/under with "{pile becomes X/Y}" on
each line. Docket as an option-annotation idea, not a defect.

## N4 — arm B budget-overrun class (P2 of my tier review) is ABSENT at FP8

Arm C: 0 empty replies in 195 decisions (arm B: 6/185 after 22-24k reasoning chars). No action
needed; recording so the wave-36 docket does not carry P2 forward against the standing pilot.
P1 (becomes-blocked triggers missing from the naive-trade blocker math, arm A deck105 games)
and P3 (number-right/name-wrong parser rejection) from the tier review remain open as filed
there; neither recurred in arm C at this seat (P1's Ichorclaw shape did not come up — the
vs105 infect list was Cystbearer/Nim/Tel-Jilad with no becomes-blocked trigger).
