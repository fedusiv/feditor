#ifndef __KEY_MAP_HPP__
#define __KEY_MAP_HPP__

#include <string>
#include <map>
#include <vector>
#include "SDL.h"

enum KeyMap
{
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyUpA,
    KeyDownA,
    KeyLeftA,
    KeyRightA,
    KeyEnter,
    KeyText,
    KeysMax
};

typedef std::vector<KeyMap> KeyMapVector;

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
        KeyMap GetLogicKey(SDL_Keycode keyCode);


    private:
        KeyMapOperation();

        static KeyMapOperation * _keyMapOperation;
        std::map<SDL_Keycode, KeyMap> _keyMap;  // This map to convert from hardware part of SDL to logic keymap
};


#endif // __KEY_MAP_HPP__
