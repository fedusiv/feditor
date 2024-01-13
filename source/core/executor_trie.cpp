#include "executoroc.hpp"
#include <executor_trie.hpp>
#include <stack>

int ExecutorTrie::CalculateIndex(char c)
{
    // since childrens are just array of symbols, we need to count index from 0 to TRIE_ALPHABET. 
    // and we know, that all cmd are lowercase letters. only one exception is '_'     
    int index = c - 'a';
    if(index < 0 ){
      // index can be less, than 0 only in one case, when there is '_'. because it's 0x5f and 'a' is 0x61 
      index = TRIE_ALPHABET - 1;
    }
    return index;
}

bool ExecutorTrie::IsSymbolAcceptable(int symbol)
{
    // 0x61 is 'a' symbol is ASCII 0x5f is '_' symbol in ASCII
    if(symbol < 0x61 && symbol != 0x5f){
      return false; // Finish execution, suggest symbol not in the variant
    }
    if(symbol > 0x7a){ // 0x7a is 'z' symbol
      return false; // Finish execution, suggest symbol not in the variant
    }
    return true;
}

void ExecutorTrie::Insert(std::string word, ExecutorOpCode opCode){
    ExecutorTrieNode* curNode;
    int index;
    
    curNode = _root;
    for(auto c: word){
        index = ExecutorTrie::CalculateIndex(c);
        if(nullptr == curNode->childrens[index]){
            curNode->childrens[index] = new ExecutorTrieNode(); // children is missing, creating new one
        }
        curNode = curNode->childrens[index]; // node for next iterations is set to children of current node
    }
    curNode->isEndOfWord = true; // on latest childer(node), this is end of word.
    curNode->opCode = opCode;   // store opcode to call executor
}

void ExecutorTrie::Search(BufferLine* prefixData, ExecutorCompleteVariants& variants)
{
    ExecutorTrieNode *node, *searchNode;
    std::stack<ExecutorCompleteVariantSearch> stack;
    ExecutorCompleteVariantSearch searchPair;
    std::string prefix;
    std::string opName;
    int index;
    int variantCount = 5; // TODO: take it from configs??

    node = _root;
    for(auto c: *prefixData){
        // let's convert from ints to string. But we need to check symbols first. Because this autocomplete will work only for cmds
        if(!IsSymbolAcceptable(c)){
          return;
        }
        index = ExecutorTrie::CalculateIndex(c);
        if(node->childrens[index]==nullptr){
            // no children exists
            break; // exit loop
        }else{
            // children exists
            node = node->childrens[index];
            prefix.push_back(c);
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
                if(i == TRIE_ALPHABET -1){
                  opName.push_back('_');
                }else{
                  opName.push_back(static_cast<char>('a'+i));
                }
                stack.push(std::make_pair(searchNode->childrens[i], opName));
            }
        }
    }
}


ExecutorOpCode ExecutorTrie::SearchCmd(BufferLine* name)
{
  ExecutorTrieNode* node;
  ExecutorOpCode opCode;
  int index;

  opCode = ExecutorOpCode::ExecutorOpCodeMax;
  node = _root;
  for(auto c: *name){
    if(!IsSymbolAcceptable(c)){
      break;
    }
    index = ExecutorTrie::CalculateIndex(c);
    node = node->childrens[index];
    if(nullptr == node){
      break; // no such node or we reached end
    }
  }
  if(node->isEndOfWord){
    opCode = node->opCode;
  }
  return opCode;
}

#ifdef DEBUG_INFO
#include <iostream>

void ExecutorTrie::PrintTrieContent(void)
{
  ExecutorTrieNode *node;
  std::stack<ExecutorCompleteVariantSearch> stack;
  ExecutorCompleteVariants variants;
  ExecutorCompleteVariantSearch pair;
  std::string call;
  
  node = _root;
  call = "";
  stack.push({node, call});

    while(!stack.empty()){
        pair = stack.top();
        stack.pop();
        node = std::get<0>(pair);
        if(node->isEndOfWord){
            variants.push_back({
                std::get<1>(pair),
                node->opCode
            });
            continue;
        }
        
        for(int i = 0; i < TRIE_ALPHABET; i++){
            node = std::get<0>(pair);
            if(nullptr != node->childrens[i]){
                call= std::get<1>(pair);
                if(i == TRIE_ALPHABET -1){
                  call.push_back('_');
                }else{
                  call.push_back(static_cast<char>('a'+i));
                }
                stack.push(std::make_pair(node->childrens[i], call));
            }
        }
    }

  std::cout << "List of executor cmd's name" << std::endl;
  for(auto v: variants){
    std::cout << "name: " << std::get<0>(v) << std::endl;
  }
  
}
#endif // DEBUG_INFO
