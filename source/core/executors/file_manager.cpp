#include "file_manager.hpp"
#include "editor_state.hpp"
#include "executor.hpp"
#include "keymap.hpp"
#include <iostream>
#include <vector>

void FileManager::CreateBuffer(ExecutorAccess * execA, void * data)
{
    ExecDataTypeCreateBuffer * d;
    std::string filename;
    bool verticalDirection;

    verticalDirection = true;   // by default vreate vertical split
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



void FileManager::Init()
{
    Executor * exec = Executor::Instance();

    exec->AddExecutorElement(FileManager::CreateBuffer, ExecutorOpCode::CreateBuffer, std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyV}), std::vector<EditorState>(1, EditorState::InsertState), "new_file", "foo");
    exec->AddExecutorElement(FileManager::CreateBufferHorizontal, ExecutorOpCode::CreateBufferHorizontal, std::vector<KeyMap>({KeyMap::KeyAlt, KeyMap::KeyH}), std::vector<EditorState>(1, EditorState::InsertState), "new_file_horizontal", "foo");
}
