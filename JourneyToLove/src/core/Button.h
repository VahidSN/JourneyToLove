#pragma once
#include "raylib.h"

// Enum to define the button's state
enum class ButtonState
{
    NORMAL,
    HOVERED,
    CLICKED
};

class Button
{
public:
    Button(Rectangle rect, const char* text, int fontSize = 20);

    void Update(const Camera2D& camera);
    void Draw();
    bool IsClicked() const;
    void SetCenterPosition(float x, float y);

    float GetWidth() const;
    float GetHeight() const;

private:
    Rectangle rect;
    const char* text;
    int fontSize;
    ButtonState state;
    bool wasClicked;
};