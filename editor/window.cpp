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
    SDL_Color color = {.r = 50, .g = 100, .b = 150};
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); // Make window bg black.
    SDL_RenderClear(_renderer);

    DrawText("Hello world", 50, 50, color);
    //DrawTextTest();
    DrawTestRect();
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
        SDL_WINDOW_SHOWN
    );
    _renderer = SDL_CreateRenderer(_window,-1,SDL_RENDERER_ACCELERATED );

    glyphHandler = new GlyphHandler(_renderer);
    glyphHandler->GlyphBuilder(10, "/Users/ilafedusiv/prj/cplus/feditor/res/FiraCode-Retina.ttf");

}

void Window::DrawText(std::string text, int x, int y, SDL_Color color)
{
    int i, character;
    SDL_Rect *glyph, dest;

    SDL_SetTextureColorMod(glyphHandler->_fontTexture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(glyphHandler->_fontTexture, 0);

    i = 0;

    character = text.c_str()[i++];

    while (character)
    {
        glyph = &(glyphHandler->_glyphs[i]);

        dest.x = x;
        dest.y = y;
        dest.w = glyph->w;
        dest.h = glyph->h;

        SDL_RenderCopy(_renderer, glyphHandler->_fontTexture, glyph, &dest);

        x += glyph->w;

        character = text.c_str()[i++];
    }
}

void Window::DrawTextTest()
{
    SDL_Surface * TextSurface;
    SDL_Rect TextRect;
    SDL_Texture* TextTexture;
    SDL_Color color = {.r = 50, .g = 100, .b = 150, .a = 255};
    const char Message[] = "Test msg";
    TTF_Font *font = TTF_OpenFont("/Users/ilafedusiv/prj/cplus/feditor/res/FiraCode-Retina.ttf", 12);
    if (!font)
        std::cout << "Couldn't find/init open ttf font." << std::endl;
    TextSurface = TTF_RenderText_Solid(font, Message, color);
    TextTexture = SDL_CreateTextureFromSurface(_renderer, TextSurface);
    TextRect.x = 800 - TextSurface->w * 0.5; // Center horizontaly
    TextRect.y = 600 - TextSurface->h * 0.5; // Center verticaly
    TextRect.w = TextSurface->w;
    TextRect.h = TextSurface->h;
    // After you create the texture you can release the surface memory allocation because we actually render the texture not the surface.
    SDL_FreeSurface(TextSurface);
    TTF_CloseFont(font);
    SDL_RenderCopy(_renderer, TextTexture, NULL, &TextRect);
}

void Window::DrawTestRect()
{
    SDL_Rect rect;

    rect.h = 100;
    rect.w = 150;
    rect.x = 400;
    rect.y = 300;

    SDL_SetRenderDrawColor( _renderer, 0, 0, 255, 255 );
    SDL_RenderFillRect( _renderer, &rect );
}