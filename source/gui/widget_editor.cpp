#include <math.h>
#include <iostream>

#include "SDL.h"
#include "widget_editor.hpp"

WidgetEditor::WidgetEditor(Vector2 size, Vector2 location, Buffer *buffer) : Widget(size, location), _buffer(buffer)
{
}

void WidgetEditor::Render(void)
{
    Widget::Render();

    // Get current glyph size
    _glyphSize = _glyphHandler->ElementSize();

    DrawLinesNumber();
    DrawData();

    if(_active)
    {
        // widget is active, need to draw cursor for it
        DrawCursor(CalculateRealPosForCursor());
    }

}

Vector2 WidgetEditor::CalculateRealPosForCursor()
{
    Vector2 logicPos, relativePos;

    logicPos = _buffer->CursorPosition();
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
    SDL_Color color;
    Vector2 pos;    // position to draw
    BufferLine * lineData;  // data of current line
    BufferLine::iterator iL; // iterator of line

    color = _colorStorage->GetColor(ColorWidgetEditorLineNumber);

    linesNumber = _buffer->LinesNumber();
    pos.x = _textStartX * _glyphSize.x;
    pos.y = 0;

    for(lineNumber = 0; lineNumber <= linesNumber; lineNumber++)
    {
        lineData = _buffer->LineData(lineNumber);
        for( iL = lineData->begin(); iL != lineData->end(); iL++)
        {
            DrawCharacter((*iL), pos, color);
            pos.x++;
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
    Vector2 pos;    // position to draw
    SDL_Color color;

    color = _colorStorage->GetColor(ColorWidgetEditorLineNumber);

    linesNumber = _buffer->LinesNumber();
    if(linesNumber < 1)
    {
        linesNumber = 1;
    }

    pos.x = 0;
    pos.y = 0; // draw from begginning of widget
    digitsAmount = int(log10(linesNumber) + 1);
    _textStartX = 4 + digitsAmount; // for text drawing, need to know what offset to draw text
    for(lineNumber = 1; lineNumber <= linesNumber; lineNumber++)
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
            DrawCharacter((int)(ch), pos, color);
            pos.x += _glyphSize.x;
        }

        pos.y += _glyphSize.y;   // for next step
        if(pos.y + _glyphSize.y >= _widgetSize.y)
        {
            // reached maximum avaliable size. do not draw
            break;
        }
    }
}
