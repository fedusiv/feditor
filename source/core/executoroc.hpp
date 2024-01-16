#ifndef __EXECUTOROC_HPP__
#define __EXECUTOROC_HPP__

#include <fstring.hpp>

// Opcode of all Executors
enum class ExecutorOpCode
{
    TextInsert,
    TextInsertNewLine,
    DeleteBeforeCursor,
    DeleteAfterCursor,
    MoveCursorUp,
    MoveCursorDown,
    MoveCursorLeft,
    MoveCursorRight,
    MoveCursorTo,
    ScrollUp,
    ScrollDown,
    ScrollLeft,
    ScrollRight,

    CreateBuffer,
    CreateBufferHorizontal,
    SwitchBetweenEditorsInTabUp,
    SwitchBetweenEditorsInTabDown,
    SwitchBetweenEditorsInTabLeft,
    SwitchBetweenEditorsInTabRight,
    CreateNewTab,
    SwitchToTab,

    ExitApp,
    GuiResize,

    ChangeEditorModeToInsert,
    ChangeEditorModeToCmd,

    ExecutorOpCodeMax
};


#endif //__EXECUTOROC_HPP__
