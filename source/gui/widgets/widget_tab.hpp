#ifndef __WIDGET_TAB_HPP__
#define __WIDGET_TAB_HPP__

#include <list>
#include <utility>
#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"
#include "widget_label.hpp"
#include "buffer.hpp"
#include "gui_layout.hpp"

typedef std::pair<WidgetLabel*, WidgetEditor* > EditorEntity;

class WidgetTab: public Widget
{

    public:
        WidgetTab(Rect rect);
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget
        void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page
        void AttachBuffer(Buffer * buffer, LayoutDirection direction);         // attaching buffer to tab. Creates widget editor

    protected:
        void CalculateDrawingOffset(void) override;
        EditorEntity CreateEditorEntity(Buffer * buffer);
        void SetActiveWidgetEditor(WidgetEditor * we);

    private:
        std::list<WidgetEditor*> _widgetsEditorList; // all editor widgets in gui window
        std::list<WidgetLabel*> _widgetsLabelList; // all label widgets related to editor widgets in gui window
        std::vector<GuiLayout*> _layoutsV; // vertical layouts inside widget tab
        std::vector<GuiLayout*> _layoutsH; // horizontal layouts inside widget tab

};

#endif // __WIDGET_TAB_HPP__
