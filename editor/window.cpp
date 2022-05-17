#include <iostream>

#include "window.hpp"
#include "configs.hpp"

Window::Window()
{
    RunWindow();
}

Window::~Window()
{
    // destroy renderer
    SDL_DestroyRenderer(_renderer);
    // destroy window
    SDL_DestroyWindow(_window);
    // Close ttf module
    TTF_Quit();
    // close SDL
    SDL_Quit();
}

void Window::Render()
{
    SDL_SetRenderDrawColor(_renderer,   // set background for window
    _coloringValues[ColoringTypes::Backgroud].r, 
    _coloringValues[ColoringTypes::Backgroud].g, 
    _coloringValues[ColoringTypes::Backgroud].b, 
    _coloringValues[ColoringTypes::Backgroud].a); 
    SDL_RenderClear(_renderer);

    ProcessLayouts();


    SDL_RenderPresent(_renderer);
    SDL_Delay(30);
}

void Window::RunWindow()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    if( TTF_Init() < 0 )
    {
        std::cout << "Error to Init TTF_INIT" << std::endl;
    }
    _window = SDL_CreateWindow(
        "feditor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_RESIZABLE
    );
    _renderer = SDL_CreateRenderer(_window,-1,SDL_RENDERER_ACCELERATED );
    // getting size of window
    SDL_GetWindowSize(_window, &_currentWindowSize.x, &_currentWindowSize.y);

    // obtains glyphs to draw
    _glyphHandler = new GlyphHandler(_renderer);
    _glyphHandler->GlyphBuilder(14, "./assets/FiraCode-Retina.ttf");
    // after this line we have size of one element
    // Colors configuration
    ConfigureColors();
    // Configuration of layouts
    ConfigureLayout();


}

void Window::SetCurrentBuffer(Buffer* buffer)
{
    _buffer = buffer;
    // here should happen redrawn operation of buffer
}

/*
*   For now just place holder to keep track of different colors
*/
void Window::ConfigureColors()
{
    _coloringValues[ColoringTypes::Backgroud] = SDL_Color{.r = 21, .g = 20, .b = 28, .a = 255};
    _coloringValues[ColoringTypes::Text] = SDL_Color{ .r = 100, .g = 255, .b = 150, .a = 255};
    _coloringValues[ColoringTypes::Cursor] = SDL_Color{ .r = 160, .g = 190, .b = 170, .a = 255};
    _coloringValues[ColoringTypes::Lines] = SDL_Color{ .r = 130, .g = 140, .b = 135, .a = 255};
}

void Window::ConfigureLayout()
{
    Vector2  windowSize, currentGlyphSize;

    currentGlyphSize = _glyphHandler->ElementSize();
    SDL_GetWindowSize(_window, &windowSize.x, &windowSize.y);

    _layout.mainOffset = Vector2{.x = 3, .y = 3};   // some hardcoded
    _layout.size = windowSize;

    // lines area
    _layout.linesArea.startPoint = _layout.mainOffset;
    _layout.linesArea.size.x = currentGlyphSize.x * 3;
    _layout.linesArea.size.y = _layout.size.y - _layout.mainOffset.y * 2;
    CalculateLayoutArea(&(_layout.linesArea));

    // text editing area
    _layout.textArea.startPoint.x = _layout.mainOffset.x + _layout.linesArea.size.x + 1;
    _layout.textArea.startPoint.y = _layout.mainOffset.y;
    _layout.textArea.size.x = _layout.size.x - _layout.linesArea.size.x - _layout.mainOffset.x * 2;
    _layout.textArea.size.y = _layout.size.y - _layout.mainOffset.y * 2;
    _layout.textArea.displayPoint.x = 0;
    _layout.textArea.displayPoint.y = 0;
    CalculateLayoutArea(&(_layout.textArea));

}

void Window::DrawTextEditorLayout()
{
    int upperLine, endLine, charactersToDraw;
    BufferLineType * characters;
    BufferLineType::iterator charIt, endIt;
    Vector2 netPos;    // logic postion to place glyph
    Vector2 realPos;    // real position in coordinates

    upperLine = _layout.textArea.displayPoint.y;
    endLine = upperLine + _layout.textArea.sizeNet.y;
    
    // draw all required glyphs ( text basically)
    netPos.x = 0; netPos.y = 0; // starting position to draw. Logic positions of window. Not of buffer
    while (upperLine <= endLine)
    {
        characters = _buffer->GetLineFromBuffer(upperLine);
        if(characters == nullptr)
        {
            DrawEmptyLines(netPos.y);
            netPos.y += 1;
            break;
        }
        charIt = characters->begin() + _layout.textArea.displayPoint.x;
        charactersToDraw = characters->size() - _layout.textArea.displayPoint.x;    // amount of characters can be possible drawn
        // calculate end point to draw
        if( charactersToDraw > 0 )
        {
            // if we have enough characters in line to draw after scroll
            endIt = characters->begin() + _layout.textArea.displayPoint.x + (_layout.textArea.sizeNet.x <= charactersToDraw ? _layout.textArea.sizeNet.x : charactersToDraw);
            while(charIt != endIt)
            {
                realPos = _layout.textArea.layoutPositions[netPos.y][netPos.x];
                DrawCharacter(*charIt, realPos, _coloringValues[ColoringTypes::Text]);
                netPos.x += 1;
                charIt++;
            }
        }

        netPos.x = 0;
        netPos.y += 1;
        upperLine++; // increase counter
    }

    // time to draw cursor
    netPos = _buffer->CursorPosition();
    // check are cursor even need to be dispalyed
    if(netPos.y >= _layout.textArea.displayPoint.y && netPos.y <= _layout.textArea.displayPoint.y + _layout.textArea.sizeNet.y)
    {
        if(netPos.x >= _layout.textArea.displayPoint.x && netPos.x <= _layout.textArea.displayPoint.x + _layout.textArea.sizeNet.x)
        {
            // netPos now is total logic position of cursor.
            // We need to convert it to current logic position
            netPos.y -= _layout.textArea.displayPoint.y;
            netPos.x -= _layout.textArea.displayPoint.x;

            realPos = _layout.textArea.layoutPositions[netPos.y][netPos.x];
            DrawCursor(realPos);
        }
    }

}


/*
*   x and y are real pixel place
*/
void Window::DrawCharacter(int character, Vector2 pos, SDL_Color color)
{
    SDL_Rect *glyph, dest;

    SDL_SetTextureColorMod(_glyphHandler->_fontTexture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(_glyphHandler->_fontTexture,color.a);

    glyph = &(_glyphHandler->_glyphs[character]);
    dest.x = pos.x;
    dest.y = pos.y;
    dest.w = glyph->w;
    dest.h = glyph->h;
    SDL_RenderCopy(_renderer, _glyphHandler->_fontTexture, glyph, &dest);
}

/*
* Draw in x and y pos in real coordinates
*/
void Window::DrawCursor(Vector2 pos)
{
    SDL_Rect cursor;

    cursor = {
        .h = _glyphHandler->ElementSize().y,
        .w = _glyphHandler->ElementSize().x,
        .x = pos.x,
        .y = pos.y
    };
    SDL_SetRenderDrawColor(_renderer, _coloringValues[ColoringTypes::Cursor].r, 
                            _coloringValues[ColoringTypes::Cursor].g, _coloringValues[ColoringTypes::Cursor].b
                            ,_coloringValues[ColoringTypes::Cursor].a );
    SDL_RenderFillRect(_renderer, &cursor);
}

/*
*  Draw layouts of editor
*/
void Window::ProcessLayouts()
{

    Vector2  windowSize;
    SDL_GetWindowSize(_window, &windowSize.x, &windowSize.y);

    // Detecting window size to recalculate if needed
    if(windowSize != _currentWindowSize)
    {
        // Window has changed size
        _currentWindowSize = windowSize;
        ConfigureLayout();
    }

    DrawLinesNumber();
    DrawTextEditorLayout();
}

/*
* Draw empty lines from specific line number where we already do not have any data
*/
void Window::DrawEmptyLines(int startLine)
{
    SDL_Color color = _coloringValues[ColoringTypes::Text];
    for(int y = startLine; y < _layout.textArea.sizeNet.y; y++ )
    {
        DrawCharacter('~', _layout.textArea.layoutPositions[y][0], color);
    }
}

void Window::DrawLinesNumber()
{
    int characterPos;
    int lineNumber;
    SDL_Color color = _coloringValues[ColoringTypes::Lines];
    color.a = 80;

    lineNumber = _layout.textArea.displayPoint.y + 1;
    for(int y = 0; y < _layout.linesArea.sizeNet.y; y++)
    {
        // for coloring active line
        if(lineNumber == _buffer->CursorPosition().y + 1)
        {
            color.a = 220;
        }

        auto s = std::to_string(lineNumber);
        characterPos = 0;
        for(char& c : s)
        {
            DrawCharacter((int)c, _layout.linesArea.layoutPositions[y][characterPos], color);
            characterPos++;
        }

        // for coloring active line
        // moving back alpha value
        if(lineNumber == _buffer->CursorPosition().y + 1)
        {
            color.a = 80;
        }

        lineNumber++;
    }
}

/*
* Calculating layout data for text editor area
*/
void Window::CalculateLayoutArea(LayoutArea * area)
{
    int y, x;
    Vector2 currrentElementSize, tmpVec2;

    currrentElementSize = _glyphHandler->ElementSize();

    // deallocation first
    if(area->layoutPositions != nullptr)
    {
        // deallocate
        for(y = 0; y < area->sizeNet.y; y++)
        {
            delete[] area->layoutPositions[y];
        }
        delete[] area->layoutPositions;
    }

    // assigment new size of net layout in text editor
    area->sizeNet.x = area->size.x / currrentElementSize.x;
    area->sizeNet.y = area->size.y / currrentElementSize.y;

    // allocate new Vector2 2d array
    area->layoutPositions = new Vector2* [area->sizeNet.y];
    for(y = 0; y < area->sizeNet.y; y++)
    {
        area->layoutPositions[y] = new Vector2[area->sizeNet.x];
    }

    // recalculate positions for elements
    for(y = 0; y < area->sizeNet.y; y++) // lines
    {
        tmpVec2.y = (y * currrentElementSize.y) + area->startPoint.y;
        for(x = 0; x < area->sizeNet.x; x++) // characters inside lines
        {
            tmpVec2.x = (x * currrentElementSize.x) + area->startPoint.x;
            area->layoutPositions[y][x] = tmpVec2;
        }
    }
}

/*
* Change values of representing scrolling of active buffer
*/
void Window::ScrollActiveTextEditor(ScrollWindowType type)
{
    LayoutArea * textArea;

    textArea = &(_layout.textArea);

    switch (type)
    {
        case ScrollWindowType::ScrollUp:
        {
            // trying to scroll window up
            if(textArea->displayPoint.y == 0)
            {
                break; // can not scroll upper than 0
            }
            // but we can scroll up until we reached zero.
            // cause we doing one step scrolling it will be okay
            textArea->displayPoint.y -= 1;
            break;
        }
        case ScrollWindowType::ScrollDown:
        {
            if(textArea->displayPoint.y + 1 >= _buffer->LinesAmount())
            {
                // we can not go further at least one line with data should be on the display
                break;
            }
            textArea->displayPoint.y += 1;
            break;
        }
        case ScrollWindowType::ScrollLeft:
        {
            if(textArea->displayPoint.x == 0)
            {
                // can not go more left
                break;
            }
            textArea->displayPoint.x -= 1;
            break;
        }
        case ScrollWindowType::ScrollRight:
        {
            // first need to get values of maximum size of buffer line
            if(textArea->displayPoint.x + textArea->sizeNet.x + Configs::LinesCursorReachedBeforeScroll > _buffer->MaximumLinesSize())
            {
                // in this case can not move more right.
                // we can go a little bit righter than maximum amount of characters in line from the whole buffer
                // this a little bit is spicified in configs
                break;
            }
            // otherwise go right
            textArea->displayPoint.x += 1;
            break;
        }
        
        default:
            break;
    }
}

void Window::ScrollActiveTextEditorDueCursor()
{
    int tmp;
    Vector2 cursorPos;  // position already was updated

    cursorPos = _buffer->CursorPosition();

    // cursor position is position in buffer not in layouts of text editor area. So first let's check in height
    if(cursorPos.y - Configs::LinesCursorReachedBeforeScroll < _layout.textArea.displayPoint.y)
    {
        // in this case we moved cursor upper and need to move layout higher
        tmp = cursorPos.y - Configs::LinesCursorReachedBeforeScroll;
        _layout.textArea.displayPoint.y = tmp > 0 ? tmp : 0;
    }
    else
    if( (cursorPos.y + Configs::LinesCursorReachedBeforeScroll) > (_layout.textArea.displayPoint.y + _layout.textArea.sizeNet.y) )
    {
        // in this case we moved below 
        _layout.textArea.displayPoint.y = cursorPos.y + Configs::LinesCursorReachedBeforeScroll - _layout.textArea.sizeNet.y;
    }

    // next need to work with width scrolling
    // basically it has same logic as above just for x coordinates
    if(cursorPos.x - Configs::LinesCursorReachedBeforeScroll < _layout.textArea.displayPoint.x)
    {
        tmp = cursorPos.x - Configs::LinesCursorReachedBeforeScroll;
        _layout.textArea.displayPoint.x = tmp > 0 ? tmp : 0;
    }
    else
    if( (cursorPos.x + Configs::LinesCursorReachedBeforeScroll) > (_layout.textArea.displayPoint.x + _layout.textArea.sizeNet.x) )
    {
        _layout.textArea.displayPoint.x = cursorPos.x + Configs::LinesCursorReachedBeforeScroll - _layout.textArea.sizeNet.x;
    }
    
}
