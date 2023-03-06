#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>
#include <string>

#include "vec2.hpp"
#include "keymap.hpp"

typedef typename std::vector<int> BufferLine;
typedef typename std::vector<BufferLine> BufferData;

enum MoveCursorDirection
{
    CursorUp,
    CursorDown,
    CursorLeft,
    CursorRight,
};

enum DeleteOperations
{
    BeforeCursor,
    AfterCursor
};

class Buffer
{
    public:
        Buffer(void);
        int LinesNumber(void);
        int bufferId;
        void Append(KeysInsertedText data); // append text data on current cursor position
        void InsertNewLine(void);   // insert new line.
        BufferLine * LineData(int lineNumber);
        Vec2 CursorPosition(); // return cursor position
        void DeleteAtCursor(DeleteOperations operation);
        void MoveCursor(MoveCursorDirection direction);

    private:
        static int _globalId;
        Vec2 _cursorPosition;
        BufferData _buffer;
};

#endif // __BUFFER_HPP__