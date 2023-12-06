#ifndef __KEYMAP_HPP__
#define __KEYMAP_HPP__

#include <vector>

// To add new key you need to add new key enum name to KeyMap enum and add new sdl to enum conversion to keys.hpp

enum KeyMap
{
    KeyZero,
    KeyExit,    // pressed exit key, or basically gui related api detected exit call (close window button for example)
    KeyResize,  // gui event, that window was resized
    KeyUp,
    KeyDown,
    KeyLeft,
    KeyRight,
    KeyEnter,
    KeyText,
    KeyBackspace,
    KeyDelete,
    KeyEsc,
    KeySemicolon,
    KeyQuotes,
    KeyCtrl,
    KeyAlt,

    KeyMouseL,
    KeyMouseR,
    KeyMouseM,
    KeyWheelUp,
    KeyWheelDown,
    KeyWheelLeft,
    KeyWheelRight,

    KeyP,
    KeyL,
    KeyI,
    KeyN,
    KeyH,
    KeyV,
    KeyT,
    KeysMax
};

typedef std::vector<KeyMap> KeysMapList;
typedef std::vector<int> KeysInsertedText;

#endif // __KEYMAP_HPP__
