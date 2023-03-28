#ifndef __EXECUTOR_HPP__
#define __EXECUTOR_HPP__

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "executoraccess.hpp"
#include "executoroc.hpp"
#include "keymap.hpp"
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
            ExecutorElement(ExecutorMethod * execM, ExecutorOpCode opCode, 
                    std::vector<KeyMap> keyMap,
                    std::vector<EditorState> states,
                    std::string name, std::string desc, int id = 1)
                : execM(execM), opCode(opCode),keyMap(keyMap), states(states), name(name), desc(desc)
            {};
            ExecutorMethod * execM;
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


            /*
                Get editor state and keys sequence.
                It will parse all keys to find one key combination to return executor pointer
                Also it edits keyMap variable and keeps only keys on which executor is find
            */
            ExecutorElement * GetExecutor(EditorState state, KeysMapList& keyMap)
            {
                Node * parent;
                ExecutorElement * executor;
                std::vector<Node*>::iterator iN;
                KeysMapList::iterator iK;

                // Get initial state
                executor = nullptr;
                parent = _parent[state];

                for(iK = keyMap.begin(); iK != keyMap.end();)
                {
                    iN =  std::find_if(parent->childs.begin(), parent->childs.end(), 
                                [&iK](const Node* n) { return (*iK) == n->key;} );
                    if(iN != parent->childs.end())
                    {
                        // found key in this state
                        parent = (*iN); // this is new parent, where we will look and continue

                        // check if this key has childs, means, there are combinations
                        if((*iN)->childs.size())
                        {
                            iK++; // size bigger than zero, means it has childs
                            continue;   // go to next loop
                        }
                        else {
                            // There is no childs there for this key.
                            executor = parent->executor;    // save executor
                            // remove all other keys from keyMap
                            iK++;
                            while(iK != keyMap.end())
                            {
                                iK = keyMap.erase(iK);
                            }
                            // everything is removed. Exit
                            break;
                        }
                    }
                    else {
                        // did not find any childs with given key.
                        iK = keyMap.erase(iK); // bring next one key to parse
                        // keyMap was decreased, because we did not used this key
                        continue;   // go to next loop
                    }
                }

                return executor;
            }

            // Adding executor element inro tree
            void AddNode(ExecutorElement * execElement)
            {
                Node * parent;
                KeysMapList::iterator iK;
                std::vector<Node*>::iterator iN;

                // some key combination can be run from any editor state
                // First check, if executor element if related to maximum editor state, attach that executor element to all modes
                if(execElement->states[0] == EditorState::EditorStateMax)
                {
                    for(int i = 0; i < EditorState::EditorStateMax; i++)
                    {
                        execElement->states.push_back(static_cast<EditorState>(i));
                    }
                    execElement->states.erase(execElement->states.begin()); // remove first element, which is EditorStateMax
                }

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
        std::unordered_map<ExecutorOpCode, ExecutorElement *> _executorList;   // list with all executors, which can be access by executor opcode
        ExecutorMapTree * _executorKeyMapTree;     // Map tree with executors, parsed be keymap
        ExecutorAccess * _execAccess;   // object, which keeps access to all important modules, which responce for application functionality

        Executor()
        {
            _executorKeyMapTree = new ExecutorMapTree();
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

        void AttachAccess(ExecutorAccess * access)
        {
            _execAccess = access;
        }

        // Create and add executor element.
        // Here key value is keyMap. If other executor attached to given keyMap in specific editor state, other executor will be deleted and new one will be placed
        void AddExecutorElement(
            ExecutorMethod * execM,
            ExecutorOpCode opCode,
            KeysMapList keyMap,
            std::vector<EditorState> states,
            std::string name, std::string desc
        );

        bool CallExecutor(EditorState state, KeysMapList& keys, void * data = nullptr); // Run executor by keymap list
        bool CallExecutor(ExecutorOpCode opCode, void * data = nullptr);    // run executor directly by opcode
};




#endif //__EXECUTOR_HPP__