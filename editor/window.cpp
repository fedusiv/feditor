#include "window.hpp"
#include "SDL.h"

Window::Window()
{
    RunWindow();
}

Window::~Window()
{

}

void Window::PollingProcess()
{
    SDL_Event e;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&e)){
            if (e.type == SDL_QUIT){
                quit = true;
            }
            if (e.type == SDL_KEYDOWN){
                quit = true;
            }
        }
    }
}

void Window::RunWindow()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window * window = SDL_CreateWindow(
        "feditor",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        600,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer * render = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED );

    PollingProcess();

    // destroy renderer
    SDL_DestroyRenderer(render);
 
    // destroy window
    SDL_DestroyWindow(window);
     
    // close SDL
    SDL_Quit();
}