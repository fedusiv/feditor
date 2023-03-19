#include <iostream>
#include <algorithm>

#include "SDL.h"
#include "input.hpp"

Input::Input()
{

}


/*
    Input module is based on very pimitive logic
    In update it checks which keys are pressed and add keys to list
    In update it checks which keys were released and remove keys from list

    in KeysMap module checks which keys should be acted. Basically all keys need to be acted, except keys which repeats actions.
    For repeat actions we need to have some delay

*/
void Input::Update(bool inputRead)
{
    SDL_Event e;
    bool quit;

    quit = false;
    
    SDL_Delay(1); // rest for other applications, waited for events

    _keysText.clear(); // empty string
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
            {
                _keysAct.clear();   // kind of dirty hack (kostyl if you know). Remove all keys to act and insert Exit
                 _keysAct.push_back(new KeyAction(KeyMap::KeyExit));    // Exit is high priority in this case
                return; // exit even from function
            }
            case SDL_TEXTINPUT:
            {
                if(inputRead)
                {
                    _keysText.push_back(static_cast<int>(e.text.text[0]));  // This is relevant only for English insert text!
                }
                break;
            }
            case SDL_KEYDOWN:
            {
                KeyPressed(e.key.keysym.sym);
                break;
            }
            case SDL_KEYUP:
            {
                KeyReleased(e.key.keysym.sym);
                break;
            }
        }
    }
}

void Input::KeyPressed(int code)
{
    KeysActionList::iterator kaIt;

    kaIt = std::find_if(_keysAct.begin(), _keysAct.end(),
            [&code](KeyAction* k) { return code == k->keyCode;} );
    if(kaIt == _keysAct.end())
    {
        // No element there
        _keysAct.push_back(new KeyAction(code));
    }
}

void Input::KeyReleased(int code)
{

    KeysActionList::iterator kaIt;

    for(kaIt = _keysAct.begin(); kaIt != _keysAct.end();)
    {
        if((*kaIt)->keyCode == code)
        {
            // found iterator which should be removed
            auto it = *kaIt;
            kaIt = _keysAct.erase(kaIt);  // remove from events
            delete it;
            break;
        }
        else
        {
            kaIt++;
        }
    }

}

KeysMapList Input::KeysMap()
{
    double currentTime, actionTime;

    currentTime = SDL_GetTicks();

    _keysMap.clear();

    for(auto key: _keysAct)
    {
        actionTime = 200;
        if(key->acted == 0)
        {
            _keysMap.push_back(key->keyMap);
            continue;
        }
        if(key->acted > 1)
        {
            actionTime = 50;
        }
        if(currentTime - key->time > actionTime)
        {
            // key need to be acted. Add to list
            _keysMap.push_back(key->keyMap);
        }
    }

    return _keysMap;
}

void Input::ClearKeysMap()
{
    double currentTime;
    KeysMapList::iterator it;

    currentTime = SDL_GetTicks();

    for(auto key: _keysAct)
    {
        it = std::find(_keysMap.begin(), _keysMap.end(), key->keyMap);
        if(it != _keysMap.end())
        {
            // found keymap in keyact
            key->acted++;
            key->time = currentTime;
            _keysMap.erase(it); // remove found keymap
            if(_keysMap.empty())
            {
                break;  // everything is found, exit loop
            }
        }
    }
}

KeysInsertedText& Input::KeysText(void)
{
    return _keysText;
}