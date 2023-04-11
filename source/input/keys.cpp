#include "keys.hpp"

KeyMap KeyAction::ConvertMouseWheel(Vec2 direction)
{
    // first priority on y changes. it's height
    if(0 > direction.y)
    {
        return KeyWheelDown;
    }
    if(0 < direction.y)
    {
        return KeyWheelUp;
    }

    if(0 > direction.x)
    {
        return KeyWheelRight;
    }
    if(0 < direction.x)
    {
        return KeyWheelLeft;
    }

    return KeyZero;
}

KeyMap KeyAction::ConvertMouseButtons(int sdlButton)
{
    KeyMap keyMap;
    switch(sdlButton){
        case SDL_BUTTON_LEFT:   {keyMap = KeyMap::KeyMouseL; break;}
        case SDL_BUTTON_RIGHT:   {keyMap = KeyMap::KeyMouseR; break;}
        case SDL_BUTTON_MIDDLE:   {keyMap = KeyMap::KeyMouseM; break;}
        default:
                keyMap = KeyMap::KeyZero;
                break;
    }

    return keyMap;
}


