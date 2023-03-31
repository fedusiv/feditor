#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__


#include <list>
#include <tuple>

#include "keys.hpp"
#include "vec2.hpp"

class Input
{
    private:
        KeysActionList _keysAct;
        KeysMapList _keysMap;
        KeysInsertedText _keysText;
        Vec2 _mousePosition;

        void ClearOneTimeActs(void);

    public:
        Input();
        void Update(bool inputRead);
        KeysMapList KeysMap(void);
        KeysInsertedText& KeysText(void);
        void ClearKeysMap(void);
        void KeyPressed(int code);
        void KeyPressed(KeyMap keyMap);
        void KeyReleased(int code);
        void KeyReleased(KeyMap keyMap);
        Vec2 CurrentMousePosition(void);
};

#endif // __INPUT_HANDLER_HPP__
