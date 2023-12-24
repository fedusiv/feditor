#include "file_manager.hpp"
#include "editor_state.hpp"
#include <executor_description.hpp>
#include "executor.hpp"
#include "keymap.hpp"
#include <iostream>
#include <vector>

void FileManager::CreateBuffer(ExecutorAccess * execA, void * data)
{
    ExecDataTypeCreateBuffer * d;
    std::string filename;
    bool verticalDirection;

    verticalDirection = true;   // by default create vertical split
    d = reinterpret_cast<ExecDataTypeCreateBuffer*>(data);
    if(nullptr == data)
    {
        filename = "";
    }else{
        filename = d->filename;
        verticalDirection = d->verticalDirection;
    }

    auto buffer = execA->bufferHandler->CreateBuffer(filename);
    if(nullptr != buffer)
    {
        execA->gui->AttachWidgetEditor(buffer, verticalDirection);
    }
}

void FileManager::CreateBufferHorizontal(ExecutorAccess * execA, void * data)
{
    ExecDataTypeCreateBuffer  d;
    if(nullptr == data)
    {
        d.filename = ""; // empty buffer. no name
        d.verticalDirection = false; // this is horizontal split
        FileManager::CreateBuffer(execA, &d);
    }else{
        FileManager::CreateBuffer(execA, data);
    }
}

void FileManager::SwitchBetweenEditorsInTabUp(ExecutorAccess *execA, void *data)
{
    FileManager::SwitchBetweenEditorsInTab(execA, MoveCursorDirection::CursorUp);
}
void FileManager::SwitchBetweenEditorsInTabDown(ExecutorAccess *execA, void *data)
{
    FileManager::SwitchBetweenEditorsInTab(execA, MoveCursorDirection::CursorDown);
}
void FileManager::SwitchBetweenEditorsInTabLeft(ExecutorAccess *execA, void *data)
{
    FileManager::SwitchBetweenEditorsInTab(execA, MoveCursorDirection::CursorLeft);
}
void FileManager::SwitchBetweenEditorsInTabRight(ExecutorAccess *execA, void *data)
{
    FileManager::SwitchBetweenEditorsInTab(execA, MoveCursorDirection::CursorRight);
}

void FileManager::SwitchBetweenEditorsInTab(ExecutorAccess *execA, MoveCursorDirection direction)
{
    // Verify if switch of buffer is possible.
    if(execA->gui->SwitchBuffer(direction)) // SwitchBuffer function is related to Gui module. Gui module will update all required filed by itself
    {
        // switch happened
        execA->bufferHandler->UpdateActiveBuffer();
    }
}

void FileManager::CreateNewTab(ExecutorAccess *execA, void *data)
{
    execA->bufferHandler->CreateNewTab();
    execA->gui->AttachTab();    // attach new tab and swtich to it
}

void FileManager::SwitchToTab(ExecutorAccess *execA, void *data)
{
    int newTabId;

    newTabId = *static_cast<int*>(data);  
    execA->bufferHandler->SwitchToTab(newTabId); // change current active tab for buffer handler
    execA->gui->SwitchTab(newTabId);
}


void FileManager::Init()
{
    Executor * exec = Executor::Instance();
    using namespace ExecutorsDescriptions;

        ExecutorElementStorage storage[]={
        {
            FileManager::CreateBuffer,
            ExecutorOpCode::CreateBuffer,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyV}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "new_file",
            &(Descriptions[static_cast<int>(ExecutorOpCode::CreateBuffer)]),
        },
        {
            FileManager::CreateBufferHorizontal,
            ExecutorOpCode::CreateBufferHorizontal,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyH}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "new_file_horizontal",
            &(Descriptions[static_cast<int>(ExecutorOpCode::CreateBufferHorizontal)]),
        },
        //Switch widget in tab widget
        {
            FileManager::SwitchBetweenEditorsInTabUp,
            ExecutorOpCode::SwitchBetweenEditorsInTabUp,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyUp}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "sw_editor_up",
            &(Descriptions[static_cast<int>(ExecutorOpCode::SwitchBetweenEditorsInTabUp)]),
        },
        {
            FileManager::SwitchBetweenEditorsInTabUp,
            ExecutorOpCode::SwitchBetweenEditorsInTabUp,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyUp}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "sw_editor_up",
            &(Descriptions[static_cast<int>(ExecutorOpCode::SwitchBetweenEditorsInTabUp)]),
        },
        {
            FileManager::SwitchBetweenEditorsInTabDown,
            ExecutorOpCode::SwitchBetweenEditorsInTabDown,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyDown}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "sw_editor_down",
            &(Descriptions[static_cast<int>(ExecutorOpCode::SwitchBetweenEditorsInTabDown)]),
        },
        {
            FileManager::SwitchBetweenEditorsInTabLeft,
            ExecutorOpCode::SwitchBetweenEditorsInTabLeft,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyLeft}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "sw_editor_left",
            &(Descriptions[static_cast<int>(ExecutorOpCode::SwitchBetweenEditorsInTabLeft)]),
        },
        {
            FileManager::SwitchBetweenEditorsInTabRight,
            ExecutorOpCode::SwitchBetweenEditorsInTabRight,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyRight}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "sw_editor_left",
            &(Descriptions[static_cast<int>(ExecutorOpCode::SwitchBetweenEditorsInTabRight)]),
        },
        // Create new tab
        {
            FileManager::CreateNewTab,
            ExecutorOpCode::CreateNewTab,
            std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyT}),
            std::vector<EditorState>(1, EditorState::InsertState),
            "create_new_tab",
            &(Descriptions[static_cast<int>(ExecutorOpCode::CreateNewTab)]),
        },
        // Swtich to tab
        {
            FileManager::SwitchToTab,
            ExecutorOpCode::SwitchToTab,
            std::vector<KeyMap>(),
            std::vector<EditorState>(1, EditorState::InsertState),
            "switch_to_tab",
            &(Descriptions[static_cast<int>(ExecutorOpCode::SwitchToTab)]),
        },
    };
    
    // This function will add all executors to executors element list
    for(auto e: storage){
        exec->AddExecutorElement(&e);
    }
}
