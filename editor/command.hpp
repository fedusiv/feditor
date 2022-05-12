#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

enum class CommandType
{
    EditMode,
    InputMode,
    EnterPressed,
    Cursor
};

class Command
{
    public:
        Command(CommandType type): type(type){}

        CommandType type;
};

enum class CursorCommandType
{
    CursorUp,
    CursorDown,
    CursorLeft,
    CursorRight
};

class CommandCursor: public Command
{
    public:
        CommandCursor(CommandType type ,CursorCommandType cType): Command(type) ,cursorType(cType){}

        CursorCommandType cursorType;
};


#endif  // __COMMAND_HPP__