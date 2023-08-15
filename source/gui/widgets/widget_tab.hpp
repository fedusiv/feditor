#ifndef __WIDGET_TAB_HPP__
#define __WIDGET_TAB_HPP__

#include <list>
#include <vector>
#include <utility>
#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"
#include "widget_label.hpp"
#include "widget_editorentity.hpp"
#include "buffer.hpp"
#include "gui_layout.hpp"


class WidgetTab: public Widget
{

    public:
        WidgetTab(Rect rect);
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget
        void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page
        void AttachBuffer(Buffer * buffer, LayoutDirection direction);         // attaching buffer to tab. Creates widget editor
        bool SwitchBuffer(MoveCursorDirection direction);   // if swtich happened return true

    protected:
        void CalculateDrawingOffset(void) override;
        WidgetEditorEntity* CreateEditorEntity(Buffer * buffer);
        void SetActiveWidgetEditor(WidgetEditorEntity * we);

    private:
        std::list<WidgetEditorEntity*> _widgetsEntityList; // all editor entities widgets in gui window
        std::vector<GuiLayout*> _layoutsV; // vertical layouts inside widget tab
        std::vector<GuiLayout*> _layoutsH; // horizontal layouts inside widget tab
        WidgetEditorEntity* _currentActiveEntity; // pointer to current active editor entity

};

#endif // __WIDGET_TAB_HPP__
