#ifndef __KEYMAP_HPP__
#define __KEYMAP_HPP__

#include <vector>

enum KeyMap
{
    KeyZero,
    KeyExit,    // 
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyEnter,
    KeyText,
    KeyBackspace,
    KeyDelete,
    KeysMax
};

typedef std::vector<KeyMap> KeysMapList;
typedef std::vector<int> KeysInsertedText;

#endif // __KEYMAP_HPP__