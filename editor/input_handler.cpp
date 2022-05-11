#include <iostream>

#include "SDL.h"

#include "input_handler.hpp"

InputHandler::InputHandler(){}

Command* InputHandler::ProcessInput()
{
    Command * cmd = nullptr;

    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    if(keyState[SDL_SCANCODE_I])
    {
        cmd  = new Command(CommandType::InputMode);
    }
    else
    if(keyState[SDL_SCANCODE_ESCAPE])
    {
        cmd  = new Command(CommandType::EditMode);
    }
    else
    if(keyState[SDL_SCANCODE_RETURN])
    {
        cmd = new Command(CommandType::EnterPressed);
    }

    return cmd;
}