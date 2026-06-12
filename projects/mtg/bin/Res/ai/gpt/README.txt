AIPlayerGPT - the LLM-backed opponent (enable with WAGIC_AI=gpt)
================================================================

Both files here are runtime-loaded and meant to be edited:

system_prompt.txt
    The system prompt sent to the model once per duel. Edit freely.
    Placeholders filled in by the game:
        {MY_DECK}         the AI's deck list with counts and rules text
        {OPPONENT_DECK}   the opponent's card names and rules text
        {STRATEGY_GUIDE}  the contents of <deckfile>_strategy.txt next to
                          the AI's deck (e.g. ai/baka/deck1_strategy.txt),
                          with a heading; empty if no guide exists

endpoints.txt
    Endpoint, model, key and option configuration; see comments inside.

Per-user overrides: a copy of either file under the user folder
(~/.Wagic/ai/gpt/ on Linux) takes precedence over this shipped copy.
Keep private endpoints and API keys in the per-user copy only.
