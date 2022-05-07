#include "editor.hpp"
#include "window.hpp"

Editor::Editor()
{
    _window = new Window();
    MainLoop();
}

void Editor::MainLoop()
{
    bool feditorRunState;
    bool rc;

    feditorRunState = true;
    while(feditorRunState)
    {
        rc = PollingProcess();
        if(rc)
        {
            delete _window;
            feditorRunState = false;
        }
        else
        {
            _window->Render();
        }
    }
}


bool Editor::PollingProcess()
{
    SDL_Event e;
    bool quit = false;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            quit = true;
        }
        if (e.type == SDL_KEYDOWN)
        {
            
        }
    }

    return quit;
}