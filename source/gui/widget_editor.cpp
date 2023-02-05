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
    DrawLinesNumber();
}

void WidgetEditor::DrawLinesNumber(void)
{
    int linesNumber; // whole amount of lines
    int digitsAmount;   // maximum digits amount if lines numbering
    int lineNumber; // current lineNumber
    Vector2 pos;    // position to draw
    Vector2 glyphSize;
    SDL_Color color;

    color = _colorStorage->GetColor(ColorWidgetEditorLineNumber);
    glyphSize = _glyphHandler->ElementSize();

    linesNumber = _buffer->LinesNumber();
    if(linesNumber < 1)
    {
        linesNumber = 1;
    }

    pos.x = 0;
    pos.y = 0; // draw from begginning of widget
    digitsAmount = int(log10(linesNumber) + 1);
    _textStartY = 4 + digitsAmount; // for text drawing, need to know what offset to draw text
    for(lineNumber = 1; lineNumber <= linesNumber; lineNumber++)
    {
        pos.x = 0;  // clear x position
        // draw line with line number. it should at least two character before first digit, and at least two characters after
        auto s = std::to_string(lineNumber);
        // calculate first position to draw. we have at least two character before
        // but if number is less, means we need to leave some space to align on right corner
        // digitsAmount - size of string will get amount of additional space
        pos.x += (2 + digitsAmount - s.size()) * glyphSize.x;
        for(auto ch : s)
        {
            DrawCharacter((int)(ch), pos, color);
            pos.x += glyphSize.x;
        }

        pos.y += glyphSize.y;   // for next step
        if(pos.y + glyphSize.y >= _widgetSize.y)
        {
            // reached maximum avaliable size. do not draw
            break;
        }
    }
}
