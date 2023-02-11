#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>
#include <string>

#include "vector2.hpp"

typedef typename std::vector<int> BufferLine;
typedef typename std::vector<BufferLine> BufferData;

class Buffer
{
    public:
        Buffer(void);
        int LinesNumber(void);
        int bufferId;
        void Append(std::string data);
        BufferLine * LineData(int lineNumber);
        Vector2 CursorPosition();

    private:
        static int _globalId;
        Vector2 _cursorPosition;
        BufferData _buffer;
};

#endif // __BUFFER_HPP__