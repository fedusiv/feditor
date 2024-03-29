#include <math.h>

#include "editor_state.hpp"
#include "vec2.hpp"
#include "widget_editor.hpp"
#include "macros.h"

WidgetEditor::WidgetEditor(Rect rect, Buffer *buffer) : Widget(rect), _buffer(buffer)
{
    _widgetType = WidgetType::WidgetTypeEditor;
    _currentUpperLine = 0;
    _linesPageMoveOffset = 1;
    _currentLeftLine = 0;
    _linesPageShiftOffset = 1;
    _widgetBorderThick = 1;
    CalculateAvaliableLines();
    CalculateAvaliableColumns();
    CalculateDrawingOffset();

    _colorBgWidget = ColorPurpose::ColorWidgetEditorBg; 
    _cursorPrevPos = _buffer->CursorPosition();
}

void WidgetEditor::Render(void)
{
    PageUpdate();

    Widget::Render();

    DrawLinesNumber();
    DrawData();

    if(_active)
    {
        // widget is active, need to draw cursor for it
        if(_currentEditorState == EditorState::InsertState){
            // Draw only is insert mode, i.e. edtiro mode
            DrawCursor(CalculateRealPosForCursor());
        }
    }

}

void WidgetEditor::SetActive(bool status)
{
    Widget::SetActive(status);
    _buffer->RequestToSetActive(status);
}

void WidgetEditor::Resize(Rect newRect)
{
    Widget::Resize(newRect);
    CalculateAvaliableLines();
    CalculateAvaliableColumns();
    CalculateDrawingOffset();
}

Vec2 WidgetEditor::CalculateRealPosForCursor()
{
    Vec2 logicPos, relativePos;

    logicPos = _buffer->CursorPosition();
    logicPos.y -= _currentUpperLine;
    logicPos.x -= _currentLeftLine;
    relativePos.x = (logicPos.x + _textStartX) * _glyphSize.x;
    relativePos.y = logicPos.y * _glyphSize.y;

    return relativePos;
}

/*
    Draw actual data of text buffer
*/
void WidgetEditor::DrawData(void)
{
    int linesNumber; // whole amount of lines
    int lineNumber; // current lineNumber
    int columnSize; // amount of possible characters in one line
    Vec2 pos;    // position to draw
    FString* lineData;  // data of current line
    FString::iterator iL; // iterator of line

    pos.x = _textStartX * _glyphSize.x;
    pos.y = 0;

    linesNumber = _buffer->LinesNumber();   // whole amount of lines in file
    if(linesNumber - _currentUpperLine > _availableLines)   // calculate last line number in the current page
    {
        linesNumber = _availableLines + _currentUpperLine;
    }

    for(lineNumber = _currentUpperLine; lineNumber < linesNumber; lineNumber++)
    {
        lineData = _buffer->LineData(lineNumber);
        if(lineData == nullptr)
        {
            // no lines left to draw. Or lines just empty
            continue;
        }
        columnSize = _availableColumns - _textStartX;
        if(lineData->size() > _currentLeftLine)
        {
            for( iL = lineData->begin() + _currentLeftLine; iL != lineData->end(); iL++)
            {
                DrawCharacter((*iL), pos, ColorPurpose::ColorWidgetEditorText);
                pos.x+= _glyphSize.x;
                if(--columnSize < 1)
                {   // limit amount of drawing characters to one page of editor
                    break;
                }
            }
        }
        pos.x = _textStartX * _glyphSize.x;
        pos.y += _glyphSize.y;
    }
}

void WidgetEditor::DrawLinesNumber(void)
{
    int linesNumber; // whole amount of lines
    int digitsAmount;   // maximum digits amount if lines numbering
    int lineNumber; // current lineNumber
    Vec2 pos;    // position to draw

    linesNumber = _buffer->LinesNumber();   // get maximum lines in the file
    if(linesNumber < 1)
    {
        linesNumber = 1;
    }
    if(linesNumber - _currentUpperLine > _availableLines)
    {
        linesNumber = _availableLines + _currentUpperLine;
    }

    pos = Vec2(0,0);
    digitsAmount = int(log10(linesNumber) + 1);
    _textStartX = 4 + digitsAmount; // for text drawing, need to know what offset to draw text
    for(lineNumber = _currentUpperLine + 1; lineNumber <= linesNumber; lineNumber++)
    {
        pos.x = 0;  // clear x position
        // draw line with line number. it should at least two character before first digit, and at least two characters after
        auto s = std::to_string(lineNumber);
        // calculate first position to draw. we have at least two character before
        // but if number is less, means we need to leave some space to align on right corner
        // digitsAmount - size of string will get amount of additional space
        pos.x += (2 + digitsAmount - s.size()) * _glyphSize.x;
        for(auto ch : s)
        {
            DrawCharacter((int)(ch), pos, ColorPurpose::ColorWidgetEditorLineNumber);
            pos.x += _glyphSize.x;
        }

        pos.y += _glyphSize.y;   // for next step
        if(pos.y + _glyphSize.y >= _widgetRect.h)
        {
            // reached maximum avaliable size. do not draw
            break;
        }
    }
}

/*
    Based on cursor position, need to decide, widget need to move page, move lines, etc
*/
void WidgetEditor::PageUpdate(void)
{
    Vec2 cursorPos;

    cursorPos = _buffer->CursorPosition();
    if(cursorPos == _cursorPrevPos)
    {
        return; // no need to update paging
    }
    // Vertical offset
    if(cursorPos.y - _linesPageMoveOffset < _currentUpperLine)
    {   // This is moving page up
        IncreaseUpperLine(-1);  // moving line by one step
    }
    if(cursorPos.y + _linesPageMoveOffset >= _currentUpperLine + _availableLines)
    {   // This is moving page down
        IncreaseUpperLine(1); // moving line by one step
    }

    // Horizontal offset
    if(cursorPos.x - _linesPageShiftOffset < _currentLeftLine)
    {   // This is shifting page left
        IncreaseLeftLine(-1);// shift line by one step to the left
    }
    if(cursorPos.x + _linesPageShiftOffset  >= _currentLeftLine + _availableColumns  - _textStartX)  // available lines here is all availables lines in editor widget including lines numbers
    {   // This is shifting page right
        IncreaseLeftLine(1);// shift line by one step to the right
    }
     
    // This part is to shifting page horizontal location when cursor changes drastically positon.
    // Good example when you are going from rightest position in line to leftest position in line and vise versa
    if(cursorPos.x < _currentLeftLine)
    {
        _currentLeftLine = cursorPos.x;
    }
    if(cursorPos.x > _currentLeftLine + _availableColumns - _textStartX)
    {
        _currentLeftLine = cursorPos.x - _availableLines  + _textStartX;
    }

    _cursorPrevPos = cursorPos;

}

// Page scrolling is explicitly called to change paging of buffer
void WidgetEditor::PageScrolling(Vec2 direction, Vec2 mousePosition)
{
    Vec2 cursorPos;

    UNUSED(mousePosition);
    cursorPos = _buffer->CursorPosition();
    // Vertical update of page scrolling
    if(direction.y > 0)
    {
        IncreaseUpperLine(-1 * SCROLL_VERTICAL_STEP);
    }
    else if(direction.y < 0)
    {
        IncreaseUpperLine(SCROLL_VERTICAL_STEP);
    }
    if(_currentUpperLine == _buffer->LinesNumber())
    {   // at least one line should be visible
        _currentUpperLine--;
    }
    if(cursorPos.y < _currentUpperLine)
    {   // cursor need to follow visible area of buffer
        cursorPos.y = _currentUpperLine;
    }
    if(cursorPos.y >= _currentUpperLine + _availableLines)
    {   // cursor need to follow visible area of buffer
        cursorPos.y = _currentUpperLine + _availableLines -1;
    }

    // Horizontal update of page scrolling
    if(direction.x > 0)
    {
        IncreaseLeftLine(-1 * SCROLL_HORIZONTAL_STEP);
    }
    else if(direction.x < 0)
    {
        IncreaseLeftLine(SCROLL_HORIZONTAL_STEP);
    }
    if(_currentLeftLine >= _buffer->GetLineSize(cursorPos.y))
    {
        _currentLeftLine = _buffer->GetLineSize(cursorPos.y);   // left line can not be bigger than current line size
    }
    if(cursorPos.x < _currentLeftLine)
    {   // cursor need to follow visible area of buffer
        cursorPos.x = _currentLeftLine;
    }
    if(cursorPos.x >= _currentLeftLine + _availableColumns)
    {   // cursor need to follow visible area of buffer
        cursorPos.x = _currentLeftLine + _availableColumns -1;
    }

    _buffer->SetCursorPosition(cursorPos);

}

void WidgetEditor::CalculateDrawingOffset(void)
{
    int verticalOffset;
    verticalOffset = _widgetRect.h - _availableLines * _glyphSize.y; // calculate how many unused space we have in drawing data
    // Set offset of drawing as half of available, to make kind of centered text data
    _drawingOffset = Vec2(0,verticalOffset/2);
}

void WidgetEditor::SetCursorPosition(Vec2 position)
{
    bool found;
    Vec2 lineStartPos;  // current coordinates, that we are investigating
    Vec2 cursorPos;     // cursor position at that we pointed
    lineStartPos.y = _drawingOffset.y;  // we are starting looking from offset.

    // Here we calculate to simulate widget starting from 0,0 that's why need to decrease position of cursor to widget starting point
    position.y -= _widgetFullRect.y;
    position.x -= _widgetFullRect.x;
    found = false;
    while(lineStartPos.y < _widgetFullRect.h)
    {
        lineStartPos.x = _textStartX * _glyphSize.x;    // coordinate where text is starting
        if(lineStartPos.y + _glyphSize.y > position.y)
        {
            // it's inside this line
            if(lineStartPos.x > position.x)
            {
                // position somewhere before text.
                // it means, that user pointed into line numbers field
                break;  // exit loop. we found everything we need
            }
            while(lineStartPos.x < _widgetFullRect.w)
            {
                if(lineStartPos.x + _glyphSize.x > position.x)
                {
                    // found x position
                    found = true;
                    break;
                }
                lineStartPos.x += _glyphSize.x;
                cursorPos.x++;
            }
        }
        if(found)
        {
            break;
        }
        lineStartPos.y += _glyphSize.y;
        cursorPos.y++;
    }
    cursorPos.y += _currentUpperLine;   // need to add logic offset with current upperline. Because we get cursorPos relative to current page, but need to convert it to full buffer size
    // here we have requested position of cursor, need to verify, that buffer exists these values
    _buffer->SetCursorPosition(cursorPos);  // set explicitly postiion of cursor. Buffer will handle by itself appropriate position to requested
}

void WidgetEditor::CalculateAvaliableLines(void)
{
    _availableLines = _widgetRect.h / _glyphSize.y;
}

void WidgetEditor::CalculateAvaliableColumns(void)
{
    _availableColumns = _widgetRect.w / _glyphSize.x;
}

void WidgetEditor::IncreaseUpperLine(int value)
{
    int linesNumber;

    linesNumber = _buffer->LinesNumber();
    _currentUpperLine += value;
    if(_currentUpperLine < 0)
    {
        _currentUpperLine = 0;
    }
    if(_currentUpperLine > linesNumber)
    {
        _currentUpperLine = linesNumber;
    }

}

void WidgetEditor::IncreaseLeftLine(int value)
{
    int columnsNumber;

    columnsNumber = _buffer->ColumnsNumber();
    _currentLeftLine += value;
    if(_currentLeftLine < 0)
    {
        _currentLeftLine  = 0;
    }
    if(_currentLeftLine > columnsNumber)
    {
        _currentLeftLine  = columnsNumber;
    }

}

Buffer * WidgetEditor::GetCurrentBuffer()
{
    return _buffer;
}
