#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <random>
#include "GameObject.h"
#include "EnemyPool.h"
#include "BulletPool.h"

class Game
{
#define WINWIDTH 800
#define WINHEIGHT 600
#define MOVESPEED 500
#define SPAWNINTERVAL 500
#define SPAWNAMOUNT 10

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
	static unsigned int UpdateSpawn(unsigned int, void*);
	void Spawn();
	void GameLogic();
	static bool Compare(SDL_Rect&, SDL_Rect&);
	void Physics();
	void Update();
	void Render();

	Uint64 prevTicks;
	float deltaTime;
	inline static bool doSpawn;
	bool playing;
	int down;
	int right;
	std::default_random_engine rng;
	EnemyPool* enemyPool;
	BulletPool* bulletPool;
	SDL_Window* wnd;
	SDL_Renderer* render;

	GameObject player;
};

#endif