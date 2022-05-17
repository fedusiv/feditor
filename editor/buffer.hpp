#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>

#include "utils.hpp"

typedef typename std::vector<int> BufferLineType;

class Buffer
{
    public:
        Buffer();

        int LinesAmount(void);
        int MaximumLinesSize(void); // get value of maximum characters in whole lines
        Vector2 CursorPosition(void);
        void SetCursorPosition(Vector2 pos);
        void Append(int character, Vector2 pos);
        void InsertNewLine(Vector2 pos);
        BufferLineType * GetLineFromBuffer(int line);

    private:
        std::vector<BufferLineType> _buffer;
        Vector2 _cursorPosition;
        int _maximumLineSize;
};



#endif //__BUFFER_HPP__ 