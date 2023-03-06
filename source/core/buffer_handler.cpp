#include "buffer_handler.hpp"

BufferHandler * BufferHandler::_handler;   // need because of static

Buffer * BufferHandler::CreateBuffer(void)
{
    Buffer * buffer;
    buffer = new Buffer();
    _bufferList.push_back(buffer);
    _activeBuffer = buffer;

    return buffer;
}

void BufferHandler::AppendToActiveBuffer(KeysInsertedText text)
{
    _activeBuffer->Append(text);
}

void BufferHandler::InsertNewLine()
{
    _activeBuffer->InsertNewLine();
}

void BufferHandler::MoveCursor(MoveCursorDirection direction)
{
    _activeBuffer->MoveCursor(direction);
}

void BufferHandler::DeleteAtCursor(DeleteOperations operation)
{
    _activeBuffer->DeleteAtCursor(operation);
}