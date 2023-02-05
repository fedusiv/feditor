#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__

#include <list>
#include <tuple>


typedef typename std::list<SDL_Keycode> KeyCodesList_t;

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

        std::list<KeyEvent> * _kEvents; // container to hold information about pressed keys
        KeyCodesList_t * _keysToAct; // list of keys, which should be parsed

    public:
        InputHandler();
        bool Polling();
        void KeyPressed(SDL_KeyboardEvent event);   // SDL_KEYDOWN
        void KeyReleased(SDL_KeyboardEvent event);  // SDL_KEYUP
        void AddKeyToAct(SDL_Keycode key);  // add key to act list

};

#endif // __INPUT_HANDLER_HPP__