#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <list>
#include "widget.hpp"
#include "buffer.hpp"
#include "widget_statusline.hpp"
#include "widget_editor.hpp"
#include "widget_tab.hpp"
#include "widget_tab_list.hpp"
#include "widget_float.hpp"


class Gui
{

    public:
        Gui();
        ~Gui();
        void Update(void);  // main function of gui process
        void CreateWindow(void);    // create main window
        void AttachWidgetEditor(Buffer * buffer, bool vertical=true);  // create empty widget for edititng text
        void AttachFloatBuffer(Buffer * buffer);  // attach float buffer to currernt float widget
        void AttachTab();   // creates new tab in tab list
        bool NeedExit(void);    // report when application need to be exited
        void RequestExit(void); // request to close app
        void Resize(void);      // resize request
        void SetEditorState(EditorState state); // update or set editor mode
        void UpdateMousePosition(Vec2 mousePosition);   // update mouse position. It's done each frame update
        void AlignCursorPositionByMouse(void);      // this function looking for widget under required coordinates and trying to change cursor position
        void PageScrolling(Vec2 direction); // scroll page of widget under mouse position
        bool SwitchBuffer(MoveCursorDirection direction);   // change active widget inside active widget tab, in direction used arrow keys. Returns true if switch has happened
        void SwitchTab(int id);  // set current tab to given tab id
        void StatusLineUpdate();   // update status line with the new buffer name
        
    private:
        void InitWidgets(void);     // initialize starting widgets
        void CreateLayout(void);    // making init layout for all widgets.
        void CreateStatusLine(void);// create status line. Status line can be only once
        void CreateWidgetTab(void);// creates new widget tab
        Widget* GetWidgetUnderMouse(void);  // get pointer to widget, under which is mouse position right now
        void CreateFloatWidget();  // instantiate flaot widget
        void DeleteFloatWidget();   // delete current float widget
        void UpdateWidgetList(Widget* w = nullptr);  // append widget to the list of widgets. It's needed for the render layers.
                                                    // Also need to use, when widget changes render layer
    
        std::list<Widget*> _widgetsList; // sorted by render layer all widgets in gui window
        WidgetTabList * _widgetTabList = nullptr; // control for tabs
        WidgetStatusLine * _statusLine = nullptr;  // pointer to status line widget
        WidgetFloat * _floatWidget = nullptr;   // current float widget
        bool _needExit;     // flag if need to exit
        Vec2 _windowsSize;  // current window size
        int _fontSize;      // current font size
        Vec2 _mousePosition;    // current mouse position
        GuiLayout * _verticalLayout = nullptr; // vertical layout of main gui

};

#endif //__WINDOW_HPP__
