#include "editor.hpp"
#include "editor_state.hpp"
#include "executor.hpp"
#include "keymap.hpp"
#include <iostream>
#include <vector>

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

void Editor::Init()
{
    Executor * exec = Executor::Instance();

    exec->AddExecutorElement(Editor::OpenFile, ExecutorOpCode::OpenFileEditorWidget, std::vector<KeyMap>(0), std::vector<EditorState>(1, EditorState::NormalState), "new_file", "foo");
    exec->AddExecutorElement(Editor::InsertText, ExecutorOpCode::TextInsert, std::vector<KeyMap>(0), std::vector<EditorState>(1, EditorState::InsertState), "insert_text", "foo");
    exec->AddExecutorElement(Editor::InsertNewLine, ExecutorOpCode::TextInsertNewLine, std::vector<KeyMap>(1, {KeyMap::KeyEnter}), std::vector<EditorState>(1, EditorState::InsertState), "insert_new_line", "foo");
    exec->AddExecutorElement(Editor::DeleteBeforeCursor, ExecutorOpCode::DeleteBeforeCursor, std::vector<KeyMap>(1, {KeyMap::KeyBackspace}), std::vector<EditorState>(1, EditorState::InsertState), "delete_before_cursor", "foo");
    exec->AddExecutorElement(Editor::DeleteAfterCursor, ExecutorOpCode::DeleteAfterCursor, std::vector<KeyMap>(1, {KeyMap::KeyDelete}), std::vector<EditorState>(1, EditorState::InsertState), "delete_after_cursor", "foo");


    exec->AddExecutorElement(Editor::MoveCursorStepUp, ExecutorOpCode::MoveCursorUp, std::vector<KeyMap>(1, {KeyMap::KeyUp}), std::vector<EditorState>(1, EditorState::InsertState), "move_cursor_up", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepDown, ExecutorOpCode::MoveCursorDown, std::vector<KeyMap>(1, {KeyMap::KeyDown}), std::vector<EditorState>(1, EditorState::InsertState), "move_cursor_down", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepLeft, ExecutorOpCode::MoveCursorLeft, std::vector<KeyMap>(1, {KeyMap::KeyLeft}), std::vector<EditorState>(1, EditorState::InsertState), "move_cursor_left", "foo");
    exec->AddExecutorElement(Editor::MoveCursorStepRight, ExecutorOpCode::MoveCursorRight, std::vector<KeyMap>(1, {KeyMap::KeyRight}), std::vector<EditorState>(1, EditorState::InsertState), "move_cursor_right", "foo");

    exec->AddExecutorElement(Editor::Exit, ExecutorOpCode::ExitApp, std::vector<KeyMap>(1, {KeyMap::KeyExit}), std::vector<EditorState>(1, EditorState::EditorStateMax), "exit", "foo");
    exec->AddExecutorElement(Editor::GuiResize, ExecutorOpCode::GuiResize, std::vector<KeyMap>(1, {KeyMap::KeyResize}), std::vector<EditorState>(1, EditorState::EditorStateMax), "resize_app", "foo");

}
