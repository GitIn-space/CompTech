#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <SDL.h>

class Arkanoid;

class GameObject
{
public:
	GameObject() = delete;
	GameObject(Uint8, Uint8, Uint8);
	virtual GameObject* InitialMove(int, int);
	virtual GameObject* InitialDimensions(int, int);
	SDL_Rect* GetRect();
	SDL_Point* GetCentre();
	virtual void Render(SDL_Renderer*);
	virtual void Update();
	virtual void Collision(GameObject*);

protected:
	Uint8 r;
	Uint8 g;
	Uint8 b;
	SDL_Rect go;
	SDL_Point centre;
};

#endif