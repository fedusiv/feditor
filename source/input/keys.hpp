#ifndef __KEYS_HPP__
#define __KEYS_HPP__

#include <list>
#include <vector>

#include "SDL.h"
#include "keymap.hpp"

class KeyAction
{
    public:

        int keyCode;
        int acted;  // how many times this key was participated to be acted on it.
        KeyMap keyMap;
        double time;

        KeyAction(int keycode): keyCode(keycode), acted(0), time(0.0)
        {
            keyMap = ConvertFromKeyCode();
        }

        KeyAction(KeyMap keymap) : acted(0), keyMap(keymap), time(0.0)
        {

        }

    private:
        KeyMap ConvertFromKeyCode()
        {
            KeyMap keyMap;
            switch (keyCode) {
                case SDLK_UP:        {keyMap = KeyMap::KeyUp; break;}
                case SDLK_DOWN:      {keyMap = KeyMap::KeyDown; break;}
                case SDLK_LEFT:      {keyMap = KeyMap::KeyLeft; break;}
                case SDLK_RIGHT:     {keyMap = KeyMap::KeyRight; break;}
                case SDLK_RETURN:     {keyMap = KeyMap::KeyEnter; break;}
                case SDLK_BACKSPACE: {keyMap = KeyMap::KeyBackspace; break;}
                case SDLK_DELETE:    {keyMap = KeyMap::KeyDelete; break;}


                default:
                    keyMap = KeyMap::KeyZero;
                    break;
            }
            return keyMap;
        }


};

typedef std::list<KeyAction*> KeysActionList;

#endif // __KEYS_HPP__