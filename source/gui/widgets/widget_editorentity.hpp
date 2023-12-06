#ifndef __WIDGET_EDITORENTITY_HPP__
#define __WIDGET_EDITORENTITY_HPP__

#include <list>
#include <vector>
#include <utility>
#include "graphics.hpp"
#include "vec2.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"
#include "widget_label.hpp"
#include "buffer.hpp"
#include "gui_layout.hpp"

/* Complex widget consist of all the widgets related to text editor widget */

class WidgetEditorEntity: public Widget
{

    public:
        WidgetEditorEntity(Rect rect, Buffer * buffer);
        void SetActive(bool status) override;
        void Render(void) override;
        void Resize(Rect newRect) override;
        void SetCursorPosition(Vec2 position) override; // setting cursor position of widget
        void PageScrolling(Vec2 direction, Vec2 mousePosition) override; // moving editor page based on explicit commands to scroll page

        WidgetEditor * GetWidgetEditor(); // return widget editor of editor entity
    
    private:
        GuiLayout* _layout;
        WidgetEditor* _editor;
        WidgetLabel* _bufferLabel;

};

#endif // __WIDGET_EDITORENTITY_HPP__
