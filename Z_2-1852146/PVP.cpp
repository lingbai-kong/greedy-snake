#include"pvp.h"
using namespace std;

void Pvp::input(Opt& P1, Opt& P2)
{
	if (_kbhit() == 0)
		P1 = P2 = STAY;
	else {
		int c = _getch();
		if (c == 224)
		{
			switch (_getch())
			{
			case 72:P2 = UP; break;
			case 80:P2 = DOWN; break;
			case 75:P2 = LEFT; break;
			case 77: P2 = RIGHT; break;
			default:P2 = STAY; break;
			}
			if (_kbhit() == 0)
				P1 = STAY;
			else {
				switch (_getch())
				{
				case 'Q':
				case 'q':P1 = QUIT; break;
				case 'W':
				case 'w':P1 = UP; break;
				case 'S':
				case 's':P1 = DOWN; break;
				case 'A':
				case'a':P1 = LEFT; break;
				case 'D':
				case'd':P1 = RIGHT; break;
				default:P1 = STAY; break;
				}
			}
		}
		else
		{
			switch (c)
			{
			case 'Q':
			case 'q':P1 = QUIT; break;
			case 'W':
			case 'w':P1 = UP; break;
			case 'S':
			case 's':P1 = DOWN; break;
			case 'A':
			case'a':P1 = LEFT; break;
			case 'D':
			case'd':P1 = RIGHT; break;
			default:P1 = STAY; break;
			}
			if (_kbhit() == 0)
				P2 = STAY;
			else
			{
				if (_getch() == 224)
				{
					switch (_getch())
					{
					case 72:P2 = UP; break;
					case 80:P2 = DOWN; break;
					case 75:P2 = LEFT; break;
					case 77: P2 = RIGHT; break;
					default:P2 = STAY; break;
					}
				}
			}
		}
	}
}

void Pvp::initPlayers(int num, Player p[], int life, int delaynum)
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
	}
}
void Pvp::flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num)
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
	}
}
void Pvp::flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy)
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
			}
			else
			{
				p[i].energy = 0;
			}
		}
	}
}
bool Pvp::isOver(int num, Player p[], Snake s[])
{
	for (int i = 0; i < num; i++)
		if (p[i].life > 0 && s[i].alive)
			return false;
	return true;
}
void Pvp::pvp()
{
	int life = 5;
	int foodnum = 30;
	int mapX = 128, mapY = 96;
	int delay_unit = 50;
	int delay_num = 2;
	int step_count = 0;
	int FoodRemainStep = 100;
	int denergy = 3;
	int pnum = 2;
	Player p[2];
	initPlayers(pnum, p, life, delay_num);
	Canvas n(mapX, mapY);
	Walls w(mapX, mapY);
	Snake s[2] = { { w,mapX,mapY},{ w,mapX,mapY } };
	Foods f(foodnum, mapX, mapY);

	clock_t startT, endT;
	double runtime = 0;
	startT = clock();

	while (!isOver(pnum, p, s))
	{
		if (f.left_num == 0 || step_count >= FoodRemainStep)
		{
			f.produceFood(pnum, s, w);
			step_count = 0;
		}
		input(p[0].op, p[1].op);
		if (p[0].op == QUIT)
			return;
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

		BeginBatchDraw();
		setbkcolor(WHITE);
		clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
		n.defineShow(p[0].hx, p[0].hy);
		n.drawWalls(w);
		n.drawFoods(f);
		n.drawSnake(s[0]);
		n.drawSnake(s[1], { 0x80,0x80,0x00 });

		setbkcolor(WHITE);
		clearrectangle(WINDOW_W - 200, 0, (WINDOW_W - 200) * 2, WINDOW_H);
		n.setorigin_p(640, 0);
		n.defineShow(p[1].hx, p[1].hy);
		n.drawWalls(w);
		n.drawFoods(f);
		n.drawSnake(s[0]);
		n.drawSnake(s[1], { 0x80,0x80,0x00 });
		n.setorigin_p(0, 0);
		n.showMeg(p[0].score, p[1].score, p[0].life, p[1].life, p[0].energy, p[1].energy, f.left_num, runtime);
		FlushBatchDraw();
		flushPlayers2(pnum, p, s, w, denergy);
		Sleep(delay_unit);
		step_count++;
	}
}