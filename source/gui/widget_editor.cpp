#include <math.h>
#include <iostream>

#include "vec2.hpp"
#include "widget_editor.hpp"

WidgetEditor::WidgetEditor(Rect rect, Buffer *buffer) : Widget(rect), _buffer(buffer)
{
    _drawingOffset = Vec2(0,5);
    _currentUpperLine = 0;
    _linesPageMoveOffset = 1;
    CalculateAvaliableLines();

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
        DrawCursor(CalculateRealPosForCursor());
    }

}

Vec2 WidgetEditor::CalculateRealPosForCursor()
{
    Vec2 logicPos, relativePos;

    logicPos = _buffer->CursorPosition();
    logicPos.y -= _currentUpperLine;
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
    Vec2 pos;    // position to draw
    BufferLine * lineData;  // data of current line
    BufferLine::iterator iL; // iterator of line

    pos.x = _textStartX * _glyphSize.x;
    pos.y = 0;

    linesNumber = _buffer->LinesNumber();   // whole amount of lines in file
    if(linesNumber - _currentUpperLine > _availableLines)   // calculate last line number in the current page
    {
        linesNumber = _availableLines + _currentUpperLine;
    }

    for(lineNumber = _currentUpperLine + 1; lineNumber < linesNumber; lineNumber++)
    {
        lineData = _buffer->LineData(lineNumber);
        if(lineData == nullptr)
        {
            // no lines left to draw. Or lines just empty
            continue;
        }
        for( iL = lineData->begin(); iL != lineData->end(); iL++)
        {
            DrawCharacter((*iL), pos, ColorPurpose::ColorWidgetEditorText);
            pos.x+= _glyphSize.x;
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
    int linesNumber;

    cursorPos = _buffer->CursorPosition();
    if(cursorPos == _cursorPrevPos)
    {
        return; // no need to update paging
    }

    linesNumber =  _buffer->LinesNumber();


    if(cursorPos.y - _linesPageMoveOffset < _currentUpperLine)
    {   // This is moving page up
        _currentUpperLine--; // moving line by one step
    }
    if(cursorPos.y + _linesPageMoveOffset >= _currentUpperLine + _availableLines)
    {   // This is moving page down
        _currentUpperLine++; // moving line by one step
    }

    // Making borders of it
    if(_currentUpperLine < 0)
    {
        _currentUpperLine = 0;
    }
    if(_currentUpperLine > linesNumber)
    {
        _currentUpperLine = linesNumber;
    }

    _cursorPrevPos = cursorPos;

}

void WidgetEditor::CalculateAvaliableLines(void)
{
    _availableLines = _widgetRect.h / _glyphSize.y;
}