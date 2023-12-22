#include <gui_configs.hpp>
#include <widget_float.hpp>
#include <colors.hpp>
#include <vec2.hpp>
#include <string>


WidgetFloat::WidgetFloat(Rect rect, std::string name, int renderLayer): Widget(rect)
{
  _name = name;
  _rowDataAmount = WIDGET_FLOAT_DEFAULT_ROW_AMOUNT;
  _widgetBorderThick = 0; // this widget does not use this logic here
  _borderLineThickness = 1;
  _borderOffset = WIDGET_FLOAT_DATA_DRAW_OFFSET;
  _widthProportion = 0.4;
  _heightProportion = 0.3;
  _cursorHeightAdd = 0;
  _cursorWidth = 1;
  _completionLinesAmount = 0; // by default no completeion is provided
  _complRectHeightGap = WIDGET_FLOAT_GAP_BETWEEN_USER_COMPL;
  _layerRender = renderLayer;
  Resize(rect);
}

WidgetFloat::~WidgetFloat()
{
    
}

void WidgetFloat::Update()
{
  // More rect for autocompletion or whatever should be there
  // Render function is called each update frame. Here check if there is new data.
  if( (_buffer->LinesNumber() - 1) !=  _completionLinesAmount ){
    // This is basically update function. So if update function will exist and have separate logic of render, this code need to be moved there
    _completionLinesAmount = _buffer->LinesNumber() - 1; // update amount
    _rowDataAmount = WIDGET_FLOAT_DEFAULT_ROW_AMOUNT + _completionLinesAmount;
    // This is called the auto resize. Float widget is special widget and it has ability to cal self resize.
    Resize(_latestWindowRect);
  }
}

void WidgetFloat::Render(void)
{
  Vec2 dataPos; // point to draw data, symbols
  int heightPoint; // point of height to draw also
  int currentLine; // for drawing what is the current variant to choose
  ColorPurpose color;

  // This part of the code is not expliciplty related to render functionality.
  Update();
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
  dataPos = _userInputDrawStartPoint;
  // We draw only first line
  for(auto c: *(_buffer->LineData(0))){
    DrawCharacter(c, dataPos, ColorPurpose::ColorFloatWidgetText);
    dataPos.x += _glyphSize.x;
  }
  // DrawCursor for user input
  DrawCursor(CalculateRealPosForCursor());
  // Draw completion field
  if(_completionLinesAmount){
    // Draw rect first
    DrawRect(_complFieldRect, _borderLineThickness, ColorPurpose::ColorFloatWidgetBorderLine, ColorPurpose::ColorFloatWidgetBg);
    currentLine = _buffer->CursorPosition().y - 1;
    for(int i = 0; i < _completionLinesAmount; i++){
      // height point - calculate current height to draw line
      heightPoint = _complFieldRect.y - _widgetRect.y + i * (_glyphSize.y + _borderLineThickness);
      // Calculate position of first symbol to draw
      dataPos = _borderOffset + Vec2(_borderLineThickness, heightPoint);
      for(auto c: *(_buffer->LineData(i+1))){
        color = ColorPurpose::ColorFloatWidgetText;
        if(i == currentLine){
          color = ColorPurpose::ColorFloatWidgetTextSelected;
        }
        DrawCharacter(c, dataPos, color);
        dataPos.x += _glyphSize.x;
      }
    }

  }
  
}

void WidgetFloat::Resize(Rect newRect)
{
  // Resizing appears based on window's size
  Rect baseRect;
  // Float widget received size of full windows rect size
  // Float widget will calculate self size based on window full size
  baseRect = newRect; // first copy the windows size
  _latestWindowRect = newRect; // store latest used window rect
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
  // Recalculate size of _complFieldRect
  ResizeAutoCompl();
  Widget::Resize(baseRect);
  // Update data for each frame drawing
  _userInputDrawStartPoint =  _borderOffset + Vec2(_borderLineThickness, _glyphSize.y + _borderLineThickness);
}

void WidgetFloat::ResizeAutoCompl(void)
{
  // we have already calculated _userFieldRect.
  // x and w will be same.
  _complFieldRect.x = _userFieldRect.x;
  _complFieldRect.w = _userFieldRect.w;
  _complFieldRect.y = _userFieldRect.y + _userFieldRect.h + ( _complRectHeightGap * _glyphSize.y);
  _complFieldRect.h = _completionLinesAmount * (_glyphSize.y + _borderOffset.y);
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
  int cursor; // working cursor of operation
  Vec2 curPos;
  // we only update one axis
  curPos = _buffer->CursorPosition();
  // There is two logic sides there.
  // First line, user input is a default, like in editor. But since we have only one line, it only variants on x axis
  // Other part is related to completion rect is only to choose from y axis.
  if( _userFieldRect.x < position.x && (_userFieldRect.x + _userFieldRect.w) > position.x ){
    if(_userFieldRect.y < position.y && (_userFieldRect.y + _userFieldRect.h) > position.y){
      // inside user input
      curPos.x = 0; // we will change cursor position x. We are starting to look for it. That's why set it to 0
      cursor = _widgetRect.x + _userInputDrawStartPoint.x + _glyphSize.x; // we need to calculate where on x axis starts cursor after first symbol
      while(cursor < _userFieldRect.x + _userFieldRect.w){
        if(position.x < cursor){
          // found position
          break;
        }
        // next iteration
        curPos.x += 1;
        cursor+= _glyphSize.x;
      }
      goto setCursorFunctionEnd;  // yeah, goto is not better practice. it will lead to the end of function. where will be set cursor position. avoidi y axis
    }
  }

  if( _complFieldRect.x < position.x && (_complFieldRect.x + _complFieldRect.w) > position.x ){
    if(_complFieldRect.y < position.y && (_complFieldRect.y + _complFieldRect.h) > position.y){
      // inside completion. look for y axis
      curPos.y = 1; // We are starting to look for cursor position on y axis. And will do it step by step. Starting from step 1
      cursor = _complFieldRect.y + _glyphSize.y + _borderOffset.y;
      while(cursor < _complFieldRect.y + _complFieldRect.h){
        if(position.y < cursor){
          break; // y position is found.
        }
        // increment for next iteration
        curPos.y += 1;
        cursor += _glyphSize.y;
      }
    }
  }


setCursorFunctionEnd:
  _buffer->SetCursorPosition(curPos);
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

  // Next code is only for dev.
  _buffer->Append("wine 1", 1);
  _buffer->Append("beer 2", 2);
  _buffer->Append("whiskey 3", 3);
  _buffer->Append("vodka 4", 4);
}
