#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include <vector>
#include <list>

#include "window.hpp"
#include "input_handler.hpp"
#include "buffer.hpp"

class Editor
{
    // This fields is private by default.
    // In the begging I decided to write 
    enum class FeditorState
    {
        EditState,
        InputTextState
    };

    struct
    {
        std::list<SDL_KeyboardEvent> * events;  // pointer to list, which holds data about keys, which were pressed
        std::vector<int> * amountOfPressed; // amount of times it was pressed.
        // need to describe this mechanism.
        // When you hold one key it should react immidiately, but if you still hold, so it means you want this to continue react.
        // good example is moving cursor, once it pressed it will move cursor, but after some time it will moving it faster
        // anyway this mechanism is to make controlls move smooth and nicer
    }_kEvents;

    struct 
    {
        float averageRenderTime;  // average time required to render (after low pass filter)
        Uint64 lastRenderTime;  // on what tick value was rendered window
        float averageFrameRate;
    }_frameRate;
    

    public:
        Editor();

    private:
        Window * _window;
        InputHandler * _inputHandler;
        FeditorState _state;    // keeps current state of feditor at all. Is it in Insert mode or Edit mode or whatever mode
        std::vector<Buffer*> _bufferArray;   // array of all buffers
        

        Buffer * _bufferActive;
        int _bufferId;  // current buffer id, which is shown (active buffer)


        void Init(void);
        void MainLoop(void);    // Main loop of application
        void Render(void);  // render function
        bool PollingProcess(void);  // Polling process of sdl events. Basically input recognition
        void CommandProcess(Command * cmd); // parsing and making decision based on command, which user has entered
        void InsertText(char * text); // inserting text to somewhere
        void InsertNewLine(void);   // insert new line
        void CursorOperations(CommandCursor * cmd);  // process only cursor operations
        void UpdateBufferCursorPosition(Vector2 pos);   // changing position of buffer in text editor
        void ScrollWindowOperations(CommandScroll * cmd);   // process commands to scroll window

};

#endif //__EDITOR_HPP__