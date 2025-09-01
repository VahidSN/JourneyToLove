#pragma once
#include "raylib.h"
#include <memory>

class Button;

class UIController
{
public:
	UIController();
	~UIController();

	void ShowTutorial();
	bool UpdateAndDrawEndPanel(const Camera2D& camera);
	void UpdateDistanceText(const Camera2D& camera, const int distance);

private:
	int distanceTextSize{ 0 };
	std::unique_ptr<Button> resetButton;

	Texture2D loveTexture;
	Texture2D loverTexture;

};
