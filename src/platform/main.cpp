#include <iostream>

// External libraries
#include <raylib.h>
#include <imgui_layer.h>

// Project headers
#include <gameMain.h>
#include <asserts.h>

int main()
{
#if PRODUCTION_BUID == 1
	SetTraceLogLevel(LOG_NONE);
#endif

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1920, 1080, "Making Game");

	SetExitKey(KEY_NULL);
	// SetTargetFPS(nullptr);

	if (!initImGui())
	{
		return 0;
	}

	if (!initGame())
	{
		return 0;
	}

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(BLACK);

		beginImGuiFrame();

		if (!updateGame())
		{
			break;
		}

		endImGuiFrame();
		EndDrawing();
	}

	closeGame();
	shutdownImGui();
	CloseWindow();

	return 0;
}