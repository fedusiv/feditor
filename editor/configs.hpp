#ifndef __CONFIGS_HPP__
#define __CONFIGS_HPP__

class Configs
{
    public:
        static const int FirstKeyPress = 500;
        static const int SecondKeyPress = 100;
        static const int LinesCursorReachedBeforeScroll = 3;    // when cursor is close to any borders need to move window, scroll window basically
        static const int DefaultFrameRate = 30;     // default frame rate of application
        static const int SampleRateForFrameRate = 10;
};

#endif // __CONFIGS_HPP__