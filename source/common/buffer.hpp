#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <vector>

typedef typename std::vector<int> BufferLine;
typedef typename std::vector<BufferLine> BufferData;

class Buffer
{
    public:
        Buffer(void);
        int LinesNumber(void);
        int bufferId;

    private:
        static int _globalId;
        BufferData _buffer;
};

#endif // __BUFFER_HPP__