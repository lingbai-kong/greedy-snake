#include"food.h"
using namespace std;

Foods::Foods(int n, int X, int Y)
{
	mapX = X; mapY = Y;
	max_num = n;
	food = new(nothrow)fNode[n]{ 0 };
	if (!food)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	left_num = 0; num = 0;
}
Foods::~Foods()
{
	delete[]food;
}
bool Foods::eatFood(int x, int y, fType& fkind)
{
	for (int i = 0; i < num; i++)
	{
		if (food[i].exist && food[i].x == x && food[i].y == y)
		{
			food[i].exist = false;
			fkind = food[i].kind;
			left_num--;
			return true;
		}
	}
	return false;
}
bool Foods::isFood(int x, int y)
{
	for (int i = 0; i < num; i++)
	{
		if (food[i].exist && food[i].x == x && food[i].y == y)
		{
			return true;
		}
	}
	return false;
}
bool Foods::isFood(int x, int y, fType& kind)
{
	for (int i = 0; i < num; i++)
	{
		if (food[i].exist && food[i].x == x && food[i].y == y)
		{
			kind = food[i].kind;
			return true;
		}
	}
	kind = D;
	return false;
}
void Foods::produceFood(int pnum, Snake s[], Walls& w)
{
	for (int i = 0; i < num; i++)
		food[i].exist = false;
	int leftspace = 0;
	int hx, hy;
	int p;
	bool flag;
	wType wkind;
	for (int j = 0; j < mapY; j++)
		for (int i = 0; i < mapX; i++)
		{
			flag = true;
			for (int k = 0; k < pnum; k++)
			{
				s[k].retHead(hx, hy);
				if ((i == hx && j == hy) || s[k].isBody(i, j))
				{
					flag = false;
					break;
				}
			}
			if (!isFood(i, j) && !w.isWall(i, j, wkind) && flag)
				leftspace++;
		}
	srand(int(time(0)));
	num = rand() % max_num + 1;
	if (num > leftspace)
		num = leftspace;
	left_num = num;
	for (int i = 0; i < num; i++)
	{
		int u, v;
		bool flag;
		do {
			u = rand() % mapX;
			v = rand() % mapY;
			flag = true;
			for (int k = 0; k < pnum; k++)
			{
				s[k].retHead(hx, hy);
				if (s[k].isBody(u, v) || (u == hx && v == hy))
				{
					flag = false;
					break;
				}
			}
		} while (isFood(u, v) || w.isWall(u, v, wkind) || !flag);
		food[i].x = u;
		food[i].y = v;
		p = 1 + rand() % 100;
		if (p <= pS)
			food[i].kind = S;
		else if (p <= pA)
			food[i].kind = A;
		else if (p <= pB)
			food[i].kind = B;
		else if (p <= pC)
			food[i].kind = C;
		else //if (p <= pD)
			food[i].kind = D;
		food[i].exist = true;
	}
}
fNode* Foods::retFood()
{
	return food;
}
int Foods::retFoodnum()
{
	return num;
}