#include "core.hpp"
#include "gui.hpp"
#include "input.hpp"


int main()
{
    Input in = Input();
    Gui gui = Gui();
    Core feditor = Core(&gui, &in);

    return 0;
}


// int main()
// {
//     // Initialization
//     //--------------------------------------------------------------------------------------
//     InitWindow(800, 600, "raylib [core] example - basic window");

//     SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
//     //--------------------------------------------------------------------------------------

//     // Main game loop
//     while (!WindowShouldClose())    // Detect window close button or ESC key
//     {
//         BeginDrawing();

//             ClearBackground(RAYWHITE);

//             DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

//         EndDrawing();
//     }

//     // De-Initialization
//     //--------------------------------------------------------------------------------------
//     CloseWindow();        // Close window and OpenGL context
//     //--------------------------------------------------------------------------------------

//     return 0;
// }
