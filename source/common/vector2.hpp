#ifndef __VECTOR2_HPP__
#define __VECTOR2_HPP__

#include <utility>

typedef std::pair<int,int> Vector2Pair;

// basic data structure to hold Vector2
class Vector2
{
    public:
        int x;
        int y;

        bool operator== (Vector2 vec)
        {
            if(x == vec.x && y == vec.y)
            {
                return true;
            }
            return false;
        };

        bool operator!= (Vector2 vec)
        {
            if(x != vec.x || y != vec.y)
            {
                return true;
            }
            return false;
        }

        Vector2& operator= (Vector2 vec)
        {
            x = vec.x;
            y = vec.y;

            return *this;
        }

        Vector2& operator= (Vector2Pair pair)
        {
            x = pair.first;
            y = pair.second;

            return *this;
        }

};

#endif // __VECTOR2_HPP__