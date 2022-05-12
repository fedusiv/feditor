#ifndef __INPUT_HANDLER_HPP__
#define __INPUT_HANDLER_HPP__

#include "command.hpp"

class InputHandler
{
    public:
        InputHandler();

        Command* ProcessInput(SDL_Keycode keyValue);

};

#endif // __INPUT_HANDLER_HPP__