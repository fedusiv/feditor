#include "widget_editorentity.hpp"
#include "editor_state.hpp"
#include "gui_configs.hpp"
#include "widget.hpp"
#include "widget_editor.hpp"

WidgetEditorEntity::WidgetEditorEntity(Rect rect, Buffer * buffer): Widget(rect)
{
    _layout = new GuiLayout(_widgetRect, LayoutDirection::Vertical);
    _editor = new WidgetEditor(Rect(_widgetRect.x, _widgetRect.y, _widgetRect.w, _widgetRect.h), buffer);

    _bufferLabel = new WidgetLabel(Rect(0,0,0,0), buffer->FileName(), ColorPurpose::ColorWidgetEditorFileName, WIDGET_EDITOR_BUFFER_NAME_WRITE_OFFSET);
    _layout->Insert(_bufferLabel, true);

    _layout->Insert(_editor, false);
}

void WidgetEditorEntity::SetActive(bool status)
{
    Widget::SetActive(status);
    _editor->SetActive(status);
}

void WidgetEditorEntity::Render(void)
{
    Widget::Render();
    _editor->Render();

    _bufferLabel->Render();

}

void WidgetEditorEntity::Resize(Rect newRect)
{
    Widget::Resize(newRect);
    _layout->Resize(_widgetRect);
}

void WidgetEditorEntity::SetCursorPosition(Vec2 position)
{
    _editor->SetCursorPosition(position);
}

void WidgetEditorEntity::PageScrolling(Vec2 direction, Vec2 mousePosition)
{
    _editor->PageScrolling(direction, mousePosition);
}

WidgetEditor * WidgetEditorEntity::GetWidgetEditor()
{
    return _editor;
}

void WidgetEditorEntity::SetEditorState(EditorState state)
{
    Widget::SetEditorState(state);
    _editor->SetEditorState(state);
    _bufferLabel->SetEditorState(state);
}

