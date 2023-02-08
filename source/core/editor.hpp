#ifndef __EDITOR_HPP__
#define __EDITOR_HPP__

#include "../gui/gui_handler.hpp"
#include "../input/input_handler.hpp"
#include "executor.hpp"
#include "editor_state.hpp"
#include "insert_module.hpp"

class Editor
{
    public:
        Editor(GuiHandler * gui, InputHandler * input);

    private:
        void Init(void);
        void MainLoop(void);
        bool InputParsing(void);


        GuiHandler * _guiHandler;
        InputHandler * _inputHandler;
        Executor * _executor;

        InsertModule * _insertModule;

        EditorState _editorState;

};

#endif //__EDITOR_HPP__
