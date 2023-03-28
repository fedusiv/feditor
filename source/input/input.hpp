#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__


#include <list>
#include <tuple>

#include "keys.hpp"

class Input
{
    private:
        KeysActionList _keysAct;
        KeysMapList _keysMap;
        KeysInsertedText _keysText;

        void ClearOneTimeActs(void);

    public:
        Input();
        void Update(bool inputRead);
        KeysMapList KeysMap(void);
        KeysInsertedText& KeysText(void);
        void ClearKeysMap(void);
        void KeyPressed(int code);
        void KeyReleased(int code);
};

#endif // __INPUT_HANDLER_HPP__
