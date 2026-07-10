AIPlayerGPT - the LLM-backed opponent
=====================================

The easiest way to set it up is in-game: Options -> the "GPT" tab.
Pick a provider preset (or enter a custom endpoint URL), enter your API
key if the endpoint needs one, press "Test connection", switch
"LLM opponent" to On, and Save. The tab writes the per-user config file
below, so hand editing and the GUI stay in sync.

Configuration precedence (highest first):
    1. Environment variables: WAGIC_AI=gpt (force on; any other value
       forces off), WAGIC_GPT_URL, WAGIC_GPT_MODEL, WAGIC_GPT_KEY,
       WAGIC_GPT_THINKING, WAGIC_GPT_HINTS, WAGIC_GPT_MAXTOKENS,
       WAGIC_GPT_TIMEOUT (seconds)
    2. The per-user config file (~/.Wagic/ai/gpt/endpoints.txt on
       Linux) - the file the GPT options tab reads and writes
    3. The shipped Res/ai/gpt/endpoints.txt (a template)

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
    Endpoint, model, key and option configuration. Keys:
        enabled=0/1       the master switch (the GPT tab's first row)
        url=...           candidate endpoint, repeatable; probed in order,
                          first /v1/models answer wins
        model=...         served model name; omit to auto-detect
        key=...           bearer token for keyed endpoints. Saved by the
                          GUI as key=obf1:<blob>, scrambled against a
                          per-install random salt (the keysalt file next
                          to this one) so the config file never shows the
                          key in the clear on a stream or screenshot.
                          This is obfuscation, not encryption. Plaintext
                          key= values also work and are migrated to the
                          scrambled form the next time the GUI saves.
                          Configs copied to another machine need the key
                          re-entered (the salt does not travel).
        thinking=0/1      Qwen-style thinking toggle (stronger, slower)
        maxtokens=N       completion budget override
        timeout=N         per-call HTTP timeout in seconds

Per-user overrides: a copy of either file under the user folder
(~/.Wagic/ai/gpt/ on Linux) takes precedence over this shipped copy.
Keep private endpoints and API keys in the per-user copy only - the
GPT options tab always writes the per-user copy, never this one.
