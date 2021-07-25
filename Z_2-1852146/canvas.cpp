#include"canvas.h"

using namespace std;

Canvas::Canvas(int X, int Y)
{
	mapX = X; mapY = Y;
	smapX = (WINDOW_W - 200) / 10;
	smapY = WINDOW_H / 10;
	showX = 0; showY = 0;
	fun.set(getwidth(), getheight());
}
void Canvas::setorigin_p(int x, int y)
{
	setorigin(x, y);
	fun.setorigin_p(x, y);
}
void Canvas::defineShow(int x, int y)
{
	if (x - smapX / 2 < 0)
		showX = 0;
	else if (x + smapX / 2 >= mapX)
		showX = mapX - smapX;
	else
		showX = x - smapX / 2;
	if (y - smapY / 2 < 0)
		showY = 0;
	else if (y + smapY / 2 >= mapY)
		showY = mapY - smapY;
	else
		showY = y - smapY / 2;
}
void Canvas::drawSnake(Snake& s, Color color)
{
	int x, y;
	sNode* head = s.retSnake();
	if (head->next == NULL)
	{
		cerr << "error:no snake body\n";
		exit(SNAKE_ERROR);
	}
	sNode* p = head->next;
	Circle c;
	while (p != NULL)
	{
		x = p->x - showX;
		y = p->y - showY;
		if (x < 0 || x >= smapX || y < 0 || y >= smapY)
		{
			p = p->next;
			continue;
		}
		c = { {10 * x + 5, 10 * y + 5},5,color ,0 };
		fun.floodCircle(c, GRADUAL);
		p = p->next;
	}
}
void Canvas::drawWalls(Walls& w)
{
	int x, y;
	Circle c;
	wType** wall = w.retWall();
	for (int j = 0; j < mapY; j++)
	{
		for (int i = 0; i < mapX; i++)
		{
			x = i - showX;
			y = j - showY;
			if (x < 0 || x >= smapX || y < 0 || y >= smapY)
				continue;
			switch (wall[j][i])
			{
			case SOFT:
				setlinestyle(PS_NULL);
				setfillcolor(0xFFBF00);
				fillrectangle(10 * x + 2, 10 * y + 2, 10 * x + 10 - 2, 10 * y + 10 - 2);
				break;
			case HARD:
				setlinestyle(PS_NULL);
				setfillcolor(0x0000FF);
				fillrectangle(10 * x + 2, 10 * y + 2, 10 * x + 10 - 2, 10 * y + 10 - 2);
				break;
			case DBODY:
				c = { {10 * x + 5, 10 * y + 5},5,{0,0,0} ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case SUP:
				setlinestyle(PS_NULL);
				setfillcolor(0xD4F2D7);
				fillrectangle(10 * x, 10 * y, 10 * x + 10, 10 * y + 10);
				break;
			case SDOWN:
				setlinestyle(PS_NULL);
				setfillcolor(0xE1E4FF);
				fillrectangle(10 * x, 10 * y, 10 * x + 10, 10 * y + 10);
				break;
			default:;
			}
		}
	}
}
void Canvas::drawFoods(Foods& f)
{
	int x, y;
	Circle c;
	fNode* food = f.retFood();
	int num = f.retFoodnum();
	for (int i = 0; i < num; i++)
	{
		if (food[i].exist)
		{
			x = food[i].x - showX;
			y = food[i].y - showY;
			if (x < 0 || x >= smapX || y < 0 || y >= smapY)
				continue;
			switch (food[i].kind)
			{
			case S:
				c = { {10 * x + 5, 10 * y + 5},10,{0xFF,0xD7,0} ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case A:
				c = { {10 * x + 5, 10 * y + 5},4,{0x6A,0x5A,0xCD} ,0 };
				fun.floodCircle(c);
				break;
			case B:
				c = { {10 * x + 5, 10 * y + 5},3,{0,0x80,0} ,0 };
				fun.floodCircle(c);
				break;
			case C:
				c = { {10 * x + 5, 10 * y + 5},3,{0x80,0x80,0x80} ,0 };
				fun.floodCircle(c);
				break;
			case D:
				c = { {10 * x + 5, 10 * y + 5},3,{0xBA,0x55,0xD3} ,0 };
				fun.floodCircle(c);
				break;
			}
		}
	}
}
void Canvas::showMeg(int score, int hightest_score, int len, int life, int left_food, int grade, LPTSTR name, double time, int energy)
{
	setbkcolor(0xD3D3D3);
	clearrectangle(WINDOW_W - 200, 0, WINDOW_W, WINDOW_H);
	setorigin(getwidth() - 200, 30);

	TCHAR s[100];
	RECT r = { 0, 0, 200, 30 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWidth = 0;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_BOLD;
	settextstyle(&f);
	_stprintf_s(s, _T("AUTOSNAKE5"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 0,30,100,70 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	switch (grade)
	{
	case 0:_stprintf_s(s, _T("入门")); break;
	case 1:_stprintf_s(s, _T("进阶")); break;
	case 2:_stprintf_s(s, _T("大师")); break;
	default:_stprintf_s(s, _T("错误")); break;
	}

	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 100,30,200,70 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	drawtext(name, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 0,70,200,170 };
	settextcolor(BLACK);
	f.lfHeight = 80;
	f.lfWeight = FW_BOLD;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	_stprintf_s(s, _T("%d"), score);
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 0,170,200,190 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	_stprintf_s(s, _T("最高分：%d"), hightest_score);
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if (energy <= 1600)
	{
		setlinestyle(PS_NULL);
		setfillcolor(BLACK);
		fillrectangle(20, 210, 20 + energy / 10, 230);
	}
	else
	{
		setlinestyle(PS_NULL);
		setfillcolor(0x0000FF);
		fillrectangle(20, 210, 20 + 160, 230);
	}

	r = { 20,260,200,280 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("长度：%d"), len);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 20,300,200,320 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	if (life < 100)
	{
		_stprintf_s(s, _T("生命：%d"), life);
	}
	else
	{
		_stprintf_s(s, _T("生命：∞"));
	}
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 20,340,200,360 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("食物：%d"), left_food);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 20,380,200,400 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("时间：%d:%d"), int(time) / 60, int(time) % 60);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 20,430,200,450 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("按Q退出"));
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	setorigin(0, 0);
	setbkcolor(WHITE);
}
void Canvas::showMeg(int score1, int score2, int life1, int life2, int energy1, int energy2, int left_food, double time)
{
	setorigin(0, 0);
	setbkcolor(0xD3D3D3);
	clearrectangle(0, WINDOW_H, getwidth(), getheight());
	setlinestyle(PS_SOLID, 4);
	setlinecolor(BLACK);
	line(getwidth() / 2, 0, getwidth() / 2, WINDOW_H);
	line(0, WINDOW_H, getwidth(), WINDOW_H);

	setorigin(0, WINDOW_H);
	TCHAR s[100];
	RECT r = { 520, 30, 760, 60 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWidth = 0;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_BOLD;
	settextstyle(&f);
	_stprintf_s(s, _T("AUTOSNAKE5 PVP"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 600, 0, 720, 30 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("按Q退出"));
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 580,70,760,100 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("食物：%d"), left_food);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 580,110,760,140 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("时间：%d:%d"), int(time) / 60, int(time) % 60);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);


	r = { 0,40,200,130 };
	settextcolor(BLACK);
	f.lfHeight = 80;
	f.lfWeight = FW_BOLD;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	_stprintf_s(s, _T("%d"), score1);
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 200,50,380,70 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	if (life1 < 100)
	{
		_stprintf_s(s, _T("生命：%d"), life1);
	}
	else
	{
		_stprintf_s(s, _T("生命：∞"));
	}
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);


	if (energy1 <= 1600)
	{
		setlinestyle(PS_NULL);
		setfillcolor(BLACK);
		fillrectangle(200, 100, 200 + energy1 / 10, 120);
	}
	else
	{
		setlinestyle(PS_NULL);
		setfillcolor(0x0000FF);
		fillrectangle(200, 100, 200 + 160, 120);
	}

	setorigin(800, WINDOW_H);

	r = { 0,40,200,130 };
	settextcolor(BLACK);
	f.lfHeight = 80;
	f.lfWeight = FW_BOLD;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	_stprintf_s(s, _T("%d"), score2);
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 200,50,380,70 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	if (life1 < 100)
	{
		_stprintf_s(s, _T("生命：%d"), life2);
	}
	else
	{
		_stprintf_s(s, _T("生命：∞"));
	}
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);


	if (energy2 <= 1600)
	{
		setlinestyle(PS_NULL);
		setfillcolor(BLACK);
		fillrectangle(200, 100, 200 + energy2 / 10, 120);
	}
	else
	{
		setlinestyle(PS_NULL);
		setfillcolor(0x0000FF);
		fillrectangle(200, 100, 200 + 160, 120);
	}

	setorigin(0, 0);
	setbkcolor(WHITE);
}
void Canvas::showMeg(int pnum, Player p[], int left_food, double time)
{
	setbkcolor(0xD3D3D3);
	clearrectangle(WINDOW_W - 200, 0, WINDOW_W, WINDOW_H);
	setorigin(getwidth() - 200, 30);

	TCHAR s[100];
	RECT r = { 0, 0, 200, 30 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWidth = 0;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_BOLD;
	settextstyle(&f);
	_stprintf_s(s, _T("AUTOSNAKE5"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 0,380,100,400 };
	settextcolor(BLACK);
	f.lfHeight = 15;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("食物：%d"), left_food);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	r = { 100,380,200,400 };
	settextcolor(BLACK);
	f.lfHeight = 15;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("时间：%d:%d"), int(time) / 60, int(time) % 60);
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	for (int i = 0; i < pnum; i++)
	{
		setorigin(getwidth() - 200, 30 + i * 80);
		r = { 0,70,100,100 };
		switch (i)
		{
		case 0:settextcolor(0x006400); break;
		case 1:settextcolor(0x008080); break;
		case 2:settextcolor(0x8B8B00); break;
		case 3:settextcolor(0xD30094); break;
		default:break;
		}
		f.lfHeight = 30;
		f.lfWeight = FW_BOLD;
		_tcscpy_s(f.lfFaceName, _T("楷体"));
		settextstyle(&f);
		_stprintf_s(s, _T("%d"), p[i].score);
		drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (p[i].energy <= 1600)
		{
			setlinestyle(PS_NULL);
			setfillcolor(BLACK);
			fillrectangle(20, 110, 20 + p[i].energy / 10, 120);
		}
		else
		{
			setlinestyle(PS_NULL);
			setfillcolor(0x0000FF);
			fillrectangle(20, 110, 20 + 160, 120);
		}

		r = { 100,70,200,83 };
		settextcolor(BLACK);
		f.lfHeight = 13;
		f.lfWeight = FW_NORMAL;
		_tcscpy_s(f.lfFaceName, _T("仿宋"));
		settextstyle(&f);
		_stprintf_s(s, _T("长度：%d"), p[i].len);
		drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		r = { 100,87,200,100 };
		settextcolor(BLACK);
		f.lfHeight = 13;
		f.lfWeight = FW_NORMAL;
		_tcscpy_s(f.lfFaceName, _T("仿宋"));
		settextstyle(&f);
		if (p[i].life < 100)
		{
			_stprintf_s(s, _T("生命：%d"), p[i].life);
		}
		else
		{
			_stprintf_s(s, _T("生命：∞"));
		}
		drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	setorigin(getwidth() - 200, 30);
	r = { 20,430,200,450 };
	settextcolor(BLACK);
	f.lfHeight = 20;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("仿宋"));
	settextstyle(&f);
	_stprintf_s(s, _T("按Q退出"));
	drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	setorigin(0, 0);
	setbkcolor(WHITE);
}
void Canvas::drawMap(char map[])
{
	int x, y;
	Circle c;
	for (int j = 0; j < mapY; j++)
	{
		for (int i = 0; i < mapX; i++)
		{
			x = i - showX;
			y = j - showY;
			if (x < 0 || x >= smapX || y < 0 || y >= smapY)
				continue;
			wType wt = (wType)(map[j * mapX + i] % 6);
			fType ft = (fType)((map[j * mapX + i] / 6) % 6 - 1);
			short st = map[j * mapX + i] / 36 - 1;
			switch (wt)
			{
			case SOFT:
				setlinestyle(PS_NULL);
				setfillcolor(0xFFBF00);
				fillrectangle(10 * x + 2, 10 * y + 2, 10 * x + 10 - 2, 10 * y + 10 - 2);
				break;
			case HARD:
				setlinestyle(PS_NULL);
				setfillcolor(0x0000FF);
				fillrectangle(10 * x + 2, 10 * y + 2, 10 * x + 10 - 2, 10 * y + 10 - 2);
				break;
			case DBODY:
				c = { {10 * x + 5, 10 * y + 5},5,{0,0,0} ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case SUP:
				setlinestyle(PS_NULL);
				setfillcolor(0xD4F2D7);
				fillrectangle(10 * x, 10 * y, 10 * x + 10, 10 * y + 10);
				break;
			case SDOWN:
				setlinestyle(PS_NULL);
				setfillcolor(0xE1E4FF);
				fillrectangle(10 * x, 10 * y, 10 * x + 10, 10 * y + 10);
				break;
			default:break;
			}
			switch (ft)
			{
			case S:
				c = { {10 * x + 5, 10 * y + 5},10,{0xFF,0xD7,0} ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case A:
				c = { {10 * x + 5, 10 * y + 5},4,{0x6A,0x5A,0xCD} ,0 };
				fun.floodCircle(c);
				break;
			case B:
				c = { {10 * x + 5, 10 * y + 5},3,{0,0x80,0} ,0 };
				fun.floodCircle(c);
				break;
			case C:
				c = { {10 * x + 5, 10 * y + 5},3,{0x80,0x80,0x80} ,0 };
				fun.floodCircle(c);
				break;
			case D:
				c = { {10 * x + 5, 10 * y + 5},3,{0xBA,0x55,0xD3} ,0 };
				fun.floodCircle(c);
				break;
			}
			switch (st)
			{
			case 0:
				c = { {10 * x + 5, 10 * y + 5},5,{0,0x64,0} ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case 1:
				c = { {10 * x + 5, 10 * y + 5},5,{ 0x80,0x80,0x00 } ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case 2:
				c = { {10 * x + 5, 10 * y + 5},5,{ 0x00,0x8B,0x8B } ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			case 3:
				c = { {10 * x + 5, 10 * y + 5},5,{ 0x94,0x00,0xD3 } ,0 };
				fun.floodCircle(c, GRADUAL);
				break;
			default:break;
			}
		}
	}
}