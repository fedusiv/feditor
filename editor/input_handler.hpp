#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__

#include <list>
#include <tuple>

#include "command.hpp"

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

        std::list<KeyEvent> * _kEvents; // container to hold information about pressed keys
        std::list<SDL_Keycode> * _keysToAct; // list of keys, which should be parsed
        std::tuple<Command*, std::list<SDL_Keycode>::iterator> ParsingKeys(std::list<SDL_Keycode>::iterator &iterator); // in this function making parsing

    public:
        InputHandler();

        Command* ProcessInput();    // function to call parsing operation
        void KeyPressed(SDL_KeyboardEvent event);   // SDL_KEYDOWN
        void KeyReleased(SDL_KeyboardEvent event);  // SDL_KEYUP

};

#endif // __INPUT_HANDLER_HPP__