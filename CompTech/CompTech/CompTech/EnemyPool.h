#ifndef ENEMYPOOL_H
#define ENEMYPOOL_H
#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <algorithm>
#include "GameObject.h"

class EnemyPool
{
#define PREALLOCAMOUNT 100
#define INSERTTHRESHOLD 5

public:
	int Activate(unsigned int);
	void Deactivate(unsigned int);
	int InitialMove(int, int, int);
	void Update(float, int, int);
	std::vector<SDL_Rect>* Physics();
	void Render(SDL_Renderer*);

	static EnemyPool& getInstance();
	int getActive();

	EnemyPool(EnemyPool const&) = delete;
	void operator=(EnemyPool const&) = delete;

private:
	EnemyPool();
	void Alloc();
	static bool Compare(std::tuple<float, float>&, std::tuple<float, float>&);
	void InsertionSort(int, int, int);
	/*void Merge(int, std::vector<std::tuple<float, float>>&, int, int, int, int, int, int);
	void MergeSortNoCopy(std::vector<std::tuple<float, float>>&, int, int);
	void MergeSort();*/
	void Deactivate();

	unsigned int active = 0;
	std::vector<SDL_Rect> rects;
	std::vector<std::tuple<float, float>> positions;
	std::vector<SDL_Point> dirs;
	std::vector<GameObject> gos;
	std::vector<int> deactivation;

	std::default_random_engine rng;
};

#endif