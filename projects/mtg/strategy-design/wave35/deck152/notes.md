# deck152 — wave-35 dev notes (engine/harness/render lane; all repros arm C `matchups-20260822-111102`)

1. **Intrepid Adversary valor menu is not affordability-filtered.** vs139 s19 (t9): the ETB
   menu offered "don't add any counter" through "add 20 counters" (21 options) with 5 total
   mana on the mana line (~3 spendable after the cast). The engine then PARTIAL-PAYS whatever
   it can (events s20: exactly 2 valor counters resolved off a chosen "add 10"). Either cap the
   menu at the affordable count or annotate the affordable maximum on the menu ("you can
   afford N") — 21 options where ~2 are real is exactly the shape that produced the s19
   mis-echo below. Partial-pay itself is fine (it silently lands on the guide-correct outcome)
   but is undocumented on the surface.

2. **Echo/index conflict executed the index, remap-to-echo was available.** vs139 s19: reply
   `CHOICE: 11 (add 5 counters)` — the echo names OFFERED option 6; index 11 ("add 10
   counters") executed. Mechanism-A shape (echo names a present option ≠ index): a unique-match
   remap would have preserved intent. Harmless here (partial-pay), invisible to every fallback
   counter (`fallback` absent). Datum for the standing echo-policy discussion; HL11.4's
   named-choice cross-check catches it (`CHOICE: N (name)` where name = a different offered
   option).

3. **Noise-only priority window drew a hallucinated future cast.** vs105 s32 (t15, YOUR
   upkeep): options were 2 Flip-Side display toggles + Lair animation + Katilda mass-counter —
   every option ignore-class or wrong-time; the model answered `CHOICE: Cast Tovolar's
   Huntmaster` (numberless, not offered; it wanted its main-phase play) → `unparsed_reply` →
   heuristic passed; Huntmaster was cast normally at s39. 97s + 8.1k reasoning chars spent on
   a window with no real decision. Consider whether upkeep priority needs to surface
   ignore-class-only windows to GPT at all (or auto-pass when every option is a display toggle
   / a legal-but-plan-irrelevant activation the ask-cache already declined).

4. **MDFC land flip annotation stops one sentence short of the recipe.** vs158 s21 option 2:
   the render explains both faces and "only the currently-shown face can actually be played as
   a land. Choose Play Land to play the current face." It never states the actionable
   corollary: flip FIRST, then Play Land, to play the other face. The guide now teaches it
   (deck-lane), but the annotation is the better home — one added sentence ("to play the other
   face, choose Flip Side first, then Play Land") would carry it for every deck with Pathways.
   Render-lane, low priority.

5. **Blocker trade-parenthetical: representation is present and correct, belief overrides it
   stochastically.** vs146 s14 ("The parenthetical is likely a template error. I'll stick with
   blocking" — Sigarda into first-strike deathtouch), vs146 s21, vs36 s12 (all three blocked
   into a printed "(your blocker dies, attacker lives)"). Same seat correctly obeyed the same
   render at vs146 s10. NOT an engine defect — logged so the wave-36 seat knows the guide
   clause shipped this wave is a belief-correcting crutch (2-corpora retirement bar) and that
   escalation, if the class recurs post-teach, should go to render emphasis, not louder prose.

6. **`stale_echo` safe-direction catch, count datum.** vs146 s38: on Brutal Cathar's
   "Cast Card Normally" sub-menu the reply echoed a different card (Elite Spellbinder, the
   NEXT planned play); stale_echo dropped it and the fallback completed the Cathar cast
   correctly (Moonrage Brute on board by s46). Working as designed; no action.

7. **One-off translog check suggested by the tier review (item 3 there) stands**: B-vs105
   ended t15 with no final blockers record; not re-run here (arm B file), carrying the pointer
   forward rather than duplicating the check.
