#include <iostream>

#include "window.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"
#include "vector2.hpp"
#include "glyph_handler.hpp"
#include "colors.hpp"

Window::Window()
{
    _colorStorage = Colors::Instance();
    TTF_Init();
    CreateWindow();
}

Window::~Window()
{
    // destroy renderer
    SDL_DestroyRenderer(_sdlRenderer);
    // destroy window
    SDL_DestroyWindow(_sdlWindow);
    // Close ttf module
    TTF_Quit();
    // close SDL
    SDL_Quit();
}

void Window::Render()
{
    SDL_Color bgColor;

    bgColor = _colorStorage->GetColor(ColorPurpose::ColorWindowBg);
    SDL_SetRenderDrawColor(_sdlRenderer,   // set background for window
    bgColor.r,
    bgColor.g,
    bgColor.b,
    bgColor.a);
    SDL_RenderClear(_sdlRenderer);

    for(auto widget: _widgets)
    {
        widget->Render();
    }

    SDL_RenderPresent(_sdlRenderer);
}

void Window::CreateWidgetEditor(Buffer *buffer)
{
    Vector2 size, location;
    typedef std::pair<int,int> vp;

    size = vp(1276,714);
    location = vp(2,2);
    WidgetEditor * widget = new WidgetEditor(size, location, buffer);

    _widgets.push_back(widget);
}

void Window::CreateWindow()
{
    if( SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        std::cout << "Failed to initialize the SDL2 library\n";
    }

    _sdlWindow = SDL_CreateWindow(
        "feditor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,
        720,
        SDL_WINDOW_RESIZABLE
    );
    _sdlRenderer = SDL_CreateRenderer(_sdlWindow,-1,SDL_RENDERER_ACCELERATED);  // create renderer, where everything will be drawn
    Widget::SetSdlRenderer(_sdlRenderer);   // set renderer for all widgets
    // set glyphs
    _glyphHandler = GlyphHandler::Instance();
    _glyphHandler->SetRenderer(_sdlRenderer);
    _glyphHandler->GlyphBuilder(16, "./assets/FiraCode-Retina.ttf");

    // Creating widgets
    InitWidgets();
}

void Window::InitWidgets(void)
{

}
