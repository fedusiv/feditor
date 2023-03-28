#include <iostream>
#include <fstream>
#include "buffer.hpp"
#include "vec2.hpp"

int Buffer::_globalId = 1;

Buffer::Buffer(std::string filepath): _filepath(filepath), _largestLineSize(0)
{
    std::ifstream file;

    bufferId = _globalId++;
    if(filepath.empty()){
        _buffer.push_back(BufferLine(0));   // creates empty buffer with one empty line
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

void Buffer::Append(KeysInsertedText data)
{
    BufferLine line;
    BufferData::iterator it;

    if(_buffer.empty())
    {
        // if buffer empty we need to create new line there
        _buffer.push_back(BufferLine(0));
    }
    it = ( _buffer.begin() + _cursorPosition.y );

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

void Buffer::InsertNewLine(void)
{
    BufferData::iterator it;

    if(_buffer.empty())
    {
        // if buffer empty we need to create new line there
        _buffer.push_back(BufferLine(0));
    }
    it = ( _buffer.begin() + _cursorPosition.y + 1 );
    _buffer.insert(it, (BufferLine(0)));
    if(_cursorPosition.x < (*(it-1)).size())
    {
        // new line request is pressed in the middle of line. Move all content right after cursor to new line
        (*it).insert((*it).begin(), (*(it-1)).begin() + _cursorPosition.x, (*(it-1)).end()); // this abracadabra it inserting data from previous line to new one
        (*(it-1)).erase((*(it-1)).begin() + _cursorPosition.x, (*(it-1)).end());
    }
    _cursorPosition.x = 0;
    _cursorPosition.y += 1;
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
                // When moving cursor from begin of line, cursor will go to last available position in upper line
                _cursorPosition.y -= 1;
                _cursorPosition.x = _buffer[_cursorPosition.y].size();
            }
            break;
        }

        case MoveCursorDirection::CursorRight:
        {
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

/*
    Cursor position is represented in logic coordinates here.
    Logic means, that it says where in related to symbols cursor should be located
*/
Vec2 Buffer::CursorPosition()
{
    return _cursorPosition;
}
