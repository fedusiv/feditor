#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "executoraccess.hpp"
#include "editor_state.hpp"

class Editor
{
    public:
        static void Init(); // add all executor's functors to executor
        static EditorState GetEditorState(void);

        static void OpenFile(ExecutorAccess * execA, void * data); // create new empty widget with blank buffer data
        static void InsertText(ExecutorAccess * execA, void * data);    // insert text to active buffer
        static void InsertNewLine(ExecutorAccess * execA, void * data);    // insert new line
        static void DeleteBeforeCursor(ExecutorAccess * execA, void * data);    // Backspace delete, or delete before cursor
        static void DeleteAfterCursor(ExecutorAccess * execA, void * data);    // delete by pressing Delete key as default, or delete after cursor


        static void MoveCursorStepUp(ExecutorAccess * execA, void * data);      // moving cursor by one step to the up
        static void MoveCursorStepDown(ExecutorAccess * execA, void * data);    // moving cursor by one step to the down
        static void MoveCursorStepLeft(ExecutorAccess * execA, void * data);    // moving cursor by one step to the left
        static void MoveCursorStepRight(ExecutorAccess * execA, void * data);   // moving cursor by one step to the right
        static void MoveCursorTo(ExecutorAccess * execA, void * data);         // moving cursor to specific position determined by coordinate

        static void Exit(ExecutorAccess * execA, void * data);  // when user requested to exit application
        static void GuiResize(ExecutorAccess * execA, void * data);  // when user requested to resize whole application
        static void ChangeEditorModeToNormal(ExecutorAccess * execA, void * data); // when changing mode of editor
        static void ChangeEditorModeToInsert(ExecutorAccess * execA, void * data); // when changing mode of editor

    private:
        static EditorState _editorState;
};

#endif // __EDITOR_HPP__
