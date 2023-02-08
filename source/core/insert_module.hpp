#ifndef __INSERT_MODULE_HPP__
#define __INSERT_MODULE_HPP__

#include <map>
#include <list>

#include "executor_module.hpp"
#include "executor.hpp"
#include "../common/buffer.hpp"

class InsertModule : ExecutorModule
{

    public:
        InsertModule(int id);
        void Execute(ExecutorOpCode opCode, void * data) override;
        Buffer * CreateNewBuffer();


    private:
        Executor * _executor;
        std::map<ExecutorOpCode, void(InsertModule::*)(void*)> _executorFunctions;

        std::list<Buffer*> _bufferList;
        Buffer * _activeBuffer;

        void AttachExecutors(void);

        // Executors functions
        void InsertText(void * data);
};

#endif // __INSERT_MODULE_HPP__