#include <iostream>
#include "core/editor.hpp"
#include "gui/gui_handler.hpp"
#include "input/input_handler.hpp"


int main()
{
    InputHandler in = InputHandler();
    GuiHandler gui = GuiHandler();
    Editor feditor = Editor(&gui, &in);

    return 0;
}
