#include "buffer_handler.hpp"
#include "buffer.hpp"

BufferHandler * BufferHandler::_handler;   // need because of static

BufferHandler::BufferHandler()
{
    _bufferTabId = 0;
    _floatBuffer = nullptr;
    CreateNewTab();
}

Buffer * BufferHandler::CreateBuffer(std::string filepath)
{
    auto buffer = new Buffer(filepath);
    _bufferList[_bufferTabId].push_back(buffer);
    if(_activeBuffer[_bufferTabId]->IsFake() ){
        // this is fake logic. if it fake buffer. let's delete it
        auto bufPnt = _activeBuffer[_bufferTabId];
        delete bufPnt;
    }
    _activeBuffer[_bufferTabId] = buffer;

    return buffer;
}

// Fake buffer is buffer which never will be displayed to anybody.
// Because tab starts with zero buffer on it, commands to call invsert symbol and etc to manipulate with buffer will bring to segfault
// That's why on creating new empty tab we will create fake buffer and in the moment of creating real buffer we will delete fake buffer
void BufferHandler::CreateFakeBuffer()
{
    auto buffer = new Buffer("fake.fake");
    // bufferTabId is newest, because it was updated after CreateNewTab function call
    _activeBuffer[_bufferTabId] = buffer;
    buffer->MarkFake(); // marking fake
}

void BufferHandler::CreateNewTab(void)
{
    auto l = new BufferList;    // new buffer list
    _bufferList.push_back(*l);  // push it to the list, so now we have list of buffer for new tab
    _bufferTabId = _bufferList.size()-1;    // get current tab id, means new tab's id
    _activeBuffer.resize(_activeBuffer.size()+1); // for active buffer holder increse capacity
    CreateFakeBuffer();
}

void BufferHandler::SwitchToTab(int id)
{
    _bufferTabId = id;
}

void BufferHandler::AppendToActiveBuffer(KeysInsertedText text)
{
    if(_floatBuffer != nullptr){
        _floatBuffer->Append(text);
    }else{
        _activeBuffer[_bufferTabId]->Append(text);
    }
}

void BufferHandler::InsertNewLine()
{
    _activeBuffer[_bufferTabId]->InsertNewLine();
}

void BufferHandler::MoveCursor(MoveCursorDirection direction)
{
    if(nullptr != _floatBuffer){
        _floatBuffer->MoveCursor(direction);
    }else{
        _activeBuffer[_bufferTabId]->MoveCursor(direction);
    }
}

void BufferHandler::DeleteAtCursor(DeleteOperations operation)
{
    if(nullptr != _floatBuffer){
        _floatBuffer->DeleteAtCursor(operation);
    }else{
        _activeBuffer[_bufferTabId]->DeleteAtCursor(operation);
    }
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

Buffer* BufferHandler::InstantiateFloatBuffer()
{
    if(nullptr != _floatBuffer){
        delete _floatBuffer;
        _floatBuffer = nullptr;
    }
    _floatBuffer = new Buffer();
    _floatBuffer->MarkOneLine();
    return _floatBuffer;
}

void BufferHandler::RemoveFloatBuffer()
{
    delete _floatBuffer;
    _floatBuffer = nullptr;
}

BufferLine* BufferHandler::GetLine(int lineId)
{
    return GetActiveBuffer()->LineData(lineId);
}

void BufferHandler::SetLine(int lineId, std::string lineData)
{
    Buffer* buf;

    buf = GetActiveBuffer();
    buf->CleanLine(lineId);
    buf->Append(lineData, lineId);
}

Buffer* BufferHandler::GetActiveBuffer()
{
    Buffer* buf;

    if(nullptr != _floatBuffer){
        buf = _floatBuffer;
    }else{
        buf = _activeBuffer[_bufferTabId];
    }
    return buf;
}

void BufferHandler::DeleteLine(int lineId)
{
    GetActiveBuffer()->DeleteLine(lineId);
}
