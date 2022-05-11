#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>

#include "utils.hpp"

class Buffer
{
    public:
        Buffer();

        Vector2 CursorPosition(void);
        void SetCursorPosition(Vector2 pos);
        void Append(int character, Vector2 pos);
        std::vector<int> * GetLineFromBuffer(int line);

    private:
        std::vector<std::vector<int>> _buffer;
        Vector2 _cursorPosition;
};



#endif //__BUFFER_HPP__ 