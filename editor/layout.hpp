#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include "utils.hpp"

class LayoutArea
{
    public:
        Vector2 startPoint; // coordinate from where layout is drawn
        Vector2 size;   // size of layout in real coordinates
        Vector2 sizeNet;    // size of layout in elements representation. rows and columns
        Vector2 ** layoutPositions = nullptr;  // postions in real coordinates for elements, each element here is related to sizeNet
                                                // maybe not good to declare nullptr here, do not judge me and sugegst how to change it
        Vector2 displayPoint;   // from what point buffer will be shown. Basically used in scroll layout
                                // this is position inside buffer, not related to layout

};

class Layout
{
    public:
        Vector2 mainOffset; // this is main offset
        Vector2 size; // real size of layout
        Vector2 cursorPosition;

        LayoutArea textArea;    // area of text
        LayoutArea linesArea;   // place where lines are drawn
};

#endif // __LAYOUT_HPP__