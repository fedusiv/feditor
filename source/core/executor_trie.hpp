#ifndef __EXECUTOR_TRIE_HPP__
#define __EXECUTOR_TRIE_HPP__

#include <executoroc.hpp>
#include <string>
#include <vector>
#include <buffer.hpp>

#define TRIE_ALPHABET 27 // this is size of the trie data structure alphabet. 26 letters + '_' symbol

typedef std::pair<std::string, ExecutorOpCode> ExecutorCompleteVariant;
typedef std::vector<ExecutorCompleteVariant> ExecutorCompleteVariants;
/*
* Executor Trie is container of all names of executors. It's needed to call executor by it's name
* Names of executor cmds and opcode are stored in trie. or Prefix Tree
*/
class ExecutorTrieNode {
    public:
        // Unordered map has ability to of fast access to elements inside. In 
        ExecutorTrieNode* childrens[TRIE_ALPHABET];
        ExecutorOpCode opCode;
        bool isEndOfWord;

        ExecutorTrieNode(): isEndOfWord(false){
            std::memset(childrens, 0, TRIE_ALPHABET * sizeof(ExecutorTrieNode*));
        }
};

typedef std::pair<ExecutorTrieNode*, std::string> ExecutorCompleteVariantSearch;
class ExecutorTrie{

    private:
        ExecutorTrieNode* _root; // This is pointer to the current root node.

    public:
        ExecutorTrie(){
            _root = new ExecutorTrieNode();
        };
        static int CalculateIndex(char c);

        // Function definitions are in cpp file
        void Insert(std::string word, ExecutorOpCode opCode);
        void Search(BufferLine* prefixData, ExecutorCompleteVariants& variants);
#ifdef DEBUG_INFO
        void PrintTrieContent(void);
#endif // DEBUG_INFO
};

#endif // __EXECUTOR_TRIE_HPP__
