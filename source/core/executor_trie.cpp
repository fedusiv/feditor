#include <executor_trie.hpp>
#include <stack>

int ExecutorTrie::CalculateIndex(char c)
{
    return c - 'a'; // since childrens are just array of symbols, we need to count index from 0 to TRIE_ALPHABET. 
                     // and we know, that all cmd are lowercase letters. only one exception is '_'     
}

void ExecutorTrie::Insert(std::string word, ExecutorOpCode opCode){
    ExecutorTrieNode* curNode;
    int index;
    
    curNode = _root;
    for(auto c: word){
        index = ExecutorTrie::CalculateIndex(c);
        if(index < 0){
            // index can be less, than 0 only in one case, when there is '_'. because it's 0x5f and 'a' is 0x61 
            index = TRIE_ALPHABET-1; // last element
        }
        if(nullptr == curNode->childrens[index]){
            curNode->childrens[index] = new ExecutorTrieNode(); // children is missing, creating new one
        }
        curNode = curNode->childrens[index]; // node for next iterations is set to children of current node
    }
    curNode->isEndOfWord = true; // on latest childer(node), this is end of word.
    curNode->opCode = opCode;   // store opcode to call executor
}

void ExecutorTrie::Search(std::string prefix, ExecutorCompleteVariants& variants)
{
    ExecutorTrieNode *node, *searchNode;
    std::stack<ExecutorCompleteVariantSearch> stack;
    ExecutorCompleteVariantSearch searchPair;
    std::string opName;
    int index;
    int variantCount = 5; // TODO: take it from configs??

    node = _root;
    for(auto c: prefix){
        index = ExecutorTrie::CalculateIndex(c);
        if(node->childrens[index]==nullptr){
            // no children exists
            break; // exit loop
        }else{
            // children exists
            node = node->childrens[index];
        }
    }
    if(node == _root){
        // no available variants. Exit
        return;
    }
    stack.push({node,prefix});
    
    while(!stack.empty()){
        searchPair = stack.top();
        stack.pop();
        searchNode = std::get<0>(searchPair);
        if(searchNode->isEndOfWord){
            variants.push_back({
                std::get<1>(searchPair),
                searchNode->opCode
            });
            variantCount--;
            if(variantCount==0){
                break; // amount of required elements for autocompletion is reached
            }
        }
        
        for(int i = 0; i < TRIE_ALPHABET; i++){
            searchNode = std::get<0>(searchPair);
            if(nullptr != searchNode->childrens[i]){
                opName = std::get<1>(searchPair);
                opName.push_back('a'+i);
                stack.push(std::make_pair(searchNode, opName));
            }
        }
    }
}

