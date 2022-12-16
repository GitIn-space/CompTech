#include "GameObject.h"

GameObject::GameObject()
{
	index = -1;
	rect = {0, 0, WIDTH, HEIGHT};
	position = std::make_tuple(0.f, 0.f);
	direction = { 0, 0 };
}

GameObject::GameObject(int index)
{
	this->index = index;
	rect = SDL_Rect();
	position = std::make_tuple(0.f, 0.f);
	direction = { 0, 0 };
}

GameObject* GameObject::InitialMove(int x, int y)
{
	std::get<0>(position) = (float) x;
	std::get<1>(position) = (float) y;

	rect.x = x;
	rect.y = y;

	return this;
}

GameObject* GameObject::InitialIndex(int index)
{
	this->index = index;

	return this;
}

void GameObject::SetDirection(int x, int y)
{
	direction.x = x * SPEED;
	direction.y = y * SPEED;
}

void GameObject::Update(float deltaTime)
{
	std::get<0>(position) += direction.x * deltaTime;
	std::get<1>(position) += direction.y * deltaTime;

	rect.x = (int) round(std::get<0>(position));
	rect.y = (int) round(std::get<1>(position));
}

void GameObject::Render(SDL_Renderer* render)
{
	SDL_RenderFillRect(render, &rect);
}

GameObject* GameObject::InitialDimensions(int w, int h)
{
	rect.w = w;
	rect.h = h;

	return this;
}

void GameObject::Collision(GameObject* collider)
{

}

int GameObject::GetIndex()
{
	return index;
}

SDL_Rect* GameObject::GetRect()
{
	return &rect;
}