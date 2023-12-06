#include "buffer_handler.hpp"
#include "buffer.hpp"

BufferHandler * BufferHandler::_handler;   // need because of static

BufferHandler::BufferHandler()
{
    _bufferTabId = 0;
    CreateNewTab();
}

Buffer * BufferHandler::CreateBuffer(std::string filepath)
{
    Buffer * buffer;
    buffer = new Buffer(filepath);
    _bufferList[_bufferTabId].push_back(buffer);
    _activeBuffer[_bufferTabId] = buffer;

    return buffer;
}

void BufferHandler::CreateNewTab(void)
{
    auto l = new BufferList;
    _bufferList.push_back(*l);
    _bufferTabId = _bufferList.size()-1;
    _activeBuffer.reserve(_activeBuffer.capacity()+1);
}

void BufferHandler::SwitchToTab(int id)
{
    _bufferTabId = id;
}

void BufferHandler::AppendToActiveBuffer(KeysInsertedText text)
{
    _activeBuffer[_bufferTabId]->Append(text);
}

void BufferHandler::InsertNewLine()
{
    _activeBuffer[_bufferTabId]->InsertNewLine();
}

void BufferHandler::MoveCursor(MoveCursorDirection direction)
{
    _activeBuffer[_bufferTabId]->MoveCursor(direction);
}

void BufferHandler::DeleteAtCursor(DeleteOperations operation)
{
    _activeBuffer[_bufferTabId]->DeleteAtCursor(operation);
}

Buffer * BufferHandler::UpdateActiveBuffer()
{
    for(auto b: _bufferList[_bufferTabId])
    {
        if(b->RequestActive())
        {
            _activeBuffer[_bufferTabId] = b;  // set new active buffer
        }
    }
    return _activeBuffer[_bufferTabId];
}
