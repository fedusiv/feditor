#include <iostream>

#include "window.hpp"

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
    DrawCursor(_layout.cursorPosition);


    SDL_RenderPresent(_renderer);
    SDL_Delay(10);
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
    CalculateLayoutArea(&(_layout.textArea));

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

    DrawEmptyLines();
    DrawLinesNumber();
}


void Window::DrawEmptyLines()
{
    SDL_Color color = _coloringValues[ColoringTypes::Text];
    for(int y = 0; y < _layout.textArea.sizeNet.y; y++ )
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

    lineNumber = 100;
    for(int y = 0; y < _layout.linesArea.sizeNet.y; y++)
    {
        auto s = std::to_string(lineNumber);
        characterPos = 0;
        for(char& c : s)
        {
            DrawCharacter((int)c, _layout.linesArea.layoutPositions[y][characterPos], color);
            characterPos++;
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
