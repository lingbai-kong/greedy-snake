#include"AI.h"
using namespace std;

SearchNode::SearchNode()
{
	father = NULL;
	leftchild = NULL;
	midchild = NULL;
	rightchile = NULL;

	x = 0;
	y = 0;
	state = BLANK;
	ischeck = false;
	fn = 0;
	gn = 0;
	hn = 0;
}
void SearchNode::addchild(SearchNode* child)
{
	if (leftchild == NULL)
		leftchild = child;
	else if (midchild == NULL)
		midchild = child;
	else
		rightchile = child;
	child->father = this;
}

SearchList::SearchList()
{
	head = new(nothrow) ListNode;
	if (!head)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	head->node = NULL;
	head->next = NULL;
}
SearchList::~SearchList()
{
	ListNode* p;
	while (head != NULL)
	{
		p = head;
		head = head->next;
		delete p;
	}
}
void SearchList::addNode(SearchNode* n)
{
	ListNode* p = new(nothrow)ListNode;
	if (!p)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	p->node = n;
	p->next = NULL;
	ListNode* q = head;
	ListNode* tmp;
	while (q->next != NULL)
	{
		if (q->next->node->fn > p->node->fn)
			break;
		q = q->next;
	}
	tmp = q->next;
	q->next = p;
	p->next = tmp;
}

void SearchList::popMin(SearchNode*& s)
{
	ListNode* p = head->next;
	head->next = p->next;
	s = p->node;
	delete p;
}

bool SearchList::isEmpty()
{
	if (head->next == NULL)
		return true;
	else
		return false;
}

int AI::h(int x, int y, int tx, int ty)
{
	return abs(x - tx) + abs(y - ty);
}
bool AI::reachtail(int no, int pnum, Snake s[], Walls& w, int nhx, int nhy)
{
	bool rt = false;
	SearchList List;
	SearchNode** searchmap = new(nothrow)SearchNode * [mapY];
	int ohx, ohy;
	int tx, ty;
	s[no].retHead(ohx, ohy);
	s[no].retTail(tx, ty);
	if (!searchmap)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	for (int i = 0; i < mapY; i++)
	{
		searchmap[i] = new(nothrow)SearchNode[mapX];
		if (!searchmap)
		{
			cerr << "error:apply for new memory failed\n";
			exit(MEMORY_ERROR);
		}
	}
	wType** wall = w.retWall();
	for (int j = 0; j < mapY; j++)
		for (int i = 0; i < mapX; i++)
		{
			searchmap[j][i].x = i;
			searchmap[j][i].y = j;
			switch (wall[j][i])
			{
			case NONE:
				searchmap[j][i].state = BLANK; break;
			case SUP:
				searchmap[j][i].state = FAST; break;
			case SDOWN:
				searchmap[j][i].state = SLOW; break;
			default:
				searchmap[j][i].state = SKIP; break;
			}
			for (int k = 0; k < pnum; k++)
				if (s[k].isBody(i, j))
					searchmap[j][i].state = SKIP;
		}
	searchmap[ohy][ohx].state = SKIP;
	searchmap[ty][tx].state = BLANK;

	searchmap[nhy][nhx].gn = 0;
	searchmap[nhy][nhx].hn = h(nhx, nhy, tx, ty);
	searchmap[nhy][nhx].fn = searchmap[nhy][nhx].gn + searchmap[nhy][nhx].hn;
	if (searchmap[nhy][nhx].state != SKIP);//蛇死亡时蛇头会是SKIP
	{
		List.addNode(&searchmap[nhy][nhx]);
		searchmap[nhy][nhx].ischeck = true;
	}
	SearchNode* t = NULL;
	while (!List.isEmpty())
	{
		List.popMin(t);
		if (t->x == tx && t->y == ty)
		{
			rt = true;
			break;
		}
		int dx, dy;
		for (dx = -1; dx <= 1; dx++)
		{
			for (dy = -1; dy <= 1; dy++)
			{
				if (t->x + dx >= 0 && t->x + dx < mapX && t->y + dy >= 0 && t->y + dy < mapY && dx != dy && dx != -dy && searchmap[t->y + dy][t->x + dx].state != SKIP && !searchmap[t->y + dy][t->x + dx].ischeck)
				{
					switch (t->state)
					{
					case SLOW:searchmap[t->y + dy][t->x + dx].gn = t->gn + 1; break;
					case BLANK:searchmap[t->y + dy][t->x + dx].gn = t->gn + 1; break;
					case FAST:searchmap[t->y + dy][t->x + dx].gn = t->gn + 1; break;
					default:; break;
					}
					searchmap[t->y + dy][t->x + dx].hn = h(t->x + dx, t->y + dy, tx, ty);
					searchmap[t->y + dy][t->x + dx].fn = searchmap[t->y + dy][t->x + dx].gn + searchmap[t->y + dy][t->x + dx].hn;
					t->addchild(&searchmap[t->y + dy][t->x + dx]);
					List.addNode(&searchmap[t->y + dy][t->x + dx]);
					searchmap[t->y + dy][t->x + dx].ischeck = true;
				}
			}
		}
	}
	for (int i = 0; i < mapY; i++)
	{
		delete[]searchmap[i];
	}
	delete[]searchmap;
	return rt;
}
Opt AI::A_star(int no, int pnum, Snake s[], Walls& w, Foods& f)
{
	Opt rt = STAY;
	SearchList List;
	SearchNode** searchmap = new(nothrow)SearchNode * [mapY];
	fNode* food = f.retFood();
	fType fkind;
	int bestfno = 0;
	int hx, hy;
	s[no].retHead(hx, hy);
	if (!searchmap)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	for (int i = 0; i < mapY; i++)
	{
		searchmap[i] = new(nothrow)SearchNode[mapX];
		if (!searchmap)
		{
			cerr << "error:apply for new memory failed\n";
			exit(MEMORY_ERROR);
		}
	}
	wType** wall = w.retWall();
	for (int j = 0; j < mapY; j++)
		for (int i = 0; i < mapX; i++)
		{
			searchmap[j][i].x = i;
			searchmap[j][i].y = j;
			switch (wall[j][i])
			{
			case NONE:
				searchmap[j][i].state = BLANK; break;
			case SUP:
				searchmap[j][i].state = FAST; break;
			case SDOWN:
				searchmap[j][i].state = SLOW; break;
			default:
				searchmap[j][i].state = SKIP; break;
			}
			for (int k = 0; k < pnum; k++)
				if (s[k].isBody(i, j))
					searchmap[j][i].state = SKIP;
			if (f.isFood(i, j, fkind))
			{
				if (fkind == D)
					searchmap[j][i].state = SKIP;
				else
					searchmap[j][i].state = FOOD;
			}
		}
	for (int i = 0; i < f.num; i++)
	{
		if (food[i].exist && food[i].kind != D && h(hx, hy, food[i].x, food[i].y) < h(hx, hy, food[bestfno].x, food[bestfno].y))
		{
			bestfno = i;
		}
	}
	searchmap[hy][hx].gn = 0;
	searchmap[hy][hx].hn = h(hx, hy, food[bestfno].x, food[bestfno].y);
	searchmap[hy][hx].fn = searchmap[hy][hx].gn + searchmap[hy][hx].hn;
	if (searchmap[hy][hx].state != SKIP);//蛇死亡时蛇头会是SKIP
	{
		List.addNode(&searchmap[hy][hx]);
		searchmap[hy][hx].ischeck = true;
	}
	SearchNode* t = NULL;
	while (!List.isEmpty())
	{
		List.popMin(t);
		if (t->state == FOOD)
			break;
		int dx, dy;
		for (dx = -1; dx <= 1; dx++)
		{
			for (dy = -1; dy <= 1; dy++)
			{
				if (t->x + dx >= 0 && t->x + dx < mapX && t->y + dy >= 0 && t->y + dy < mapY && dx != dy && dx != -dy && searchmap[t->y + dy][t->x + dx].state != SKIP && !searchmap[t->y + dy][t->x + dx].ischeck)
				{
					switch (t->state)
					{
					case SLOW:searchmap[t->y + dy][t->x + dx].gn = t->gn + 4; break;
					case BLANK:searchmap[t->y + dy][t->x + dx].gn = t->gn + 2; break;
					case FAST:searchmap[t->y + dy][t->x + dx].gn = t->gn + 1; break;
					default:; break;
					}
					searchmap[t->y + dy][t->x + dx].hn = h(t->x + dx, t->y + dy, food[bestfno].x, food[bestfno].y);
					searchmap[t->y + dy][t->x + dx].fn = searchmap[t->y + dy][t->x + dx].gn + searchmap[t->y + dy][t->x + dx].hn;
					t->addchild(&searchmap[t->y + dy][t->x + dx]);
					List.addNode(&searchmap[t->y + dy][t->x + dx]);
					searchmap[t->y + dy][t->x + dx].ischeck = true;
				}
			}
		}
	}
	while (t->father != NULL && t->father != &searchmap[hy][hx])
		t = t->father;
	if (reachtail(no, pnum, s, w, t->x, t->y))
	{
		switch (t->x - hx)
		{
		case -1:rt = LEFT; break;
		case 1:rt = RIGHT; break;
		case 0:
			switch (t->y - hy)
			{
			case -1:rt = UP; break;
			case 1:rt = DOWN; break;
			case 0:rt = STAY; break;
			default:rt = STAY; break;
			}
			break;
		default:rt = STAY; break;
		}
	}
	else
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			for (int dy = -1; dy <= 1; dy++)
			{
				if (hx + dx >= 0 && hx + dx < mapX && hy + dy >= 0 && hy + dy < mapY && dx != dy && dx != -dy && searchmap[hy + dy][hx + dx].state != SKIP && reachtail(no, pnum, s, w, hx + dx, hy + dy))
				{
					switch (dx)
					{
					case -1:rt = LEFT; break;
					case 1:rt = RIGHT; break;
					case 0:
						switch (dy)
						{
						case -1:rt = UP; break;
						case 1:rt = DOWN; break;
						case 0:rt = STAY; break;
						default:rt = STAY; break;
						}
						break;
					default:rt = STAY; break;
					}
					break;
				}
			}
		}
	}
	for (int i = 0; i < mapY; i++)
	{
		delete[]searchmap[i];
	}
	delete[]searchmap;
	return rt;
}
Opt AI::greed(int no, int pnum, Snake s[], Walls& w, Foods& f)
{
	fNode* food = f.retFood();
	int hx, hy;
	int bestfno = 0;
	//bool reach[3][3] = { 0 };
	bool up, down, left, right;
	//int besti, bestj;
	wType wkind;
	s[no].retHead(hx, hy);
	for (int i = 0; i < f.num; i++)
	{
		if (food[i].exist && food[i].kind != D && h(hx, hy, food[i].x, food[i].y) < h(hx, hy, food[bestfno].x, food[bestfno].y))
		{
			bestfno = i;
		}
	}
	up = true;
	for (int k = 0; k < pnum; k++)
		if (s[k].isBody(hx, hy - 1))
		{
			up = false;
			break;
		}
	if (w.isWall(hx, hy - 1, wkind))
		up = false;
	down = true;
	for (int k = 0; k < pnum; k++)
		if (s[k].isBody(hx, hy + 1))
		{
			down = false;
			break;
		}
	if (w.isWall(hx, hy + 1, wkind))
		down = false;
	left = true;
	for (int k = 0; k < pnum; k++)
		if (s[k].isBody(hx - 1, hy))
		{
			left = false;
			break;
		}
	if (w.isWall(hx - 1, hy, wkind))
		left = false;
	right = true;
	for (int k = 0; k < pnum; k++)
		if (s[k].isBody(hx + 1, hy))
		{
			right = false;
			break;
		}
	if (w.isWall(hx + 1, hy, wkind))
		right = false;

	if (up && h(hx, hy - 1, food[bestfno].x, food[bestfno].y) <= h(hx, hy, food[bestfno].x, food[bestfno].y))
		return UP;
	else if (down && h(hx, hy + 1, food[bestfno].x, food[bestfno].y) <= h(hx, hy, food[bestfno].x, food[bestfno].y))
		return DOWN;
	else if (left && h(hx - 1, hy, food[bestfno].x, food[bestfno].y) <= h(hx, hy, food[bestfno].x, food[bestfno].y))
		return LEFT;
	else if (right && h(hx + 1, hy, food[bestfno].x, food[bestfno].y) <= h(hx, hy, food[bestfno].x, food[bestfno].y))
		return RIGHT;
	else
	{
		if (up)
			return UP;
		else if (down)
			return DOWN;
		else if (left)
			return LEFT;
		else if (right)
			return RIGHT;
		else
			return STAY;
	}
}
Opt AI::fun(int no, int pnum, Snake s[], Walls& w, Foods& f, int type)
{
	if (type == 0)
		return A_star(no, pnum, s, w, f);
	else
		return greed(no, pnum, s, w, f);
}