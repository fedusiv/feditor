#ifndef __BUFFER_HANDLER_HPP__
#define __BUFFER_HANDLER_HPP__

#include <list>
#include "buffer.hpp"

class BufferHandler
{
    public:
        static BufferHandler* Instance()
        {
            if(_handler == nullptr)
            {
                _handler = new BufferHandler();
            }
            return _handler;
        }

        Buffer * CreateBuffer(void);    // create empty buffer
        void AppendToActiveBuffer(KeysInsertedText text);   // insert text
        void InsertNewLine(void);   // insert new line
        void MoveCursor(MoveCursorDirection direction); // moving cursor by one step
        void DeleteAtCursor(DeleteOperations operation);

    private:
        static BufferHandler* _handler;
        std::list<Buffer *> _bufferList;    // list of created buffers
        Buffer * _activeBuffer;     // current active buffer
};


#endif // __BUFFER_HANDLER_HPP__