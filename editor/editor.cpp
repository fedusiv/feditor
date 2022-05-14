#include "editor.hpp"
#include "window.hpp"

#include <iostream>

Editor::Editor()
{
    Init();

    _inputHandler = new InputHandler();
    _window = new Window(); // it creates application. representt editor
    _window->SetCurrentBuffer(_bufferArray.at(_bufferId));  // set first buffer to be shown

    MainLoop(); // start main loop
}

void Editor::Init()
{
    _state = FeditorState::EditState;
    Buffer * buf = new Buffer();
    _bufferArray.push_back(buf);    // init first empty buffer
    _bufferId = 0;
}

void Editor::MainLoop()
{
    bool feditorRunState;
    bool rc;

    feditorRunState = true;
    SDL_StopTextInput();    // have no idea, but in the begging of app it starts to insert text already
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
    SDL_Keycode keyValue;
    Command * cmd;
    bool quit;

    cmd = nullptr;
    keyValue = 0;   // some unknown value
    quit = false;

    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
            {

                quit = true;
                break;
            }
            case SDL_TEXTINPUT:
            {
                // this event should appear only when any text should be inserted
                InsertText(e.text.text);
                break;
            }
            case SDL_KEYDOWN:
            {
                // in this key detect which get was pressed
                // for now it's not implemented. but in future we should react on it
                break;
            }
            case SDL_KEYUP:
            {
                // detect which key was pressed up
                keyValue = e.key.keysym.sym;
                break;
            }
        }
    }

    if(keyValue != 0)
    {
        // proccess key events
        cmd = _inputHandler->ProcessInput(keyValue);
    }

    if(cmd != nullptr)
    {
        // process commands
        CommandProcess(cmd);
        delete cmd;
    }

    return quit;
}

void Editor::CommandProcess(Command * cmd)
{
    switch (cmd->type)
    {
        case CommandType::InputMode:
            if(_state == FeditorState::EditState)
            {
                std::cout << "Moved to Insert mode" << std::endl;
                _state = FeditorState::InputTextState;
                SDL_StartTextInput();
            }
            break;
        
        case CommandType::EditMode:
            if(_state != FeditorState::EditState)
            {
                std::cout << "Moved to Edit mode" << std::endl;
                _state = FeditorState::EditState;
                SDL_StopTextInput();
            }
            break;
        case CommandType::EnterPressed:
        {
            if(_state == FeditorState::InputTextState)
            {
                // create text 
                char tmpText[] = { '\n', '\0'};
                InsertText(tmpText);
            }
            break;
        }
        case CommandType::Cursor:
        {
            // cursor parsing
            CursorOperations(static_cast<CommandCursor*>(cmd));
            break;
        }

        default:
            break;
    }
}


void Editor::InsertText(char * text)
{
    Buffer * buffer;
    Vector2 cursorPos;

    buffer = _bufferArray.at(_bufferId);
    cursorPos = buffer->CursorPosition();

    while (*text != '\0') 
    {
        buffer->Append( static_cast<int>(*text), cursorPos);
        if( *text == '\n')
        {
            cursorPos.y += 1;
            cursorPos.x = 0;
        }
        else
        {
            cursorPos.x += 1;
        }

        text++;
    }

    buffer->SetCursorPosition(cursorPos);   // this is logical (virtual cursor position of buffer)

}

void Editor::CursorOperations(CommandCursor * cmd)
{
    Vector2 cursorPos;
    Buffer * buffer;
    std::vector<int> * line;

    buffer = _bufferArray.at(_bufferId);
    cursorPos = buffer->CursorPosition();

    switch (cmd->cursorType)
    {
        case CursorCommandType::CursorUp:
        {
            if(cursorPos.y == 0)
            {   // can not go upper
                break;
            }
            cursorPos.y -= 1;   // increase cursor position, increase in matter of geometric, go upper
            line = buffer->GetLineFromBuffer(cursorPos.y);
            // this is basically should not happen, but who knows why we should have data on line 2 and no on line 1. Next line symbol is data btw
            if(line == nullptr)
            {
                // no next line, go to begin of line
                cursorPos.x = 0;
            }
            else if(line->size() < cursorPos.x)
            {
                // if next line has less size need to go to latest position in line
                cursorPos.x = line->size();
            }
            buffer->SetCursorPosition(cursorPos);
            break;
        }
        case CursorCommandType::CursorDown:
        {
            cursorPos.y += 1;   // increase cursor position, increase line
            line = buffer->GetLineFromBuffer(cursorPos.y);
            if(line == nullptr)
            {
                // no next line, go to begin of line
                cursorPos.x = 0;
            }
            else if(line->size() < cursorPos.x)
            {
                // if next line has less size need to go to latest position in line
                cursorPos.x = line->size();
            }
            buffer->SetCursorPosition(cursorPos);
            break;
        }
        case CursorCommandType::CursorLeft:
        {
            if(cursorPos.x == 0)
            {
                break; // can not got to the left
            }
            line = buffer->GetLineFromBuffer(cursorPos.y);
            if(line == nullptr)
            {
                // line does not exist. It can be. But should break upper in code, anyway let's add this verification
                break;
            }
            cursorPos.x -= 1;
            buffer->SetCursorPosition(cursorPos);
            break;
        }
        case CursorCommandType::CursorRight:
        {
            line = buffer->GetLineFromBuffer(cursorPos.y);
            if(line == nullptr)
            {
                // if no data can not move
                break;
            }
            if( cursorPos.x < line->size())
            {
                // cursor can move to one symbol upper after maximum size of line
                cursorPos.x += 1;
                buffer->SetCursorPosition(cursorPos);
            }
            else
            {
                // if biffer can not move
                break;
            }
        }
        
        default:
            break;
    }

}