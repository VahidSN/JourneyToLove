#include "Scene.h"
#include "raymath.h"
#include "core/GameSetting.h"
#include "UIController.h"
#include "Player.h"

constexpr const char* MAIN_MUSIC_PATH = "Assets/Audios/Music.ogg";
constexpr const char* END_SOUND_PATH = "Assets/Audios/End.ogg";

Scene::Scene() : camera({ 0 }), player()
{
	level = std::make_unique<Level>();
	uiController = std::make_unique<UIController>();

	mainMusic = LoadMusicStream(MAIN_MUSIC_PATH);
	endSound = LoadSound(END_SOUND_PATH);
}

Scene::~Scene()
{
	UnloadMusicStream(mainMusic);
	UnloadSound(endSound);
}

void Scene::Load()
{
	player.Spawn(level->GetStartBlockPosition(), level->GetLevelXLength());

	camera.target = player.GetPosition();
	camera.offset = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	PlayMusicStream(mainMusic);
	mainMusic.looping = true;
}

void Scene::Update()
{
	BeginMode2D(camera);

	if (!level->CheckFinish())
	{
		UpdateMusicStream(mainMusic);
		player.Update(this);

		if (player.GetHealth() > 0)
		{
			level->Render();
			camera.target = { player.GetPosition().x, SCREEN_HEIGHT / 2 };
		}

		player.Render(this);

		uiController->UpdateDistanceText(camera, (int)(player.GetDistanceToEnd() / BLOCK_SIZE) + 1);

		//Only show tutorial at the beginning of the level
		if (camera.target.x < SCREEN_WIDTH / 2)
			uiController->ShowTutorial();
	}
	else if (uiController->UpdateAndDrawEndPanel(camera))
	{
		Reset();
	}

	EndMode2D();
}

void Scene::EndLevel() const
{
	PlaySound(endSound);
	level->Finish();
}

void Scene::SaveCheckpoint(const Vector2& position)
{
	level->ActivateCheckpoint(position);
}

int Scene::DetectCellTypeOnPosition(const Vector2& position) const
{
	return level->DetectCellType(position);
}

Vector2 Scene::GetPlayerPosition() const
{
	return player.GetPosition();
}

void Scene::Reset()
{
	level->Reset();
	player.Spawn(level->GetStartBlockPosition(), level->GetLevelXLength());
	ResetMusic();
}

void Scene::ResetMusic()
{
	//Reset music
	StopMusicStream(mainMusic);
	SeekMusicStream(mainMusic, 0.0f);
	PlayMusicStream(mainMusic);
	mainMusic.looping = true;
}
