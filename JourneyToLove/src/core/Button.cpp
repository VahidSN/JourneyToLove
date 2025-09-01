#include "Button.h"

Button::Button(Rectangle rect, const char* text, int fontSize)
    : rect(rect), text(text), fontSize(fontSize), state(ButtonState::NORMAL), wasClicked(false)
{}

void Button::Update(const Camera2D& camera)
{
    Vector2 mouseScreenPos = GetMousePosition();
    Vector2 mouseWorldPos = GetScreenToWorld2D(mouseScreenPos, camera);
    wasClicked = false;

    if (CheckCollisionPointRec(mouseWorldPos, rect))
    {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            state = ButtonState::CLICKED;
        }
        else
        {
            state = ButtonState::HOVERED;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
        {
            wasClicked = true;
        }
    }
    else
    {
        state = ButtonState::NORMAL;
    }
}

void Button::Draw()
{
    Color buttonColor = WHITE;
    switch (state)
    {
        case ButtonState::NORMAL:
            buttonColor = WHITE;
            break;
        case ButtonState::HOVERED:
            buttonColor = LIGHTGRAY;
            break;
        case ButtonState::CLICKED:
            buttonColor = DARKGRAY;
            break;
    }
    DrawRectangleRec(rect, buttonColor);

    // Calculate text position to center it
    int textWidth = MeasureText(text, fontSize);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;
    DrawText(text, textX, textY, fontSize, BLACK);
}

bool Button::IsClicked() const
{
    return wasClicked;
}

void Button::SetCenterPosition(float x, float y)
{
    rect.x = x - rect.width / 2.0f;
    rect.y = y - rect.height / 2.0f;
}

float Button::GetWidth() const
{
	return rect.width;
}

float Button::GetHeight() const
{
    return rect.height;
}
