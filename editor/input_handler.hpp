#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__

#include <list>
#include <tuple>

#include "command.hpp"


typedef typename std::list<SDL_Keycode> KeyCodesList_t;
typedef typename std::tuple<Command*, KeyCodesList_t::iterator> KeyParseResult_t;

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
        KeyParseResult_t ParsingKeysCommon(KeyCodesList_t::iterator &iterator); // in this function making parsing of common to use keys
        KeyParseResult_t ParsingKeysEditor(KeyCodesList_t::iterator &iterator); // in this function making parsing of commands in editor mode
        KeyParseResult_t ParsingKeysInsert(KeyCodesList_t::iterator &iterator); // in this function making parsing commands in insert mode

    public:
        InputHandler();

        Command* ProcessInput(InputModes mode);    // function to call parsing operation
        void KeyPressed(SDL_KeyboardEvent event);   // SDL_KEYDOWN
        void KeyReleased(SDL_KeyboardEvent event);  // SDL_KEYUP
        void AddKeyToAct(SDL_Keycode key);  // add key to act list

};

#endif // __INPUT_HANDLER_HPP__