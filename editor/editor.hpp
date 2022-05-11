#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include <vector>

#include "window.hpp"
#include "input_handler.hpp"
#include "buffer.hpp"

class Editor
{
    enum class FeditorState
    {
        EditState,
        InputTextState
    };

    public:
        Editor();

    private:
        Window * _window;
        InputHandler * _inputHandler;
        FeditorState _state;
        std::vector<Buffer*> _bufferArray;   // array of all buffers

        int _bufferId;  // current buffer id, which is shown

        void Init(void);
        void MainLoop(void);    // Main loop of application
        bool PollingProcess(void);  // Polling process of sdl events. Basically input recognition
        void CommandProcess(Command * cmd); // parsing and making decision based on command, which user has entered
        void InsertText(char * text); // inserting text to somewhere

};

#endif //__EDITOR_HPP__