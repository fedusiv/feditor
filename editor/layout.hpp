#ifndef __LAYOUT_HPP__
#define __LAYOUT_HPP__

#include "utils.hpp"

class Layout
{
    public:
        Vector2 size;   // size of layout in lines and columns
        Vector2 cursorPosition;
        Vector2 ** layoutPositions;  // postions in real coordinates for elements

};

#endif // __LAYOUT_HPP__