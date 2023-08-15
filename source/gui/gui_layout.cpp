#include <cassert>
#include <algorithm>
#include <iostream>

#include "macros.h"
#include "gui_layout.hpp"
#include "vec2.hpp"
#include "widget_tab.hpp"
#include "widget_label.hpp"
#include "widget_editor.hpp"
#include "widget_statusline.hpp"

GuiLayout::GuiLayout(Rect rect, LayoutDirection direction) : _layoutDirection(direction), _layoutRect(rect)
{
    _layoutType = LayoutType::WidgetBase;
    _glyphHardSizeAdd = 2;
    _glyphSize = Graphics::GlyphMaxSize();
}

void GuiLayout::AppendHorizontalWidget(Widget *widget, bool hardSize)
{
    // TODO: not implemented
}

void GuiLayout::AppendVerticalWidget(Widget *widget, bool hardSize)
{
    LayoutElement *element = new LayoutElement{.hardSize = hardSize, .widget = widget};
    CalculateAndResizeVerticalWidget(element);
}

void GuiLayout::AppendHorizontalLayout(GuiLayout * layout)
{
    _layoutLList.push_back(layout);
    CalculateAndResizeHorizontalLayout();
}

void GuiLayout::CalculateAndResizeHorizontalLayout()
{
    Rect newRect; 

    newRect = _layoutRect;
    newRect.w = _layoutRect.w /  _layoutLList.size();   // for now equal size for all layouts
    for(auto l : _layoutLList)
    {
        l->Resize(newRect); // resize layout
        newRect.x += newRect.w; // x position will be moved to width of newRect. It's horizontal shift
    }
}

void GuiLayout::AppendVerticalLayout(GuiLayout * layout)
{
    _layoutLList.push_back(layout);
    CalculateAndResizeVerticalLayout();
}

void GuiLayout::CalculateAndResizeVerticalLayout()
{
    Rect newRect; 
    newRect = _layoutRect;
    newRect.h = _layoutRect.h /  _layoutLList.size();   // for now equal size for all layouts
    for(auto l : _layoutLList)
    {
        l->Resize(newRect); // resize layout
        newRect.y += newRect.h; // x position will be moved to width of newRect. It's horizontal shift
    }
}

int GuiLayout::CalculateHardSizeOnGlyph()
{
    if(_layoutDirection == LayoutDirection::Vertical)
    {
        return _glyphSize.y + _glyphHardSizeAdd;
    }
    // For now I can say, that hardcoded widgets of horizontal orientation is not yet implemented. It's kind of temp plug 
    return _glyphSize.x + _glyphHardSizeAdd;
}

void GuiLayout::Resize(Rect newRect)
{
    _layoutRect = newRect;
    if(_layoutType == LayoutType::WidgetBase)
    {
        if(_layoutDirection == LayoutDirection::Vertical)
        {
            CalculateAndResizeVerticalWidget();
        }else{
            CalculateAndResizeHorizontalWidget();
        }
    }else{
        if(_layoutDirection == LayoutDirection::Vertical)
        {
            CalculateAndResizeVerticalLayout();
        }else{
            CalculateAndResizeHorizontalLayout();
        }
    }
}

void GuiLayout::CalculateAndResizeVerticalWidget(LayoutElement * le)
{
    Rect rect;
    int hardSizedSize = 0;        // size in pixels, what amout of length in width or height is taken by hardcoded size
    int flexibleWidgetsAmout = 0; // amount of flexible size
    int height = 0;

    // calculate height for widgets
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            hardSizedSize += e->widget->GetRect().h;
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }
    if(nullptr != le)
    {
        _layoutWList.push_back(le);
        if (le->hardSize)
        { // request to add hard sized widget
            hardSizedSize += CalculateHardSizeOnGlyph();
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }

    height = (_layoutRect.h - hardSizedSize)  / flexibleWidgetsAmout;
    rect = _layoutRect; // here we need to get width of layout and first place of widget, mean x and y. height will be configured below
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            rect.h = CalculateHardSizeOnGlyph();
        }
        else
        {
            rect.h = height; // required height for widget
        }
        CallResize(e->widget, rect);
        rect.y +=  e->widget->GetRect().h; // change y coordinate for futher columns
    }
}

void GuiLayout::CalculateAndResizeHorizontalWidget(LayoutElement * le)
{
    Rect rect;
    int hardSizedSize = 0;        // size in pixels, what amout of length in width or width is taken by hardcoded size
    int flexibleWidgetsAmout = 0; // amount of flexible size
    int width = 0;

    // calculate width for widgets
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            hardSizedSize += e->widget->GetRect().w;
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }
    if(nullptr != le)
    {
        _layoutWList.push_back(le);
        if (le->hardSize)
        { // request to add hard sized widget
            hardSizedSize += CalculateHardSizeOnGlyph();
        }
        else
        {
            flexibleWidgetsAmout++;
        }
    }

    width = (_layoutRect.w - hardSizedSize)  / flexibleWidgetsAmout;
    rect = _layoutRect; // here we need to get width of layout and first place of widget, mean x and y. width will be configured below
    for (auto e : _layoutWList)
    {
        if (e->hardSize)
        {
            rect.w = CalculateHardSizeOnGlyph();
        }
        else
        {
            rect.w = width; // required width for widget
        }
        CallResize(e->widget, rect);
        rect.x +=  e->widget->GetRect().w; // change x coordinate for futher columns
    }
}

void GuiLayout::Append(Widget *widget, bool hardSize)
{
    assert(_layoutLList.size() == 0 && "Guilayout should be only one typed. It was already typed as layouts");
    _layoutType = LayoutType::WidgetBase;

    switch (_layoutDirection) {
        case LayoutDirection::Vertical:
            AppendVerticalWidget(widget, hardSize);
            break;
        case LayoutDirection::Horizontal:
            AppendVerticalWidget(widget, hardSize);
            break;
        default:
            break;
    }
}

void GuiLayout::Append(GuiLayout* layout)
{
    assert(_layoutWList.size() == 0 && "Guilayout should be only one typed. It was already typed as widgets");
    _layoutType = LayoutType::LayoutBase;
    switch (_layoutDirection) {
        case LayoutDirection::Vertical:
            AppendVerticalLayout(layout);
            break;
        case LayoutDirection::Horizontal:
            AppendHorizontalLayout(layout);
            break;
        default:
            break;
    }
}

void GuiLayout::CallResize(Widget *widget, Rect rect)
{
    switch (widget->GetWidgetType())
    {
    case WidgetType::WidgetTypeTab:
        static_cast<WidgetTab *>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeEditor:
        static_cast<WidgetEditor *>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeStatusLine:
        static_cast<WidgetStatusLine *>(widget)->Resize(rect);
        break;
    case WidgetType::WidgetTypeLabel:
        static_cast<WidgetLabel *>(widget)->Resize(rect);
        break;
    default:
        widget->Resize(rect);
        break;
    }
}

// Function looks in layouts, which is inside parent layout
bool GuiLayout::IsInLayout(Widget *w)
{
    bool res;

    res = false;
    if(_layoutType == LayoutType::LayoutBase)
    {
        for(auto ll: _layoutLList)
        {
            if(ll->IsInLayout(w))
            {
                res = true;
                break;
            }
        }
    }else{
        for(auto wl: _layoutWList)
        {
            if(wl->widget == w)
            {
                res = true;
                break;
            }
        }
    }
    return res;
}

void GuiLayout::IndexingFunc(IndexingQueue * stack, Widget * w)
{
    if(_layoutType == LayoutType::LayoutBase)
    {
        for(int i = 0; i < _layoutLList.size(); i++)
        {
            if(_layoutLList[i]->IsInLayout(w))
            {
                _layoutLList[i]->IndexingFunc(stack,w);
                stack->push_back({.layout = this, i});    // push to stack queue struct 
                break;
            }
        }
    }else{
        for(int i = 0; i <  _layoutWList.size(); i++)
        {
            if(w == _layoutWList[i]->widget)
            {
                stack->push_back({.layout = this, i});    // push to queue Indexing struct 
                break;
            }
        }
    }
}

Widget* GuiLayout::GetNextWidget(Widget* source, MoveCursorDirection direction)
{
    IndexingQueue queue; 
    Widget* result;
    
    result = nullptr;
    IndexingFunc(&queue, source);   // calculate indexing position of given widget
    if(!queue.empty())
    {
        // queue is not empty, widget is found, and in indexqueue we have indexes of source widget.
        // in the function below let's find and calculate map index for neighbour widget
        if(GetNextWidgetIndexCalculation(&queue, direction))
        {
            // widget has neighbour. index of position of new widget is calculated and located in stack. it can be found
            queue.reverse();   // reverse list, to have higher in hierarchy element in the begining
            result = GetWidgetByMapIndex(&queue);
        }
    }

    return result;
}

Widget* GuiLayout::GetWidgetByMapIndex(IndexingQueue * queue)
{
    int currentIndex;

    currentIndex = queue->front().index;    // get index, which element should be accessed
    // check on extremums
    if(currentIndex < 0)
    {
        currentIndex = 0;
    }
    if(currentIndex >= ElementsAmount())
    {
        currentIndex = ElementsAmount() - 1;    // index should be less on one, because indexing starts from zero
    }
    queue->pop_front();     // remove element from list
    if(_layoutType == LayoutType::LayoutBase)
    {
        return _layoutLList[currentIndex]->GetWidgetByMapIndex(queue);
    }else{
        return _layoutWList[currentIndex]->widget;
    }
}

bool GuiLayout::GetNextWidgetIndexCalculation(IndexingQueue * queue, MoveCursorDirection direction)
{
    int changeKf;
    LayoutDirection ldirection;
    IndexingQueue::iterator it;
    bool haveNeighbour;

    haveNeighbour = false;
    for(it = queue->begin(); it != queue->end(); ++it )
    {
        changeKf = 1;
        ldirection = it->layout->GetLayoutDirection();
        switch(direction)
        {
            // Vertical movement
            case MoveCursorDirection::CursorUp:
            case MoveCursorDirection::CursorDown:
            {
                if(ldirection == LayoutDirection::Horizontal)
                {
                    // this direction is not okay for this
                    continue; // go to next loop iteration
                }
                if(direction == MoveCursorDirection::CursorUp)
                {
                    changeKf = -1;
                }
                it->index += changeKf;
                if(it->index < 0 || it->index >= it->layout->ElementsAmount())
                {
                    // reached extremum. need to continue to look for right place to change widget
                    continue;
                }else{
                    haveNeighbour = true;   // have neighbour in required direction 
                    break;  // exit from loop. Found the place where will appeared logic move. The parent layout
                }
                continue;   // continue loop
            }
            // Horizontal movement
            case MoveCursorDirection::CursorLeft:
            case MoveCursorDirection::CursorRight:
            {
                if(ldirection == LayoutDirection::Vertical)
                {
                    // this direction is not okay for this
                    continue; // go to next loop iteration
                }

                if(direction == MoveCursorDirection::CursorLeft)
                {
                    changeKf = -1;
                }
                it->index += changeKf;
                if(it->index < 0 || it->index >= it->layout->ElementsAmount())
                {
                    // reached extremum. need to continue to look for right place to change widget
                    continue;
                }else{
                    haveNeighbour = true;   // have neighbour in required direction 
                    break;  // exit from loop. Found the place where will appeared logic move. The parent layout
                }
                continue; // continue loop
            }
        }
    }

    return haveNeighbour;
}

LayoutDirection GuiLayout::GetLayoutDirection()
{
    return _layoutDirection;
}

int GuiLayout::ElementsAmount()
{
    if(_layoutType == LayoutType::LayoutBase)
    {
        return _layoutLList.size();
    }else{
        return _layoutWList.size();
    }
}
