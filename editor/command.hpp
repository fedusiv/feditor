#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

enum class CommandType
{
    EditMode,
    InputMode,
    EnterPressed,
    Cursor,
    Scroll,
    BackspacePressed
};

class Command
{
    public:
        Command(CommandType type): type(type){}

        CommandType type;
};

/* *** Cursor movement *** */
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

/* *** Scroll Area *** */
enum class ScrollWindowType
{
    ScrollUp,
    ScrollDown,
    ScrollLeft,
    ScrollRight
};

class CommandScroll: public Command
{
    public:
        CommandScroll(CommandType type ,ScrollWindowType sType): Command(type) ,scrollType(sType){}

        ScrollWindowType scrollType;
};

#endif  // __COMMAND_HPP__