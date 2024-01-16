#ifndef __BUFFER_HANDLER_HPP__
#define __BUFFER_HANDLER_HPP__

#include <list>
#include <vector>
#include "buffer.hpp"

typedef std::vector<Buffer *> BufferList ;

class BufferHandler
{
    public:

        BufferHandler();
        static BufferHandler* Instance()
        {
            if(_handler == nullptr)
            {
                _handler = new BufferHandler();
            }
            return _handler;
        }

        Buffer * CreateBuffer(FString filepath);    // create empty buffer
        void CreateNewTab(void);  // creating new instance of buffer list to hold information about buffers in the tab and set new tab the current one
        void SwitchToTab(int id); // change current active tab to new one
        void AppendToActiveBuffer(KeysInsertedText text);   // insert text
        void InsertNewLine(void);   // insert new line
        void MoveCursor(MoveCursorDirection direction); // moving cursor by one step
        void DeleteAtCursor(DeleteOperations operation);
        Buffer * UpdateActiveBuffer();  // function go over all created buffers, looks for buffer, which is requested to be active, and makes decision
        Buffer * InstantiateFloatBuffer();  // creates new float buffer
        void RemoveFloatBuffer(); // delete and remove current used flaot buffer
        FString* GetLine(int lineId); // returns line by it's id from current active buffer
        void SetLine(int lineId, std::string lineData);  // explicitly set value for gevin line in buffer
        void DeleteLine(int lineId); // delete completely line from buffer

    private:
        void CreateFakeBuffer();    // logic for fake buffers
        Buffer* GetActiveBuffer();  // getting current active buffer
        static BufferHandler* _handler;
        std::vector<BufferList> _bufferList;    // list of created buffers
        BufferList _activeBuffer;     // list of current active buffers. depend of tab
        Buffer* _floatBuffer;
        int _bufferTabId;  // id of the current buffer tab
};


#endif // __BUFFER_HANDLER_HPP__
