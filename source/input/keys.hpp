#ifndef __KEYS_HPP__
#define __KEYS_HPP__

#include <list>
#include <vector>

#include "SDL.h"
#include "vec2.hpp"
#include "SDL_keycode.h"
#include "SDL_mouse.h"
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

        static KeyMap ConvertMouseButtons(int sdlButton);
        static KeyMap ConvertMouseWheel(Vec2 direction);

    private:
        KeyMap ConvertFromKeyCode()
        {
            KeyMap keyMap;
            switch (keyCode) {
                case SDLK_UP:        {keyMap = KeyMap::KeyUp; break;}
                case SDLK_DOWN:      {keyMap = KeyMap::KeyDown; break;}
                case SDLK_LEFT:      {keyMap = KeyMap::KeyLeft; break;}
                case SDLK_RIGHT:     {keyMap = KeyMap::KeyRight; break;}
                case SDLK_RETURN:    {keyMap = KeyMap::KeyEnter; break;}
                case SDLK_BACKSPACE: {keyMap = KeyMap::KeyBackspace; break;}
                case SDLK_DELETE:    {keyMap = KeyMap::KeyDelete; break;}
                case SDLK_ESCAPE:    {keyMap = KeyMap::KeyEsc; break;}
                case SDLK_SEMICOLON: {keyMap = KeyMap::KeySemicolon; break;}
                case SDLK_QUOTE:     {keyMap = KeyMap::KeyQuotes; break;}

                case SDLK_l:         {keyMap = KeyMap::KeyL; break;}
                case SDLK_p:         {keyMap = KeyMap::KeyP; break;}
                case SDLK_i:         {keyMap = KeyMap::KeyI; break;}


                default:
                    keyMap = KeyMap::KeyZero;
                    break;
            }
            return keyMap;
        }


};

typedef std::list<KeyAction*> KeysActionList;

#endif // __KEYS_HPP__
