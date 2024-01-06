#include <iostream>
#include <fstream>
#include "buffer.hpp"
#include "vec2.hpp"
#include "macros.h"

int Buffer::_globalId = 1;

Buffer::Buffer(std::string filepath): _filepath(filepath), _largestLineSize(0)
{
    std::ifstream file;

    DefaultInit();

    if(filepath.empty()){
        _buffer.push_back(BufferLine(0));   // creates empty buffer with one empty line
        _filename = "untitled";
    }
    else
    {
        auto pos = filepath.find_last_of('/');  // get position of begin filename
        _filename = filepath.substr(pos + 1);   // substring filename
        file.open(_filepath,std::fstream::in|std::fstream::out);
        if(file.fail())
        {
            // fail to open the file
            // TODO: made this as error handling
        }
        else
        {   // file is opened successfully, fill buffer with file
            for (std::string line; std::getline(file, line); ) 
            {
                BufferLine bufferline;
                // opened one line.
                for(auto c: line)
                {
                    bufferline.push_back(static_cast<int>(c));
                }
                if(line.size() > _largestLineSize)
                {   // obtain largest size of charcter in lines
                    _largestLineSize = line.size();
                }
                _buffer.push_back(bufferline);
            }
            file.close();
        }
    }
}

Buffer::Buffer(void): _largestLineSize(0)
{
    DefaultInit();
    _buffer.push_back(BufferLine(0));
}

void Buffer::DefaultInit()
{
    _requestActive = false;
    bufferId = _globalId++;
    _isFake = false;
    _isOneLine = false;
    
}

void Buffer::Append(KeysInsertedText data)
{
    BufferLine line;
    BufferData::iterator it;
    int yPos;

    if(_buffer.empty())
    {
        // if buffer empty we need to create new line there
        _buffer.push_back(BufferLine(0));
    }
    // One of the OneLine logic manifestation. User append only to line 0.
    if(_isOneLine){
        yPos = _cursorPosition.y = 0; // Okay better to write here. Here is logic for autocompletion, where there is changes in userinput content.
                                      // We reset completion variant and variant choose for it
    }else{
        yPos = _cursorPosition.y;
    }
    it = ( _buffer.begin() + yPos);

    for(auto c: data)
    {
        (*it).insert((*it).begin() + _cursorPosition.x, c);
        _cursorPosition.x++;
    }
    // check if largest size can be updated
    if((*it).size() > _largestLineSize)
    {
        _largestLineSize = (*it).size();
    }
}

/*
    This function has no cursor operation, because it kind of function of internal editor calls
*/
void Buffer::Append(std::string data, int line)
{
    int curSize;

    curSize = _buffer.size();
    curSize = line - curSize + 1; // amount of line, which need to be added
    while(curSize > 0){
        _buffer.push_back(BufferLine(0));
        curSize--;
    }
    for(auto c: data){
        _buffer[line].push_back(c);
    }
}

void Buffer::InsertNewLine(void)
{
    BufferData::iterator it;

    if(_buffer.empty())
    {
        // if buffer empty we need to create new line there
        _buffer.push_back(BufferLine(0));
    }
    it = ( _buffer.begin() + _cursorPosition.y + 1 );
    it = _buffer.insert(it, (BufferLine(0))); // save it to new inserted buffer
    if(_cursorPosition.x < (*(it-1)).size())
    {
        // new line request is pressed in the middle of line. Move all content right after cursor to new line
        (*it).insert((*it).begin(), (*(it-1)).begin() + _cursorPosition.x, (*(it-1)).end()); // this abracadabra it inserting data from previous line to new one
        (*(it-1)).erase((*(it-1)).begin() + _cursorPosition.x, (*(it-1)).end());
    }
    _cursorPosition.x = 0;
    _cursorPosition.y += 1;
}

void Buffer::CleanLine(int lineId)
{
    if(lineId > _buffer.size() - 1){
        return; // there is no such a line
    }
    _buffer.at(lineId).clear();
}

void Buffer::DeleteLine(int lineId)
{
    if(lineId > _buffer.size() - 1){
        return; // there is no such a line
    }
    _buffer.erase(_buffer.begin()+lineId);
}

// This method moves cursor one step
void Buffer::MoveCursor(MoveCursorDirection direction)
{
    int tmp;
    
    switch (direction) {
        case MoveCursorDirection::CursorUp:
        {
            if(_cursorPosition.y <= 0)
            {
                break; // can not go upper if cursor is already on first line
            }
            _cursorPosition.y -= 1; // move cursor up
            break;
        }

        case MoveCursorDirection::CursorDown:
        {
            if(_cursorPosition.y + 1 >= _buffer.size()) // cursor position from 0, buffer size is from 1.
            {
                if(_isOneLine){
                    _cursorPosition.y = 1; // It will make cycle for oneLineBuffer. from last line to the the first line with variants.
                }
                break;  // can not go more down, than amount of lines
            }
            _cursorPosition.y += 1; // move cursor up
            break;
        }

        case MoveCursorDirection::CursorLeft:
        {
            if(_cursorPosition == Vec2(0,0))
            {
                break; // can not move cursor to left, it's already in the most left position
            }
            _cursorPosition.x -= 1;

            if(_cursorPosition.x < 0)
            {
                if(_isOneLine){
                    _cursorPosition.x = 0;  // For one line logic we do not go to the different line, we are staying at current
                }else{ // Here is default one logic.
                    // When moving cursor from begin of line, cursor will go to last available position in upper line
                    _cursorPosition.y -= 1;
                    _cursorPosition.x = _buffer[_cursorPosition.y].size();
                }
            }
            break;
        }

        case MoveCursorDirection::CursorRight:
        {
            if(_isOneLine){
                // OneLine logic is entering the club. We can not change line position by moving cursor to the sides. That's why need to keep one line position
                _cursorPosition += 1;
                if(_cursorPosition.x > _buffer[0].size()){
                    _cursorPosition.x = _buffer[0].size();
                }
                break;  // Exit the case
            }
            
            tmp = _buffer.size() - 1; // get id of last line in buffer
            if(_cursorPosition == Vec2( _buffer[tmp].size(),tmp))
            {
                break; // can not move cursor to right
            }
            _cursorPosition.x += 1;

            if(_cursorPosition.x > _buffer[_cursorPosition.y].size())
            {
                // When moving cursor from begin of line, cursor will go to last available position in upper line
                _cursorPosition.y += 1;
                _cursorPosition.x = 0;
            }
            break;
        }
    }

    if(_isOneLine){
        // Cursor moved, it means, that need to update content
        CopyOneLineToAnother(_cursorPosition.y, 0);
        return; // Exit function. OneLine logic ends here
    }
    // if move cursor to line where is less characters when in previous need to align with it
    if(direction == MoveCursorDirection::CursorDown || direction == MoveCursorDirection::CursorUp)
    {
        tmp = _buffer[_cursorPosition.y].size();
        if(tmp < _cursorPosition.x)
        {   
            _cursorPosition.x = tmp;
        }
    }
}

void Buffer::DeleteAtCursor(DeleteOperations operation)
{
    BufferLine* line;
    BufferLine::iterator it;

    if(_isOneLine){
        DeleteAtCursorOneLine(operation);
        return;
    }

    line = &_buffer[_cursorPosition.y];
    if(operation == DeleteOperations::BeforeCursor)
    {
        if(_cursorPosition.x != 0)
        {   // default removing one character
            _cursorPosition.x -= 1; // update cursor position
            it = line->begin() + _cursorPosition.x;
            line->erase(it);
        }
        else
        {
            if(_cursorPosition.y == 0)
            {
                return; // can not provide operation in the beginning of file
            }
            // delete current line, move data to line upper.
            auto lineprev = &_buffer[_cursorPosition.y-1];
            auto lineprevSize = lineprev->size();
            lineprev->insert(lineprev->end(), line->begin(), line->end());
            _buffer.erase(_buffer.begin()+_cursorPosition.y);
            _cursorPosition.y -= 1; // it goes to the live above
            _cursorPosition.x = lineprevSize;
        }
    }
    else if(operation == DeleteOperations::AfterCursor)
    {
        if(_cursorPosition.x == line->size())
        {
            // this is in the end of the line
            auto linenext = &_buffer[_cursorPosition.y+1];  // get next line, it will be deleted
            auto linenextSize = linenext->size();
            line->insert(line->end(), linenext->begin(), linenext->end());  // copy data from next line to current line
            _buffer.erase(_buffer.begin() + _cursorPosition.y+1);   // erase next line
            _cursorPosition.x = linenextSize;
        }
        else
        {
            // somewhere not in the end of line
            it = line->begin() + _cursorPosition.x;
            line->erase(it);
        }
    }
}

void Buffer::DeleteAtCursorOneLine(DeleteOperations operation)
{
    BufferLine* line;
    BufferLine::iterator it;

    line = &_buffer[0];
    if(operation == DeleteOperations::BeforeCursor)
    {
        if(_cursorPosition.x != 0){
            // default removing one character
            _cursorPosition.x -= 1; // update cursor position
            it = line->begin() + _cursorPosition.x;
            line->erase(it);
        }
    }
    else if(operation == DeleteOperations::AfterCursor)
    {
        // somewhere not in the end of line
        it = line->begin() + _cursorPosition.x;
        line->erase(it);
    }
    // After delete operation.Focus are moving to line 0
    _cursorPosition.y = 0;
    
}

void Buffer::SetCursorPosition(Vec2 position)
{
    if(position.y >= _buffer.size())
    {
        position.y = _buffer.size() - 1;    // position of y is bigger, than size of buffer. Set it to maximum size of buffer data. Another words set to last line
        // -1 only because we need position of, and position of last line is size - 1 (array starts from 0, rule of language)
    }
    auto line = LineData(position.y);
    if(line->size() < position.x)
    {
        position.x = line->size();      // same with cursor position on x axis, it can not be bigger, than line width.
    }
    if(position.x < 0)
    {
        position.x = 0;
    }
    _cursorPosition = position;
    if(_isOneLine){
        // Cursor moved, it means, that need to update content
        CopyOneLineToAnother(_cursorPosition.y, 0);
    }
}

BufferLine * Buffer::LineData(int lineNumber)
{
    return &_buffer[lineNumber];
}

int Buffer::LinesNumber(void)
{
    return _buffer.size();
}

int Buffer::ColumnsNumber(void)
{
    return _largestLineSize;
}

int Buffer::GetLineSize(int lineNumber)
{
    return _buffer[lineNumber].size();
}

/*
    Cursor position is represented in logic coordinates here.
    Logic means, that it says where in related to symbols cursor should be located
*/
Vec2 Buffer::CursorPosition()
{
    return _cursorPosition;
}

std::string Buffer::FileName()
{
    return _filename;
}

// Logic of this request functions is written in header
void Buffer::RequestToSetActive(bool status)
{
    _requestActive = status;
}

// Also important logic is desctibed for this function in header
bool Buffer::RequestActive()
{
    bool status;
    status = _requestActive;
    if(_requestActive)
    {
        _requestActive = false;
    }
    return status;
}

void Buffer::MarkFake()
{
    _isFake = true;
}

bool Buffer::IsFake()
{
    return _isFake;
}

void Buffer::MarkOneLine()
{
    _isOneLine = true;
}

void Buffer::CopyOneLineToAnother(int srcId, int dstId)
{
    // There is no check of content assuming, that you aware of size. This is not external api    
    _buffer[dstId].clear();
    _buffer[dstId] = _buffer[srcId];
    // Now need update cursor x position, because words with different size
    _cursorPosition.x = _buffer[dstId].size();
}
