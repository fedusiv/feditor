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
    DrawText("Hello~", 100, 100, color);
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

    glyphHandler = new GlyphHandler(_renderer);
    glyphHandler->GlyphBuilder(14, "./assets/FiraCode-Retina.ttf");

}

void Window::DrawText(std::string text, int x, int y, SDL_Color color)
{
    int i, character;
    SDL_Rect *glyph, dest;

    SDL_SetTextureColorMod(glyphHandler->_fontTexture, color.r, color.g, color.b);

    i = 0;

    character = text.c_str()[i++];

    while (character)
    {
        glyph = &(glyphHandler->_glyphs[character]);
        dest.x = x;
        dest.y = y;
        dest.w = glyph->w;
        dest.h = glyph->h;

        SDL_RenderCopy(_renderer, glyphHandler->_fontTexture, glyph, &dest);

        x += glyph->w;

        character = text.c_str()[i++];
    }
}

void Window::DrawCursor(int x, int y, SDL_Color color)
{
    SDL_Rect cursor;

    cursor = {
        .h = glyphHandler->ElementSize().y,
        .w = glyphHandler->ElementSize().x,
        .x = x,
        .y = y
    };
    SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a );
    SDL_RenderFillRect(_renderer, &cursor);
}