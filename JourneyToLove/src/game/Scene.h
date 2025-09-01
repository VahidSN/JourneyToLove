#pragma once
#include "raylib.h"
#include "Player.h"
#include "Level.h"
#include <vector>
#include <memory>

class UIController;

class Scene
{
public:
    Scene();
    ~Scene();

    void Load();
    void Update();
    void EndLevel() const;
    void SaveCheckpoint(const Vector2& poisition);

    int DetectCellTypeOnPosition(const Vector2& position) const;
    Vector2 GetPlayerPosition() const;

private:
    Player player;
    Camera2D camera;
    std::unique_ptr<Level> level;
    std::unique_ptr<UIController> uiController;

    Music mainMusic;
    Sound endSound;

    void Reset();
    void ResetMusic();
};