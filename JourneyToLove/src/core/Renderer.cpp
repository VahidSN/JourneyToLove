#include "Renderer.h"

Renderer::Renderer() : color(BLACK), texture{ 0 } // Zero-initialize texture
{
}

Renderer::~Renderer()
{
	if (texture.id != 0)
	{
		UnloadTexture(texture);
	}
}

void Renderer::Update(const Vector2& position, const float size) const
{
	DrawTexture(position, { size, size });
}

void Renderer::SetColor(const Color _color)
{
	color = _color;
}

void Renderer::SetTexture(const std::string& texturePath)
{
	if (texture.id != 0) 
	{
		UnloadTexture(texture);
	}
	texture = LoadTexture(texturePath.c_str());
	if (texture.id == 0) 
	{
		throw std::runtime_error("Failed to load texture: " + texturePath);
	}
}

void Renderer::DrawTexture(const Vector2& poisition, const Vector2& size) const
{
	// Get the original texture dimensions
	float originalWidth = (float)texture.width;
	float originalHeight = (float)texture.height;

	// Define the source rectangle (the whole texture)
	Rectangle sourceRec = { 0.0f, 0.0f, originalWidth, originalHeight };

	// Define the destination rectangle (the new size and position)
	Rectangle destRec = { poisition.x, poisition.y, size.x, size.y };

	// Define the origin for rotation/scaling (usually the top-left corner)
	Vector2 origin = { 0.0f, 0.0f };

	// Draw the texture to the new size
	DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}
