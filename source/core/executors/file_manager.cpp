#include "file_manager.hpp"
#include "editor_state.hpp"
#include "executor.hpp"
#include "keymap.hpp"
#include <iostream>
#include <vector>

void FileManager::CreateBuffer(ExecutorAccess * execA, void * data)
{
    std::string filename;

    if(nullptr == data)
    {
        filename = "";
    }else{
        filename = *reinterpret_cast<std::string*>(data);
    }

    auto buffer = execA->bufferHandler->CreateBuffer(filename);
    if(nullptr != buffer)
    {
        execA->gui->AttachWidgetEditor(buffer);
    }
}


void FileManager::Init()
{
    Executor * exec = Executor::Instance();

    exec->AddExecutorElement(FileManager::CreateBuffer, ExecutorOpCode::CreateBuffer, std::vector<KeyMap>({KeyMap::KeyCtrl, KeyMap::KeyN}), std::vector<EditorState>(1, EditorState::InsertState), "new_file", "foo");
}
