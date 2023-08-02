#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <list>
#include "widget.hpp"
#include "buffer.hpp"
#include "widget_statusline.hpp"
#include "widget_editor.hpp"
#include "widget_tab.hpp"


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
        void UpdateMousePosition(Vec2 mousePosition);   // update mouse position. It's done each frame update
        void AlignCursorPositionByMouse(void);      // this function looking for widget under required coordinates and trying to change cursor position
        void PageScrolling(Vec2 direction); // scroll page of widget under mouse position

    private:
        void InitWidgets(void);     // initialize starting widgets
        void CreateLayout(void);    // making init layout for all widgets.
        void CreateStatusLine(void);// create status line. Status line can be only once
        void CreateWidgetTab(void);// creates new widget tab
        Widget* GetWidgetUnderMouse(void);  // get pointer to widget, under which is mouse position right now

        std::list<Widget*> _widgetsList; // all widgets in gui window
        std::list<WidgetTab*> _widgetsTabsList; // all widget tabs
        WidgetStatusLine * statusLine = nullptr;  // pointer to status line widget
        WidgetTab * _widgetTabActive = nullptr;   // pointer to active tab widget
        bool _needExit;     // flag if need to exit
        Vec2 _windowsSize;  // current window size
        int _fontSize;      // current font size
        Vec2 _mousePosition;    // current mouse position

};

#endif //__WINDOW_HPP__
