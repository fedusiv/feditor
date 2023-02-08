#include "buffer.hpp"

int Buffer::_globalId = 1;

Buffer::Buffer(void)
{
    bufferId = _globalId++;
}

int Buffer::LinesNumber(void)
{
    return _buffer.size();
}
