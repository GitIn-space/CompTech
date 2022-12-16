#ifndef BULLETPOOL_H
#define BULLETPOOL_H
#include <vector>
#include <random>
#include <math.h>
#include <iostream>
#include <algorithm>
#include "GameObject.h"

class BulletPool
{
#define PREALLOCAMOUNT 100
#define INSERTTHRESHOLD 5

public:
	int Activate(unsigned int);
	void Deactivate(unsigned int);
	int InitialMove(int, int, int);
	int InitialDirection(int, int, int);
	void Update(float, int, int);
	std::vector<SDL_Rect>* Physics();
	void Render(SDL_Renderer*);

	static BulletPool& getInstance();
	int getActive();

	BulletPool(BulletPool const&) = delete;
	void operator=(BulletPool const&) = delete;

private:
	BulletPool();
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
	std::vector<unsigned int> deactivation;

	std::default_random_engine rng;
};

#endif