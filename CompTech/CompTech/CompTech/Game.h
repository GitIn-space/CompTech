#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include "GameObject.h"

class Game
{
#define WIDTH 800
#define HEIGHT 600
#define MOVESPEED 500

public:
	void Start();
	void Remove(GameObject*);

	static Game& getInstance();

	Game(Game const&) = delete;
	void operator=(Game const&) = delete;

private:
	Game();
	void CalcFrameRate();
	void EventHandler();
	void GameLogic();
	void Physics();
	void Update();
	void Render();
	void DeleteObjects();

	Uint64 prevTicks;
	float deltaTime;
	bool playing;
	bool left;
	bool right;
	SDL_Window* wnd;
	SDL_Renderer* render;
	std::vector<std::shared_ptr<GameObject>> renderQueue;
	std::vector<std::shared_ptr<GameObject>> updateQueue;
	std::vector<std::shared_ptr<GameObject>> physicsQueue;
	std::vector<GameObject*> deletionQueue;
};

#endif