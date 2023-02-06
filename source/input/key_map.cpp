#include "key_map.hpp"

KeyMapOperation * KeyMapOperation::_keyMapOperation;

void KeyMapOperation::ReadMap(std::string configPath)
{

}

KeyMap KeyMapOperation::GetLogicKey(SDL_KeyCode keyCode)
{
    return _keyMap.at(keyCode);
}

KeyMapOperation::KeyMapOperation()
{
    // Load default parameters
    _keyMap.insert({SDLK_UP, KeyMap::KeyUp});
    _keyMap.insert({SDLK_DOWN, KeyMap::KeyDown});
    _keyMap.insert({SDLK_LEFT, KeyMap::KeyLeft});
    _keyMap.insert({SDLK_RIGHT, KeyMap::KeyRight});
    _keyMap.insert({SDLK_p, KeyMap::KeyUp});
    _keyMap.insert({SDLK_SEMICOLON, KeyMap::KeyDown});
    _keyMap.insert({SDLK_l, KeyMap::KeyLeft});
    _keyMap.insert({SDLK_QUOTEDBL, KeyMap::KeyRight});

}
