#include "Game.h"

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	wnd = SDL_CreateWindow("Space Shooter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINWIDTH, WINHEIGHT, 0);
	render = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);

	prevTicks = SDL_GetPerformanceCounter();
	deltaTime = 0.0f;
	doSpawn = false;
	playing = false;
	down = 0;
	right = 0;
	enemyPool = &EnemyPool::getInstance();
	bulletPool = &BulletPool::getInstance();

	player = *(GameObject().InitialMove((int) WINWIDTH / 2, (int) WINHEIGHT / 2));
}

Game& Game::getInstance()
{
	static Game instance;
	return instance;
}

void Game::CalcFrameRate()
{
	Uint64 ticks = SDL_GetPerformanceCounter();

	deltaTime = (float) (ticks - prevTicks) / SDL_GetPerformanceFrequency();

	prevTicks = ticks;
}

void Game::EventHandler()
{
	SDL_Event event;
	int scancode;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			playing = false;
			break;

		case SDL_KEYDOWN:
		{
			scancode = event.key.keysym.scancode;

			if (scancode == SDL_SCANCODE_ESCAPE)
				playing = false;

			if (scancode == SDL_SCANCODE_A)
				right = -1;
			if (scancode == SDL_SCANCODE_D)
				right = 1;
			if (scancode == SDL_SCANCODE_W)
				down = -1;
			if (scancode == SDL_SCANCODE_S)
				down = 1;
			if (scancode == SDL_SCANCODE_B)
				UpdateSpawn(0, 0);

			player.SetDirection(right, down);
			break;
		}

		case SDL_KEYUP:
		{
			scancode = event.key.keysym.scancode;

			if (scancode == SDL_SCANCODE_A)
				right = 0;
			if (scancode == SDL_SCANCODE_D)
				right = 0;
			if (scancode == SDL_SCANCODE_W)
				down = 0;
			if (scancode == SDL_SCANCODE_S)
				down = 0;

			player.SetDirection(right, down);
			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_Rect* rect = player.GetRect();
				bulletPool->InitialDirection(bulletPool->InitialMove(bulletPool->Activate(1), rect->x, rect->y), event.button.x - rect->x, event.button.y - rect->y);
			}
			break;
		}
		}
	}
}

unsigned int Game::UpdateSpawn(unsigned int interval, void* param = 0)
{
	doSpawn = true;

	return interval;
}

void Game::Spawn()
{
	if (doSpawn)
	{
		std::uniform_int_distribution<int> widthDist(0, WINWIDTH);
		std::uniform_int_distribution<int> heightDist(0, WINHEIGHT);

		int newEnemies = getInstance().enemyPool->Activate(SPAWNAMOUNT);
		for (unsigned int c = 0; c < SPAWNAMOUNT; c++, newEnemies++)
			getInstance().enemyPool->InitialMove(newEnemies, widthDist(rng), heightDist(rng));

		doSpawn = false;
	}
}

void Game::GameLogic()
{
	
}

bool Game::Compare(SDL_Rect& a, SDL_Rect& b)
{
	if (a.x == b.x)
		return a.y > b.y;
	return a.x > b.x;
}

void Game::Physics()
{
	std::vector<SDL_Rect>* posEn = enemyPool->Physics();
	std::vector<SDL_Rect>* posBu = bulletPool->Physics();
	std::vector<SDL_Rect*> merged;
	std::vector<int> enemyIndex;
	std::vector<int> bulletIndex;

	merged.reserve(posEn->size() + posBu->size());
	bulletIndex.reserve(posBu->size());
	std::vector<SDL_Rect>::iterator enemy = posEn->begin();
	std::vector<SDL_Rect>::iterator bullet = posBu->begin();
	int eActive = enemyPool->getActive();
	int bActive = bulletPool->getActive();

	while ((enemy != posEn->begin() + eActive) || (bullet != posBu->begin() + bActive))
	{
		if (enemy == posEn->begin() + eActive)
		{
			merged.push_back(&*bullet);
			bulletIndex.push_back(merged.size());
			bullet++;
		}
		else if (bullet == posBu->begin() + bActive)
		{
			merged.push_back(&*enemy);
			enemyIndex.push_back(merged.size());
			enemy++;
		}
		else
		{
			if (Compare(*enemy, *bullet))
			{
				merged.push_back(&*bullet);
				bulletIndex.push_back(merged.size());
				bullet++;
			}
			else
			{
				merged.push_back(&*enemy);
				enemy++;
			}
		}
	}
	
	/*bool comparing = true;
	bool hypSmaller = true;
	int offset;
	std::vector<std::tuple<int, int>> buffer;
	for (unsigned int c = 0; c < bulletIndex.size(); c++)
	{
		offset = -1;
		while (comparing)
		{
			while (bulletIndex[c] + offset > 0 && bulletIndex[c] + offset < merged.size())//(hypSmaller)
			{
				hypSmaller = std::hypot((*merged.at(bulletIndex[c] + offset)).x - (*merged.at(bulletIndex[c])).x, (*merged.at(bulletIndex[c] + offset)).y - (*merged.at(bulletIndex[c])).y) < ( int(HEIGHT) * 2);
				if (offset <= 0)
				{
					if (SDL_HasIntersection(merged.at(bulletIndex[c] + offset), merged.at(bulletIndex[c])))
						buffer.emplace_back(c, bulletIndex[c] + offset - c);
					offset--;
				}
				else
				{
					if (SDL_HasIntersection(merged.at(bulletIndex[c] + offset), merged.at(bulletIndex[c])))
						buffer.emplace_back(c, bulletIndex[c] + offset - c);
					offset++;
				}
			}
			offset = 1;
			if(!hypSmaller)
				comparing = false;
		}
	}
	for (std::tuple<int, int>& each : buffer)
	{
		bulletPool->Deactivate(std::get<0>(each));
		enemyPool->Deactivate(std::get<1>(each));
	}*/
}

void Game::Update()
{
	enemyPool->Update(deltaTime, WINWIDTH, WINHEIGHT);

	player.Update(deltaTime);

	bulletPool->Update(deltaTime, WINWIDTH, WINHEIGHT);
}

void Game::Render()
{
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	enemyPool->Render(render);

	SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
	player.Render(render);

	bulletPool->Render(render);

	SDL_RenderPresent(render);

	SDL_Delay(4);
}

void Game::Remove(GameObject* go)
{
	
}

void Game::Start()
{
	playing = true;
	SDL_AddTimer(SPAWNINTERVAL, &UpdateSpawn, 0);
	
	while (playing)
	{
		CalcFrameRate();
		EventHandler();
		Spawn();
		GameLogic();
		Physics();
		Update();
		Render();

		std::cout << "fps: " << 1 / deltaTime << std::endl;
	}

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(wnd);
}