#include "EnemyPool.h"

EnemyPool::EnemyPool()
{
	Alloc();
}

EnemyPool& EnemyPool::getInstance()
{
	static EnemyPool instance;
	return instance;
}

int EnemyPool::getActive()
{
	return active;
}

void EnemyPool::Alloc()
{
	std::uniform_int_distribution<int> dist(0, 2);
	int reservation = (int) (ceil(active / PREALLOCAMOUNT) + 1) * PREALLOCAMOUNT;
	rects.reserve(reservation);
	positions.reserve(reservation);
	dirs.reserve(reservation);
	gos.reserve(reservation);

	for (int c = 0; c < PREALLOCAMOUNT; c++)
	{
		rects.emplace_back(SDL_Rect{ 0, 0, WIDTH, HEIGHT });
		positions.emplace_back(std::make_tuple(0.f, 0.f));
		dirs.emplace_back(SDL_Point{ static_cast<int>(dist(rng) - 1) * SPEED, static_cast<int>(dist(rng) - 1) * SPEED});
		gos.emplace_back();
		gos.back().InitialIndex(gos.size() - 1);
	}
}

bool EnemyPool::Compare(std::tuple<float, float>& a, std::tuple<float, float>& b)
{
    if (std::get<0>(a) == std::get<0>(b))
        return std::get<1>(a) > std::get<1>(b);
    return std::get<0>(a) > std::get<0>(b);
}

void EnemyPool::InsertionSort(int index, int l, int r)
{
	SDL_Rect rtemp;
	std::tuple<float, float> ptemp;
	SDL_Point dtemp;
	GameObject gtemp;
	for (int c = 1, q; c < r + 1; c++)
	{
		rtemp = rects[index + c];
		ptemp = positions[index + c];
		dtemp = dirs[index + c];
		gtemp = gos[index + c];
		q = c;
		while ((q > 0) && Compare(positions[index + q - 1], ptemp))
		{
			rects[index + q] = rects[index + q - 1];
			positions[index + q] = positions[index + q - 1];
			dirs[index + q] = dirs[index + q - 1];
			gos[index + q] = gos[index + q - 1];
			q = q - 1;
		}
		rects[index + q] = rtemp;
		positions[index + q] = ptemp;
		dirs[index + q] = dtemp;
		gos[index + q] = gtemp;
	}
}

/*void EnemyPool::Merge(int startIndex, std::vector<std::tuple<float, float>>& aux, int auxL, int startAuxL, int sizeAuxL, int auxR, int startAuxR, int sizeAuxR)
{
	for (int c = startAuxL, q = startAuxR, z = startIndex; z < (sizeAuxL + sizeAuxR); z++)
	{
		if (c == sizeAuxL)
		{
			positions[z] = aux[auxR + q++];
			continue;
		}
		if (q == sizeAuxR)
		{
			positions[z] = aux[auxL + c++];
			continue;
		}
		positions[z] = Compare(aux[auxL + c], aux[auxR + q]) ? aux[auxL + c++] : aux[auxR + q++];
	}
}

void EnemyPool::MergeSortNoCopy(std::vector<std::tuple<float, float>>& aux, int left, int right)
{
	if (right - left <= INSERTTHRESHOLD)
	{
		InsertionSort(left, left, right - left);
		return;
	}
	int halfSize = (left + right) / 2;
	MergeSortNoCopy(aux, left, halfSize);
	MergeSortNoCopy(aux, halfSize + 1, right);
	Merge(left, aux, halfSize, left, halfSize - left + 1, halfSize + 1, halfSize + 1, right - halfSize);
}

void EnemyPool::MergeSort()
{
	std::vector<std::tuple<float, float>> aux = positions;
	MergeSortNoCopy(aux, 0, active - 1);
}*/

int EnemyPool::InitialMove(int index, int x, int y)
{
	std::get<0>(positions[index]) = (float) x;
	std::get<1>(positions[index]) = (float) y;

	rects[index].x = x;
	rects[index].y = y;

	return index;
}

int EnemyPool::Activate(unsigned int amount)
{
	while (gos.size() < active + amount)
		Alloc();

	int newBlock = active;
	active += amount;
	return newBlock;
}

void EnemyPool::Deactivate(unsigned int index)
{
	deactivation.push_back(index);
}

void EnemyPool::Deactivate()
{
	std::sort(deactivation.begin(), deactivation.end(), std::greater<int>());
	for (int& each : deactivation)
	{
		if (each > active)
			continue;

		std::swap(rects[each], rects[active - 1]);
		std::swap(positions[each], positions[active - 1]);
		std::swap(dirs[each], dirs[active - 1]);
		gos[active - 1].InitialIndex(each);
		std::swap(gos[each], gos[active - 1]);

		active--;
	}
	deactivation.clear();
}

void EnemyPool::Update(float deltaTime, int WINWIDTH, int WINHEIGHT)
{
	for (unsigned int c = 0; c < active; c++)
	{
		std::get<0>(positions[c]) += dirs[c].x * deltaTime;
		std::get<1>(positions[c]) += dirs[c].y * deltaTime;

		rects[c].x = (int) round(std::get<0>(positions[c]));
		rects[c].y = (int) round(std::get<1>(positions[c]));

		if ((std::get<0>(positions[c]) < 0 - WIDTH || std::get<0>(positions[c]) > WINWIDTH + WIDTH) || (std::get<1>(positions[c]) < 0 - HEIGHT || std::get<1>(positions[c]) > WINHEIGHT + HEIGHT))
			deactivation.push_back(c);
	}

    std::cout << "EnemyPool active: " << active << std::endl;
}

std::vector<SDL_Rect>* EnemyPool::Physics()
{
	InsertionSort(0, 0, active - 1);

	return &rects;
}

void EnemyPool::Render(SDL_Renderer* render)
{
	SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
	for (unsigned int c = 0; c < active; c++)
		SDL_RenderFillRect(render, &rects[c]);

	Deactivate();
}