#ifndef __EXECUTOROC_HPP__
#define __EXECUTOROC_HPP__


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

    OpenFileEditorWidget,

    ExitApp,
    GuiResize,

    ChangeEditorModeToNormal,
    ChangeEditorModeToInsert,

    ExecutorOpCodeMax
};

#endif //__EXECUTOROC_HPP__
