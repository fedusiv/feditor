#include <iostream>
#include <string>
#include <iostream>

#include "insert_module.hpp"

InsertModule::InsertModule(int id): ExecutorModule(id)
{
    _executor = Executor::Instance();

    _activeBuffer = nullptr;

    AttachExecutors();
}


void InsertModule::InsertText(void *data)
{
    std::string * strPnt;
    strPnt = reinterpret_cast<std::string*>(data);

}

Buffer *InsertModule::CreateNewBuffer()
{
    auto buffer = new Buffer();
    _bufferList.push_back(buffer);

    _activeBuffer = buffer;

    return buffer;
}




/*
    This function is a part of executor mechanism.
    This function is called from executor, and runs parsed function
*/
void InsertModule::Execute(ExecutorOpCode opCode, void * data)
{
    if(_executorFunctions.count(opCode))
    {
        auto pnt = _executorFunctions.at(opCode);
        (this->*pnt)(data);
        
    }
}

void InsertModule::AttachExecutors(void)
{
    _executor->AddExecutorElement(this, ExecutorOpCode::TextInsert, 
                                KeyMapVector(1,KeyMap::KeyText), std::vector<EditorState>(1,EditorState::InsertState), 
                                "insert_text", "inserting text");
    _executorFunctions.insert({ExecutorOpCode::TextInsert, &InsertModule::InsertText});
}
