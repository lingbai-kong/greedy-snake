#include"snake.h"
using namespace std;
Snake::Snake(Walls& w, int X, int Y, int l)
{
	mapX = X;
	mapY = Y;
	len = 1;
	int hx, hy;
	wType wkind;
	for (int j = 1; j < mapY - 1; j++)
	{
		for (int i = 1; i < mapX - 1; i++)
		{
			alive = true;
			if (w.isWall(i, j, wkind))
			{
				alive = false;
			}
			if (alive)
				break;
		}
		if (alive)
			break;
	}
	srand(int(time(0)));
	do {
		hx = 1 + rand() % (mapX - 2);
		hy = 1 + rand() % (mapY - 2);
	} while (alive && w.isWall(hx, hy, wkind));
	dlen = l - 1;
	if (!w.isWall(hx, hy - 1, wkind))
		direct = UP;
	else if (!w.isWall(hx, hy + 1, wkind))
		direct = DOWN;
	else if (!w.isWall(hx - 1, hy, wkind))
		direct = LEFT;
	else if (!w.isWall(hx + 1, hy, wkind))
		direct = RIGHT;
	else
		direct = UP;
	head = new(nothrow)sNode;
	if (!head)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	head->x = -1; head->y = -1; head->next = NULL;
	head->next = new(nothrow)sNode;
	if (!head->next)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	head->next->x = hx; head->next->y = hy; head->next->next = NULL;
}
void Snake::resetSnake(Walls& w, int l)
{
	this->~Snake();
	len = 1;
	int hx, hy;
	wType wkind;
	for (int j = 1; j < mapY - 1; j++)
	{
		for (int i = 1; i < mapX - 1; i++)
		{
			alive = true;
			if (w.isWall(i, j, wkind))
			{
				alive = false;
			}
			if (alive)
				break;
		}
		if (alive)
			break;
	}
	srand(int(time(0)));
	do {
		hx = 1 + rand() % (mapX - 2);
		hy = 1 + rand() % (mapY - 2);
	} while (alive && w.isWall(hx, hy, wkind));
	dlen = l - 1;
	if (!w.isWall(hx, hy - 1, wkind))
		direct = UP;
	else if (!w.isWall(hx, hy + 1, wkind))
		direct = DOWN;
	else if (!w.isWall(hx - 1, hy, wkind))
		direct = LEFT;
	else if (!w.isWall(hx + 1, hy, wkind))
		direct = RIGHT;
	else
		direct = UP;
	head = new(nothrow)sNode;
	if (!head)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	head->x = -1; head->y = -1; head->next = NULL;
	head->next = new(nothrow)sNode;
	if (!head->next)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	head->next->x = hx; head->next->y = hy; head->next->next = NULL;
}
Snake::~Snake()
{
	sNode* p = NULL;
	while (head != NULL)
	{
		p = head;
		head = head->next;
		delete p;
	}
}
void Snake::addHead()
{
	sNode* p = new(nothrow)sNode;
	if (!p)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	switch (direct)
	{
	case UP:
		p->x = head->next->x;
		p->y = head->next->y - 1;
		break;
	case DOWN:
		p->x = head->next->x;
		p->y = head->next->y + 1;
		break;
	case LEFT:
		p->x = head->next->x - 1;
		p->y = head->next->y;
		break;
	case RIGHT:
		p->x = head->next->x + 1;
		p->y = head->next->y;
		break;
	}
	p->next = head->next;
	head->next = p;
	len++;
}
void Snake::delHead()
{
	sNode* p;
	if (head->next == NULL)
	{
		cerr << "error:no snake body\n";
		exit(SNAKE_ERROR);
	}
	p = head->next;
	head->next = p->next;
	delete p;
	len--;
}
void Snake::delTail()
{
	sNode* p = head;
	if (head->next == NULL)
	{
		cerr << "error:no snake body\n";
		exit(SNAKE_ERROR);
	}
	while (p->next->next != NULL)
		p = p->next;
	delete p->next;
	p->next = NULL;
	len--;
}
void Snake::setState(Opt d)
{
	if (head->next->next != NULL)
	{
		switch (d)
		{
		case UP:
			if (head->next->y - 1 != head->next->next->y)
				direct = UP;
			break;
		case DOWN:
			if (head->next->y + 1 != head->next->next->y)
				direct = DOWN;
			break;
		case LEFT:
			if (head->next->x - 1 != head->next->next->x)
				direct = LEFT;
			break;
		case RIGHT:
			if (head->next->x + 1 != head->next->next->x)
				direct = RIGHT;
			break;
		default:;
		}
	}
}
void Snake::retHead(int& hx, int& hy)
{
	if (head->next == NULL)
	{
		cerr << "error:no snake body\n";
		exit(SNAKE_ERROR);
	}
	hx = head->next->x;
	hy = head->next->y;
}
void Snake::retTail(int& tx, int& ty)
{
	sNode* p = head->next;
	while (p->next != NULL)
	{
		p = p->next;
	}
	tx = p->x;
	ty = p->y;
}
bool Snake::isBody(int x, int y)
{
	if (head->next == NULL)
	{
		cerr << "error:no snake body\n";
		exit(SNAKE_ERROR);
	}
	sNode* p = head->next->next;
	while (p != NULL)
	{
		if (p->x == x && p->y == y)
			return true;
		p = p->next;
	}
	return false;
}
void Snake::move()
{
	if (dlen > 0)
	{
		addHead();
		dlen--;
	}
	else if (dlen < 0)
	{
		while (dlen < 0)
		{
			delHead();
			dlen++;
		}
		switch (direct)
		{
		case UP:
			if (head->next->y - 1 == head->next->next->y)
				direct = DOWN;
			break;
		case DOWN:
			if (head->next->y + 1 == head->next->next->y)
				direct = UP;
			break;
		case LEFT:
			if (head->next->x - 1 == head->next->next->x)
				direct = RIGHT;
			break;
		case RIGHT:
			if (head->next->x + 1 == head->next->next->x)
				direct = LEFT;
			break;
		default:;
		}
	}
	else
	{
		addHead();
		delTail();
	}
}
void Snake::dieToWall(Walls& w)
{
	wType** wall = w.retWall();
	sNode* p = head->next->next;
	while (p != NULL)
	{
		wall[p->y][p->x] = DBODY;
		p = p->next;
	}
}
sNode* Snake::retSnake()
{
	return head;
}