#include "editor.hpp"
#include "buffer_handler.hpp"
#include "editor_state.hpp"
#include "executor.hpp"
#include "executor_description.hpp"
#include "executor_trie.hpp"
#include "executoroc.hpp"
#include "keymap.hpp"
#include <iostream>
#include <memory>
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
    Editor::TextHasEdited(execA);
}

void Editor::TextHasEdited(ExecutorAccess * execA)
{
    int completeVarants = 5; // TODO: move it to settings parameter
    int currentVariantsSize;
    BufferHandler* handler;

    if(_editorState == EditorState::CmdState){
        // need to update autocompletion variants
        ExecutorCompleteVariants variants;
        execA->eTrie->Search(execA->bufferHandler->GetLine(0),variants);
        handler = execA->bufferHandler;
        currentVariantsSize = variants.size();
        // Lines of autocompletion begins from 1 index. That's why i starts from 1.
        for(int i = 1; i <= completeVarants; i++){
            handler->DeleteLine(i);
            if(i <= currentVariantsSize){
                handler->SetLine(i, std::get<0>(variants.at(i-1)));
            }
        }
    }
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
    Editor::TextHasEdited(execA);
}

void Editor::DeleteAfterCursor(ExecutorAccess * execA, void * data)
{
    execA->bufferHandler->DeleteAtCursor(DeleteOperations::AfterCursor);
    Editor::TextHasEdited(execA);
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
    execA->bufferHandler->RemoveFloatBuffer();
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
    using namespace ExecutorsDescriptions;

    // It's a bulcky function because of this storage variable.
    // There is option where to keep or delcared it.
    // For now I decided to keep it in the stack of function call.
    // This storage is not need after this function, because it used for simplicitly the code reading, it much more clearly to edit and see executors elements  
    ExecutorElementStorage storage[]={
        // Inserting, editing text
        {
            Editor::InsertText,
            ExecutorOpCode::TextInsert,
            std::vector<KeyMap>(0),
            std::vector<EditorState>(1, EditorState::EditorStateMax),
            "insert_text",
            &Descriptions[static_cast<int>(ExecutorOpCode::TextInsert)],
        },
        {
            Editor::InsertNewLine,
            ExecutorOpCode::TextInsertNewLine,
            std::vector<KeyMap>(1, {KeyMap::KeyEnter}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "insert_new_line",
            &(Descriptions[static_cast<int>(ExecutorOpCode::TextInsertNewLine)]),
        },
        {
            Editor::DeleteBeforeCursor,
            ExecutorOpCode::DeleteBeforeCursor,
            std::vector<KeyMap>(1, {KeyMap::KeyBackspace}),
            std::vector<EditorState>({EditorState::InsertState,
            EditorState::CmdState}),
            "delete_before_cursor",
            &(Descriptions[static_cast<int>(ExecutorOpCode::DeleteBeforeCursor)]),
        },
        {
            Editor::DeleteAfterCursor,
            ExecutorOpCode::DeleteAfterCursor,
            std::vector<KeyMap>(1, {KeyMap::KeyDelete}),
            std::vector<EditorState>({EditorState::InsertState, EditorState::CmdState}),
            "delete_after_cursor",
            &(Descriptions[static_cast<int>(ExecutorOpCode::DeleteAfterCursor)]),
        },
        // Moving cursor by arrows
        {
            Editor::MoveCursorStepUp,
            ExecutorOpCode::MoveCursorUp,
            std::vector<KeyMap>(1, {KeyMap::KeyUp}),
            std::vector<EditorState>{EditorState::EditorStateMax},
            "move_cursor_up",
            &(Descriptions[static_cast<int>(ExecutorOpCode::MoveCursorUp)]),
        },
        {
            Editor::MoveCursorStepDown,
            ExecutorOpCode::MoveCursorDown,
            std::vector<KeyMap>(1, {KeyMap::KeyDown}),
            std::vector<EditorState>{EditorState::EditorStateMax},
            "move_cursor_down",
            &(Descriptions[static_cast<int>(ExecutorOpCode::MoveCursorDown)]),
        },
        {
            Editor::MoveCursorStepLeft,
            ExecutorOpCode::MoveCursorLeft,
            std::vector<KeyMap>(1, {KeyMap::KeyLeft}),
            std::vector<EditorState>{EditorState::EditorStateMax},
            "move_cursor_left",
            &(Descriptions[static_cast<int>(ExecutorOpCode::MoveCursorLeft)]),
        },
        {
            Editor::MoveCursorStepRight,
            ExecutorOpCode::MoveCursorRight,
            std::vector<KeyMap>(1, {KeyMap::KeyRight}),
            std::vector<EditorState>{EditorState::EditorStateMax},
            "move_cursor_right",
            &(Descriptions[static_cast<int>(ExecutorOpCode::MoveCursorRight)]),
        },
        // Move cursor by mouse
        {
            Editor::MoveCursorTo,
            ExecutorOpCode::MoveCursorTo,
            std::vector<KeyMap>(1, {KeyMap::KeyMouseL}),
            std::vector<EditorState>{EditorState::EditorStateMax},
            "move_cursor_to",
            &(Descriptions[static_cast<int>(ExecutorOpCode::MoveCursorTo)]),
        },
        // Scroll widget
        {
             Editor::ScrollUp,
             ExecutorOpCode::ScrollUp,
             std::vector<KeyMap>(1, {KeyMap::KeyWheelUp}),
             std::vector<EditorState>{EditorState::EditorStateMax},
             "scroll_up",
             &(Descriptions[static_cast<int>(ExecutorOpCode::ScrollUp)]),
        },
        {
             Editor::ScrollDown,
             ExecutorOpCode::ScrollDown,
             std::vector<KeyMap>(1, {KeyMap::KeyWheelDown}),
             std::vector<EditorState>{EditorState::EditorStateMax},
             "scroll_down",
             &(Descriptions[static_cast<int>(ExecutorOpCode::ScrollDown)]),
        },
        {
             Editor::ScrollLeft,
             ExecutorOpCode::ScrollLeft,
             std::vector<KeyMap>(1, {KeyMap::KeyWheelLeft}),
             std::vector<EditorState>{EditorState::EditorStateMax},
             "scroll_left",
             &(Descriptions[static_cast<int>(ExecutorOpCode::ScrollLeft)]),
        },
        {
             Editor::ScrollRight,
             ExecutorOpCode::ScrollRight,
             std::vector<KeyMap>(1, {KeyMap::KeyWheelRight}),
             std::vector<EditorState>{EditorState::EditorStateMax},
             "scroll_right",
             &(Descriptions[static_cast<int>(ExecutorOpCode::ScrollRight)]),
        },
        // App control
        {
            Editor::Exit,
            ExecutorOpCode::ExitApp,
            std::vector<KeyMap>(1, {KeyMap::KeyExit}),
            std::vector<EditorState>(1, EditorState::EditorStateMax),
            "exit",
            &(Descriptions[static_cast<int>(ExecutorOpCode::ExitApp)]),
        },
        {
            Editor::GuiResize,
            ExecutorOpCode::GuiResize,
            std::vector<KeyMap>(1, {KeyMap::KeyResize}),
            std::vector<EditorState>(1, EditorState::EditorStateMax),
            "resize_app",
            &(Descriptions[static_cast<int>(ExecutorOpCode::GuiResize)]),
        },
        // Change modes
        {
            Editor::ChangeEditorModeToInsert,
            ExecutorOpCode::ChangeEditorModeToInsert,
            std::vector<KeyMap>(1, {KeyMap::KeyEsc}),
            std::vector<EditorState>(1, EditorState::EditorStateMax),
            "set_mode_insert",
            &(Descriptions[static_cast<int>(ExecutorOpCode::ChangeEditorModeToInsert)]),
        },
        {
            Editor::ChangeEditorModeToCmd,
            ExecutorOpCode::ChangeEditorModeToCmd,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyX}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "set_mode_cmd",
            &(Descriptions[static_cast<int>(ExecutorOpCode::ChangeEditorModeToCmd)]),
         },
    };
    // This function will add all executors to executors element list
    for(auto c: storage){
        exec->AddExecutorElement(&c);
    }

}
