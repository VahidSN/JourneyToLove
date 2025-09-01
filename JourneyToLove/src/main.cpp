#include "raylib.h"
#include "game/Scene.h"
#include "core/GameSetting.h"

//Disable console window on Windows in release mode
#if defined(WIN32) && !defined(_DEBUG)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

constexpr const char* PROJECT_NAME = "Journey to Love";
constexpr const char* ICON_PATH = "Assets/Textures/Character.png";

int main()
{
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, PROJECT_NAME);

	//Resize and go fullscreen if the screen is smaller than the game resolution
	int display = GetCurrentMonitor();
	int displayWidth = GetMonitorWidth(display);
	int displayHeight = GetMonitorHeight(display);
	if (displayWidth <= SCREEN_WIDTH && displayHeight <= SCREEN_HEIGHT) 
	{
		ToggleFullscreen();
	}

	SetTargetFPS(60);

	Image icon = LoadImage(ICON_PATH);
	SetWindowIcon(icon);
	UnloadImage(icon);

	InitAudioDevice();

	Scene scene;
	scene.Load();

	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(GRAY);

		scene.Update();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
