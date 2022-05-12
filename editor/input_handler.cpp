#include <iostream>

#include "SDL.h"

#include "input_handler.hpp"

InputHandler::InputHandler(){}

Command* InputHandler::ProcessInput(SDL_Keycode keyValue)
{
    Command * cmd = nullptr;

    if(keyValue == SDLK_i)
    {
        cmd  = new Command(CommandType::InputMode);
    }
    else
    if(keyValue == SDLK_ESCAPE)
    {
        cmd  = new Command(CommandType::EditMode);
    }
    else
    if(keyValue == SDLK_RETURN)
    {
        cmd = new Command(CommandType::EnterPressed);
    }

    return cmd;
}