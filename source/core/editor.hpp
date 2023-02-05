#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "../gui/gui_handler.hpp"
#include "../input/input_handler.hpp"

class Editor
{
    public:
        Editor(GuiHandler * gui, InputHandler * input);

    private:
        void Init(void);
        void MainLoop(void);


        GuiHandler * _guiHandler;
        InputHandler * _inputHandler;

};

#endif //__EDITOR_HPP__