#include"host.h"

using namespace NetworkHelper;
using namespace std;

Opt Host::input()
{
	if (_kbhit() == 0)
		return STAY;
	switch (_getch())
	{
	case 'Q':
	case 'q':return QUIT; break;
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

void Host::initPlayers(int num, Player p[], int life, int delaynum)
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
void Host::flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num)
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
void Host::flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy)
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
bool Host::isOver(int num, Player p[], Snake s[])
{
	for (int i = 0; i < num; i++)
		if (p[i].life > 0)
			return false;
	return true;
}

void Host::creatMap(int mapX, int mapY, char map[], int num, Snake s[], Walls& w, Foods& f)
{
	wType** wall = w.retWall();
	fNode* food = f.retFood();

	for (int j = 0; j < mapY; j++)
	{
		for (int i = 0; i < mapX; i++)
		{
			map[j * mapX + i] = (char)wall[j][i];
		}
	}
	for (int k = 0; k < f.num; k++)
	{
		if (food[k].exist)
		{
			map[food[k].y * mapX + food[k].x] = 6 * ((char)food[k].kind + 1);
		}
	}
	for (int k = 0; k < num; k++)
	{
		sNode* head = s[k].retSnake();
		head = head->next;
		while (head != NULL)
		{
			map[head->y * mapX + head->x] = 36 * ((char)k + 1);
			head = head->next;
		}
	}
}

void Host::getName(char name[])
{
	int t = time(0);
	for (int i = 0; i < 10; i++)
	{
		name[i] = t % 10 + '0';
		t = t / 10;
	}
	name[10] = '\0';
}

int Host::retNameNo(char nameList[][40], int pnum, const char name[40])
{
	for (int i = 0; i < pnum; i++)
	{
		if (!strcmp(nameList[i], name))
			return i;
	}
	cerr << "error:can not find room member\n";
	exit(ONLINE_ERR);
}

bool Host::hostRoom(ClientHelper& _host, uint32_t& roomId, int& pnum, int mapX, int mapY, char nameList[][40])
{
	char namebuffer[40];
	getName(namebuffer);
	namebuffer[0] = 'A';
	namebuffer[1] = 'T';
	namebuffer[2] = 'S';
	if (!_host.hostNewRoom(namebuffer, roomId)) {
		cerr << "建立房间失败！\n";
		exit(ONLINE_ERR);
	}

	TCHAR s[100];
	RECT r = { 0,160,640,280 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 40;
	f.lfWidth = 0;
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	_stprintf_s(s, _T("创建房间成功：房号是%u\n******按Q键注销房间******\n*****按其他键开始游戏*****"), roomId);
	drawtext(s, &r, DT_CENTER | DT_EXPANDTABS);

	room_t room;
	while (true)
	{
		_host.getRoomInfo(roomId, room);
		_host.sendBinaryMsg((const char*)&"WT", 3 * sizeof(char));
		Sleep(20);
		if (room.existing == 4)
		{
			break;
		}
		if (_kbhit())
		{
			char c = _getch();
			if (c == 'q' || c == 'Q')
			{
				_host.sendBinaryMsg((const char*)&"LO", 3 * sizeof(char));
				return false;
			}
			else
				break;
		}
		clearrectangle(0, 350, 640, 450);
		r = { 0,350,640,450 };
		f.lfHeight = 20;
		_tcscpy_s(f.lfFaceName, _T("仿宋"));
		settextstyle(&f);
		_stprintf_s(s, _T("当前房间人数：%d 达到4人时自动开始游戏"), room.existing);
		drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
	}
	_host.sendBinaryMsg((const char*)&"IN", 3 * sizeof(char));
	Sleep(20);
	pnum = room.existing;
	for (int i = 0; i < pnum; i++)
	{
		strcpy_s(nameList[i], room.everyOne[i].name);
	}
	_host.sendBinaryMsg((const char*)&pnum, sizeof(int));
	Sleep(20);
	_host.sendBinaryMsg((const char*)&mapX, sizeof(int));
	Sleep(20);
	_host.sendBinaryMsg((const char*)&mapY, sizeof(int));
	Sleep(20);
	_host.sendBinaryMsg((const char*)nameList, pnum * 40 * sizeof(char));
	Sleep(20);
	return true;
}

void Host::searveGame(ClientHelper& _host, int pnum, int mapX, int mapY, char nameList[][40])
{
	int life = 5;
	int foodnum = 30;
	int delay_unit = 0;
	int delay_num = 2;
	int step_count = 0;
	int FoodRemainStep = 100;
	int denergy = 3;

	Player p[4];
	initPlayers(pnum, p, life, delay_num);
	Canvas n(mapX, mapY);
	Walls w(mapX, mapY);
	Snake s[4] = { { w,mapX,mapY},{ w,mapX,mapY },{ w,mapX,mapY},{ w,mapX,mapY } };
	Foods f(foodnum, mapX, mapY);

	msg_package_t m;
	char* map = new(nothrow)char[mapX * mapY]{ 0 };
	if (!map)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
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

		p[0].op = input();
		if (p[0].op == QUIT)
			return;
		for (int i = 1; i < pnum; i++)
			p[i].op = STAY;
		if (_host.recvMsg(m))
		{
			for (unsigned int i = 0; i < m.msgNum; i++)
				_memccpy((char*)&p[retNameNo(nameList, pnum, m.msgs[i]->name)].op, m.msgs[i]->msgContent, 1, sizeof(Opt));
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

		_host.sendBinaryMsg((const char*)&"GO", 3 * sizeof(char));
		Sleep(1);
		_host.sendBinaryMsg((const char*)p, pnum * sizeof(Player));
		Sleep(1);
		creatMap(mapX, mapY, map, pnum, s, w, f);
		_host.sendBinaryMsg((const char*)map, mapX * mapY * sizeof(char));
		Sleep(1);
		_host.sendBinaryMsg((const char*)&f.left_num, sizeof(int));
		Sleep(1);
		_host.sendBinaryMsg((const char*)&runtime, sizeof(double));

		BeginBatchDraw();
		setbkcolor(WHITE);
		clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
		n.defineShow(p[0].hx, p[0].hy);
		n.drawWalls(w);
		n.drawFoods(f);
		if (pnum >= 1)
			n.drawSnake(s[0]);
		if (pnum >= 2)
			n.drawSnake(s[1], { 0x80,0x80,0x00 });
		if (pnum >= 3)
			n.drawSnake(s[2], { 0x00,0x8B,0x8B });
		if (pnum >= 4)
			n.drawSnake(s[3], { 0x94,0x00,0xD3 });
		n.showMeg(pnum, p, f.left_num, runtime);
		FlushBatchDraw();
		flushPlayers2(pnum, p, s, w, denergy);
		Sleep(delay_unit);
		step_count++;
	}
	delete[] map;
}

void Host::hostOnline()
{
	setbkcolor(WHITE);
	clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
	ClientHelper host;
	char name[40] = "Host\n";
	uint32_t roomId;
	//host.connectToServer("127.0.0.1", 10001, name);
	host.connectToServer("111.231.112.136", 10001, name);
	int pnum = 1;
	int mapX = 70, mapY = 70;
	char nameList[4][40];
	if (!hostRoom(host, roomId, pnum, mapX, mapY, nameList))
		return;
	searveGame(host, pnum, mapX, mapY, nameList);
}