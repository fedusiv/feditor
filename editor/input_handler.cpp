#include <iostream>

#include "SDL.h"

#include "input_handler.hpp"
#include "configs.hpp"

InputHandler::InputHandler()
{
    _kEvents = new std::list<KeyEvent>(); // initialize pointer
    _keysToAct = new std::list<SDL_Keycode>();
}

void InputHandler::KeyPressed(SDL_KeyboardEvent event)
{
    bool found;
    int timeToCompare;

    found = false;


    // Key was pressed or still om pressing state
    // Check is it was pressed before
    for(auto e: *_kEvents)
    {
        if(e.event.keysym.sym == event.keysym.sym)
        {
            found = true;
            // key was already pressed and still is pressing
            if(e.pressedAmount == 1)
            {
                timeToCompare = Configs::FirstKeyPress;
            }
            else
            {
                timeToCompare = Configs::SecondKeyPress;
            }

            if( (event.timestamp - e.event.timestamp) >= timeToCompare )
            {
                e.event.timestamp = event.timestamp;    // save previous timestamp time
                e.pressedAmount += 1;
                // put key to be acted
                AddKeyToAct(event.keysym.sym);
            }

            break; // exit searching loop
        }
    }

    if(!found)
    {
        // insert new key
        auto ke = KeyEvent(event);
        _kEvents->push_back(ke);
        AddKeyToAct(event.keysym.sym);

    }
}

void InputHandler::AddKeyToAct(SDL_Keycode key)
{
    bool found;

    found = (std::find(_keysToAct->begin(), _keysToAct->end(), key) != _keysToAct->end());
    if(!found)
    {
        _keysToAct->push_back(key);
    }
}
/*
* Once key released it should be removed from all lists
*/
void InputHandler::KeyReleased(SDL_KeyboardEvent event)
{
    SDL_Keycode code;   // code of event which should operate
    std::list<KeyEvent>::iterator keIt; // key ivent iterator
    std::list<SDL_Keycode>::iterator kcIt;  // key code to act iterator

    code = event.keysym.sym;

    for(keIt = _kEvents->begin(); keIt != _kEvents->end();)
    {
        if((*keIt).event.keysym.sym == code)
        {
            // found iterator which should be removed
            keIt = _kEvents->erase(keIt);  // remove from events
            break;
        }
        else
        {
            keIt++;
        }

    }

    // now remove everything from keys to act
    for(kcIt = _keysToAct->begin(); kcIt != _keysToAct->end();)
    {
        if(*kcIt == code)
        {
            kcIt = _keysToAct->erase(kcIt);
        }
        else
        {
            kcIt++;
        }
    }

    // remove everything from list
}

/*
* Parsing commands only related to operations in edit mode
*/
KeyParseResult_t InputHandler::ParsingKeysEditor(KeyCodesList_t::iterator &iterator)
{
    Command * cmd = nullptr;
    KeyCodesList_t::iterator endit;

    endit = iterator;

    switch(*iterator)
    {

    // Moving to Insert Mode
    case SDLK_i:
    {
        cmd = new Command(CommandType::InputMode);
        break;
    }
    // Cursor manipulation zone
    // Alternative buttons to manipulate cursor movements
    case SDLK_o:
    {
        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorUp);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    case SDLK_l:
    {

        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorDown);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    case SDLK_k:
    {
        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorLeft);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    case SDLK_SEMICOLON:
    {
        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorRight);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    // Scrolling text editor
    case SDLK_s:
    {
        // scroll manipulation
        endit++;
        switch (*endit)
        {
        case SDLK_UP:
        {
            CommandScroll *scroll = new CommandScroll(CommandType::Scroll, ScrollWindowType::ScrollUp);
            cmd = dynamic_cast<Command *>(scroll);
            break;
        }
        case SDLK_DOWN:
        {
            CommandScroll *scroll = new CommandScroll(CommandType::Scroll, ScrollWindowType::ScrollDown);
            cmd = dynamic_cast<Command *>(scroll);
            break;
        }
        case SDLK_LEFT:
        {
            CommandScroll *scroll = new CommandScroll(CommandType::Scroll, ScrollWindowType::ScrollLeft);
            cmd = dynamic_cast<Command *>(scroll);
            break;
        }
        case SDLK_RIGHT:
        {
            CommandScroll *scroll = new CommandScroll(CommandType::Scroll, ScrollWindowType::ScrollRight);
            cmd = dynamic_cast<Command *>(scroll);
            break;
        }
        default:
            break;
        }
        break;
    }

    default:
        break;
    }

    return std::make_tuple(cmd, endit);
}

KeyParseResult_t InputHandler::ParsingKeysInsert(KeyCodesList_t::iterator &iterator)
{
    Command * cmd = nullptr;
    KeyCodesList_t::iterator endit;

    endit = iterator;

    switch (*iterator)
    {
    // Backspace is for deleting characters
    case SDLK_BACKSPACE:
    {
        cmd = new Command(CommandType::BackspacePressed);
        break;
    }
    
    default:
        break;
    }

    return std::make_tuple(cmd, endit);
}

KeyParseResult_t InputHandler::ParsingKeysCommon(KeyCodesList_t::iterator &iterator)
{
    Command * cmd = nullptr;
    KeyCodesList_t::iterator endit;

    endit = iterator;

    switch (*iterator)
    {
    // Moving to edit mode
    case SDLK_ESCAPE:
    {
        cmd = new Command(CommandType::EditMode);
        break;
    }
    // Pressing Enter
    case SDLK_RETURN:
    {
        cmd = new Command(CommandType::EnterPressed);
        break;
    }
    // Cursor manipulation zone
    case SDLK_UP:
    {
        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorUp);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    case SDLK_DOWN:
    {

        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorDown);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    case SDLK_LEFT:
    {
        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorLeft);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }
    case SDLK_RIGHT:
    {
        CommandCursor *cursor = new CommandCursor(CommandType::Cursor, CursorCommandType::CursorRight);
        cmd = dynamic_cast<Command *>(cursor);
        break;
    }

    default:
        break;
    }

    return std::make_tuple(cmd, endit);
}

Command* InputHandler::ProcessInput(InputModes mode)
{
    Command * cmd = nullptr;
    KeyCodesList_t::iterator it;
    std::list<KeyCodesList_t::iterator> iteratorsToRemove;
    KeyParseResult_t res;

    // all combination starts with first pressed button, so it's important operate first pressed key event
    it = _keysToAct->begin();
    // first need to parse specific, then common
    switch (mode)
    {
    case InputModes::InsertInputMode :
        res = ParsingKeysInsert(it);
        break;
    case InputModes::EditInputMode:
        res = ParsingKeysEditor(it);
        break;
    default:
        break;
    }
    cmd = std::get<0>(res);
    if(cmd == nullptr)  // no specific cmd is found. parse from common
    {
        res = ParsingKeysCommon(it);
        cmd = std::get<0>(res);
    }

    if(cmd != nullptr)
    {
        // command was executed
        // TODO: for now it works only with 2 keys size combinations, need to make for 3+
        _keysToAct->erase(std::get<1>(res));    // erase from act by iterators
    }

    return cmd;    // return cmd pointer
}