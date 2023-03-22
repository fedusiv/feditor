#ifndef __WIDGET_STATUSLINE_HPP__
#define __WIDGET_STATUSLINE_HPP__

#include "widget.hpp"
#include <string>
#include <vector>
#include "editor_state.hpp"

class WidgetStatusLine: public Widget
{
    public:
        WidgetStatusLine(Rect rect);    // Rect here is windows size. It uses only x and y
        ~WidgetStatusLine();
        void Render(void) override;
        void Resize(Vec2 windowSize) override;

    private:
        void DrawCurrentMode(void);
        EditorState _currentEditorState;
        std::string _editorStateName[EditorState::EditorStateMax];

};

#endif // __WIDGET_STATUSLINE_HPP__
