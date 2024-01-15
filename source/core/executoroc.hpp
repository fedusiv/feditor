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

struct ExecDataTypeCreateBuffer_t
{
    FString filename;
    bool verticalDirection;
};
using ExecDataTypeCreateBuffer = ExecDataTypeCreateBuffer_t;

#endif //__EXECUTOROC_HPP__
