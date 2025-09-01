#pragma once
#include "raylib.h"
#include "core/GameObject.h"

class Scene;

class Player : public GameObject
{
public:
	Player();
	~Player();

	void SetXMaxPos(const float xValue);
	void Update(Scene* scene) override;
	void Spawn(const Vector2& pos, const float maxXValue);

	void SetPosition(const Vector2& pos);
	void SetXPosition(const float x);
	void SetYPosition(const float y);
	Vector2 GetPosition() const;

	int GetHealth() const;
	float GetDistanceToEnd() const;

private:
	float verticalMoveTimer{ 0.0f };
	float jumpForce{ 0.0f };
	float maxXPos{ 0.0f };
	float currentYVelocity{ 0.0f };

	int health{ 1 };

	Sound jumpSound;
	Sound dieSound;

	void HorizontalMove(Scene* scene, float deltaTime);
	void VerticalMove(Scene* scene, float deltaTime);
	void Die();

	enum class VerticalMoveState
	{
		Idle,
		JumpRise,
		JumpFall,
		Fall
	};
	VerticalMoveState verticalMoveState{ VerticalMoveState::Idle };
};
