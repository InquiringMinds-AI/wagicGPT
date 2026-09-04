# Wave-60 step-one adversarial review (Codex gpt-5.6-sol, read-only, on diff 8b922abf0..1fcdeeedc; verdict: NOT safe to ship as-is)

# Adversarial findings

1. **HIGH — [AIPlayerGPT.cpp:20198](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20198):** `namedCastLifeSurcharge` returns after the first matching naming permanent, so the new win fold can still promise a win when multiple cast-punisher triggers kill the pilot first.  
   **Trigger:** Two opposing Silverquill Silencers both name Lightning Bolt; pilot is at 4, opponent at 3. Casting Bolt creates two 3-life-loss triggers, but the target row prices only 3, concludes the pilot survives at 1, and prints `THIS WINS THE GAME` even though the first two triggers reduce them to −2 before Bolt resolves. The cast-row accumulation has the same first-match `break` at [AIPlayerGPT.cpp:25528](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:25528).

2. **HIGH — [AIPlayerBaka.cpp:3698](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerBaka.cpp:3698):** The new unconditional return withholds every current target chooser whose source merely matches any live reveal, without proving that chooser belongs to that reveal driver.  
   **Trigger:** Keep an interactive reveal display live while another targeted ability from the same source card becomes the current chooser. `drivingFor` returns true solely from source identity/display liveness ([AllAbilities.cpp:907](/home/magi/Projects/wagicGPT/projects/mtg/src/AllAbilities.cpp:907)), so `computeActions` never calls `chooseTarget`; unless the reveal driver happens to consume that unrelated chooser, the game remains parked until the 1,800-second structural force-close. This is the wave-58 hang-class shape and removes a legal targeting window from its normal driver.

3. **MED — [AIPlayerGPT.cpp:9703](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:9703):** The X-row treats the current post-draw hand surplus as a guaranteed cleanup discard even when the player has intervening phases and legal ways to reduce the hand.  
   **Trigger:** Cast Sphinx’s Revelation for X=9 during first main with two Liliana’s Caresses opposing, then cast several newly drawn cards before cleanup. The row nevertheless includes all eleven hypothetical discards in `NET -53`, although those discards need not occur. PARSETEST explicitly pins this incorrect guarantee at [AIPlayerGPT.cpp:45332](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:45332), so the green test protects a false expectation.

4. **MED — [AIPlayerGPT.cpp:4086](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:4086):** The new attack aggregate equates power with life loss and therefore states false resulting-life and guaranteed-kill facts for infect, double strike, damage prevention, and creatures that cannot deal combat damage.  
   **Trigger:** Declare an unblocked 1/1 infect creature against an opponent at 10 life and zero poison. The line says it “puts them at 9”; combat instead leaves life at 10 and gives one poison counter. The guaranteed floor is computed from the same raw power at [AIPlayerGPT.cpp:31617](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:31617).

5. **MED — [AIPlayerGPT.cpp:20574](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:20574):** The tap/untap annotation asserts engine consequences from card type alone and ignores `CANBLOCKTAPPED` and does-not-untap effects.  
   **Trigger:** Teferi’s +1 targets an opposing creature able to block while tapped, or a creature under an effect saying it does not untap during its controller’s next untap step. The prompt respectively claims it “CANNOT BLOCK” or that it “UNTAPS” next turn, both false for the live target.

6. **MED — [AIPlayerGPT.cpp:1588](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:1588):** The new sweeper roster names every creature as something the spell “destroys,” without excluding indestructible creatures or other destruction survivors.  
   **Trigger:** Cast a `destroy all(creature)` sweeper into an opposing Darksteel Myr. `boardCreatureCounts` adds it unconditionally to `theirNames`, producing `destroys 1 ... THEIRS: Darksteel Myr`, although it remains on the battlefield.

7. **MED — [AIPlayerGPT.cpp:16332](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:16332):** Exile-hostage power/toughness is read from the exile object even though `ABlink::returnCardIntoPlay` resets the returning permanent to original P/T, so both the descriptor and crack-back power can be stale.  
   **Trigger:** Exile a 1/1 creature carrying a +1/+1 counter “until this leaves.” The exile version retains copied counter-derived stats, so the row/crack-back can advertise 2 power; return clears counters and resets it to 1/1.

8. **LOW — [AIPlayerGPT.cpp:14593](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:14593):** The promised plan-truncation marker is usually removed immediately by the subsequent “trim to last sentence” pass.  
   **Trigger:** Any plan over 400 characters with a sentence terminator after its midpoint. `planCarryBound` appends the marker, but lines 14596–14601 see the marker’s final `]`, find the preceding period, and truncate back through it. PARSETEST checks `planCarryBound` in isolation, not this composed path.

9. **LOW — [AIPlayerGPT.cpp:11323](/home/magi/Projects/wagicGPT/projects/mtg/src/AIPlayerGPT.cpp:11323):** A nonempty HTTP 200 response that is malformed or has an unexpected schema is cleared and later classified as `empty_reply`, despite an actual reply body having arrived.  
   **Trigger:** Endpoint returns `200 {"choices":[]}` or non-JSON text. JSON extraction throws, `content` becomes empty, while curl remains 0 and HTTP remains 200; `noAnswerClassFor` consequently reports `empty_reply`, hiding a protocol/schema failure. Scalar PARSETEST classification cases cannot exercise this integration path.

## Verdict

**Not safe to ship as-is.** The diff retains a game-result lie under multiple naming punishers, introduces a source-only early-return gate capable of suppressing a legal target window for up to the reveal stall wall, and adds several confident render claims not implied by engine state. One PARSETEST case actively fixes the wrong cleanup-cost assumption in place, while another tests the truncation helper without catching that its caller removes the required marker.