#include "executor.hpp"

#include <iostream>
Executor * Executor::_executorSingleton = nullptr;


void Executor::CallExecutor(EditorState state, KeyMapVector keys, void * data)
{
    ExecutorElement * element = _executorMapTree->GetExecutor(state, keys);
    if(element != nullptr)
    {
        element->exec->Execute(element->opCode, data);
    }
}