#include "executor.hpp"

#include <iostream>
Executor * Executor::_executorSingleton = nullptr;


bool Executor::CallExecutor(EditorState state, KeyMapVector keys, void * data)
{
    bool res = false;
    ExecutorElement * element = _executorMapTree->GetExecutor(state, keys);
    if(element != nullptr)
    {
        element->exec->Execute(element->opCode, data);
        res = true;
    }

    return res;
}