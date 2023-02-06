#ifndef __KEY_MAP_HPP__
#define __KEY_MAP_HPP__

#include <string>
#include <map>
#include "SDL.h"

enum KeyMap
{
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeysMax
};

class KeyMapOperation
{
    public:
        static KeyMapOperation * Instance()
        {
            if(_keyMapOperation == nullptr)
            {
                _keyMapOperation = new KeyMapOperation();
            }
            return _keyMapOperation;
        }
        void ReadMap(std::string configPath);
        KeyMap GetLogicKey(SDL_KeyCode keyCode);


    private:
        KeyMapOperation();

        static KeyMapOperation * _keyMapOperation;
        std::map<SDL_KeyCode, KeyMap> _keyMap;  // This map to convert from hardware part of SDL to logic keymap
};


#endif // __KEY_MAP_HPP__