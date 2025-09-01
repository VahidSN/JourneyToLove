#pragma once
#include "Renderer.h"

class Scene;

class GameObject
{
public:
	GameObject();
	~GameObject();


	virtual void Start();
	virtual void Update(Scene* scene);
	virtual void Render(Scene* scene) const;

	float GetSize() const;

protected:
	Renderer renderer{};
	Vector2 position{ 0 };
	float size{ 0.0f };
};