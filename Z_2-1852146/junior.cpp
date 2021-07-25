#include"junior.h"
using namespace std;
Opt JuniorGame::input()
{
	if (_kbhit() == 0)
		return STAY;
	switch (_getch())
	{
	case 'Q':
	case 'q':return QUIT;
	case 'W':
	case 'w':return  UP; break;
	case 'S':
	case 's':return DOWN; break;
	case 'A':
	case'a':return LEFT; break;
	case 'D':
	case'd': return RIGHT; break;
	case 224:
		switch (_getch())
		{
		case 72:return  UP; break;
		case 80:return DOWN; break;
		case 75:return LEFT; break;
		case 77: return RIGHT; break;
		default:return STAY; break;
		}
		break;
	default:return STAY; break;
	}
}
void JuniorGame::initPlayers(int num, Player p[], int life, int delaynum)
{
	for (int i = 0; i < num; i++)
	{
		p[i].score = 0;
		p[i].nowdelay_num = delaynum;
		p[i].delay_count = 0;
		p[i].life = life;
		p[i].energy = 1600;
		p[i].hx = 0;
		p[i].hy = 0;
		p[i].op = STAY;
		p[i].len = 0;
	}
}
void JuniorGame::flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num)
{
	for (int i = 0; i < num; i++)
	{
		wType wkind;
		fType fkind;
		s[i].retHead(p[i].hx, p[i].hy);
		for (int j = 0; j < num; j++)
		{
			if (s[j].isBody(p[i].hx, p[i].hy))
			{
				p[i].score -= 10;
				s[i].alive = false;
				break;
			}
		}
		if (w.isWall(p[i].hx, p[i].hy, wkind))
		{
			switch (wkind)
			{
			case SOFT:s[i].dlen = -2; p[i].score -= 5; break;
			case HARD:s[i].dlen = -s[i].len / 2; p[i].score -= 10; break;
			case DBODY:s[i].dlen = -2; p[i].score -= 3; break;
			default:break;
			}
		}
		else
		{
			switch (wkind)
			{
			case SUP:p[i].nowdelay_num = delay_num / 2; break;
			case SDOWN:p[i].nowdelay_num = 2 * delay_num; break;
			default:p[i].nowdelay_num = delay_num; break;
			}
		}
		if (f.eatFood(p[i].hx, p[i].hy, fkind))
		{
			switch (fkind)
			{
			case S:s[i].dlen += 5; p[i].score += 20; p[i].life++; p[i].energy += 1600; break;
			case A:s[i].dlen += 2; p[i].score += 8; p[i].energy += 800; break;
			case B:s[i].dlen += 1; p[i].score += 5; p[i].energy += 400; break;
			case C:s[i].dlen += 0; p[i].score += 2; p[i].energy += 100; break;
			case D:s[i].alive = false; break;
			default:;
			}
		}
		if (s[i].len + s[i].dlen < 4 || p[i].energy <= 0)
			s[i].alive = false;
		if (p[i].score < 0)
			p[i].score = 0;
		p[i].len = s[i].len;
	}
}
void JuniorGame::flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy)
{
	for (int i = 0; i < num; i++)
	{
		p[i].delay_count++;
		p[i].energy -= denergy;
		if (!s[i].alive)
		{
			s[i].dieToWall(w);
			if (p[i].life > 0)
			{
				s[i].resetSnake(w);
				p[i].life--;
				p[i].energy = 1600;
				break;
			}
			else
			{
				p[i].energy = 0;
			}
		}
	}
}
bool JuniorGame::isOver(int num, Player p[], Snake s[])
{
	if (p[0].life <= 0)
		return true;
	else
		return false;
}
void JuniorGame::juniorGame(LPTSTR username, int highestscore, int& score, Gmode mode)
{
	int life;
	int foodnum;
	int mapX, mapY;
	int delay_unit;
	int delay_num = 2;
	int step_count = 0;
	int FoodRemainStep;
	int temphighest = highestscore;

	int pnum = 1;
	int energy = 1600;
	int denergy;

	switch (mode)
	{
	case INTRODUCTION:
		life = 1;
		foodnum = 20;
		mapX = 64;
		mapY = 48;
		delay_unit = 250;
		FoodRemainStep = INT_MAX;
		denergy = 1;
		break;
	case ADVANCED:
		life = INT_MAX / 2;
		foodnum = 15;
		mapX = 64;
		mapY = 48;
		delay_unit = 100;
		FoodRemainStep = 200;
		denergy = 3;
		break;
	case MASTER:
		life = 5;
		foodnum = 15;
		mapX = 128;
		mapY = 96;
		delay_unit = 50;
		FoodRemainStep = 100;
		denergy = 5;
		break;
	default:
		cerr << "error:illigal gmode\n";
		exit(GMODE_ERROR);
	}

	Player p[1];
	initPlayers(pnum, p, life, delay_num);
	Canvas n(mapX, mapY);
	Walls w(mapX, mapY);
	Snake s[1] = { { w,mapX,mapY} };
	Foods f(foodnum, mapX, mapY);

	clock_t startT, endT;
	double runtime;
	startT = clock();

	while (!isOver(pnum, p, s))
	{
		if (f.left_num == 0 || step_count >= FoodRemainStep)
		{
			f.produceFood(pnum, s, w);
			step_count = 0;
		}
		p[0].op = input();
		if (p[0].op == QUIT)
		{
			return;
		}
		for (int i = 0; i < pnum; i++)
		{
			s[i].setState(p[i].op);
			if (p[i].delay_count >= p[i].nowdelay_num && s[i].alive)
			{
				s[i].move();
				p[i].delay_count = 0;
			}
		}
		flushPlayers1(pnum, p, s, w, f, delay_num);

		endT = clock();
		runtime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;

		if (p[0].score > highestscore)
			temphighest = p[0].score;
		else
			temphighest = highestscore;

		BeginBatchDraw();
		setbkcolor(WHITE);
		clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
		n.defineShow(p[0].hx, p[0].hy);
		n.drawWalls(w);
		n.drawFoods(f);
		n.drawSnake(s[0]);
		n.showMeg(p[0].score, temphighest, s[0].len, p[0].life, f.left_num, int(mode), username, runtime, p[0].energy);
		FlushBatchDraw();
		flushPlayers2(pnum, p, s, w, denergy);
		Sleep(delay_unit);
		step_count++;
	}
	score = p[0].score;
	if (score < 0)
		score = 0;
}
