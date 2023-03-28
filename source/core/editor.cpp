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

void Editor::OpenFile(ExecutorAccess * execA, void * data)
{
    std::string filename;

    filename = *reinterpret_cast<std::string*>(data);
    auto buffer = execA->bufferHandler->CreateBuffer(filename);
    if(nullptr != buffer)
    {
        execA->gui->CreateWidgetEditor(buffer);
    }
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

void Editor::ChangeEditorModeToNormal(ExecutorAccess * execA, void * data)
{
    _editorState = EditorState::NormalState;
    execA->gui->SetEditorState(EditorState::NormalState);
}

void Editor::ChangeEditorModeToInsert(ExecutorAccess * execA, void * data)
{
    _editorState = EditorState::InsertState;
    execA->gui->SetEditorState(EditorState::InsertState);
}

void Editor::Init()
{
    Executor * exec = Executor::Instance();

    exec->AddExecutorElement(Editor::OpenFile, ExecutorOpCode::OpenFileEditorWidget, std::vector<KeyMap>(0), std::vector<EditorState>(1, EditorState::NormalState), "new_file", "foo");
    exec->AddExecutorElement(Editor::InsertText, ExecutorOpCode::TextInsert, std::vector<KeyMap>(0), std::vector<EditorState>(1, EditorState::InsertState), "insert_text", "foo");
    exec->AddExecutorElement(Editor::InsertNewLine, ExecutorOpCode::TextInsertNewLine, std::vector<KeyMap>(1, {KeyMap::KeyEnter}), std::vector<EditorState>(1, EditorState::InsertState), "insert_new_line", "foo");
    exec->AddExecutorElement(Editor::DeleteBeforeCursor, ExecutorOpCode::DeleteBeforeCursor, std::vector<KeyMap>(1, {KeyMap::KeyBackspace}), std::vector<EditorState>(1, EditorState::InsertState), "delete_before_cursor", "foo");
    exec->AddExecutorElement(Editor::DeleteAfterCursor, ExecutorOpCode::DeleteAfterCursor, std::vector<KeyMap>(1, {KeyMap::KeyDelete}), std::vector<EditorState>(1, EditorState::InsertState), "delete_after_cursor", "foo");

    // Moving cursor by arrows
    exec->AddExecutorElement(Editor::MoveCursorStepUp, ExecutorOpCode::MoveCursorUp, std::vector<KeyMap>(1, {KeyMap::KeyUp}), std::vector<EditorState>{EditorState::NormalState, EditorState::InsertState}, "move_cursor_up", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepDown, ExecutorOpCode::MoveCursorDown, std::vector<KeyMap>(1, {KeyMap::KeyDown}), std::vector<EditorState>{EditorState::NormalState, EditorState::InsertState}, "move_cursor_down", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepLeft, ExecutorOpCode::MoveCursorLeft, std::vector<KeyMap>(1, {KeyMap::KeyLeft}), std::vector<EditorState>{EditorState::NormalState, EditorState::InsertState}, "move_cursor_left", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepRight, ExecutorOpCode::MoveCursorRight, std::vector<KeyMap>(1, {KeyMap::KeyRight}), std::vector<EditorState>{EditorState::NormalState, EditorState::InsertState}, "move_cursor_right", "foo");
    // Moving cursor in normal modes by text keys
    exec->AddExecutorElement(Editor::MoveCursorStepUp, ExecutorOpCode::MoveCursorUp, std::vector<KeyMap>(1, {KeyMap::KeyP}), std::vector<EditorState>{EditorState::NormalState}, "move_cursor_up", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepDown, ExecutorOpCode::MoveCursorDown, std::vector<KeyMap>(1, {KeyMap::KeySemicolon}), std::vector<EditorState>{EditorState::NormalState}, "move_cursor_down", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepLeft, ExecutorOpCode::MoveCursorLeft, std::vector<KeyMap>(1, {KeyMap::KeyL}), std::vector<EditorState>{EditorState::NormalState}, "move_cursor_left", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepRight, ExecutorOpCode::MoveCursorRight, std::vector<KeyMap>(1, {KeyMap::KeyQuotes}), std::vector<EditorState>{EditorState::NormalState}, "move_cursor_right", "foo");

    exec->AddExecutorElement(Editor::Exit, ExecutorOpCode::ExitApp, std::vector<KeyMap>(1, {KeyMap::KeyExit}), std::vector<EditorState>(1, EditorState::EditorStateMax), "exit", "foo");
    exec->AddExecutorElement(Editor::GuiResize, ExecutorOpCode::GuiResize, std::vector<KeyMap>(1, {KeyMap::KeyResize}), std::vector<EditorState>(1, EditorState::EditorStateMax), "resize_app", "foo");

    // Change modes
    exec->AddExecutorElement(Editor::ChangeEditorModeToNormal, ExecutorOpCode::ChangeEditorModeToNormal, std::vector<KeyMap>(1, {KeyMap::KeyEsc}), std::vector<EditorState>(1, EditorState::InsertState), "set_mode_normal", "foo");
    exec->AddExecutorElement(Editor::ChangeEditorModeToInsert, ExecutorOpCode::ChangeEditorModeToNormal, std::vector<KeyMap>(1, {KeyMap::KeyI}), std::vector<EditorState>(1, EditorState::NormalState), "set_mode_insert", "foo");
}
