#pragma once
#include "raylib.h"
#include <stdexcept>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Update(const Vector2& position, const float size) const;

	void SetColor(const Color _color);
	void SetTexture(const std::string& texturePath);

private:
	Color color;
	Texture2D texture;

	void DrawTexture(const Vector2& poisition, const Vector2& size) const;
};
