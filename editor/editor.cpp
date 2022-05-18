#include "editor.hpp"
#include "window.hpp"
#include "configs.hpp"

#include <iostream>

Editor::Editor()
{
    Init();

    _inputHandler = new InputHandler();
    _window = new Window(); // it creates application. representt editor
    _window->SetCurrentBuffer(_bufferArray.at(_bufferId));  // set first buffer to be shown
    _window->ConfigureLayout(); // need to call configure layout only after attaching buffer

    // init keyboards events mechanism holder
    _kEvents.amountOfPressed = new std::vector<int>();
    _kEvents.events = new std::list<SDL_KeyboardEvent>();

    MainLoop(); // start main loop
}

void Editor::Init()
{
    _state = FeditorState::EditState;
    Buffer * buf = new Buffer();
    _bufferArray.push_back(buf);    // init first empty buffer
    _bufferId = 0;
    _bufferActive = _bufferArray.at(_bufferId); // set activve buffer
    memset(&_frameRate,0, sizeof(_frameRate));
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
            Render();
        }
    }
}

void Editor::Render()
{
    Uint64 currentTime, endTime, delta;
    //float renderWaitingTime;    // time we need to wait until render will happen


    currentTime = SDL_GetTicks64();
    delta = currentTime - _frameRate.lastRenderTime;

    // need to render not faster than required frame rate
    // and also tyring to fit into required framerate
    // TODO: implement this
    if( delta >= (1000 / Configs::DefaultFrameRate) )
    {
        _window->Render();
        endTime = SDL_GetTicks64();
        _frameRate.lastRenderTime = endTime;

        endTime -= currentTime;   // calculate current required time to render
        if (_frameRate.averageRenderTime == 0)
        {
            // first render time
            _frameRate.averageRenderTime = endTime;
        }
        else
        {
            _frameRate.averageFrameRate += ( ( 1000 / delta) - _frameRate.averageFrameRate) / Configs::SampleRateForFrameRate ;
            _frameRate.averageRenderTime += ( (endTime - _frameRate.averageRenderTime) / Configs::SampleRateForFrameRate );
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
                keyValue = e.key.keysym.sym; // store value to know, do we need to call parser of inputs
                _inputHandler->KeyPressed(e.key);
                break;
            }
            case SDL_KEYUP:
            {
                keyValue = e.key.keysym.sym;    // store value to know, do we need to call parser of inputs
                _inputHandler->KeyReleased(e.key);
                break;
            }
        }
    }

    if(keyValue != 0)
    {
        // proccess key events
        cmd = _inputHandler->ProcessInput();
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
                // need to insert new line
                InsertNewLine();
            }
            break;
        }
        case CommandType::Cursor:
        {
            // cursor parsing
            CursorOperations(static_cast<CommandCursor*>(cmd));
            break;
        }
        case CommandType::Scroll:
        {
            ScrollWindowOperations(static_cast<CommandScroll*>(cmd));
            break;
        }

        default:
            break;
    }
}


void Editor::InsertText(char * text)
{
    Vector2 cursorPos;

    cursorPos = _bufferActive->CursorPosition();

    while (*text != '\0')
    {
        _bufferActive->Append( static_cast<int>(*text), cursorPos);
        cursorPos.x += 1;

        text++;
    }

    UpdateBufferCursorPosition(cursorPos);   // this is logical (virtual cursor position of buffer)

}

/*
* Function just operates inserting new line to text editor area
*/
void Editor::InsertNewLine()
{
    Vector2 cursorPos;
    
    cursorPos = _bufferActive->CursorPosition();
    // when we inserting new line, we need to move all other lines to one down
    _bufferActive->InsertNewLine(cursorPos);    // call operations inside buffer
    // change position of cursor
    cursorPos.y += 1;
    cursorPos.x = 0;
    UpdateBufferCursorPosition(cursorPos);
}

void Editor::CursorOperations(CommandCursor * cmd)
{
    Vector2 cursorPos;
    BufferLineType * line;

    cursorPos = _bufferActive->CursorPosition();

    switch (cmd->cursorType)
    {
        case CursorCommandType::CursorUp:
        {
            if(cursorPos.y == 0)
            {   // can not go upper
                break;
            }
            cursorPos.y -= 1;   // increase cursor position, increase in matter of geometric, go upper
            line = _bufferActive->GetLineFromBuffer(cursorPos.y);
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
            UpdateBufferCursorPosition(cursorPos);
            break;
        }
        case CursorCommandType::CursorDown:
        {
            cursorPos.y += 1;   // increase cursor position, increase line
            line = _bufferActive->GetLineFromBuffer(cursorPos.y);
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
            UpdateBufferCursorPosition(cursorPos);
            break;
        }
        case CursorCommandType::CursorLeft:
        {
            if(cursorPos.x == 0)
            {
                break; // can not got to the left
            }
            line = _bufferActive->GetLineFromBuffer(cursorPos.y);
            if(line == nullptr)
            {
                // line does not exist. It can be. But should break upper in code, anyway let's add this verification
                break;
            }
            cursorPos.x -= 1;
            UpdateBufferCursorPosition(cursorPos);
            break;
        }
        case CursorCommandType::CursorRight:
        {
            line = _bufferActive->GetLineFromBuffer(cursorPos.y);
            if(line == nullptr)
            {
                // if no data can not move
                break;
            }
            if( cursorPos.x < line->size())
            {
                // cursor can move to one symbol upper after maximum size of line
                cursorPos.x += 1;
                UpdateBufferCursorPosition(cursorPos);
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

void Editor::UpdateBufferCursorPosition(Vector2 pos)
{
    _bufferActive->SetCursorPosition(pos);
    // updated position of cursor
    // now need to updated text editor window representation due to this changes
    _window->ScrollActiveTextEditorDueCursor();
}

void Editor::ScrollWindowOperations(CommandScroll * cmd)
{
    _window->ScrollActiveTextEditor(cmd->scrollType);
}