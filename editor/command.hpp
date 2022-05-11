#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

enum class CommandType
{
    EditMode,
    InputMode,
    EnterPressed
};

class Command
{
    public:
        Command(CommandType type): type(type){}

        CommandType type;
};


#endif  // __COMMAND_HPP__