#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <vector>
#include <map>
#include <algorithm>

#include "executor_module.hpp"
#include "../input/key_map.hpp"
#include "editor_state.hpp"

/*
 Basis logic of editor.
    All operations and actions in editor are basically executor element
    You can compare it with orders to robot. Any action, that user can make: insert text, search for something, open new window is a operation.
    And executor is class which parse and runs these operations.
    Further all operations will be called executor element. Because one action is one executor element
*/
class Executor
{
    /*
     ExecutorElement is object, which represent logic operator and executor of editor functionality
     It says which shortcut will be pressed to activate functionality
     And also in which editor state on this shortcut it will work.
     This class is connectinon between keymapping and real functionality
    */
    class ExecutorElement
    {
        public:
            ExecutorElement(ExecutorModule * exec, ExecutorOpCode opCode, 
                    std::vector<KeyMap> keyMap,
                    std::vector<EditorState> states,
                    std::string name, std::string desc, int id = 1)
                : exec(exec), opCode(opCode),keyMap(keyMap), states(states), name(name), desc(desc)
            {};
            ExecutorModule * exec;
            ExecutorOpCode opCode;
            std::vector<KeyMap> keyMap;
            std::vector<EditorState> states;
            std::string name;
            std::string desc;
    };


    class ExecutorMapTree
    {
        /*
            Tree structure for one node. One node - one specific editor mode key bind
            parent                  keymap none, executor none
            childs(level 1)  ctrl                               Shift
                            keymap ctrl, executor none          keymap shift, executor none
            childs(level 2) v                                   v
                            keymap v, executor - paste          keymap v, executor none
            childs(level 3)                                     i
                                                                keymap i, executor - paste lines

            Executors functionality is for example only, but I think it will represent at least how function add works.
        */
        struct Node
        {
            KeyMap key;
            ExecutorElement * executor;
            std::vector<Node *>childs;
        };
        Node * _parent[EditorState::EditorStateMax];    // each tree for specific editor mode. Means, that some keybinds only work in specific keyMode

        public:
            ExecutorMapTree(){ 
                for(int i = 0; i < EditorState::EditorStateMax; i ++)
                {
                    _parent[i] = new Node();    // create empty parents node for all editor states
                }
            }


            ExecutorElement * GetExecutor(EditorState state, KeyMapVector keyMap)
            {
                Node * parent;
                ExecutorElement * executor;

                executor = nullptr;
                parent = _parent[state];
                std::vector<Node*>::iterator iN;
                iN =  std::find_if(parent->childs.begin(), parent->childs.end(), 
                            [&keyMap](const Node* n) { return (keyMap[0]) == n->key;} );
                if(iN != parent->childs.end())
                {
                    executor = (*iN)->executor;
                }

                return executor;
            }

            // Adding executor element inro tree
            void AddNode(ExecutorElement * execElement)
            {
                Node * parent;
                KeyMapVector::iterator iK;
                std::vector<Node*>::iterator iN;

                for(EditorState state: execElement->states)
                {
                    parent = _parent[state];
                    // iterator end is past-to-the-end element
                    // we iterate through all key map vector bindigs, to find, that there are can be an allocated tree nodes for keys.
                    // we iterate through all key map vector:
                    //      if not last key does not exist before we will create new with empty executor.
                    //      if last key already has executor will replace it
                    for(iK = execElement->keyMap.begin(); iK != execElement->keyMap.end(); iK++)
                    {
                        iN =  std::find_if(parent->childs.begin(), parent->childs.end(), 
                            [&iK](const Node* n) { return (*iK) == n->key;} );

                        if (iN == parent->childs.end()) // Node does not exist
                        {
                            // In child node, there is no Node with with this Key.
                            auto node = new Node(); // create Node
                            node->key = (*iK);  // attach Key
                            if(iK == execElement->keyMap.end() -1)
                            {
                                // if we are on last key in map sequence, insert required executor
                                node->executor = execElement;
                            }
                            else
                            {
                                // We are not on the last key in sequence. Set nullptr to double check to prevent calling executor function
                                node->executor = nullptr;
                            }
                            iN = parent->childs.insert( parent->childs.end(), node); // insert new node into vector and get iterator
                        }
                        else { // Node exists
                            // For this case, we are only care if this is last node
                            if(iK == execElement->keyMap.end() -1)
                            {
                                // delete previous executor
                                delete (*iN)->executor;
                                // Place new one
                                (*iN)->executor = execElement;
                            }
                        }

                        parent = (*iN); // get next parent, current child
                    }
                }
            }
    };


    private:
        static Executor * _executorSingleton;
        std::vector<ExecutorElement *> _executorList;   // list with all executors
        ExecutorMapTree * _executorMapTree;     // Map tree with executors

        Executor()
        {
            _executorMapTree = new ExecutorMapTree();
        }
    
    
    public:
        static Executor * Instance()
        {
            if(_executorSingleton == nullptr)
            {
                _executorSingleton = new Executor();
            }
            return _executorSingleton;
        }
        // Create and add executor element.
        // Here key value is keyMap. If other executor attached to given keyMap in specific editor state, other executor will be deleted and new one will be placed
        void AddExecutorElement(
            ExecutorModule * exec,
            ExecutorOpCode opCode,
            KeyMapVector keyMap,
            std::vector<EditorState> states,
            std::string name, std::string desc
        )
        {
            auto element = new ExecutorElement(exec, opCode, keyMap, states, name, desc);
            _executorList.push_back( element);
            _executorMapTree->AddNode(element);
        }

        void CallExecutor(EditorState state, KeyMapVector keys, void * data = nullptr);
};




#endif //__EXECUTOR_HPP__