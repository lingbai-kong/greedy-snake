#include"wall.h"
using namespace std;
Walls::Walls(int X, int Y)
{
	mapX = X; mapY = Y;
	srand(int(time(0)));
	wall = new(nothrow)wType * [mapY];
	if (!wall)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	for (int i = 0; i < mapY; i++)
	{
		wall[i] = new(nothrow)wType[mapX]{ NONE };
		if (!wall[i])
		{
			cerr << "error:apply for new memory failed\n";
			exit(MEMORY_ERROR);
		}
	}
	for (int i = 0; i < mapY; i++)
	{
		if (rand() % 100 < pHARD)
		{
			wall[i][0] = HARD;
			wall[i][mapX - 1] = HARD;
		}
		else
		{
			wall[i][0] = SOFT;
			wall[i][mapX - 1] = SOFT;
		}
	}
	for (int i = 0; i < mapX; i++)
	{
		if (rand() % 100 < pHARD)
		{
			wall[0][i] = HARD;
			wall[mapY - 1][i] = HARD;
		}
		else
		{
			wall[0][i] = SOFT;
			wall[mapY - 1][i] = SOFT;
		}
	}
	int size = 3;
	int num = mapY / 2;
	for (int n = 0; n < num; n++)
	{
		int x, y;
		x = 1 + size + rand() % (mapX - 2 * (1 + size));
		y = 1 + size + rand() % (mapY - 2 * (1 + size));
		for (int j = -size; j <= size; j++)
			for (int i = -size; i <= size; i++)
				wall[y + j][x + i] = SUP;
		x = 1 + size + rand() % (mapX - 2 * (1 + size));
		y = 1 + size + rand() % (mapY - 2 * (1 + size));
		for (int j = -size; j <= size; j++)
			for (int i = -size; i <= size; i++)
				wall[y + j][x + i] = SDOWN;
	}
	size = 2;
	num = mapY / 10;
	for (int n = 0; n < num; n++)
	{
		int x, y;
		x = 1 + size + rand() % (mapX - 2 * (1 + size));
		y = 1 + size + rand() % (mapY - 2 * (1 + size));
		for (int j = -size; j <= size; j++)
			for (int i = -size; i <= size; i++)
				wall[y + j][x + i] = SOFT;
		x = 1 + size + rand() % (mapX - 2 * (1 + size));
		y = 1 + size + rand() % (mapY - 2 * (1 + size));
		for (int j = -size; j <= size; j++)
			for (int i = -size; i <= size; i++)
				wall[y + j][x + i] = HARD;
	}
}
Walls::~Walls()
{
	for (int i = 0; i < mapY; i++)
	{
		delete[] wall[i];
	}
	delete[] wall;
}
bool Walls::isWall(int x, int y, wType& wkind)
{
	if (x < 0 || x >= mapX || y < 0 || y >= mapY)
		return true;
	wkind = wall[y][x];
	if (wall[y][x] == NONE || wall[y][x] == SUP || wall[y][x] == SDOWN)
		return false;
	else
		return true;
}
wType** Walls::retWall()
{
	return wall;
}