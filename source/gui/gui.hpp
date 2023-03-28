#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <list>
#include "widget.hpp"
#include "buffer.hpp"
#include "widget_statusline.hpp"


class Gui
{

    public:
        Gui();
        ~Gui();
        void Update(void);  // main function of gui process
        void CreateWindow(void);    // create main window
        void CreateWidgetEditor(Buffer * buffer);  // create empty widget for edititng text
        bool NeedExit(void);    // report when application need to be exited
        void RequestExit(void); // request to close app
        void Resize(void);      // resize request
        void SetEditorState(EditorState state); // update or set editor mode

    private:
        void InitWidgets(void);     // initialize starting widgets
        void CreateStatusLine(void);// create status line. Status line can be only once

        std::list<Widget*> _widgetsList; // all widgets in gui window
        WidgetStatusLine * statusLine = nullptr;  // pointer to status line widget
        bool _needExit;     // flag if need to exit
        Vec2 _windowsSize;  // current window size
        int _fontSize;      // current font size

};

#endif //__WINDOW_HPP__
