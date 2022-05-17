#include "buffer.hpp"

Buffer::Buffer()
{
    _cursorPosition.x = 0;
    _cursorPosition.y = 0;
    _maximumLineSize = 0;

    // create new line for empty buffer
    auto newLine = new BufferLineType();
    _buffer.push_back(*newLine);
    delete newLine;
}

int Buffer::LinesAmount()
{
    return _buffer.size();
}

int Buffer::MaximumLinesSize()
{
    return _maximumLineSize;
}

std::vector<int> * Buffer::GetLineFromBuffer(int line)
{
    if(line+1 > _buffer.size())
    {
        return nullptr; // no existing line
    }
    return &(_buffer.at(line));
}

Vector2 Buffer::CursorPosition()
{
    return _cursorPosition;
}

void Buffer::SetCursorPosition(Vector2 pos)
{
    _cursorPosition = pos;
}

void Buffer::Append(int character, Vector2 pos)
{
    std::vector<int> lineVector;


    int currentLineAmount = _buffer.size() - 1; // no idea, but if do not calculate before it will not work correctly
    // currentLineAmount says on which lates line we have data
    if( pos.y > currentLineAmount )
    {
        std::vector<int> * newline;
        // feditor allow to enter text after multiple empty lines
        while(pos.y > currentLineAmount )
        {
            currentLineAmount++;    // increase counter to represent on which line we are going to insert data
            // new line will be inserted
            newline = new std::vector<int>();
            if(currentLineAmount == pos.y)  // if positions are equal let's inset required element
            {
                // insert character only to last required line
                newline->push_back(character);
            }
            _buffer.push_back(*newline);
            delete newline;
        }
        
    }
    else
    {
        // new character at existing line
        _buffer.at(pos.y).insert(_buffer.at(pos.y).begin() + pos.x, character);
        if(_buffer.at(pos.y).size() > _maximumLineSize)
        {
            _maximumLineSize = _buffer.at(pos.y).size();
        }
    }

}


/*
*   Inserting new line to buffer.
*   It means, that divide current line and move other line one line down
*/
void Buffer::InsertNewLine(Vector2 pos)
{
    BufferLineType * newLine;
    BufferLineType::iterator it;

    // first let's formulate new line
    newLine = new BufferLineType();
    it = _buffer.at(pos.y).begin() + pos.x;

    // insert values from previous line
    newLine->insert(newLine->begin(), it, _buffer.at(pos.y).end());

    // remove values from previous line
    _buffer.at(pos.y).erase(it, _buffer.at(pos.y).end());

    // insert new line to whole buffer lines
    _buffer.insert(_buffer.begin() + pos.y + 1, *newLine);

    //remove allocated temp line
    delete newLine;
}