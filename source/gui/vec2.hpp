#ifndef __VEC2_HPP__
#define __VEC2_HPP__

#include <utility>

class Rect {
    public:
        int x;                // Rectangle top-left corner position x
        int y;                // Rectangle top-left corner position y
        int w;            // Rectangle width
        int h;           // Rectangle height

        Rect(int x = 0, int y = 0, int w = 0, int h = 0) :
        x(x), y(y), w(w), h(h)
        {

        }

        Rect& operator= (Rect rect)
        {
            if(rect.x > -1)
            {
                x = rect.x;
            }
            if(rect.y > -1)
            {
                y = rect.y;
            }
            if(rect.w > -1)
            {
                w = rect.w;
            }
            if(rect.h > -1)
            {
                h = rect.h;
            }

            return *this;
        }

        Rect& operator-= (int thick)
        {
            x += thick;
            y += thick;
            w -= thick * 2;
            h -= thick * 2;

            return *this;
        }
};
class Vec2
{
    public:
        int x;
        int y;

        Vec2(int xVal = 0, int yVal = 0)
        {
            x = xVal;
            y = yVal;
        }

        bool operator== (Vec2 vec)
        {
            if(x == vec.x && y == vec.y)
            {
                return true;
            }
            return false;
        };

        bool operator!= (Vec2 vec)
        {
            if(x != vec.x || y != vec.y)
            {
                return true;
            }
            return false;
        }

        Vec2& operator= (Vec2 vec)
        {
            x = vec.x;
            y = vec.y;

            return *this;
        }

        Vec2& operator+=(const Vec2& addVec)
        {
            x += addVec.x;
            y += addVec.y;
            return *this;
        }

        Vec2& operator*=(const int& scale)
        {
            x *= scale;
            y *= scale;
            return *this;
        }

        Vec2& operator*=(const Vec2& mulVec)
        {
            x *= mulVec.x;
            y *= mulVec.y;
            return *this;
        }
        Vec2& operator* (const Vec2& mulVec)
        {
            x *= mulVec.x;
            y *= mulVec.y;
            return *this;
        }

        void FromRect(Rect rect)
        {
            x = rect.x;
            y = rect.y;
        }


};

#endif // __VECTOR2_HPP__
