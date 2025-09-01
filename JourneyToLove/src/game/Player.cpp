#include "Player.h"
#include "Scene.h"
#include "raymath.h"
#include "core/GameSetting.h"
#include "CollisionUtils.h"

constexpr const char* CHARACTER_TEXTURE_PATH = "Assets/Textures/Character.png";
constexpr const char* JUMP_SOUND_PATH = "Assets/Audios/JumpSFX.ogg";
constexpr const char* DIE_SOUND_PATH = "Assets/Audios/Die.ogg";

const float JUMP_DURATION = 0.25f; // 0.25 Sec.
const float FALL_TERMINAL_VELOCITY = 1500.f;
const float CHARACTER_SIZE = BLOCK_SIZE * 0.6f;
const float VERTICAL_SPEED = 400.0;
const float JUMP_HEIGHT = 4.0f * BLOCK_SIZE;
const float GRAVITY = (2.0f * JUMP_HEIGHT) / (JUMP_DURATION * JUMP_DURATION); // g = (-2H/T^2)

Player::Player()
{
	renderer.SetTexture(CHARACTER_TEXTURE_PATH);
	size = CHARACTER_SIZE;
	health = 1;

	jumpSound = LoadSound(JUMP_SOUND_PATH);
	dieSound = LoadSound(DIE_SOUND_PATH);
}

Player::~Player()
{
	UnloadSound(jumpSound);
	UnloadSound(dieSound);
}

void Player::SetXMaxPos(const float xValue)
{
	maxXPos = xValue - CHARACTER_SIZE;
}

void Player::Update(Scene* scene)
{
	GameObject::Update(scene);

	float deltaTime = GetFrameTime();
	if (health > 0)
	{
		HorizontalMove(scene, deltaTime);
		VerticalMove(scene, deltaTime);
	}
	else //Die efffect: falling down until out of screen
	{
		verticalMoveTimer += deltaTime;
		currentYVelocity += GRAVITY * deltaTime;
		position.y += currentYVelocity * deltaTime;
		if (position.y > SCREEN_HEIGHT)
			scene->Load();
	}
}

int Player::GetHealth() const
{
	return health;
}

float Player::GetDistanceToEnd() const
{
	return maxXPos - position.x - BLOCK_SIZE;
}

void Player::Spawn(const Vector2& startPos, const float maxXValue)
{
	// Center the player on the starting block
	position = { startPos.x + BLOCK_SIZE / 2 - size / 2, startPos.y - size };
	maxXPos = maxXValue - CHARACTER_SIZE;

	health = 1;
	verticalMoveState = VerticalMoveState::Idle;
	currentYVelocity = 0;
	jumpForce = 0;
	verticalMoveTimer = 0.0f;
}

void Player::SetPosition(const Vector2& pos)
{
	position = pos;
}
void Player::SetXPosition(const float x)
{
	position.x = x;
}
void Player::SetYPosition(const float y)
{
	position.y = y;
}

Vector2 Player::GetPosition() const
{
	return position;
}

void Player::HorizontalMove(Scene* scene, float deltaTime)
{
	if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
		return;

	// Direction: -1 = left, +1 = right
	int direction = IsKeyDown(KEY_A) ? -1 : 1;

	float nextXPos = position.x + direction * VERTICAL_SPEED * deltaTime;

	// Clamp to world bounds
	if (direction < 0 && nextXPos < 0)
	{
		position.x = 0;
		return;
	}
	else if (direction > 0 && nextXPos > maxXPos)
	{
		position.x = maxXPos;
		return;
	}

	// Collision detection depends on direction
	CollisionUtils::CellType cellType = (direction < 0)
		? CollisionUtils::DetectLeftCellOfObject(scene, { nextXPos, position.y }, size)
		: CollisionUtils::DetectRightCellOfObject(scene, { nextXPos, position.y }, size);

	switch (cellType)
	{
	case CollisionUtils::CellType::Block:
		if (direction < 0)
			nextXPos = (int(nextXPos / BLOCK_SIZE) + 1) * BLOCK_SIZE; // snap left
		else
			nextXPos = (int(nextXPos / BLOCK_SIZE) + 1) * BLOCK_SIZE - size; // snap right
		break;
	case CollisionUtils::CellType::Hazard:
		Die();
		break;
	case CollisionUtils::CellType::EndPoint:
		scene->EndLevel();
		break;
	case CollisionUtils::CellType::Checkpoint:
		// Save checkpoint at the left/top corner of the block
		if (direction < 0)
			scene->SaveCheckpoint({ (int(position.x / BLOCK_SIZE)) * BLOCK_SIZE , (int(position.y / BLOCK_SIZE)) * BLOCK_SIZE });
		else
			scene->SaveCheckpoint({ (int(position.x / BLOCK_SIZE) + 1) * BLOCK_SIZE , (int(position.y / BLOCK_SIZE)) * BLOCK_SIZE });
		position.x = nextXPos;
		break;
	case CollisionUtils::CellType::None:
	default:
		position.x = nextXPos;
		break;
	}

	//Check ground
	if (verticalMoveState == VerticalMoveState::Idle &&
		CollisionUtils::DetectDownCellOfObject(scene, position, size) != CollisionUtils::CellType::Block)
	{
		verticalMoveState = VerticalMoveState::Fall;
		currentYVelocity = 0;
	}
}

void Player::VerticalMove(Scene* scene, float deltaTime)
{
	//Check Jump Input
	if (IsKeyPressed(KEY_W) &&
		(verticalMoveState == VerticalMoveState::Idle || verticalMoveState == VerticalMoveState::Fall))
	{
		verticalMoveState = VerticalMoveState::JumpRise;
		verticalMoveTimer = 0.0f;

		// Initial jump velocity (negative = up on screen)
		jumpForce = (2.0f * JUMP_HEIGHT) / JUMP_DURATION; // V0 = 2H/T
		currentYVelocity = -jumpForce;

		PlaySound(jumpSound);
	}

	//Apply physics if in air (jumping or falling)
	if (verticalMoveState != VerticalMoveState::Idle)
	{
		verticalMoveTimer += deltaTime;
		currentYVelocity += GRAVITY * deltaTime; //V = gt + V0

		// Clamp to terminal fall speed
		if (currentYVelocity > FALL_TERMINAL_VELOCITY)
			currentYVelocity = FALL_TERMINAL_VELOCITY;

		// Calculate next position
		float nextYPos = position.y + currentYVelocity * deltaTime;
		bool isMovingUp = currentYVelocity < 0;

		// Collision Detection
		auto cell = isMovingUp
			? CollisionUtils::DetectUpCellOfObject(scene, { position.x, nextYPos }, size)
			: CollisionUtils::DetectDownCellOfObject(scene, { position.x, nextYPos }, size);
		switch (cell)
		{
		case CollisionUtils::CellType::Block:
			if (isMovingUp)
			{
				verticalMoveState = VerticalMoveState::JumpFall; // start falling
			}
			else
			{
				position.y = (int(nextYPos / BLOCK_SIZE) + 1) * BLOCK_SIZE - size; // snap to ground
				verticalMoveState = VerticalMoveState::Idle;
			}
			currentYVelocity = 0;
			break;
		case CollisionUtils::CellType::Hazard:
			Die();
			break;
		case CollisionUtils::CellType::EndPoint:
			scene->EndLevel();
			break;
		case CollisionUtils::CellType::Checkpoint:
			// Save checkpoint at the left/top corner of the block
			if (isMovingUp) 
				scene->SaveCheckpoint({ (int(position.x / BLOCK_SIZE)) * BLOCK_SIZE , (int(position.y / BLOCK_SIZE)) * BLOCK_SIZE });
			else 
				scene->SaveCheckpoint({ (int(position.x / BLOCK_SIZE)) * BLOCK_SIZE , (int(position.y / BLOCK_SIZE) + 1) * BLOCK_SIZE });
			position.y = nextYPos;
			break;
		case CollisionUtils::CellType::None:
		default:
			position.y = nextYPos;
			break;
		}
	}

	//Keep within screen bounds
	int screenHeight = GetScreenHeight();
	if (position.y < 0)
	{
		position.y = 0;
		currentYVelocity = 0;
		verticalMoveState = VerticalMoveState::Idle;
	}
	if (position.y > screenHeight - size)
	{
		position.y = float(screenHeight - size);
		currentYVelocity = 0;
		verticalMoveState = VerticalMoveState::Idle;
	}
}

void Player::Die()
{
	health -= 1;
	currentYVelocity = 0;
	PlaySound(dieSound);
}
