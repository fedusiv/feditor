#ifndef __GUI_HANDLER_HPP__
#define __GUI_HANDLER_HPP__

#include "window.hpp"
#include "../common/buffer.hpp"

class GuiHandler
{

    public:
        GuiHandler();
        void Render();

        void CreateWidgetEditor(Buffer * buffer);

    private:
        Window * _window;

};

#endif // __GUI_HANDLER_HPP__