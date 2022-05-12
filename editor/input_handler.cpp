#include <iostream>

#include "SDL.h"

#include "input_handler.hpp"

InputHandler::InputHandler(){}

Command* InputHandler::ProcessInput(SDL_Keycode keyValue)
{
    Command * cmd = nullptr;

    switch (keyValue)
    {
        case SDLK_i:
        {
            cmd  = new Command(CommandType::InputMode);
            break;
        }
        case SDLK_ESCAPE:
        {
            cmd  = new Command(CommandType::EditMode);
            break;
        }
        case SDLK_RETURN:
        {
            cmd = new Command(CommandType::EnterPressed);
            break;
        }
        // Cursor manipulation zone
        case SDLK_UP:
        {
            CommandCursor* cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorUp);
            cmd = dynamic_cast<Command*>(cursor);
            break;
        }
        case SDLK_DOWN:
        {
            CommandCursor* cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorDown);
            cmd = dynamic_cast<Command*>(cursor);
            break;
        }
        case SDLK_LEFT:
        {
            CommandCursor* cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorLeft);
            cmd = dynamic_cast<Command*>(cursor);
            break;
        }
        case SDLK_RIGHT:
        {
            CommandCursor* cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorRight);
            cmd = dynamic_cast<Command*>(cursor);
            break;
        }
    
    default:
        break;
    }

    return cmd;
}