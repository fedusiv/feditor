#ifndef __KEYMAP_HPP__
#define __KEYMAP_HPP__

#include <vector>

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

    KeyMouseL,
    KeyMouseR,
    KeyMouseM,

    KeyP,
    KeyL,
    KeyI,
    KeysMax
};

typedef std::vector<KeyMap> KeysMapList;
typedef std::vector<int> KeysInsertedText;

#endif // __KEYMAP_HPP__
