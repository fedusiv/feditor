#ifndef __UTILS_HPP__
#define __UTILS_HPP__

// basic data strucct to hold Vector2
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

};

#endif // __UTILS_HPP__