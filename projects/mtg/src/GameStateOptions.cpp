#include "PrecompiledHeader.h"

#include "GameStateOptions.h"
#include "GameStateMenu.h"
#include "GameApp.h"
#include "OptionItem.h"
#ifdef WITH_GPT_AI
#include "OptionGpt.h"
#endif
#include "SimpleMenu.h"
#include "SimplePad.h"
#include "Translate.h"
#include <mutex> //std::lock_guard over GptMutex (Vita seam)
#ifdef WITH_GPT_AI
#include "qrcodegen.hpp"
#endif

namespace GameStateOptionsConst
{
    const int kSaveAndBackToMainMenuID = 1;
    const int kBackToMainMenuID = 2;
    const int kNewProfileID = 4;
    const int kReloadID = 5;
    const int kTelemetryYesID = 6;
    const int kTelemetryNoID = 7;
    const int kResetTutorialsID = 8;
    const int kTelemetryMenuID = -103;
#ifdef WITH_GPT_AI
    const int kModelPickerMenuID = -104;
    //Item ids inside the picker menus: list indexes start at 0, so the
    //specials live far above any real catalog size.
    const int kPickerManualID = 900001;
    const int kPickerBackID = 900002;
    const int kPickerCancelID = 900003;
#endif
}

#ifdef WITH_GPT_AI
namespace
{
//Picker phases and deferred-build requests (menus are only built in Update).
enum { PICKER_FETCHING, PICKER_VENDORS, PICKER_MODELS };
enum { kPickerBuildNone = -1, kPickerBuildVendors = -2 }; //>= 0: models of that bucket
} //namespace

//Shared state between the screen and the /v1/models fetch worker. The worker
//owns a shared_ptr copy, so leaving the options screen mid-fetch is safe: the
//worker finishes writing into memory only it still references.
struct GptModelFetch
{
    GptMutex mtx;
    int status; //1 in flight, 2 done
    bool ok;
    std::vector<std::string> models;
    GptModelFetch() : status(1), ok(false) {}
};

namespace
{
struct ModelFetchCtx
{
    std::shared_ptr<GptModelFetch> state;
    std::string url;
    std::string key;
};

void ModelFetchMain(void * p)
{
    ModelFetchCtx * ctx = static_cast<ModelFetchCtx *>(p);
    std::vector<std::string> models;
    bool ok = gptListModels(ctx->url, ctx->key, models, 20000);
    {
        std::lock_guard<GptMutex> g(ctx->state->mtx);
        ctx->state->models.swap(models);
        ctx->state->ok = ok;
        ctx->state->status = 2;
    }
    delete ctx;
}
} //namespace
#endif //WITH_GPT_AI

static std::string kBgFile = "";

GameStateOptions::GameStateOptions(GameApp* parent) :
    GameState(parent, "options"), mReload(false), grabber(NULL), mResetTutorialsLabel(NULL), optionsMenu(NULL), optionsTabs(NULL)
{
#ifdef WITH_GPT_AI
    gptTab = NULL;
    telemetryMenu = NULL;
    modelPickerMenu = NULL;
    pickerPhase = PICKER_FETCHING;
    pickerBuildFor = kPickerBuildNone;
    pickerVendorAt = 0;
#endif
}

GameStateOptions::~GameStateOptions()
{
    kBgFile = ""; //Reset the chosen background.
}

void GameStateOptions::Start()
{
    newProfile = "";
    timer = 0;
    mState = SHOW_OPTIONS;
    JRenderer::GetInstance()->EnableVSync(true);

    WGuiList * optionsList;

    optionsList = NEW WGuiList("Settings");

    optionsList->Add(NEW WGuiHeader("General Options"));
    if (GameApp::HasMusic)
        optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::MUSICVOLUME, "Music volume", 100, 10, 100),
                        OptionVolume::getInstance()));
    optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::SFXVOLUME, "SFX volume", 100, 10, 100), OptionVolume::getInstance()));
    if (options[Options::DIFFICULTY_MODE_UNLOCKED].number)
    {
        optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::DIFFICULTY, "Difficulty", 3, 1, 0),
                        OptionDifficulty::getInstance()));
        optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::ECON_DIFFICULTY, "Economic Difficuly", Constants::ECON_EASY)));
    }
    optionsList->Add(NEW OptionInteger(Options::INTERRUPT_SECONDS, "Seconds to pause for an Interrupt", 20, 1));
    optionsList->Add(NEW OptionInteger(Options::INTERRUPTMYSPELLS, "Interrupt my spells"));
   // optionsList->Add(NEW OptionInteger(Options::INTERRUPTMYABILITIES, "Interrupt my abilities"));
    //this is a dev option, not meant for standard play. uncomment if you need to see abilities you own hitting the stack.
    optionsList->Add(NEW OptionInteger(Options::INTERRUPT_SECONDMAIN, "Interrupt opponent's end of turn"));
    optionsList->Add(NEW WGuiButton(NEW WGuiHeader("Back to Main Menu"), -102, GameStateOptionsConst::kBackToMainMenuID, this));
    optionsList->Add(NEW WGuiButton(NEW WGuiHeader("Save And Exit"), -102, GameStateOptionsConst::kSaveAndBackToMainMenuID, this));
    optionsTabs = NEW WGuiTabMenu();
    optionsTabs->Add(optionsList);

    optionsList = NEW WGuiList("Misc");
    optionsList->Add(NEW WGuiHeader("Card Display Options"));
    optionsList->Add(NEW OptionInteger(Options::SHOWBORDER, "Show Borders"));
    //black border
    optionsList->Add(NEW OptionInteger(Options::BLKBORDER, "All Black Borders"));
    //Sort deck by date
    optionsList->Add(NEW OptionInteger(Options::SORTINGDECKS, "Sort decks by date"));
    //show tokens in editor
    optionsList->Add(NEW OptionInteger(Options::SHOWTOKENS, "Show Tokens in Editor"));
    WDecoStyled * wMisc = NEW WDecoStyled(NEW WGuiHeader("Warning!!!"));
    wMisc->mStyle = WDecoStyled::DS_STYLE_ALERT;
    optionsList->Add(wMisc);
    //show large images
    optionsList->Add(NEW OptionInteger(Options::GDVLARGEIMAGE, "Show Large Images in Grid Deck View"));
    //prefetch
    if(WResourceManager::Instance()->IsThreaded())
        optionsList->Add(NEW OptionInteger(Options::CARDPREFETCHING, "Enable Prefetching"));
    optionsTabs->Add(optionsList);

    optionsList = NEW WGuiList("Game");
    optionsList->Add(NEW WGuiHeader("Interface Options"));
    optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::SORTINGSETS, "Sort sets by", Constants::BY_DATE, 1,
                    Constants::BY_NAME, "", Constants::BY_SECTOR))); // Now sets can be sorted by sector(orderindex) or name or release date.
    optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::CLOSEDHAND, "Closed hand", 1, 1, 0)));
    optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::HANDDIRECTION, "Hand direction", 1, 1, 0)));
    optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::MANADISPLAY, "Mana display", 3, 1, 0)));
    optionsList->Add(NEW OptionInteger(Options::REVERSETRIGGERS, "Reverse left and right triggers"));
    optionsList->Add(NEW OptionInteger(Options::DISABLECARDS, "Disable card images"));
    optionsList->Add(NEW OptionInteger(Options::TRANSITIONS, "Disable screen transitions"));
    optionsList->Add(NEW OptionInteger(Options::OSD, "Display InGame extra information"));
    //#W57-G (D42) / #W58-E (D42): three states, Off / Tokens only / All
    //permanents, defaulting to Tokens only.
    optionsList->Add(NEW WDecoEnum(NEW OptionInteger(Options::BOARDGROUPING, "Stack identical permanents",
                    OptionBoardGrouping::ALL, 1, OptionBoardGrouping::TOKENS, "", OptionBoardGrouping::OFF)));
    optionsTabs->Add(optionsList);

#ifdef WITH_GPT_AI
    //LLM opponent configuration (and future transformer-backed features).
    gptTab = NEW GptOptionsList();
    optionsTabs->Add(gptTab);
#endif

    optionsList = NEW WGuiList("User");
    optionsList->Add(NEW WGuiHeader("User Options"));
    WDecoConfirm * cPrf = NEW WDecoConfirm(this, NEW OptionProfile(mParent, this));
    cPrf->confirm = "Use this Profile";
    OptionThemeStyle * ots = NEW OptionThemeStyle("Theme Style");
    OptionDirectory * od = NEW OptionTheme(ots);
    WDecoConfirm * cThm = NEW WDecoConfirm(this, od);
    cThm->confirm = "Use this Theme";

    WDecoConfirm * cStyle = NEW WDecoConfirm(this, ots);
    cStyle->confirm = "Use this Style";

    optionsList->Add(NEW WGuiSplit(cPrf, cThm));
    optionsList->Add(cStyle);
    optionsList->Add(NEW WGuiButton(NEW WGuiHeader("New Profile"), -102, GameStateOptionsConst::kNewProfileID, this));
    optionsList->Add(NEW WDecoCheat(NEW OptionInteger(Options::CHEATMODE, "Enable Cheat Mode")));
    optionsList->Add(NEW WDecoCheat(NEW OptionInteger(Options::OPTIMIZE_HAND, "Optimize Starting Hand")));
    optionsList->Add(NEW WDecoCheat(NEW OptionInteger(Options::CHEATMODEAIDECK, "Unlock All Ai Decks")));

    optionsTabs->Add(optionsList);

    optionsList = NEW WGuiList("Advanced");
    optionsList->Add(NEW WGuiHeader("Advanced Options"));
    WDecoStyled * wAdv = NEW WDecoStyled(NEW WGuiHeader("The following options require a restart."));
    wAdv->mStyle = WDecoStyled::DS_STYLE_ALERT;
    optionsList->Add(wAdv);
    WDecoConfirm * cLang = NEW WDecoConfirm(this, NEW OptionLanguage("Language"));
    cLang->confirm = "Use this Language";
    optionsList->Add(cLang);
    WDecoEnum * oGra = NEW WDecoEnum(NEW OptionInteger(Options::MAX_GRADE, "Minimum Card Grade", Constants::GRADE_DANGEROUS, 1,
                    Constants::GRADE_BORDERLINE, "", Constants::GRADE_SUPPORTED));
    optionsList->Add(oGra);
    WDecoEnum * oASPhases = NEW WDecoEnum(NEW OptionInteger(Options::ASPHASES, "Phase Skip Automation", Constants::ASKIP_FULL, 1,
                    Constants::ASKIP_NONE, "", Constants::ASKIP_NONE));
    optionsList->Add(oASPhases);
    //Off by default in this fork. The tutorial messages are modal, interrupt
    //the duel, and repeat on every fresh profile; anyone who wants them can
    //turn them on here.
    optionsList->Add(NEW OptionInteger(Options::TUTORIALS, "Show tutorial messages"));
    //Action row, not a value row: there is nothing to store, and the button
    //idiom ("New Profile" above) is the only one this screen has for a verb.
    //The label doubles as the acknowledgement - it is swapped for a done
    //message once pressed, since the reset leaves nothing else on screen.
    mResetTutorialsLabel = NEW WGuiHeader("Reset tutorial messages");
    optionsList->Add(NEW WGuiButton(mResetTutorialsLabel, -102, GameStateOptionsConst::kResetTutorialsID, this));
    optionsTabs->Add(optionsList);

    WDecoEnum * oFirstPlayer = NEW WDecoEnum(NEW OptionInteger(Options::FIRSTPLAYER, "First Turn Player", Constants::WHO_R, 1,
                    Constants::WHO_P, "", Constants::WHO_P));
    optionsList->Add(oFirstPlayer);
    
    WDecoEnum * oKickerPay = NEW WDecoEnum(NEW OptionInteger(Options::KICKERPAYMENT, "Kicker Cost", Constants::KICKER_CHOICE, 1,
        Constants::KICKER_ALWAYS, "", Constants::KICKER_ALWAYS));
    optionsList->Add(oKickerPay);
#ifndef IOS
    optionsList = NEW WGuiKeyBinder("Key Bindings", this);
    optionsTabs->Add(optionsList);
#endif
    optionsList = NEW WGuiList("Credits");
    optionsList->failMsg = "";
    optionsTabs->Add(optionsList);

    optionsMenu = NEW SimpleMenu(JGE::GetInstance(), WResourceManager::Instance(), -102, this, Fonts::MAIN_FONT, 50, 170);
    optionsMenu->Add(GameStateOptionsConst::kBackToMainMenuID, "Back to Main Menu");
    optionsMenu->Add(GameStateOptionsConst::kSaveAndBackToMainMenuID, "Save And Exit");
    optionsMenu->Add(kCancelMenuID, "Cancel");

    optionsTabs->Entering(JGE_BTN_NONE);

#if !defined (PSP)
    GameApp::playMusic("Track3.mp3"); // Added music for options.
#endif
}

void GameStateOptions::End()
{
    JRenderer::GetInstance()->EnableVSync(false);
    mResetTutorialsLabel = NULL; //owned (and deleted) by optionsTabs
    SAFE_DELETE(optionsTabs);
    SAFE_DELETE(optionsMenu);
#ifdef WITH_GPT_AI
    gptTab = NULL; //owned (and deleted) by optionsTabs
    SAFE_DELETE(telemetryMenu);
    SAFE_DELETE(modelPickerMenu);
    modelFetch.reset(); //a worker still in flight keeps its own reference
#endif
    kBgFile = ""; //Reset the chosen background.
}

void GameStateOptions::Update(float dt)
{
    timer += dt * 10;

    if (options.keypadActive())
    {
        options.keypadUpdate(dt);

        if (newProfile != "")
        {
            newProfile = options.keypadFinish();
            if (newProfile != "")
            {
                options[Options::ACTIVE_PROFILE] = newProfile;
                options.reloadProfile();
                optionsTabs->Reload();
            }
            newProfile = "";
        }
    }
    else
        switch (mState)
        {
        default:
        case SAVE:
            switch (optionsTabs->needsConfirm())
            {
            case WGuiBase::CONFIRM_CANCEL:
                mState = SHOW_OPTIONS;
                break;
            case WGuiBase::CONFIRM_OK:
                optionsTabs->save();
                JSoundSystem::GetInstance()->SetSfxVolume(options[Options::SFXVOLUME].number);
                JSoundSystem::GetInstance()->SetMusicVolume(options[Options::MUSICVOLUME].number);
                mParent->DoTransition(TRANSITION_FADE, GAME_STATE_MENU);
                mState = SHOW_OPTIONS;
                GameStateMenu::genNbCardsStr();
                break;
            case WGuiBase::CONFIRM_NEED:
                optionsTabs->yieldFocus();
                break;
            }
            // Note : No break here : must continue to continue updating the menu elements.
        case SHOW_OPTIONS:
        {
            JGE* j = JGE::GetInstance();
            JButton key = JGE_BTN_NONE;
            int x, y;
            if (grabber)
            {
                LocalKeySym sym;
                if (LOCAL_KEY_NONE != (sym = j->ReadLocalKey()))
                    grabber->KeyPressed(sym);
            }
            else
                while ((key = JGE::GetInstance()->ReadButton()) || JGE::GetInstance()->GetLeftClickCoordinates(x,y))
                {
                    if (!optionsTabs->CheckUserInput(key) && key == JGE_BTN_MENU)
                        mState = SHOW_OPTIONS_MENU;
                }
            optionsTabs->Update(dt);
#ifdef WITH_GPT_AI
            //The GPT tab's Model row raises this flag; the picker itself is
            //screen-level UI (a modal menu chain), so it lives here.
            if (gptTab && gptTab->modelPickerWanted)
            {
                gptTab->modelPickerWanted = false;
                startModelPicker();
            }
            if (gptTab && gptTab->signInWanted)
            {
                gptTab->signInWanted = false;
                startOaiSignIn();
            }
#endif
            break;
        }
        case SHOW_OPTIONS_MENU:
            optionsMenu->Update(dt);
            break;
#ifdef WITH_GPT_AI
        case SHOW_TELEMETRY_CONSENT:
            if (telemetryMenu)
                telemetryMenu->Update(dt);
            break;
        case SHOW_MODEL_PICKER:
            updateModelPicker(dt);
            break;
        case SHOW_OAI_SIGNIN:
            updateOaiSignIn(dt);
            break;
#endif
        }
    if (mReload)
    {
        options.reloadProfile();
        Translator::EndInstance();
        Translator::GetInstance()->init();
        optionsTabs->Reload();
        mReload = false;
    }
}

void GameStateOptions::Render()
{
    //Erase
    JRenderer::GetInstance()->ClearScreen(ARGB(0,0,0,0));
#if !defined (PSP)
    //Now it's possibile to randomly use up to 10 background images for game settings (if random index is 0, it will be rendered the default "bgdeckeditor.jpg" image).
    JTexture * wpTex = NULL;
    if(kBgFile == ""){
        char temp[4096];
        sprintf(temp, "bgdeckeditor%i.jpg", std::rand() % 10);
        kBgFile.assign(temp);
        wpTex = WResourceManager::Instance()->RetrieveTexture(kBgFile);
        if (wpTex) {
            JQuadPtr wpQuad = WResourceManager::Instance()->RetrieveTempQuad(kBgFile);
            if (wpQuad.get())
                JRenderer::GetInstance()->RenderQuad(wpQuad.get(), 0, 0, 0, SCREEN_WIDTH_F / wpQuad->mWidth, SCREEN_HEIGHT_F / wpQuad->mHeight);
            else {
               kBgFile = "bgdeckeditor.jpg"; //Fallback to default background image for game settings.
               wpTex = NULL;
            }
        } else
            kBgFile = "bgdeckeditor.jpg"; //Fallback to default background image for game settings.
    }
    if(!wpTex)
        wpTex = WResourceManager::Instance()->RetrieveTexture(kBgFile);
    if (wpTex)
    {
        JQuadPtr wpQuad = WResourceManager::Instance()->RetrieveTempQuad(kBgFile);
        JRenderer::GetInstance()->RenderQuad(wpQuad.get(), 0, 0, 0, SCREEN_WIDTH_F / wpQuad->mWidth, SCREEN_HEIGHT_F / wpQuad->mHeight);
    }
#else
    JTexture * wpTex = WResourceManager::Instance()->RetrieveTexture("pspbgdeckeditor.jpg");
    if (wpTex)
    {
        JQuadPtr wpQuad = WResourceManager::Instance()->RetrieveTempQuad("pspbgdeckeditor.jpg");
        JRenderer::GetInstance()->RenderQuad(wpQuad.get(), 0, 0, 0, SCREEN_WIDTH_F / wpQuad->mWidth, SCREEN_HEIGHT_F / wpQuad->mHeight);
    }
#endif
    const char * const CreditsText[] = {
        "wagicGPT by InquiringMinds-AI",
        "forked from Wagic, The Homebrew?! by Wololo",
        "",
        "Updates, new cards, and more on the Wagic Discord",
        "Many thanks to the people who help this project",
        "",
        "",
        "Art:",
        "Ilya B, Julio, Jeck, J, Kaioshin, Lakeesha",
        "Check themeinfo.txt for the full credits of each theme!",
        "",
        "Dev Team:",
        "Abrasax, Almosthumane, bren17, Daddy32, DJardin, Dr.Solomat,",
        "InquiringMinds-AI, J, Jeck, kevlahnota, Leungclj, linshier,",
        "Mootpoint, Mnguyen, Ph34rbot, Psyringe, Rolzad73, Salmelo,",
        "Superhiro, Vitty85, Wololo, Yeshua, Zethfox",
        "",
        "Music by Celestial Aeon Project, http://www.jamendo.com",
        "",
        "Deck Builders:",
        "Abrasax, AzureKnight, colarchon, Excessum, Hehotfarv,",
        "Jeremy, Jog1118, JonyAS, Lachaux, Link17, Muddobbers,",
        "Nakano, Niegen, Kaioshin, Psyringe, r1c47, Superhiro,",
        "Szei, Thanatos02, Vitty85, Whismer, Wololo",
        "",
        "Thanks also go to Dr.Watson, KF1, Luruz, Orine, Raphael,",
        "Sakya, Tacoghandi, Tyranid for their help.",
        "",
        "Thanks to everyone who contributes code/content on the forums!",
        "",
        "",
        "Source:",
        "http://code.google.com/p/wagic (2009-2013)",
        "https://github.com/WagicProject/wagic (2013- )",
        "https://github.com/InquiringMinds-AI/wagicGPT (2026- )",
        "",
        "Developed with the JGE++ Library",
        "SFX From www.soundsnap.com",
        "",
        "",
        "This work is not related to or endorsed by Wizards of the Coast, Inc",
        "",
        "Please support this project with donations at  Wagic Discord",
    };

    //The credits scroller belongs to the Credits tab (an empty list it
    //shows through) - drawing it under every tab bleeds text into the row
    //gaps of the sparser ones.
    WGuiBase * currentTab = optionsTabs ? optionsTabs->Current() : NULL;
    if (currentTab && currentTab->getDisplay() == "Credits")
    {
        WFont * mFont = WResourceManager::Instance()->GetWFont(Fonts::MAGIC_FONT);
        mFont->SetColor(ARGB(255,200,200,200));
        mFont->SetScale(1.0);
        float startpos = 272 - timer;
        float pos = startpos;
        int size = sizeof(CreditsText) / sizeof(CreditsText[0]);

        for (int i = 0; i < size; i++)
        {
            pos = startpos + 20 * i;
            if (pos > -20 && pos < SCREEN_HEIGHT + 20)
            {
                mFont->DrawString(CreditsText[i], SCREEN_WIDTH / 2, pos, JGETEXT_CENTER);
            }
        }

        if (pos < -20)
            timer = 0;
    }

    optionsTabs->Render();

    if (mState == SHOW_OPTIONS_MENU)
        optionsMenu->Render();

#ifdef WITH_GPT_AI
    if (mState == SHOW_TELEMETRY_CONSENT && telemetryMenu)
        telemetryMenu->Render();
    if (mState == SHOW_MODEL_PICKER)
    {
        if (pickerPhase == PICKER_FETCHING)
        {
            WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
            font->SetColor(ARGB(255, 255, 255, 255));
            font->DrawString(_("Fetching the model list...").c_str(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, JGETEXT_CENTER);
        }
        else if (modelPickerMenu)
            modelPickerMenu->Render();
    }
    if (mState == SHOW_OAI_SIGNIN)
        renderOaiSignIn();
#endif

    if (options.keypadActive())
        options.keypadRender();
}

void GameStateOptions::ButtonPressed(int controllerId, int controlId)
{
    //Exit menu?
    if (controllerId == -102)
        switch (controlId)
        {
        case GameStateOptionsConst::kSaveAndBackToMainMenuID:
#ifdef WITH_GPT_AI
            //Telemetry consent negotiation: on saving a newly set-up
            //endpoint (and only if never decided), ask before persisting.
            //Full disclosure of what is collected lives in the GPT tab.
            if (gptTab && gptTab->wantsTelemetryConsent())
            {
                SAFE_DELETE(telemetryMenu);
                telemetryMenu = NEW SimpleMenu(JGE::GetInstance(), WResourceManager::Instance(),
                    GameStateOptionsConst::kTelemetryMenuID, this, Fonts::MENU_FONT, 50, 130,
                    "Contribute anonymized game data?");
                telemetryMenu->Add(GameStateOptionsConst::kTelemetryYesID, "Yes, help improve the AI");
                telemetryMenu->Add(GameStateOptionsConst::kTelemetryNoID, "No thanks");
                mState = SHOW_TELEMETRY_CONSENT;
                break;
            }
#endif
            mState = SAVE;
            break;
            //Set Audio volume
        case GameStateOptionsConst::kBackToMainMenuID:
            mParent->DoTransition(TRANSITION_FADE, GAME_STATE_MENU);
            break;
        case kCancelMenuID:
            mState = SHOW_OPTIONS;
            break;
        case GameStateOptionsConst::kNewProfileID:
            options.keypadStart("", &newProfile);
            options.keypadTitle("New Profile");
            break;
        case GameStateOptionsConst::kReloadID:
            mReload = true;
            break;
        case GameStateOptionsConst::kResetTutorialsID:
            options.resetTutorialMessages();
            //The reset switches tutorials back on; the toggle a row above
            //caches its value, so refresh the tab's rows from the live
            //options. Not the mReload path - that re-reads the profile from
            //disk, which would discard the flip on Vita, where the save is
            //deferred.
            if (optionsTabs)
                optionsTabs->Reload();
            if (mResetTutorialsLabel)
                mResetTutorialsLabel->setDisplay("Tutorial messages reset");
            break;
        }
#ifdef WITH_GPT_AI
    else if (controllerId == GameStateOptionsConst::kTelemetryMenuID)
    {
        if (gptTab)
            gptTab->setTelemetryConsent(controlId == GameStateOptionsConst::kTelemetryYesID ? 1 : 0);
        //Do NOT delete telemetryMenu here: this callback runs from inside
        //its own Update(). Leaving SHOW_TELEMETRY_CONSENT hides it; it is
        //freed in End() or replaced on the next ask.
        mState = SAVE; //resume the save that triggered the ask
    }
    else if (controllerId == GameStateOptionsConst::kModelPickerMenuID)
    {
        //Same UAF rule as the telemetry menu: NEVER rebuild or delete the
        //menu from inside its own callback - record what to build and let
        //updateModelPicker do it next frame.
        if (controlId == GameStateOptionsConst::kPickerCancelID)
            mState = SHOW_OPTIONS;
        else if (controlId == GameStateOptionsConst::kPickerManualID)
        {
            mState = SHOW_OPTIONS;
            if (gptTab)
            {
                SimplePad * pad = options.keypadStart(gptTab->cfg.model, &gptTab->cfg.model);
                if (pad)
                    pad->title = "Model";
            }
        }
        else if (controlId == GameStateOptionsConst::kPickerBackID)
        {
            pickerPhase = PICKER_VENDORS;
            pickerBuildFor = kPickerBuildVendors;
        }
        else if (pickerPhase == PICKER_VENDORS)
        {
            if (controlId >= 0 && controlId < (int) pickerVendors.size())
            {
                pickerPhase = PICKER_MODELS;
                pickerBuildFor = controlId;
            }
        }
        else if (pickerPhase == PICKER_MODELS && gptTab)
        {
            if (pickerVendorAt < (int) pickerVendorModels.size()
                && controlId >= 0 && controlId < (int) pickerVendorModels[pickerVendorAt].size())
            {
                gptTab->cfg.model = pickerModels[pickerVendorModels[pickerVendorAt][controlId]];
                mState = SHOW_OPTIONS;
            }
        }
    }
#endif
    else
        optionsTabs->ButtonPressed(controllerId, controlId);
}
;

#ifdef WITH_GPT_AI
void GameStateOptions::startModelPicker()
{
    if (!gptTab)
        return;
    pickerModels.clear();
    pickerVendors.clear();
    pickerVendorModels.clear();
    pickerVendorAt = 0;
    pickerPhase = PICKER_FETCHING;
    pickerBuildFor = kPickerBuildNone;
    mState = SHOW_MODEL_PICKER;

    modelFetch = std::make_shared<GptModelFetch>();
    ModelFetchCtx * ctx = new ModelFetchCtx(); //plain new: ModelFetchMain deletes it
    ctx->state = modelFetch;
    ctx->url = gptTab->cfg.primaryUrl();
    ctx->key = gptTab->cfg.key;
    if (!gptSpawnWorker(&ModelFetchMain, ctx))
        ModelFetchMain(ctx); //threadless platform: fetch synchronously (probe precedent)
}

//Bucket the fetched ids by the vendor prefix before '/' - OpenRouter-style
//catalogs (300+ ids) become ~50 buckets of navigable size, while unprefixed
//catalogs (api.openai.com, local servers) collapse into one bucket and the
//vendor step is skipped entirely.
void GameStateOptions::buildPickerIndex()
{
    pickerVendors.clear();
    pickerVendorModels.clear();
    for (size_t i = 0; i < pickerModels.size(); i++)
    {
        size_t slash = pickerModels[i].find('/');
        std::string vendor = (slash == std::string::npos) ? std::string("(unprefixed)")
                                                          : pickerModels[i].substr(0, slash);
        size_t at = pickerVendors.size();
        for (size_t v = 0; v < pickerVendors.size(); v++)
            if (pickerVendors[v] == vendor)
                at = v;
        if (at == pickerVendors.size())
        {
            pickerVendors.push_back(vendor);
            pickerVendorModels.push_back(std::vector<int>());
        }
        pickerVendorModels[at].push_back((int) i);
    }
}

void GameStateOptions::buildPickerMenu()
{
    SAFE_DELETE(modelPickerMenu);
    bool manualEntry = gptTab && !gptCodexEndpoint(gptTab->cfg.primaryUrl());
    if (pickerBuildFor == kPickerBuildVendors)
    {
        modelPickerMenu = NEW SimpleMenu(JGE::GetInstance(), WResourceManager::Instance(),
            GameStateOptionsConst::kModelPickerMenuID, this, Fonts::MENU_FONT, 50, 60,
            "Model vendor", 9);
        for (size_t v = 0; v < pickerVendors.size(); v++)
        {
            char label[128];
            sprintf(label, "%s (%d)", pickerVendors[v].c_str(), (int) pickerVendorModels[v].size());
            modelPickerMenu->Add((int) v, label);
        }
        if (manualEntry)
            modelPickerMenu->Add(GameStateOptionsConst::kPickerManualID, "Type manually...");
        modelPickerMenu->Add(GameStateOptionsConst::kPickerCancelID, "Cancel");
    }
    else
    {
        pickerVendorAt = pickerBuildFor;
        const std::vector<int>& bucket = pickerVendorModels[pickerVendorAt];
        modelPickerMenu = NEW SimpleMenu(JGE::GetInstance(), WResourceManager::Instance(),
            GameStateOptionsConst::kModelPickerMenuID, this, Fonts::MENU_FONT, 50, 60,
            pickerVendors[pickerVendorAt].c_str(), 9);
        for (size_t i = 0; i < bucket.size(); i++)
        {
            //Show the id without its vendor prefix - the menu title carries it.
            const std::string& id = pickerModels[bucket[i]];
            size_t slash = id.find('/');
            modelPickerMenu->Add((int) i, (slash == std::string::npos ? id : id.substr(slash + 1)).c_str());
        }
        if (pickerVendors.size() > 1)
            modelPickerMenu->Add(GameStateOptionsConst::kPickerBackID, "Back to vendors");
        if (manualEntry)
            modelPickerMenu->Add(GameStateOptionsConst::kPickerManualID, "Type manually...");
        modelPickerMenu->Add(GameStateOptionsConst::kPickerCancelID, "Cancel");
    }
}

void GameStateOptions::updateModelPicker(float dt)
{
    if (pickerPhase == PICKER_FETCHING)
    {
        //The fetch worker owns the state; poll it. Back/secondary cancels a
        //slow fetch rather than trapping the user on a spinner.
        JButton key;
        while ((key = JGE::GetInstance()->ReadButton()))
            if (key == JGE_BTN_SEC || key == JGE_BTN_MENU)
            {
                mState = SHOW_OPTIONS;
                return;
            }
        int status = 0;
        bool ok = false;
        if (modelFetch)
        {
            std::lock_guard<GptMutex> g(modelFetch->mtx);
            status = modelFetch->status;
            ok = modelFetch->ok;
            if (status == 2)
                pickerModels = modelFetch->models;
        }
        if (status != 2)
            return;
        if (!ok || pickerModels.empty())
        {
            //No listing (endpoint down, or a server that just does not
            //advertise): manual entry is the honest remainder.
            if (gptTab)
            {
                SimplePad * pad = options.keypadStart(gptTab->cfg.model, &gptTab->cfg.model);
                if (pad)
                    pad->title = "Model (no listing - type it)";
            }
            mState = SHOW_OPTIONS;
            return;
        }
        buildPickerIndex();
        //A single bucket makes a vendor step pure friction - skip it.
        if (pickerVendors.size() == 1)
        {
            pickerPhase = PICKER_MODELS;
            pickerBuildFor = 0;
        }
        else
        {
            pickerPhase = PICKER_VENDORS;
            pickerBuildFor = kPickerBuildVendors;
        }
    }
    if (pickerBuildFor != kPickerBuildNone)
    {
        buildPickerMenu();
        pickerBuildFor = kPickerBuildNone;
    }
    if (modelPickerMenu)
        modelPickerMenu->Update(dt);
}
void GameStateOptions::startOaiSignIn()
{
    oaiSignIn = std::make_shared<GptOaiSignIn>();
    mState = SHOW_OAI_SIGNIN;
    gptOaiSignInStart(oaiSignIn); //on refusal the state is already marked failed
}

void GameStateOptions::updateOaiSignIn(float dt)
{
    (void) dt;
    //Back cancels at every stage; any button dismisses a terminal state.
    int status;
    {
        std::lock_guard<GptMutex> g(oaiSignIn->mtx);
        status = oaiSignIn->status;
    }
    JButton key;
    while ((key = JGE::GetInstance()->ReadButton()))
    {
        bool leave = (key == JGE_BTN_SEC || key == JGE_BTN_MENU)
                     || (status >= 2 && key != JGE_BTN_NONE);
        if (leave)
        {
            {
                std::lock_guard<GptMutex> g(oaiSignIn->mtx);
                oaiSignIn->cancel = true; //no-op once the worker finished
            }
            //A completed sign-in changes what the GPT tab rows should show
            //(signed-in status; Test connection now meaningful) - the rows
            //read live state, so returning is enough.
            mState = SHOW_OPTIONS;
            return;
        }
    }
}

void GameStateOptions::renderOaiSignIn()
{
    int status;
    std::string userCode, verifyUrl, plan, error;
    {
        std::lock_guard<GptMutex> g(oaiSignIn->mtx);
        status = oaiSignIn->status;
        userCode = oaiSignIn->userCode;
        verifyUrl = oaiSignIn->verifyUrl;
        plan = oaiSignIn->plan;
        error = oaiSignIn->error;
    }
    JRenderer * r = JRenderer::GetInstance();
    WFont * font = WResourceManager::Instance()->GetWFont(Fonts::MAIN_FONT);
    font->SetColor(ARGB(255, 255, 255, 255));

    //This screen draws over the options list, and without a scrim the typed
    //address lands on top of similar-sized option text and becomes unreadable
    //(owner-reported on the tablet). One dark wash under every state; the QR
    //keeps its own white backdrop on top of it.
    r->FillRect(0, 0, (float) SCREEN_WIDTH, (float) SCREEN_HEIGHT, ARGB(235, 10, 10, 14));

    if (status == 0)
    {
        font->DrawString(_("Requesting a sign-in code...").c_str(), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, JGETEXT_CENTER);
        return;
    }
    if (status == 3)
    {
        font->DrawString(_("Sign-in failed").c_str(), SCREEN_WIDTH / 2, 90, JGETEXT_CENTER);
        font->DrawString(error.c_str(), SCREEN_WIDTH / 2, 130, JGETEXT_CENTER);
        font->DrawString(_("Press any button").c_str(), SCREEN_WIDTH / 2, 200, JGETEXT_CENTER);
        return;
    }
    if (status == 2)
    {
        font->DrawString(_("Signed in to ChatGPT").c_str(), SCREEN_WIDTH / 2, 110, JGETEXT_CENTER);
        if (plan.size())
            font->DrawString((std::string(_("Plan: ")) + plan).c_str(), SCREEN_WIDTH / 2, 140, JGETEXT_CENTER);
        font->DrawString(_("Press any button").c_str(), SCREEN_WIDTH / 2, 200, JGETEXT_CENTER);
        return;
    }

    //status 1: the QR (right) + instructions, address and code (left).
    //The QR is only a shortcut to the same address: the address renders as
    //text too, for users without a QR reader or signing in from a PC. The
    //code sits alone in a bordered box so it cannot be mistaken for prose.

    //QR first - its white backdrop must never paint over the text column.
    float qrLeft = (float) SCREEN_WIDTH - 24;
    try
    {
        using qrcodegen::QrCode;
        const QrCode qr = QrCode::encodeText(verifyUrl.c_str(), QrCode::Ecc::MEDIUM);
        //Fit into the right half: quiet zone of 4 modules per side, integer
        //pixel scale so modules stay square and scannable.
        const int quiet = 4;
        const int cells = qr.getSize() + 2 * quiet;
        int scale = (int) ((SCREEN_HEIGHT - 40) / cells);
        if (scale < 2) scale = 2;
        const float side = (float) (cells * scale);
        const float x0 = SCREEN_WIDTH - side - 24;
        const float y0 = (SCREEN_HEIGHT - side) / 2;
        qrLeft = x0;
        r->FillRect(x0, y0, side, side, ARGB(255, 255, 255, 255));
        for (int yy = 0; yy < qr.getSize(); yy++)
            for (int xx = 0; xx < qr.getSize(); xx++)
                if (qr.getModule(xx, yy))
                    r->FillRect(x0 + (float) ((xx + quiet) * scale),
                                y0 + (float) ((yy + quiet) * scale),
                                (float) scale, (float) scale, ARGB(255, 0, 0, 0));
    }
    catch (const std::exception&)
    {
        //Encoding a constant URL cannot realistically fail; if it somehow
        //does, the typed address below is the fallback the screen shows.
    }

    const float colX = 24;
    const float colW = qrLeft - colX - 12;

    //Draw a line shrunk just enough to fit the text column (GetStringWidth
    //already includes the current scale).
    struct Fit
    {
        static void draw(WFont * f, const std::string& s, float x, float y, float maxW, int align = JGETEXT_LEFT)
        {
            float old = f->GetScale();
            float w = f->GetStringWidth(s.c_str());
            if (w > maxW && w > 0)
                f->SetScale(old * maxW / w);
            f->DrawString(s, x, y, align);
            f->SetScale(old);
        }
    };

    //The scheme is noise on-screen; every browser accepts the bare host.
    std::string showUrl = verifyUrl;
    if (showUrl.compare(0, 8, "https://") == 0)
        showUrl = showUrl.substr(8);

    Fit::draw(font, _("On your phone or PC, go to:"), colX, 30, colW);
    font->SetColor(ARGB(255, 255, 230, 120));
    Fit::draw(font, showUrl, colX, 52, colW);
    font->SetColor(ARGB(255, 255, 255, 255));
    Fit::draw(font, _("(or scan the QR code)"), colX, 76, colW);
    Fit::draw(font, _("then enter this code:"), colX, 100, colW);

    //The code, alone in a high-contrast box.
    const float boxY = 118;
    const float boxH = 46;
    r->FillRect(colX, boxY, colW, boxH, ARGB(255, 12, 12, 20));
    r->DrawRect(colX, boxY, colW, boxH, ARGB(255, 255, 230, 120));
    WFont * big = WResourceManager::Instance()->GetWFont(Fonts::MAGIC_FONT);
    big->SetColor(ARGB(255, 255, 235, 140));
    float oldScale = big->GetScale();
    big->SetScale(1.8f);
    float codeW = big->GetStringWidth(userCode.c_str());
    if (codeW > colW - 16 && codeW > 0)
        big->SetScale(1.8f * (colW - 16) / codeW);
    big->DrawString(userCode, colX + colW / 2, boxY + 12, JGETEXT_CENTER);
    big->SetScale(oldScale);

    Fit::draw(font, _("Waiting for approval..."), colX, 192, colW);
    Fit::draw(font, _("(Back cancels)"), colX, 214, colW);
}
#endif //WITH_GPT_AI

void GameStateOptions::GrabKeyboard(KeybGrabber* g)
{
    grabber = g;
}
void GameStateOptions::UngrabKeyboard(const KeybGrabber* g)
{
    if (g == grabber)
        grabber = NULL;
}
