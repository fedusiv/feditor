#include <widget_float.hpp>
#include <colors.hpp>
#include <vec2.hpp>
#include <string>


WidgetFloat::WidgetFloat(Rect rect, std::string name): Widget(rect)
{
  _name = name;
  _rowDataAmount = 2;
  _widgetBorderThick = 0; // this widget does not use this logic here
  _borderLineThickness = 1;
  _borderOffset = Vec2(2,1);
  _widthProportion = 0.4;
  _heightProportion = 0.3;
  _cursorHeightAdd = 0;
  _cursorWidth = 1;
  Resize(rect);
}

WidgetFloat::~WidgetFloat()
{
    
}

void WidgetFloat::Render(void)
{
  Vec2 dataPos;

  // Draw name of widget
  dataPos = _borderOffset;
  for(auto c: _name){
    DrawCharacter(c, dataPos, ColorPurpose::ColorFloatWidgetText);
    dataPos.x += _glyphSize.x;
  }
  // Draw name of the widget
  // Float widget has background not for whole widget size
  DrawRect(_userFieldRect, _borderLineThickness, ColorPurpose::ColorFloatWidgetBorderLine, ColorPurpose::ColorFloatWidgetBg);
  // Draw content of user
  dataPos = _borderOffset + Vec2(_borderLineThickness, _glyphSize.y + _borderLineThickness);
  // We draw only first line
  for(auto c: *(_buffer->LineData(0))){
    DrawCharacter(c, dataPos, ColorPurpose::ColorFloatWidgetText);
    dataPos.x += _glyphSize.x;
  }
  // DrawCursor for user input
  DrawCursor(CalculateRealPosForCursor());
  
}

void WidgetFloat::Resize(Rect newRect)
{
  // Resizing appears based on window's size
  Rect baseRect;
  // Float widget received size of full windows rect size
  // Float widget will calculate self size based on window full size
  baseRect = newRect; // first copy the windows size
  // baseRect.h:
  // Need to calculate amount of row for glyphs which need to be drawn. By default, it's name and user input. In process it also will be autocompletion variants.
  // Next need to make some gaps between rows, so this gap is based on borderOffset. Permanent gaps are:
  // Between Name and upper border, upper border and user input, userinput and bottm border, bottom border and widget's end.
  // Next stage is autocompeteion, basically this is additional borders with information. So need to calculate this size also
  // It's new borders: Border of autocmpl to text there, text to border of auto compl, and border to end of widget
  // So when there is 2 row's of data we have 4 gaps, when 3 we have 7, becase we added 2 gaps for new borders, and one for new text data row. when 4 row of data, it will be 8 gaps
  // Eventually we can simple add 4 to amount of row to compenstate amount of gaps.
  // Plus need to take into consideration thickness of lines
  baseRect.h = _glyphSize.y * _rowDataAmount + (_borderOffset.y * (_rowDataAmount + 4) + (_borderLineThickness * 4) ); // Here better to draw, but will try to explain by words
  baseRect.w = _widthProportion * newRect.w; // calculate based on proportion coefficient
  baseRect.y = newRect.h * _heightProportion; // y position calculates based on proportion coefficient
  baseRect.x = (1 - _widthProportion) / 2 * newRect.w; // we want float windows to be sized based on proportion, but also want it to be in the middle of the window.
  // Now need to calculate rect, which is used to display user input
  _userFieldRect.y = baseRect.y + _glyphSize.y + _borderOffset.y;
  _userFieldRect.x = baseRect.x + _borderLineThickness;
  _userFieldRect.h = _glyphSize.y + _borderOffset.y * 2; // userinput files is permament size of one glyph height and two gaps
  _userFieldRect.w = baseRect.w - 2 * _borderLineThickness; // so we have only thickness of line which we need to consider

  Widget::Resize(baseRect);
}

Vec2 WidgetFloat::CalculateRealPosForCursor(void)
{
    Vec2 logicPos, relativePos;

    logicPos = _buffer->CursorPosition();
    logicPos.y = 0; // this is one line. Cursor is only at line 0
    // It's basically taken from widget editor. We are taking _borderOffset and + x based on glyphs. And y offset which is used for drawing user input rect
    relativePos = _borderOffset + Vec2( logicPos.x * _glyphSize.x , _glyphSize.y + _borderLineThickness );

    return relativePos;
}

void WidgetFloat::SetCursorPosition(Vec2 position)
{
  
}

void WidgetFloat::PageScrolling(Vec2 direction, Vec2 mousePosition)
{
  
}

void WidgetFloat::CalculateDrawingOffset(void)
{
  
}

void WidgetFloat::AttachBuffer(Buffer * buffer)
{
  _buffer = buffer;
}
