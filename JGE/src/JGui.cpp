//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
//
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
//
//-------------------------------------------------------------------------------------

#include "../include/JGE.h"
#include "../include/JGui.h"

JGE* JGuiObject::mEngine = NULL;

JGuiObject::JGuiObject(int id) :
    mId(id)
{
    mEngine = JGE::GetInstance();
}

JGuiObject::~JGuiObject()
{
    //    JGERelease();
}

bool JGuiObject::Leaving(JButton key __attribute__((unused)))
{
    return true;
}

bool JGuiObject::ButtonPressed()
{
    return false;
}

void JGuiObject::Entering()
{

}

int JGuiObject::GetId()
{
    return mId;
}

void JGuiObject::Update(float dt __attribute__((unused)))
{
}

ostream& operator<<(ostream &out, const JGuiObject &j)
{
    return j.toString(out);
}

JGuiController::JGuiController(JGE* jge, int id, JGuiListener* listener) :
    mEngine(jge), mId(id), mListener(listener)
{
    mBg = NULL;
    mShadingBg = NULL;
    mCursor = NULL;   //#W54-N (A48): was never initialised

    mCount = 0;
    mCurr = 0;

    mCursorX = SCREEN_WIDTH / 2;
    mCursorY = SCREEN_HEIGHT / 2;
    mShowCursor = false;

    mActionButton = JGE_BTN_OK;
    mCancelButton = JGE_BTN_MENU;

    mStyle = JGUI_STYLE_WRAPPING;

    mActive = true;
}

JGuiController::~JGuiController()
{
    for (int i = 0; i < mCount; i++)
        if (mObjects[i] != NULL) delete mObjects[i];
    for (size_t i = 0; i < mButtons.size(); i++)
        if (mButtons[i] != NULL) delete mButtons[i];

}

void JGuiController::Render()
{
    for (int i = 0; i < mCount; i++)
        if (mObjects[i] != NULL) mObjects[i]->Render();
}

bool JGuiController::CheckUserInput(JButton key)
{
    if (!mCount) return false;
    if (key == mActionButton)
    {
        if (!mObjects.empty() && mObjects[mCurr] != NULL && mObjects[mCurr]->ButtonPressed())
        {
            if (mListener != NULL) mListener->ButtonPressed(mId, mObjects[mCurr]->GetId());
            return true;
        }
    }
    else if (key == mCancelButton)
    {
        if (mListener != NULL)
        {
            mListener->ButtonPressed(mId, kCancelMenuID);
        }
    }
    else if (JGE_BTN_CANCEL == key)
    {
        if (mListener != NULL) mListener->ButtonPressed(mId, kInfoMenuID);
        return true;
    }
    else if ((JGE_BTN_LEFT == key) || (JGE_BTN_UP == key)) // || mEngine->GetAnalogY() < 64 || mEngine->GetAnalogX() < 64)
    {
        int n = mCurr;
        n--;
        if (n < 0)
        {
            if ((mStyle & JGUI_STYLE_WRAPPING))
                n = mCount - 1;
            else
                n = 0;
        }

        if (n != mCurr && mObjects[mCurr] != NULL && mObjects[mCurr]->Leaving(JGE_BTN_UP))
        {
            mCurr = n;
            mObjects[mCurr]->Entering();
        }
        return true;
    }
    else if ((JGE_BTN_RIGHT == key) || (JGE_BTN_DOWN == key)) // || mEngine->GetAnalogY()>192 || mEngine->GetAnalogX()>192)
    {
        int n = mCurr;
        n++;
        if (n > mCount - 1)
        {
            if ((mStyle & JGUI_STYLE_WRAPPING))
                n = 0;
            else
                n = mCount - 1;
        }

        if (n != mCurr && mObjects[mCurr] != NULL && mObjects[mCurr]->Leaving(JGE_BTN_DOWN))
        {
            mCurr = n;
            mObjects[mCurr]->Entering();
        }
        return true;
    }
    else
    { // a dude may have clicked somewhere, we're gonna select the closest object from where he clicked
        int x = -1, y = -1;
        unsigned int distance2;
        unsigned int minDistance2 = -1;
        int n = mCurr;
        if (mEngine->GetLeftClickCoordinates(x, y))
        {
            // first scan the buttons on the screen and then process the other gui elements
            for (size_t i = 0; i < mButtons.size(); i++)
            {
                if (mButtons[i] != NULL && mButtons[i]->ButtonPressed())
                {
                    mEngine->LeftClickedProcessed();
                    return true;
                }
            }
            
            if (mObjects.size())
            {
                for (int i = 0; i < mCount; i++)
                {
                    float top, left;
                    //#W54-N (A48): NULL rows exist (Add(0, ...) / vpk9); the
                    //Update/Render loops guard them, this one did not.
                    if (mObjects[i] != NULL && mObjects[i]->getTopLeft(top, left))
                    {
                        distance2 = (top - y) * (top - y) + (left - x) * (left - x);
                        if (distance2 < minDistance2)
                        {
                            minDistance2 = distance2;
                            n = i;
                        }
                        else
                            break;
                    }
                }
                
                if (n != mCurr && mObjects[mCurr] != NULL && mObjects[mCurr]->Leaving(JGE_BTN_DOWN))
                {
                    mCurr = n;
                    mObjects[mCurr]->Entering();
                }
                // if the same object was selected process click
                else if (n == mCurr && mObjects[mCurr] != NULL && mObjects[mCurr]->Leaving(JGE_BTN_OK))
                {
                    mObjects[mCurr]->Entering();
                }
                mEngine->LeftClickedProcessed();
                mEngine->ResetInput();
                return true;
            }
            mEngine->LeftClickedProcessed();
        }
    }
    return false;
}

void JGuiController::Update(float dt)
{
    for (int i = 0; i < mCount; i++)
        if (mObjects[i] != NULL)
            mObjects[i]->Update(dt);
    
    //NULL-tolerant like the objects loop above: a Vita dump (2026-09-02,
    //psp2core-1788315925) died here on a SimpleMenu whose button list held a
    //single NULL - heap damage of unknown origin; the guard turns that into a
    //skipped entry instead of a data abort at the end of a won game.
    for (size_t i = 0; i < mButtons.size(); i++ )
        if (mButtons[i] != NULL)
            mButtons[i]->Update(dt);

    if(mEngine)
    {
        JButton key = mEngine->ReadButton();
        CheckUserInput(key);
    }
}

void JGuiController::Add(JGuiObject* ctrl, bool isButton)
{
    if (!isButton)
    {
        mObjects.push_back(ctrl);
        mCount++;
    }
    else
    {
        mButtons.push_back(ctrl);
    }
}

void JGuiController::RemoveAt(int i, bool isButton)
{
    //#W54-N (A48): erase() shifts the successor into slot i, so the old
    //`erase; delete v[i]` freed the WRONG object (the successor - still in
    //the vector, so a UAF on the next Update and a double delete in the
    //destructor) and leaked the one asked for. Take the pointer first. No
    //caller today; the trap is disarmed, not moved.
    if (isButton)
    {
        if (i < 0 || (size_t)i >= mButtons.size() || !mButtons[i]) return;
        JGuiObject* gone = mButtons[i];
        mButtons.erase(mButtons.begin() + i);
        delete gone;
        return;
    }

    if (i < 0 || (size_t)i >= mObjects.size() || !mObjects[i]) return;
    JGuiObject* gone = mObjects[i];
    mObjects.erase(mObjects.begin() + i);
    delete gone;
    mCount--;
    if (mCurr >= mCount) mCurr = 0;
    return;
}

void JGuiController::Remove(int id)
{
    for (int i = 0; i < mCount; i++)
    {
        if (mObjects[i] != NULL && mObjects[i]->GetId() == id)
        {
            RemoveAt(i);
            return;
        }
    }
    
    for (size_t i = 0; i < mButtons.size(); i++)
    {
        if (mButtons[i] != NULL && mButtons[i]->GetId() == id)
        {
            RemoveAt(i, true);
            return;
        }
    }
}

void JGuiController::Remove(JGuiObject* ctrl)
{
    for (int i = 0; i < mCount; i++)
    {
        if (mObjects[i] != NULL && mObjects[i] == ctrl)
        {
            RemoveAt(i);
            return;
        }
    }

    
    for (size_t i = 0; i < mButtons.size(); i++)
    {
        if (mButtons[i] != NULL && mButtons[i] == ctrl)
        {
            RemoveAt(i, true);
            return;
        }
    }
}

void JGuiController::SetActionButton(JButton button)
{
    mActionButton = button;
}
void JGuiController::SetStyle(int style)
{
    mStyle = style;
}
void JGuiController::SetCursor(JSprite* cursor)
{
    mCursor = cursor;
}
bool JGuiController::IsActive()
{
    return mActive;
}
void JGuiController::SetActive(bool flag)
{
    mActive = flag;
}
