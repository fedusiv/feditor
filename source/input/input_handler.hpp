#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__


#include <list>
#include <tuple>

#include "SDL.h"
#include "key_act.hpp"

/*
* Represent in which mode key button was pressed or combination were entered
*/
enum class InputModes
{
    EditInputMode,
    InsertInputMode
};

class InputHandler
{
    private:
        class KeyEvent
        {
            public:
                KeyEvent(SDL_KeyboardEvent e): event(e)
                {
                    pressedAmount = 1;  // just pressed
                };
                SDL_KeyboardEvent event;
                int pressedAmount;
        };

        std::list<KeyEvent *> * _keysEvents; // container to hold information about pressed keys
        std::list<KeyAct *> * _keysAct; // list of keys, which should be parsed, or rephrase on what editor should act
        std::string _insertedString; // string, which were received. Text data
 
        void KeyPressed(SDL_KeyboardEvent event);   // SDL_KEYDOWN
        void KeyReleased(SDL_KeyboardEvent event);  // SDL_KEYUP
        void AddKeyToAct(SDL_Keycode key);  // add key to act list

    public:
        InputHandler();
        bool Polling();

};

#endif // __INPUT_HANDLER_HPP__
