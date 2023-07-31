#include "core.hpp"
#include "gui.hpp"
#include "input.hpp"

std::string ParseCliArgs(int argc, char* argv[])
{
    std::string location;

    if(argc > 1)
    {
        // Now this function only parsing file or folder location
        location = std::string(argv[1]);
    }

    return location;
}


int main(int argc, char* argv[])
{
    Input in = Input();
    Gui gui = Gui();
    Core feditor = Core(&gui, &in, ParseCliArgs(argc, argv));

    return 0;
}
