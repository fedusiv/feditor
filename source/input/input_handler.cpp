#include <iostream>
#include <algorithm>

#include "SDL.h"

#include "input_handler.hpp"

InputHandler::InputHandler()
{
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
                std::cout << keyValue << std::endl;
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

void InputHandler::KeyPressed(SDL_KeyboardEvent event)
{
    bool found;
    int timeToCompare;

    found = false;


    // Key was pressed or still om pressing state
    // Check is it was pressed before, this is mechanism for continues pressing
    for(auto e: *_keysEvents)
    {
        if(e.event.keysym.sym == event.keysym.sym)
        {
            found = true;
            // key was already pressed and still is pressing
            if(e.pressedAmount == 1)
            {
                timeToCompare = 300;
            }
            else
            {
                timeToCompare = 100;
            }

            if( (event.timestamp - e.event.timestamp) >= timeToCompare )
            {
                e.event.timestamp = event.timestamp;    // save previous timestamp time
                e.pressedAmount += 1;
                // put key to be acted
                AddKeyToAct(event.keysym.sym);
            }

            break; // exit searching loop
        }
    }

    if(!found)
    {
        // insert new key
        auto ke = new KeyEvent(event);
        _keysEvents->push_back(ke);
        AddKeyToAct(event.keysym.sym);
    }
}

/*
* Procedure of releasing key
*/
void InputHandler::KeyReleased(SDL_KeyboardEvent event)
{
    SDL_Keycode code;   // code of event which should operate
    std::list<KeyEvent>::iterator keIt; // key ivent iterator
    std::list<SDL_Keycode>::iterator kcIt;  // key code to act iterator

    code = event.keysym.sym;

    for(keIt = _kEvents->begin(); keIt != _kEvents->end();)
    {
        if((*keIt).event.keysym.sym == code)
        {
            // found iterator which should be removed
            keIt = _kEvents->erase(keIt);  // remove from events
            break;
        }
        else
        {
            keIt++;
        }
    }

    // now remove everything from keys to act
    for(kcIt = _keysToAct->begin(); kcIt != _keysToAct->end();)
    {
        if(*kcIt == code)
        {
            kcIt = _keysToAct->erase(kcIt);
        }
        else
        {
            kcIt++;
        }
    }
}


/*
    Adding key to be processed and acted
*/
void InputHandler::AddKeyToAct(SDL_Keycode key)
{
    bool found;
   
    found = (std::find(_keysToAct->begin(), _keysToAct->end(), key) != _keysToAct->end());
    if(!found)
    {
        _keysToAct->push_back(key);
    }
}




