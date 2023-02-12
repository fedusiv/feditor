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
    BufferData::iterator it;

    if(_buffer.empty())
    {
        // if buffer empty we need to create new line there
        _buffer.push_back(BufferLine(0));
    }
    it = ( _buffer.begin() + _cursorPosition.y );

    for(auto c: data)
    {
        (*it).insert((*it).begin() + _cursorPosition.x, static_cast<int>(c));
        _cursorPosition.x++;
    }
}

void Buffer::InsertNewLine(void)
{
    BufferData::iterator it;

    if(_buffer.empty())
    {
        // if buffer empty we need to create new line there
        _buffer.push_back(BufferLine(0));
    }
    it = ( _buffer.begin() + _cursorPosition.y + 1 );
    _buffer.insert(it, (BufferLine(0)));
    _cursorPosition.x = 0;
    _cursorPosition.y += 1;
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
