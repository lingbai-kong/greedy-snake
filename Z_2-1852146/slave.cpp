#include"slave.h"

using namespace NetworkHelper;
using namespace std;

Opt Slave::input()
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

void Slave::getName(char name[])
{
	int t = time(0);
	for (int i = 0; i < 10; i++)
	{
		name[i] = t % 10 + '0';
		t = t / 10;
	}
	name[10] = '\0';
}

int Slave::retNameNo(char nameList[][40], int pnum, const char name[])
{
	for (int i = 0; i < pnum; i++)
	{
		if (!strcmp(nameList[i], name))
			return i;
	}
	cerr << "error:can not find room member\n";
	exit(ONLINE_ERR);
}

bool Slave::connectRoom(ClientHelper& _client, uint32_t& roomId, int& pnum, int& mapX, int& mapY, char nameList[][40])
{
	int joinNum = 0;
	uint32_t candidate = 0;

	clock_t startT, endT;
	double waittime = 0;
	double T = 5;

	room_list_t rlt;
	if (!_client.getRoomList(rlt)) {
		cerr << "获取列表失败\n";
		return false;
	}

	TCHAR s[100];
	RECT r = { 20,0,620,60 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 40;
	f.lfWidth = 0;
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	settextstyle(&f);
	_stprintf_s(s, _T("可\t选\t房\t号"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);

	f.lfHeight = 20;
	settextstyle(&f);
	for (int i = 0; i < 15 && i < rlt.roomNum; i++)
	{
		char roomHead[4] = { 0 };
		roomHead[0] = rlt.rooms[i].roomName[0];
		roomHead[1] = rlt.rooms[i].roomName[1];
		roomHead[2] = rlt.rooms[i].roomName[2];
		if (rlt.rooms[i].existing < 4 && !strcmp(roomHead, "ATS"))
		{
			r = { 20,60 + 25 * i,570,80 + 25 * i };
			_stprintf_s(s, _T("\t房间号\t%u\t当前人数:%u"), rlt.rooms[i].roomId, rlt.rooms[i].existing);
			drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
			candidate = rlt.rooms[i].roomId;
			joinNum++;
		}
	}

	if (joinNum <= 0)
	{
		clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
		r = { 0,200,640,280 };
		gettextstyle(&f);
		f.lfHeight = 40;
		f.lfWidth = 0;
		f.lfQuality = ANTIALIASED_QUALITY;
		f.lfWeight = FW_NORMAL;
		_tcscpy_s(f.lfFaceName, _T("楷体"));
		_stprintf_s(s, _T("无可加入房间，请创建新房间"));
		settextstyle(&f);
		drawtext(s, &r, DT_CENTER | DT_EXPANDTABS);
		Sleep(1500);
		return false;
	}

	wchar_t Id[20] = { 0 };
	File file;
	_stprintf_s(s, _T("%u"), candidate);
	InputBox(Id, 11, _T("请输入10位房间号："), _T("加入房间"), s);
	roomId = file.wchars2int(Id);
	room_t room;
	msg_package_t m;
	char signal[3] = { 0 };
	_client.getRoomInfo(roomId, room);
	if (room.existing > 4)
	{
		cerr << "error:the room is full\n";
		return false;
	}
	if (!_client.connectToRoom(roomId))
	{
		cerr << "error:connect fault\n";
		return false;
	}

	clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
	r = { 0,200,640,280 };
	gettextstyle(&f);
	f.lfHeight = 40;
	f.lfWidth = 0;
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	_stprintf_s(s, _T("加入房间成功,请等待房主开始游戏\n*****按Q键退出房间*****"));
	settextstyle(&f);
	drawtext(s, &r, DT_CENTER | DT_EXPANDTABS);

	startT = clock();
	while (true)
	{
		if (_client.recvMsg(m))
		{
			memcpy((char*)signal, m.msgs[0]->msgContent, 3 * sizeof(char));
			if (!strcmp(signal, "WT"))
			{
				break;
			}
		}
		endT = clock();
		waittime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;
		if (waittime > T)
		{
			return false;
		}
	}
	while (true)
	{
		if (_client.recvMsg(m))
		{
			memcpy((char*)signal, m.msgs[0]->msgContent, 3 * sizeof(char));
			if (!strcmp(signal, "IN"))
			{
				break;
			}
			if (!strcmp(signal, "LO"))
			{
				clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
				r = { 0,200,640,280 };
				gettextstyle(&f);
				f.lfHeight = 40;
				f.lfWidth = 0;
				f.lfQuality = ANTIALIASED_QUALITY;
				f.lfWeight = FW_NORMAL;
				_tcscpy_s(f.lfFaceName, _T("楷体"));
				_stprintf_s(s, _T("房主注销了房间，您将退出房间"));
				settextstyle(&f);
				drawtext(s, &r, DT_CENTER | DT_EXPANDTABS);
				Sleep(1500);
				return false;
			}
			if (_kbhit())
			{
				char c = _getch();
				if (c == 'q' || c == 'Q')
					return false;
			}
		}
	}
	while (true)
	{
		if (_client.recvMsg(m))
		{
			memcpy((char*)&pnum, m.msgs[0]->msgContent, sizeof(int));
			break;
		}
	}
	while (true)
	{
		if (_client.recvMsg(m))
		{
			memcpy((char*)&mapX, m.msgs[0]->msgContent, sizeof(int));
			break;
		}
	}
	while (true)
	{
		if (_client.recvMsg(m))
		{
			memcpy((char*)&mapY, m.msgs[0]->msgContent, sizeof(int));
			break;
		}
	}
	while (true)
	{
		if (_client.recvMsg(m))
		{
			memcpy((char*)nameList, m.msgs[0]->msgContent, pnum * 40 * sizeof(char));
			break;
		}
	}
	return true;
}

void Slave::joinGame(ClientHelper& _client, int pnum, int mapX, int mapY, char nameList[][40], const char name[40])
{
	bool flag = true;
	char signal[3] = { 0 };
	double runtime = 0;
	int left_num = 0;
	Opt op;
	Player p[4];
	Canvas n(mapX, mapY);

	msg_package_t m;
	char* map = new(nothrow)char[mapX * mapY]{ 0 };
	if (!map)
	{
		cerr << "error:apply for new memory failed\n";
		exit(MEMORY_ERROR);
	}
	clock_t startT, endT;
	double waittime = 0;
	double T = 5;

	while (true)
	{
		op = input();
		if (op == QUIT)
			return;
		_client.sendBinaryMsg((const char*)&op, sizeof(Opt));
		startT = clock();
		while (true)
		{
			if (_client.recvMsg(m))
			{
				memcpy((char*)signal, m.msgs[0]->msgContent, 3 * sizeof(char));
				if (!strcmp(signal, "GO"))
				{
					break;
				}
			}
			endT = clock();
			waittime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;
			if (waittime > T)
			{
				flag = false;
				break;
			}
		}
		if (!flag)
			break;
		startT = clock();
		while (true)
		{
			if (_client.recvMsg(m))
			{
				memcpy((char*)p, m.msgs[0]->msgContent, pnum * sizeof(Player));
				break;
			}
			endT = clock();
			waittime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;
			if (waittime > T)
			{
				flag = false;
				break;
			}
		}
		if (!flag)
			break;
		startT = clock();
		while (true)
		{
			if (_client.recvMsg(m))
			{
				memcpy((char*)map, m.msgs[0]->msgContent, mapX * mapY * sizeof(char));
				break;
			}
			endT = clock();
			waittime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;
			if (waittime > T)
			{
				flag = false;
				break;
			}
		}
		if (!flag)
			break;
		startT = clock();
		while (true)
		{
			if (_client.recvMsg(m))
			{
				memcpy((char*)&left_num, m.msgs[0]->msgContent, sizeof(int));
				break;
			}
			endT = clock();
			waittime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;
			if (waittime > T)
			{
				flag = false;
				break;
			}
		}
		if (!flag)
			break;
		startT = clock();
		while (true)
		{
			if (_client.recvMsg(m))
			{
				memcpy((char*)&runtime, m.msgs[0]->msgContent, sizeof(double));
				break;
			}
			endT = clock();
			waittime = ((double)(endT)-(double)(startT)) / CLOCKS_PER_SEC;
			if (waittime > T)
			{
				flag = false;
				break;
			}
		}
		if (!flag)
			break;
		BeginBatchDraw();
		setbkcolor(WHITE);
		clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
		n.defineShow(p[retNameNo(nameList, pnum, name)].hx, p[retNameNo(nameList, pnum, name)].hy);
		n.drawMap(map);
		n.showMeg(pnum, p, left_num, runtime);
		FlushBatchDraw();
	}
	delete[] map;
}

void Slave::slaveOnline()
{
	setbkcolor(WHITE);
	clearrectangle(0, 0, WINDOW_W - 200, WINDOW_H);
	ClientHelper client;
	uint32_t roomId;
	char name[40];
	getName(name);
	//client.connectToServer("127.0.0.1", 10001, name);
	client.connectToServer("111.231.112.136", 10001, name);
	int pnum;
	int mapX, mapY;
	char nameList[4][40] = { 0 };
	if (!connectRoom(client, roomId, pnum, mapX, mapY, nameList))
		return;
	joinGame(client, pnum, mapX, mapY, nameList, name);
}