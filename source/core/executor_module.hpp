#ifndef __EXECUTOR_MODULE_HPP__
#define __EXECUTOR_MODULE_HPP__


// Opcode of all Executors
enum class ExecutorOpCode
{
    TextInsert,
    TextInsertNewLine,
    ExecutorOpCodeMax
};



// This is part of Executor.
// Class, which needs to provide functionality to executor, need to be derived from class ExecutorModule, this will give possibility to use methods from Executor
class ExecutorModule
{
    public:
        ExecutorModule(int id) : _moduleId(id) {}
        virtual void Execute(ExecutorOpCode opCode, void * data){}

    protected:
        int _moduleId;
};

#endif //  __EXECUTOR_MODULE_HPP__