#include "buffer.hpp"
#include "vector2.hpp"

int Buffer::_globalId = 1;

Buffer::Buffer(void)
{
    bufferId = _globalId++;
    _cursorPosition = Vector2Pair(0,0);
}

void Buffer::Append(std::string data)
{
    BufferLine line;
    BufferData::iterator it = ( _buffer.begin() + _cursorPosition.y );

    for(auto c: data)
    {
        (*it).insert((*it).begin() + _cursorPosition.x, static_cast<int>(c));
        _cursorPosition.x++;
    }
}

BufferLine * Buffer::LineData(int lineNumber)
{
    return &_buffer[lineNumber];
}

int Buffer::LinesNumber(void)
{
    return _buffer.size();
}

/*
    Cursor position is represented in logic coordinates here.
    Logic means, that it says where in related to symbols cursor should be located
*/
Vector2 Buffer::CursorPosition()
{
    return _cursorPosition;
}
