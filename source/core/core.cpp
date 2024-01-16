#include <iostream>
#include <filesystem>

#include <macros.h>
#include <fstring.hpp>
#include "editor_state.hpp"
#include "executoroc.hpp"

#include "core.hpp"
#include "editor.hpp"
#include "file_manager.hpp"
#include "fsignal.hpp"

Core::Core(Gui * gui, Input * input,std::string location) : _guiHandler(gui), _inputHandler(input), _locationPoint(location)
{
    Init();
    MainLoop();
}

void Core::Init(void)
{
    FSignalInit();
    _executor = Executor::Instance();
    _bufferHandler = BufferHandler::Instance();

    _execAccess = new ExecutorAccess(_guiHandler, _bufferHandler);
    _executor->AttachAccess(_execAccess);
    
    // init and attach to executor all executor functions
    Editor::Init();
    FileManager::Init();

    _guiHandler->CreateWindow();

    _executor->CallExecutor(ExecutorOpCode::ChangeEditorModeToInsert, nullptr); // set editor to Insert Mode as default

    // it's file or directory, first need to specify
    if( std::filesystem::is_directory(_locationPoint))
    {
        // it's directory, we can not do anything
        std::cout << "Can not open directory for now. Work in progress.." << std::endl;
        return;
    }else{
        // create buffer from a file only if file is mentioned
        if(_locationPoint != ""){
            auto convertedLine = FStringVector{FString(_locationPoint)};
            _executor->CallExecutor(ExecutorOpCode::CreateBuffer, &convertedLine);
        }
    }
}

void Core::MainLoop()
{

    while(!_guiHandler->NeedExit())
    {
        SignalHandling();
        _guiHandler->Update();
        InputHandling();
        _guiHandler->UpdateMousePosition(_inputHandler->CurrentMousePosition());
    }
}

void Core::InputHandling()
{
    EditorState editorState;
    KeysInsertedText textData;
    KeysMapList keysMap;
    bool executorResult;    // did executor make his job
    bool insertText;

    editorState = Editor::GetEditorState();

    insertText = true;
    executorResult = false;
    _inputHandler->Update(insertText);

    // Get data from input handler
    textData = _inputHandler->KeysText();
    keysMap = _inputHandler->KeysMap();


    if(!keysMap.empty())    // trigger executor if only there is at least one key
    {
        // Execute keys (find related combination of keys and run executor for it)
        executorResult = _executor->CallExecutor(editorState, keysMap);
    }
    
    if(executorResult)
    {
        _inputHandler->ClearKeysMap();
    }
    else if(!textData.empty())
    {   // if string is not empty and executor was not done it will proceed text data, otherwise just ignore.
        _executor->CallExecutor(ExecutorOpCode::TextInsert, &textData);
    }

}

void Core::SignalHandling(void)
{
    auto signal = FSignalGet();
    while(signal){

        switch (signal->opcode)
        {
        case FSignalOpCode::SwithTabListId:
             _executor->CallExecutor(ExecutorOpCode::SwitchToTab, &signal->data);
            break;
        
        default:
            break;
        }


        signal = FSignalGet();  // read one more
    }
}
