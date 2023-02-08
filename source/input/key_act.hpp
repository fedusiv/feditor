#ifndef __KEY_ACT_HPP__
#define __KEY_ACT_HPP__

#include "SDL.h"
#include "key_map.hpp"

class KeyAct
{
    public:
        SDL_KeyboardEvent event;
        KeyMap keyMap;
        bool coyoteTime; // for coyote time mechanism
        // Creates key Act. What action should be done for this key pressing.
        // Converts SDL kind of hardware code to logic code of editor
        KeyAct(SDL_KeyboardEvent event): event(event)
        {
            _keyOperation = KeyMapOperation::Instance();
            keyMap = _keyOperation->GetLogicKey(static_cast<SDL_Keycode>(event.keysym.sym));
            coyoteTime = false;
        }
    private:
        KeyMapOperation * _keyOperation;
};

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

#endif // __KEY_ACT_HPP__
