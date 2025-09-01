#pragma once
#include "raylib.h"
#include <vector>

class Scene;

struct Checkpoint
{
	Vector2 location;
	bool activated = false;
};

class Level
{
public:
	Level();
	~Level();

	void Render();
	void Reset();
	void Finish();

	int DetectCellType(const Vector2& position) const;
	float GetLevelXLength() const;
	Vector2 GetStartBlockPosition() const;
	bool CheckFinish() const;
	void ActivateCheckpoint(const Vector2& checkpointPosition);

private:
	std::vector<std::vector<int>> levelLayout{};
	std::vector<Checkpoint> checkpoints;
	float levelXLength{ 0.0f };
	bool isFinished{ false };

	Texture2D blockTexture;
	Texture2D obstacleTexture;
	Texture2D checkpointTexture;
	Texture2D endpointTexture;
	Sound checkpointSound;

	void LoadLevel();
	void ResetCheckpoints();

	void DrawTexture(const Texture2D& texture, const Vector2& poisition, const  Vector2& size,
		const float rotation = 0.0f, const Vector2& origin = { 0, 0 }, const Color& color = WHITE) const;
	void DrawObstacle(const size_t rowIndex, const size_t columnIndex) const;
};
