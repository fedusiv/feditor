#include <iostream>
#include <string>
#include <iostream>

#include "insert_module.hpp"

InsertModule::InsertModule(int id): ExecutorModule(id)
{
    _executor = Executor::Instance();

    _executor->AddExecutorElement(this, ExecutorOpCode::TextInsert, 
                                std::vector<KeyMap>(KeyMap::KeyText), std::vector<EditorState>(EditorState::InsertMode), 
                                "insert_text", "inserting text");

}

void InsertModule::InsertText(void * data)
{
    std::string * strPnt;
    strPnt = reinterpret_cast<std::string*>(data);

    std::cout << *strPnt << std::endl;
}


void InsertModule::Execute(ExecutorOpCode opCode, void * data)
{
    std::cout << "Yes" << std::endl;
}