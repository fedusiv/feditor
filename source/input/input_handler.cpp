#include <iostream>
#include <algorithm>

#include "SDL.h"

#include "input_handler.hpp"

InputHandler::InputHandler()
{
    _keyMapOperation = KeyMapOperation::Instance(); // create signleton

    _keysEvents = new std::list<KeyEvent *>(); // initialize pointer
    _keysAct = new std::list<KeyAct *>();

    SDL_StopTextInput();    // have no idea, but in the begging of app it starts to insert text already
}

bool InputHandler::Polling()
{
    SDL_Event e;
    bool quit;

    quit = false;
    
    SDL_Delay(1); // rest for other applications, waited for events

    RemoveKeyFromActProcess();
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
            {
                quit = true;
                break;
            }
            case SDL_TEXTINPUT:
            {
                _insertedString = std::string(e.text.text);
                break;
            }
            case SDL_KEYDOWN:
            {
                KeyPressed(e.key);
                break;
            }
            case SDL_KEYUP:
            {
                KeyReleased(e.key);
                break;
            }
        }
    }

    return quit;
}

std::string InputHandler::GetInsertedText()
{
    return _insertedString;
}

KeysActList_t *InputHandler::GetKeysAct()
{
    return _keysAct;
}

void InputHandler::KeyPressed(SDL_KeyboardEvent event)
{
    bool found;
    int timeToCompare;

    found = false;


    // Key was pressed or still om pressing state
    // Check is it was pressed before, this is mechanism for continues pressing
    for(auto e: *_keysEvents)
    {
        if(e->event.keysym.sym == event.keysym.sym)
        {
            found = true;
            // key was already pressed and still is pressing
            if(e->pressedAmount == 1)
            {
                timeToCompare = 300;
            }
            else
            {
                timeToCompare = 100;
            }

            if( (event.timestamp - e->event.timestamp) >= timeToCompare )
            {
                e->event.timestamp = event.timestamp;    // save previous timestamp time
                e->pressedAmount += 1;
                // put key to be acted
                AddKeyToAct(event);  // continue to acting on button press
            }

            break; // exit searching loop
        }
    }

    if(!found)
    {
        auto ke = new KeyEvent(event);  // create harware event
        _keysEvents->push_back(ke);     // put to list of hardware pressed
        AddKeyToAct(event);  // add to logic part of acting
    }
}

/*
* Procedure of releasing key
*/
void InputHandler::KeyReleased(SDL_KeyboardEvent event)
{
    SDL_Keycode code;   // code of event which should operate
    KeysEventList_t::iterator keIt; // key ivent iterator
    KeyEvent * kE;  // pointer to delete created object

    code = event.keysym.sym;

    for(keIt = _keysEvents->begin(); keIt != _keysEvents->end();)
    {
        if((*keIt)->event.keysym.sym == code)
        {
            // found iterator which should be removed
            kE = *keIt;
            keIt = _keysEvents->erase(keIt);  // remove from events
            delete kE;
            break;
        }
        else
        {
            keIt++;
        }
    }
}


/*
    Adding key to be processed and acted
*/
void InputHandler::AddKeyToAct(SDL_KeyboardEvent event)
{
   KeyAct * act;

   act = new KeyAct(event);
   _keysAct->push_back(act);
}

void InputHandler::RemoveKeyFromAct(KeyAct * act)
{
    _keysAct->remove(act);
}

/*
*  By time parameter remove key from act processing
*  This is kind of coyote time (Read coyote in game dev)
*  Even after releasing key, if it was not processed giving some time to enter second key
*  This mechanism for shortcuts
*/
void InputHandler::RemoveKeyFromActProcess(void)
{
    KeysActList_t::iterator kaIt; // key act iterator
    uint32_t timeCurrent;
    KeyAct * kA;    // pointer to keyAct, which should be deleted

    timeCurrent = SDL_GetTicks();

    for(kaIt = _keysAct->begin(); kaIt != _keysAct->end();)
    {
        if(timeCurrent - (*kaIt)->event.timestamp > 50)
        {
            kA = *kaIt;
            kaIt = _keysAct->erase(kaIt);
            delete kA;
        }
    }
}
