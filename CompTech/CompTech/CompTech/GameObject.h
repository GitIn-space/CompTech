#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SDL.h>
#include <tuple>

class GameObject
{
#define HEIGHT 30
#define WIDTH 30
#define SPEED 100
public:
	GameObject();
	GameObject(int);
	virtual GameObject* InitialMove(int, int);
	virtual GameObject* InitialIndex(int);
	virtual GameObject* InitialDimensions(int, int);
	void SetDirection(int, int);
	virtual void Update(float);
	virtual void Render(SDL_Renderer*);
	virtual void Collision(GameObject*);

	virtual int GetIndex();
	virtual SDL_Rect* GetRect();

protected:
	int index;
	SDL_Rect rect;
	std::tuple<float, float> position;
	SDL_Point direction;
};

#endif