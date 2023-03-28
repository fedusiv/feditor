#include "executor.hpp"

#include <iostream>
Executor * Executor::_executorSingleton = nullptr;


// Create and add executor element.
// Here key value is keyMap. If other executor attached to given keyMap in specific editor state, other executor will be deleted and new one will be placed
void Executor::AddExecutorElement(ExecutorMethod * execM, ExecutorOpCode opCode,
                                    KeysMapList keyMap, std::vector<EditorState> states,
                                    std::string name, std::string desc
)
{
    auto element = new ExecutorElement(execM, opCode, keyMap, states, name, desc); // create executor element
    _executorList.insert({opCode,element}); // insert element to storage of all opcodes executors
    if(!keyMap.empty())
    {   // insert only if not empty
        _executorKeyMapTree->AddNode(element);  // insert element into storage tree, based on keymap and execution state when this keymap can be executed
    }
}


bool Executor::CallExecutor(EditorState state, KeysMapList& keys, void * data)
{
    bool res = false;
    ExecutorElement * element = _executorKeyMapTree->GetExecutor(state, keys);
    if(element != nullptr)
    {
        element->execM(_execAccess, data);
        res = true;
    }

    return res;
}

bool Executor::CallExecutor(ExecutorOpCode opCode, void * data)
{
    bool res = false;
    auto it = _executorList.find(opCode);
    if(it != _executorList.end())
    {
        // have executor element with provided opcode
        (*it).second->execM(_execAccess, data);
        res = true; 
    }

    return res;
}