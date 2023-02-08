#ifndef __INSERT_MODULE_HPP__
#define __INSERT_MODULE_HPP__

#include "executor_module.hpp"
#include "executor.hpp"

class InsertModule : ExecutorModule
{
    public:
        InsertModule(int id);
        void Execute(ExecutorOpCode opCode, void * data) override;

        void InsertText(void * data);

    private:
        Executor * _executor;
};

#endif // __INSERT_MODULE_HPP__