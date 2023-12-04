#ifndef __WIDGET_TAB_LIST_HPP__
#define __WIDGET_TAB_LIST_HPP__

#include <list>
#include <vector>
#include <utility>
#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "widget_label.hpp"
#include "buffer.hpp"
#include "gui_layout.hpp"
#include "widget_tab.hpp"


class WidgetTabList: public Widget
{

    public:
        WidgetTabList(Rect rect);
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget
        void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page
        void AttachBuffer(Buffer * buffer, LayoutDirection direction);         // attaching buffer to tab. Creates widget editor
        bool SwitchBuffer(MoveCursorDirection direction);   // if swtich happened return true
        Buffer * GetActiveBuffer(); // returns current active buffer
        void CreateNewTab();    // creates new tab
        std::string NameOfCurrentTab(); // returns name of the current tab
    
    protected:
        void CalculateDrawingOffset(void) override;

    private:
        void GenerateNewLabel();    // Generating new label
        std::list<WidgetTab*> _widgetsTabList; // all tabs entities widgets in list of tabs window
        std::vector<WidgetLabel*> _widgetsLabelList; // all tabs name  widgets in list
        WidgetTab* _currentTab;
        GuiLayout* _layoutMain; // main layout with vertical orientation
        GuiLayout* _layoutV; // vertical layouts inside widget
        GuiLayout* _layoutH; // horizontal layouts inside widget
        int _currentTabId;  // id of current tab

};
#endif // __WIDGET_TAB_LIST_HPP__
