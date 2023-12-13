#include "editor.hpp"
#include "editor_state.hpp"
#include "executor.hpp"
#include "keymap.hpp"
#include <iostream>
#include <vector>

EditorState Editor::_editorState;

EditorState Editor::GetEditorState(void)
{
    return _editorState;
}

void Editor::InsertText(ExecutorAccess * execA, void * data)
{
    KeysInsertedText text;
    text = *((KeysInsertedText*)(data));
    execA->bufferHandler->AppendToActiveBuffer(text);
}

void Editor::InsertNewLine(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->InsertNewLine();
}

void Editor::MoveCursorStepUp(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->MoveCursor(MoveCursorDirection::CursorUp);
}

void Editor::MoveCursorStepDown(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->MoveCursor(MoveCursorDirection::CursorDown);
}

void Editor::MoveCursorStepLeft(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->MoveCursor(MoveCursorDirection::CursorLeft);
}

void Editor::MoveCursorStepRight(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->MoveCursor(MoveCursorDirection::CursorRight);
}

void Editor::DeleteBeforeCursor(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->DeleteAtCursor(DeleteOperations::BeforeCursor);
}

void Editor::DeleteAfterCursor(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->DeleteAtCursor(DeleteOperations::AfterCursor);
}

void Editor::Exit(ExecutorAccess * execA, void * data)
{
    execA->gui->RequestExit();
}

void Editor::GuiResize(ExecutorAccess * execA, void * data)
{
    execA->gui->Resize();
}

void Editor::ChangeEditorModeToCmd(ExecutorAccess * execA, void * data)
{
    _editorState = EditorState::CmdState;
    execA->gui->SetEditorState(EditorState::CmdState);
    auto buffer = execA->bufferHandler->InstantiateFloatBuffer();
    execA->gui->AttachFloatBuffer(buffer);
}

void Editor::ChangeEditorModeToInsert(ExecutorAccess * execA, void * data)
{
    _editorState = EditorState::InsertState;
    execA->gui->SetEditorState(EditorState::InsertState);
}

void Editor::MoveCursorTo(ExecutorAccess *execA, void *data)
{
    execA->gui->AlignCursorPositionByMouse();
    execA->bufferHandler->UpdateActiveBuffer(); // if mouse was pressed on not active buffer, need to check if appeared request to change buffer
}

void Editor::ScrollUp(ExecutorAccess * execA, void * data)
{
    execA->gui->PageScrolling(Vec2(0,1));
}

void Editor::ScrollDown(ExecutorAccess * execA, void * data)
{
    execA->gui->PageScrolling(Vec2(0,-1));
}

void Editor::ScrollLeft(ExecutorAccess * execA, void * data)
{
    execA->gui->PageScrolling(Vec2(-1,0));
}

void Editor::ScrollRight(ExecutorAccess * execA, void * data)
{
    execA->gui->PageScrolling(Vec2(1,0));
}



void Editor::Init()
{
    Executor * exec = Executor::Instance();
    // Inserting, editirng text
    exec->AddExecutorElement(Editor::InsertText, ExecutorOpCode::TextInsert, std::vector<KeyMap>(0), std::vector<EditorState>(1, EditorState::EditorStateMax), "insert_text", "foo");
    exec->AddExecutorElement(Editor::InsertNewLine, ExecutorOpCode::TextInsertNewLine, std::vector<KeyMap>(1, {KeyMap::KeyEnter}), std::vector<EditorState>(1, EditorState::InsertState), "insert_new_line", "foo");
    exec->AddExecutorElement(Editor::DeleteBeforeCursor, ExecutorOpCode::DeleteBeforeCursor, std::vector<KeyMap>(1, {KeyMap::KeyBackspace}), std::vector<EditorState>({EditorState::InsertState, EditorState::CmdState}), "delete_before_cursor", "foo");
    exec->AddExecutorElement(Editor::DeleteAfterCursor, ExecutorOpCode::DeleteAfterCursor, std::vector<KeyMap>(1, {KeyMap::KeyDelete}), std::vector<EditorState>({EditorState::InsertState, EditorState::CmdState}), "delete_after_cursor", "foo");

    // Moving cursor by arrows
    exec->AddExecutorElement(Editor::MoveCursorStepUp, ExecutorOpCode::MoveCursorUp, std::vector<KeyMap>(1, {KeyMap::KeyUp}), std::vector<EditorState>{EditorState::EditorStateMax}, "move_cursor_up", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepDown, ExecutorOpCode::MoveCursorDown, std::vector<KeyMap>(1, {KeyMap::KeyDown}), std::vector<EditorState>{EditorState::EditorStateMax}, "move_cursor_down", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepLeft, ExecutorOpCode::MoveCursorLeft, std::vector<KeyMap>(1, {KeyMap::KeyLeft}), std::vector<EditorState>{EditorState::EditorStateMax}, "move_cursor_left", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepRight, ExecutorOpCode::MoveCursorRight, std::vector<KeyMap>(1, {KeyMap::KeyRight}), std::vector<EditorState>{EditorState::EditorStateMax}, "move_cursor_right", "foo");
    // Move cursor by mouse
    exec->AddExecutorElement(Editor::MoveCursorTo, ExecutorOpCode::MoveCursorTo, std::vector<KeyMap>(1, {KeyMap::KeyMouseL}), std::vector<EditorState>{EditorState::EditorStateMax}, "move_cursor_to", "foo");
    // Scroll widget
    exec->AddExecutorElement(Editor::ScrollUp, ExecutorOpCode::ScrollUp, std::vector<KeyMap>(1, {KeyMap::KeyWheelUp}), std::vector<EditorState>{EditorState::EditorStateMax}, "scroll_up", "foo");
    exec->AddExecutorElement(Editor::ScrollDown, ExecutorOpCode::ScrollDown, std::vector<KeyMap>(1, {KeyMap::KeyWheelDown}), std::vector<EditorState>{EditorState::EditorStateMax}, "scroll_down", "foo");
    exec->AddExecutorElement(Editor::ScrollLeft, ExecutorOpCode::ScrollLeft, std::vector<KeyMap>(1, {KeyMap::KeyWheelLeft}), std::vector<EditorState>{EditorState::EditorStateMax}, "scroll_left", "foo");
    exec->AddExecutorElement(Editor::ScrollRight, ExecutorOpCode::ScrollRight, std::vector<KeyMap>(1, {KeyMap::KeyWheelRight}), std::vector<EditorState>{EditorState::EditorStateMax}, "scroll_right", "foo");

    exec->AddExecutorElement(Editor::Exit, ExecutorOpCode::ExitApp, std::vector<KeyMap>(1, {KeyMap::KeyExit}), std::vector<EditorState>(1, EditorState::EditorStateMax), "exit", "foo");
    exec->AddExecutorElement(Editor::GuiResize, ExecutorOpCode::GuiResize, std::vector<KeyMap>(1, {KeyMap::KeyResize}), std::vector<EditorState>(1, EditorState::EditorStateMax), "resize_app", "foo");

    // Change modes
    exec->AddExecutorElement(Editor::ChangeEditorModeToInsert, ExecutorOpCode::ChangeEditorModeToInsert, std::vector<KeyMap>(1, {KeyMap::KeyEsc}), std::vector<EditorState>(1, EditorState::EditorStateMax), "set_mode_insert", "foo");
    exec->AddExecutorElement(Editor::ChangeEditorModeToCmd, ExecutorOpCode::ChangeEditorModeToCmd, std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyX}), std::vector<EditorState>(1, EditorState::InsertState), "set_mode_cmd", "foo");
}
