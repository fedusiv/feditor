#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__


#include <list>
#include <tuple>

#include "SDL.h"
#include "key_act.hpp"
#include "key_map.hpp"

typedef std::list<KeyEvent *>  KeysEventList_t;
typedef std::list<KeyAct *>  KeysActList_t;



class InputHandler
{
    private:

        KeysEventList_t * _keysEvents; // container to hold information about pressed keys (kind of hardware part)
        KeysActList_t * _keysAct; // list of keys, which should be parsed, or rephrase on what editor should act (logic part)
        std::string _insertedString; // string, which were received. Text data

        void KeyPressed(SDL_KeyboardEvent event);   // SDL_KEYDOWN
        void KeyReleased(SDL_KeyboardEvent event);  // SDL_KEYUP
        void AddKeyToAct(SDL_KeyboardEvent event);  // add key to act list

    public:
        InputHandler();
        bool Polling();
        void RemoveAllKeysFromAct();
        std::string GetInsertedText(); // requested text, which was inserted, basically it will returnb onlt one character.
        KeysActList_t * GetKeysAct();   // request and return list of keys, to which editor should react

};

#endif // __INPUT_HANDLER_HPP__
