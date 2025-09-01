#include "UIController.h"
#include "core/Button.h"
#include <core/GameSetting.h>

//In-Game Panel Settings
const int DISTANCE_TEXT_FONT_SIZE = 30;

//End Panel Settings
constexpr const char* END_MESSAGE = "Congrats, you found your love.!";
const float END_MESSAGE_FONT_SIZE = 40.0f;
const float END_MESSAGE_Y_OFFSET = -150.0f;
const float RESET_BUTTON_Y_OFFSET = 150.0f;
const float END_CHARACTERS_Y_OFFSET = 250.0f;

constexpr const char* LOVE_TEXTURE_PATH = "Assets/Textures/Endpoint.png";
constexpr const char* LOVER_TEXTURE_PATH = "Assets/Textures/Character.png";

UIController::UIController()
{
	resetButton = std::make_unique<Button>(Rectangle{ 0, 0, 300, 80 }, "Reset", 40);
	distanceTextSize = MeasureText("Distance to Love: 00m", DISTANCE_TEXT_FONT_SIZE);

	loveTexture = LoadTexture(LOVE_TEXTURE_PATH);
	loverTexture = LoadTexture(LOVER_TEXTURE_PATH);
}

UIController::~UIController()
{
}

void UIController::ShowTutorial()
{
	const char* tutorialLines[] = {
		"Welcome to 'Journey to Love'!",
		"Use 'A' and 'D' keys to move left and right.",
		"Press 'W' to jump.",
		"Avoid obstacles and reach the endpoint to find your love.",
		"Good luck!"
	};
	const int lineCount = sizeof(tutorialLines) / sizeof(tutorialLines[0]);
	const int fontSize = 26;
	const int padding = 10;
	int maxWidth = 0;
	// Calculate the maximum width of the tutorial text
	for (int i = 0; i < lineCount; ++i)
	{
		int lineWidth = MeasureText(tutorialLines[i], fontSize);
		if (lineWidth > maxWidth)
			maxWidth = lineWidth;
	}
	int boxWidth = maxWidth + padding * 2;
	int boxHeight = lineCount * (fontSize + padding) + padding;
	int boxX = SCREEN_WIDTH / -2.0f + BLOCK_SIZE;
	int boxY = (SCREEN_HEIGHT - boxHeight) / 2;
	// Draw semi-transparent background
	DrawRectangle(boxX, boxY, boxWidth, boxHeight, { 80, 200, 160, 240 });
	// Draw each line of the tutorial text
	for (int i = 0; i < lineCount; ++i)
	{
		int textX = boxX + padding;
		int textY = boxY + padding + i * (fontSize + padding);
		DrawText(tutorialLines[i], textX, textY, fontSize, BLACK);
	}
}

bool UIController::UpdateAndDrawEndPanel(const Camera2D& camera)
{
	resetButton->Update(camera);
	if (resetButton->IsClicked())
	{
		return true;
	}

	// Position the reset button at the center of the screen
	resetButton->SetCenterPosition(camera.target.x, camera.target.y + RESET_BUTTON_Y_OFFSET);
	resetButton->Draw();

	// Position and show the end message above the end button
	int textWidth = MeasureText(END_MESSAGE, END_MESSAGE_FONT_SIZE);
	float textX = camera.target.x - textWidth / 2.0f;
	float textY = camera.target.y + END_MESSAGE_Y_OFFSET;
	DrawText(END_MESSAGE, textX, textY, END_MESSAGE_FONT_SIZE, BLACK);

	DrawTexture(loverTexture, camera.target.x - 70, camera.target.y - END_CHARACTERS_Y_OFFSET, WHITE);
	DrawTexture(loveTexture, camera.target.x, camera.target.y - END_CHARACTERS_Y_OFFSET, WHITE);

	return false;
}

void UIController::UpdateDistanceText(const Camera2D& camera, const int distance)
{
	float textX = camera.target.x - distanceTextSize / 2.0f;
	float textY = 30;

	//Considering background margin = 10
	DrawRectangle(textX - 10, textY - 10, distanceTextSize + 20, DISTANCE_TEXT_FONT_SIZE + 20, { 240, 125, 160, 150 });
	DrawText(TextFormat("Distance to Love: %im", distance), textX, textY, DISTANCE_TEXT_FONT_SIZE, BLACK);
}
