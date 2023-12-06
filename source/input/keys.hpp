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
                case SDL_SCANCODE_UP:        {keyMap = KeyMap::KeyUp; break;}
                case SDL_SCANCODE_DOWN:      {keyMap = KeyMap::KeyDown; break;}
                case SDL_SCANCODE_LEFT:      {keyMap = KeyMap::KeyLeft; break;}
                case SDL_SCANCODE_RIGHT:     {keyMap = KeyMap::KeyRight; break;}
                case SDL_SCANCODE_RETURN:    {keyMap = KeyMap::KeyEnter; break;}
                case SDL_SCANCODE_BACKSPACE: {keyMap = KeyMap::KeyBackspace; break;}
                case SDL_SCANCODE_DELETE:    {keyMap = KeyMap::KeyDelete; break;}
                case SDL_SCANCODE_ESCAPE:    {keyMap = KeyMap::KeyEsc; break;}
                case SDL_SCANCODE_SEMICOLON: {keyMap = KeyMap::KeySemicolon; break;}
                case SDL_SCANCODE_APOSTROPHE:     {keyMap = KeyMap::KeyQuotes; break;}
                case SDL_SCANCODE_LCTRL:      {keyMap = KeyMap::KeyCtrl; break;}
                case SDL_SCANCODE_LALT:      {keyMap = KeyMap::KeyAlt; break;}

                case SDL_SCANCODE_L:         {keyMap = KeyMap::KeyL; break;}
                case SDL_SCANCODE_P:         {keyMap = KeyMap::KeyP; break;}
                case SDL_SCANCODE_I:         {keyMap = KeyMap::KeyI; break;}
                case SDL_SCANCODE_N:         {keyMap = KeyMap::KeyN; break;}
                case SDL_SCANCODE_H:         {keyMap = KeyMap::KeyH; break;}
                case SDL_SCANCODE_V:         {keyMap = KeyMap::KeyV; break;}
                case SDL_SCANCODE_T:         {keyMap = KeyMap::KeyT; break;}

                default:
                    keyMap = KeyMap::KeyZero;
                    break;
            }
            return keyMap;
        }


};

typedef std::list<KeyAction*> KeysActionList;

#endif // __KEYS_HPP__
