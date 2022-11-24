#include "Game.h"

Game::Game()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	wnd = SDL_CreateWindow("Arkanoid-look-alike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, 0);
	render = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);

	prevTicks = SDL_GetPerformanceCounter();
	deltaTime = 0.0f;
	playing = false;
	left = false;
	right = false;

	renderQueue.emplace_back(std::shared_ptr<GameObject>((new GameObject(232, 0, 254))->InitialMove(0, 0)->InitialDimensions(800, 16)));
	physicsQueue.emplace_back(renderQueue.back());
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
	if (SDL_PollEvent(&event))
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
				left = true;
			if (scancode == SDL_SCANCODE_D)
				right = true;

			break;
		}

		case SDL_KEYUP:
		{
			scancode = event.key.keysym.scancode;

			if (scancode == SDL_SCANCODE_A)
				left = false;
			if (scancode == SDL_SCANCODE_D)
				right = false;

			break;
		}

		case SDL_MOUSEBUTTONDOWN:
		{
			std::cout << event.button.clicks << std::endl;
			break;
		}
		}
	}
}

void Game::GameLogic()
{
	
}

void Game::Physics()
{
	for (std::shared_ptr<GameObject> each : physicsQueue)
		/*/if (SDL_HasIntersection(ball->GetRect(), each->GetRect()))
			ball->Collision(each.get());*/;
}

void Game::Update()
{
	for (std::shared_ptr<GameObject> each : updateQueue)
		each.get()->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
	SDL_RenderClear(render);

	for (std::shared_ptr<GameObject> each : renderQueue)
		each->Render(render);

	SDL_RenderPresent(render);

	SDL_Delay(4);
}

void Game::Remove(GameObject* go)
{
	deletionQueue.emplace_back(go);
}

void Game::DeleteObjects()
{
	for (GameObject* go : deletionQueue)
	{
		for (std::shared_ptr<GameObject>& each : renderQueue)
			if (each.get() == go)
			{
				each = nullptr;
				renderQueue.erase(std::remove(renderQueue.begin(), renderQueue.end(), each));
				break;
			}

		for (std::shared_ptr<GameObject>& each : updateQueue)
			if (each.get() == go)
			{
				each = nullptr;
				updateQueue.erase(std::remove(updateQueue.begin(), updateQueue.end(), each));
				break;
			}

		for (std::shared_ptr<GameObject>& each : physicsQueue)
			if (each.get() == go)
			{
				each = nullptr;
				physicsQueue.erase(std::remove(physicsQueue.begin(), physicsQueue.end(), each));
				break;
			}
		go = nullptr;
	}

	deletionQueue.clear();
}

void Game::Start()
{
	playing = true;
	while (playing)
	{
		CalcFrameRate();
		EventHandler();
		GameLogic();
		Physics();
		Update();
		Render();
		DeleteObjects();
	}

	for (std::shared_ptr<GameObject> each : renderQueue)
		each = nullptr;

	for (std::shared_ptr<GameObject> each : updateQueue)
		each = nullptr;

	for (std::shared_ptr<GameObject> each : physicsQueue)
		each = nullptr;

	renderQueue.clear();
	updateQueue.clear();
	physicsQueue.clear();

	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(wnd);
}