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
    SDL_Color color = { .r = 100, .g = 255, .b = 150};
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(_renderer);

    ProcessBorders();
    DrawCursor(0,0, color);


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


    _glyphHandler = new GlyphHandler(_renderer);
    _glyphHandler->GlyphBuilder(14, "./assets/FiraCode-Retina.ttf");
    // after this line we have size of one element
    CalculateLinesAmount(); // calculate in the beginning what we have to know size of lines

}

void Window::DrawText(std::string text, int x, int y, SDL_Color color)
{
    int i, character;
    SDL_Rect *glyph, dest;

    SDL_SetTextureColorMod(_glyphHandler->_fontTexture, color.r, color.g, color.b);

    i = 0;

    character = text.c_str()[i++];

    while (character)
    {
        glyph = &(_glyphHandler->_glyphs[character]);
        dest.x = x;
        dest.y = y;
        dest.w = glyph->w;
        dest.h = glyph->h;

        SDL_RenderCopy(_renderer, _glyphHandler->_fontTexture, glyph, &dest);

        x += glyph->w;

        character = text.c_str()[i++];
    }
}

/*
*   x and y are real pixel place
*/
void Window::DrawCharacter(int character, Vector2 pos, SDL_Color color)
{
    SDL_Rect *glyph, dest;

    SDL_SetTextureColorMod(_glyphHandler->_fontTexture, color.r, color.g, color.b);

    glyph = &(_glyphHandler->_glyphs[character]);
    dest.x = pos.x;
    dest.y = pos.y;
    dest.w = glyph->w;
    dest.h = glyph->h;
    SDL_RenderCopy(_renderer, _glyphHandler->_fontTexture, glyph, &dest);
}

void Window::DrawCursor(int x, int y, SDL_Color color)
{
    SDL_Rect cursor;

    cursor = {
        .h = _glyphHandler->ElementSize().y,
        .w = _glyphHandler->ElementSize().x,
        .x = x,
        .y = y
    };
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a );
    SDL_RenderFillRect(_renderer, &cursor);
}

/*
* Drawing borders.
* If need recalculate amount of lines need to be printed and etc
*/
void Window::ProcessBorders()
{

    Vector2  windowSize;
    SDL_GetWindowSize(_window, &windowSize.x, &windowSize.y);

    if(windowSize != _currentWindowSize)
    {
        // Window has changed size
        _currentWindowSize = windowSize;
        CalculateLinesAmount();
    }

    DrawEmptyLines();



}

void Window::DrawEmptyLines()
{
    SDL_Color color = { .r = 100, .g = 255, .b = 150};
    for(int y = 0; y < _layout.size.y; y++ )
    {
        DrawCharacter('~', _layout.layoutPositions[y][0], color);
    }
}

void Window::CalculateLinesAmount()
{
    int y,x;
    Vector2 currrentElementSize, tmpVec2;

    currrentElementSize = _glyphHandler->ElementSize();

    // deallocation first
    if(_layout.layoutPositions != nullptr)
    {
        // deallocate
        for(y = 0; y < _layout.size.y; y++)
        {
            delete[] _layout.layoutPositions[y];
        }
        delete[] _layout.layoutPositions;
    }

    // assigment new size of layout
    _layout.size.x = _currentWindowSize.x / (currrentElementSize.x + 1);
    _layout.size.y = _currentWindowSize.y / (currrentElementSize.y + 1);

    // allocate new Vector2 2d array
    _layout.layoutPositions = new Vector2* [_layout.size.x];
    for(y = 0; y < _layout.size.y; y++)
    {
        _layout.layoutPositions[y] = new Vector2[_layout.size.x];
    }

    // recalculate positions for elements
    for(y = 0; y < _layout.size.y; y++) // lines
    {
        tmpVec2.y = y * currrentElementSize.y + 1; // 2 is offset
        for(x = 0; x < _layout.size.x; x++) // characters inside lines
        {
            tmpVec2.x = x * currrentElementSize.x;
            _layout.layoutPositions[y][x] = tmpVec2;
        }
    }

}