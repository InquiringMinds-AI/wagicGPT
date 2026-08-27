# Wave-50 lane V probe: the countered cast paid twice (wave-49 seat D-4)

Stacked probe decks in the roster gap (198/199; real roster ends at deck164), deleted from
`bin/Res/ai/baka/` after the runs and preserved here. `deck198` = 20 Grizzly Bears + 8 Fog +
32 Forest (a plain two-pip cast plus the instant that makes the caster's response window
exist); `deck199` = 24 Cancel + 36 Island. `stub-llm.py` is a local OpenAI-shaped stub:
it casts Grizzly Bears (or Cancel when offered), declines every other cast decision, plays
the first land, passes priority, and sleeps N seconds on any stack window (argv[2]) so the
async seat timing matches a live model.

Run: `WAGIC_HEADLESS=1 WAGIC_FASTCLOCK=0.1 WAGIC_SELFPLAY=1 WAGIC_SELFPLAY_ONESHOT=1
WAGIC_SELFPLAY_DECK0=198 WAGIC_SELFPLAY_DECK1=199 WAGIC_AI=gpt WAGIC_GPT_URL=http://127.0.0.1:8199
WAGIC_GPT_MODEL=stub WAGIC_GPT_TRANSLOG=1 ./wagic` under the 4G memory scope.

## Mechanism (traced with temporary stderr instrumentation, since removed)

`AIPlayerBaka::computeActions`' main-phase branch queues the cast's payment clicks and the
card click, then `return 1` WITHOUT clearing the `nextCardToPlay` member. When the opponent's
counterspell lands, the caster gets a response window (it holds an instant), and the
INTERRUPT branch reads the non-NULL `nextCardToPlay` as "already chosen this window": it skips
`FindCardToPlay`, `planCastSpell` declines (card not in hand), and the legacy
`payTheManaCost(... potential mana ...)` queues a fresh payment plus a click on a card that is
already on the stack. The pump taps the lands; the click is a no-op; the mana floats until
end of phase. It only shows when the remaining untapped sources can afford the cost again -
hence the correlation with counters on rich boards. The corpus stderr carries the signature
directly: `Action added to stack: Cancel` immediately followed by `AIPlayerBaka: AI attempting
to pay a mana cost. -  Target: <the countered spell>`.

This is a LATENT AIPlayerBaka defect, not something the wave-48 commits introduced (none of
E1/P/O/N/Q touch AIPlayerBaka.cpp; the wave-47 corpus stderr shows 11 of its 28 counters
followed by the same re-payment attempt, while its translog diff showed no 2x-pip cast - inference: on
those boards the re-pay found nothing affordable to tap, so the latent never surfaced).

## Evidence
`before-after.txt`: pre-fix 18 re-payment signatures in one stub game (each: two extra Forest
taps, then a click on the Bears already on the stack, {G}{G} floating); post-fix 0 in the
same matchup, game natural. Suite pin: `Res/test/ai_countered_cast_no_double_pay_wave50.txt`
(red on the archived wave-49 binary: tapped 4, expected 2; green post-fix).
