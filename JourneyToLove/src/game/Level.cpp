#include "Level.h"
#include <iostream>
#include "Core/GameSetting.h"
#include "CollisionUtils.h"
#include "core/CSVReader.h"
#include <algorithm>

constexpr const char* LEVEL_CSV_PATH = "Assets/Level.csv";
constexpr const char* BLOCK_TEXTURE_PATH = "Assets/Textures/Block.png";
constexpr const char* OBSTACLE_TEXTURE_PATH = "Assets/Textures/Obstacle.png";
constexpr const char* CHECKPOINT_TEXTURE_PATH = "Assets/Textures/Checkpoint.png";
constexpr const char* ENDPOINT_TEXTURE_PATH = "Assets/Textures/Endpoint.png";

constexpr const char* CHECKPOINT_SOUND_PATH = "Assets/Audios/Checkpoint.ogg";

const float END_POINT_SIZE = BLOCK_SIZE * 0.6f;

Level::Level()
{
	LoadLevel();
	levelXLength = levelLayout[0].size() * BLOCK_SIZE;

	blockTexture = LoadTexture(BLOCK_TEXTURE_PATH);
	obstacleTexture = LoadTexture(OBSTACLE_TEXTURE_PATH);
	checkpointTexture = LoadTexture(CHECKPOINT_TEXTURE_PATH);
	endpointTexture = LoadTexture(ENDPOINT_TEXTURE_PATH);

	checkpointSound = LoadSound(CHECKPOINT_SOUND_PATH);
}

Level::~Level()
{
	UnloadTexture(blockTexture);
	UnloadTexture(obstacleTexture);
	UnloadTexture(checkpointTexture);
	UnloadTexture(endpointTexture);

	UnloadSound(checkpointSound);
}

void Level::Render()
{
	//Fill Empty Space
	for (float i = -BLOCK_SIZE; i > -SCREEN_WIDTH / 2 - BLOCK_SIZE; i -= BLOCK_SIZE)
	{
		for (float j = 0; j < SCREEN_HEIGHT; j += BLOCK_SIZE)
		{
			DrawTexture(blockTexture, { i, j }, { BLOCK_SIZE, BLOCK_SIZE }, 0, { 0 }, LIGHTGRAY);
		}
	}
	for (float i = levelXLength; i < levelXLength + SCREEN_WIDTH / 2; i += BLOCK_SIZE)
	{
		for (float j = 0; j < SCREEN_HEIGHT; j += BLOCK_SIZE)
		{
			DrawTexture(blockTexture, { i, j }, { BLOCK_SIZE, BLOCK_SIZE }, 0, { 0 }, LIGHTGRAY);
		}
	}

	for (int i = 0; i < levelLayout.size(); ++i)
	{
		for (int j = 0; j < levelLayout[i].size(); ++j)
		{
			if (CollisionUtils::IntToCellType(levelLayout[i][j]) == CollisionUtils::CellType::Block)
			{
				DrawTexture(blockTexture, { j * BLOCK_SIZE, i * BLOCK_SIZE }, { BLOCK_SIZE, BLOCK_SIZE });
			}
			else if (CollisionUtils::IntToCellType(levelLayout[i][j]) == CollisionUtils::CellType::Hazard)
			{
				DrawObstacle(i, j);
			}
			else if (CollisionUtils::IntToCellType(levelLayout[i][j]) == CollisionUtils::CellType::Checkpoint)
			{
				if (std::any_of(checkpoints.begin(), checkpoints.end(), [i, j](const Checkpoint& cp)
					{
						return cp.location.x == j * BLOCK_SIZE && cp.location.y == i * BLOCK_SIZE && cp.activated;
					}))
				{
					//Draw activated checkpoints
					DrawTexture(checkpointTexture, { j * BLOCK_SIZE, i * BLOCK_SIZE }, { BLOCK_SIZE, BLOCK_SIZE });
				}
				else
				{
					//Draw unactivated checkpoints
					DrawTexture(checkpointTexture, { j * BLOCK_SIZE, i * BLOCK_SIZE }, { BLOCK_SIZE, BLOCK_SIZE }, 0.0f, { 0 }, GRAY);
				}
			}
			else if (CollisionUtils::IntToCellType(levelLayout[i][j]) == CollisionUtils::CellType::EndPoint)
			{
				//Place the endpoint at the bottom center of the block
				DrawTexture(endpointTexture, {
					j * BLOCK_SIZE + BLOCK_SIZE / 2.0f - END_POINT_SIZE / 2.0f,
					i * BLOCK_SIZE + BLOCK_SIZE - END_POINT_SIZE },
					{ END_POINT_SIZE, END_POINT_SIZE });
			}
		}
	}
}

int Level::DetectCellType(const Vector2& position) const
{
	return levelLayout[int(position.y / BLOCK_SIZE)][int(position.x / BLOCK_SIZE)];
}

float Level::GetLevelXLength() const
{
	return levelXLength;
}

Vector2 Level::GetStartBlockPosition() const
{
	//Return the block position under foremost activated checkpoint
	float maxSavedX = -1.0f;
	int selectedCheckpointIndex = -1;
	for (int i = checkpoints.size() - 1; i >= 0; --i)
	{
		if (checkpoints[i].activated && checkpoints[i].location.x > maxSavedX)
		{
			maxSavedX = checkpoints[i].location.x;
			selectedCheckpointIndex = i;
		}
	}
	if (maxSavedX != -1.0f) //Checkpoint is found
	{
		return { maxSavedX, checkpoints[selectedCheckpointIndex].location.y + BLOCK_SIZE };
	}

	//If no checkpoint is activated, return the first block position
	for (int j = 1; j < levelLayout.size(); ++j)
	{
		if (CollisionUtils::IntToCellType(levelLayout[j][0]) == CollisionUtils::CellType::Block &&
			CollisionUtils::IntToCellType(levelLayout[j - 1][0]) == CollisionUtils::CellType::None)
		{
			return { 0, j * BLOCK_SIZE };
		}
	}

	return Vector2();
}

void Level::Reset()
{
	isFinished = false;
	ResetCheckpoints();
}

bool Level::CheckFinish() const
{
	return isFinished;
}

void Level::Finish()
{
	isFinished = true;
}

void Level::ActivateCheckpoint(const Vector2& checkpointPosition)
{
	for (size_t i = 0; i < checkpoints.size(); i++)
	{
		if (checkpoints[i].location.x == checkpointPosition.x &&
			checkpoints[i].location.y == checkpointPosition.y &&
			!checkpoints[i].activated)
		{
			checkpoints[i].activated = true;
			PlaySound(checkpointSound);
			break;
		}
	}
}

void Level::ResetCheckpoints()
{
	for (auto& cp : checkpoints)
	{
		cp.activated = false;
	}
}

void Level::LoadLevel()
{
	CSVReader reader;
	if (reader.ReadData(LEVEL_CSV_PATH))
	{
		std::cout << "Successfully read data from data.csv." << std::endl;

		const auto& data = reader.GetData();
		if (data.empty())
		{
			std::cout << "File is empty or contains no data." << std::endl;
			return;
		}

		checkpoints.clear(); // Clear previous checkpoints

		levelLayout.resize(VERTICAL_BLOCKS_COUNT, std::vector<int>(data[VERTICAL_BLOCKS_COUNT - 1].size(), 0));
		for (int i = 0; i < data.size(); i++)
		{
			for (int j = 0; j < data[i].size(); j++)
			{
				if (data[i][j] == "1")
				{
					levelLayout[i][j] = 1;
				}
				else if (data[i][j] == "2")
				{
					levelLayout[i][j] = 2;
				}
				else if (data[i][j] == "3")
				{
					levelLayout[i][j] = 3;
					checkpoints.push_back(Checkpoint{ Vector2{ j * BLOCK_SIZE, i * BLOCK_SIZE }, false });
				}
				else if (data[i][j] == "4")
				{
					levelLayout[i][j] = 4;
				}
			}
		}
	}
	else
	{
		std::cerr << "Failed to read data from file." << std::endl;
		return;
	}

	return;
}

void Level::DrawTexture(const Texture2D& texture, const Vector2& poisition, const Vector2& size,
	const float rotation, const Vector2& origin, const Color& color) const
{
	// Get the original texture dimensions
	float originalWidth = (float)texture.width;
	float originalHeight = (float)texture.height;

	// Define the source rectangle (the whole texture)
	Rectangle sourceRec = { 0.0f, 0.0f, originalWidth, originalHeight };

	// Define the destination rectangle (the new size and position)
	Rectangle destRec = { poisition.x, poisition.y, size.x, size.y };

	// Draw the texture to the new size based on the origin for rotation/scaling (top-left corner by default)
	DrawTexturePro(texture, sourceRec, destRec, origin, rotation, color);
}

void Level::DrawObstacle(const size_t row, const size_t column) const
{
	Vector2 obstaclePosition = { column * BLOCK_SIZE, row * BLOCK_SIZE };
	int blockTypeValue = (int)CollisionUtils::CellType::Block;

	if (row < levelLayout.size() - 1 && levelLayout[row + 1][column] == blockTypeValue) //Upward
	{
		DrawTexture(obstacleTexture, obstaclePosition, { BLOCK_SIZE , BLOCK_SIZE }, 0, { 0,0 });
		return;
	}
	else if (row > 0 && levelLayout[row - 1][column] == blockTypeValue) //Downward
	{
		DrawTexture(obstacleTexture, obstaclePosition, { BLOCK_SIZE , BLOCK_SIZE }, 180, { BLOCK_SIZE, BLOCK_SIZE });
		return;
	}
	else if (column < levelLayout[row].size() - 1 && levelLayout[row][column + 1] == blockTypeValue) //Leftward
	{
		DrawTexture(obstacleTexture, obstaclePosition, { BLOCK_SIZE , BLOCK_SIZE }, 270, { BLOCK_SIZE, 0 });
		return;
	}
	else if (column > 0 && levelLayout[row][column - 1] == blockTypeValue) //Rightward
	{
		DrawTexture(obstacleTexture, obstaclePosition, { BLOCK_SIZE , BLOCK_SIZE }, 90, { 0, BLOCK_SIZE });
		return;
	}
}

