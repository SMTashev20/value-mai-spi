#include <raylib.h>

int main()
{
	InitWindow(1280, 720, "ahah");

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
	}

	CloseWindow();
}