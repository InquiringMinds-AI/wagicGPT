# Lane D live probe, 2026-09-03 (wave 57)

GPT-vs-GPT, one-shot, one full 22-turn game against `qwen36-35b-a3b` on Spark `:8084`.
Probe decks 198/199 (10x Emeria's Call + 10x Agadeem's Awakening + 10x Hengegate Pathway /
10x Pelakka Predation + filler) were created in `bin/Res/ai/baka/` for the run and DELETED
after it (roster back to 202).

56 records carry an option list:

* `Flip Side` rows: **1** (the spell-face cast-mode menu, correctly still offered - no land row
  on that menu to duplicate). **0 land-backed toggle rows.** (D14)
* `PLAY THIS AS A LAND` rows: **15, taken 7.**
* D26 arrival clause: 10 pay-or-tap (Agadeem 5, Emeria 5), 2 enters-tapped (Pelakka Caverns),
  3 none (Mistgate Pathway - the Pathway class). All three match the back face's own script.
* D28 row heads: `Agadeem, the Undercrypt with Agadeem's Awakening`, `Emeria, Shattered Skyclave`,
  `Mistgate Pathway`, `Pelakka Caverns` - printed names, not the lowercased script token.

This run is also what caught the false cast-mode toggle tail fixed under D14 (seq 10 of the
deck199 file): a LAND back face was being described as an alternative-cost cast.
