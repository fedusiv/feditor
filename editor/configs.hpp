#ifndef __CONFIGS_HPP__
#define __CONFIGS_HPP__

class Configs
{
    public:
        static const int FirstKeyPress = 500;
        static const int SecondKeyPress = 150;
        static const int LinesCursorReachedBeforeScroll = 3;    // when cursor is close to any borders need to move window, scroll window basically
};

#endif // __CONFIGS_HPP__