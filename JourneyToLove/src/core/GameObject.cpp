#include "GameObject.h"

GameObject::GameObject() : position{ 0.0f, 0.0f }, size(0.0f), renderer()
{
}

GameObject::~GameObject()
{
}

void GameObject::Start()
{
}

void GameObject::Update(Scene* scene)
{
}

void GameObject::Render(Scene* scene) const
{
	renderer.Update(position, size);
}

float GameObject::GetSize() const
{
	return size;
}
