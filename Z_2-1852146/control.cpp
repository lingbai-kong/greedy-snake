#include"control.h"
using namespace std;

int Control::mainmenu()
{
	setorigin(0, 0);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	clearrectangle(0, 0, getwidth() - 200, getheight());
	setbkcolor(0xD3D3D3);
	clearrectangle(getwidth() - 200, 0, getwidth(), getheight());
	setbkcolor(WHITE);

	MOUSEMSG mouse;
	TCHAR s[100];
	RECT r = { 20,90,620,200 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 110;
	f.lfWidth = 0;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_HEAVY;
	settextstyle(&f);
	_stprintf_s(s, _T("AUTOSNAKE5"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 300, 230,600, 250 };
	f.lfHeight = 25;
	f.lfWeight = FW_THIN;
	settextstyle(&f);
	_stprintf_s(s, _T("POWERED BY EASYX"));
	drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	r = { 100, 200,600, 220 };
	f.lfHeight = 25;
	f.lfWeight = FW_THIN;
	settextstyle(&f);
	_stprintf_s(s, _T("DESIGNED BY KLB IN TONGJI UNIVERSITY"));
	drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(WHITE);
	fillrectangle(20, 420, 60, 460);
	r = { 20, 420, 60, 460 };
	settextcolor(BLACK);
	f.lfHeight = 40;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("?"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(0xD3D3D3);
	fillrectangle(640 + 20, 25, 640 + 180, 125);
	fillrectangle(640 + 20, 135, 640 + 180, 235);
	fillrectangle(640 + 20, 245, 640 + 180, 345);
	fillrectangle(640 + 20, 355, 640 + 180, 455);

	r = { 640 + 20, 25,640 + 180, 125 };
	settextcolor(BLACK);
	f.lfHeight = 30;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("单人游戏"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 135,640 + 180 ,235 };
	_stprintf_s(s, _T("双人游戏"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 245, 640 + 180, 345 };
	_stprintf_s(s, _T("人机对战"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 355, 640 + 180, 455 };
	_stprintf_s(s, _T("联机对战"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();
	while (true)
	{
		BeginBatchDraw();
		mouse = GetMouseMsg();
		if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 25 && mouse.y <= 125)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 25, 640 + 180, 125);

			settextcolor(WHITE);
			r = { 640 + 20, 25, 640 + 180, 125 };
			_stprintf_s(s, _T("单人游戏"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return SOLO;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 135 && mouse.y <= 235)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 135, 640 + 180, 235);

			settextcolor(WHITE);
			r = { 640 + 20, 135, 640 + 180 ,235 };
			_stprintf_s(s, _T("双人游戏"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return PVP;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 245 && mouse.y <= 345)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 245, 640 + 180, 345);

			settextcolor(WHITE);
			r = { 640 + 20, 245, 640 + 180, 345 };
			_stprintf_s(s, _T("人机对战"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return PVE;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 355 && mouse.y <= 455)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 355, 640 + 180, 455);

			settextcolor(WHITE);
			r = { 640 + 20, 355, 640 + 180, 455 };
			_stprintf_s(s, _T("联机对战"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return ONLINE;
			}
		}
		else if (mouse.x >= 20 && mouse.x <= 60 && mouse.y >= 420 && mouse.y <= 460)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(20, 420, 60, 460);

			settextcolor(WHITE);
			r = { 20, 420, 60, 460 };
			_stprintf_s(s, _T("?"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return MEG;
			}
		}
		else
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(WHITE);
			fillrectangle(20, 420, 60, 460);
			r = { 20, 420, 60, 460 };
			settextcolor(BLACK);
			f.lfHeight = 40;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("?"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(0xD3D3D3);
			fillrectangle(640 + 20, 25, 640 + 180, 125);
			fillrectangle(640 + 20, 135, 640 + 180, 235);
			fillrectangle(640 + 20, 245, 640 + 180, 345);
			fillrectangle(640 + 20, 355, 640 + 180, 455);


			r = { 640 + 20, 25,640 + 180, 125 };
			settextcolor(BLACK);
			f.lfHeight = 30;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("单人游戏"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 135,640 + 180 ,235 };
			_stprintf_s(s, _T("双人游戏"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 245, 640 + 180, 345 };
			_stprintf_s(s, _T("人机对战"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 355, 640 + 180, 455 };
			_stprintf_s(s, _T("联机对战"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		FlushBatchDraw();
	}
}

int Control::solomenu()
{
	setorigin(0, 0);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	clearrectangle(0, 0, getwidth() - 200, getheight());
	setbkcolor(0xD3D3D3);
	clearrectangle(getwidth() - 200, 0, getwidth(), getheight());

	MOUSEMSG mouse;
	TCHAR s[100];
	RECT r = { 20,90,620,200 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 110;
	f.lfWidth = 0;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_HEAVY;
	settextstyle(&f);
	_stprintf_s(s, _T("AUTOSNAKE5"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 300, 230,600, 250 };
	f.lfHeight = 25;
	f.lfWeight = FW_THIN;
	settextstyle(&f);
	_stprintf_s(s, _T("POWERED BY EASYX"));
	drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	r = { 100, 200,600, 220 };
	f.lfHeight = 25;
	f.lfWeight = FW_THIN;
	settextstyle(&f);
	_stprintf_s(s, _T("DESIGNED BY KLB IN TONGJI UNIVERSITY"));
	drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(WHITE);
	fillrectangle(20, 420, 60, 460);
	r = { 20, 420, 60, 460 };
	settextcolor(BLACK);
	f.lfHeight = 40;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("!"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(WHITE);
	fillrectangle(580, 420, 620, 460);
	r = { 580, 420, 620, 460 };
	settextcolor(BLACK);
	f.lfHeight = 40;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("←"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(0xD3D3D3);
	fillrectangle(640 + 20, 25, 640 + 180, 125);
	fillrectangle(640 + 20, 135, 640 + 180, 235);
	fillrectangle(640 + 20, 245, 640 + 180, 345);
	fillrectangle(640 + 20, 355, 640 + 180, 455);

	r = { 640 + 20, 25,640 + 180, 125 };
	settextcolor(BLACK);
	f.lfHeight = 30;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("入  门"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 135,640 + 180 ,235 };
	_stprintf_s(s, _T("进  阶"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 245, 640 + 180, 345 };
	_stprintf_s(s, _T("大  师"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 355, 640 + 180, 455 };
	_stprintf_s(s, _T("(敬请期待)"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();
	while (true)
	{
		BeginBatchDraw();
		mouse = GetMouseMsg();
		if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 25 && mouse.y <= 125)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 25, 640 + 180, 125);

			settextcolor(WHITE);
			r = { 640 + 20, 25, 640 + 180, 125 };
			_stprintf_s(s, _T("入  门"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return G1;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 135 && mouse.y <= 235)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 135, 640 + 180, 235);

			settextcolor(WHITE);
			r = { 640 + 20, 135, 640 + 180 ,235 };
			_stprintf_s(s, _T("进  阶"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return G2;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 245 && mouse.y <= 345)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 245, 640 + 180, 345);

			settextcolor(WHITE);
			r = { 640 + 20, 245, 640 + 180, 345 };
			_stprintf_s(s, _T("大  师"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return G3;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 355 && mouse.y <= 455)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 355, 640 + 180, 455);

			settextcolor(WHITE);
			r = { 640 + 20, 355, 640 + 180, 455 };
			_stprintf_s(s, _T("(敬请期待)"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return G4;
			}
		}
		else if (mouse.x >= 20 && mouse.x <= 60 && mouse.y >= 420 && mouse.y <= 460)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(20, 420, 60, 460);

			settextcolor(WHITE);
			r = { 20, 420, 60, 460 };
			_stprintf_s(s, _T("!"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return LIST;
			}
		}

		else if (mouse.x >= 580 && mouse.x <= 620 && mouse.y >= 420 && mouse.y <= 460)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(580, 420, 620, 460);

			settextcolor(WHITE);
			r = { 580, 420, 620, 460 };
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return MAINMENU;
			}
		}

		else
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(WHITE);
			fillrectangle(20, 420, 60, 460);
			r = { 20, 420, 60, 460 };
			settextcolor(BLACK);
			f.lfHeight = 40;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("!"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(WHITE);
			fillrectangle(580, 420, 620, 460);
			r = { 580, 420, 620, 460 };
			settextcolor(BLACK);
			f.lfHeight = 40;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(0xD3D3D3);
			fillrectangle(640 + 20, 25, 640 + 180, 125);
			fillrectangle(640 + 20, 135, 640 + 180, 235);
			fillrectangle(640 + 20, 245, 640 + 180, 345);
			fillrectangle(640 + 20, 355, 640 + 180, 455);


			r = { 640 + 20, 25,640 + 180, 125 };
			settextcolor(BLACK);
			f.lfHeight = 30;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("入  门"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 135,640 + 180 ,235 };
			_stprintf_s(s, _T("进  阶"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 245, 640 + 180, 345 };
			_stprintf_s(s, _T("大  师"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 355, 640 + 180, 455 };
			_stprintf_s(s, _T("(敬请期待)"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		FlushBatchDraw();
	}
}

int Control::onlinemenu()
{
	setorigin(0, 0);
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	clearrectangle(0, 0, getwidth() - 200, getheight());
	setbkcolor(0xD3D3D3);
	clearrectangle(getwidth() - 200, 0, getwidth(), getheight());

	MOUSEMSG mouse;
	TCHAR s[100];
	RECT r = { 20,90,620,200 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 110;
	f.lfWidth = 0;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_HEAVY;
	settextstyle(&f);
	_stprintf_s(s, _T("AUTOSNAKE5"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 300, 230,600, 250 };
	f.lfHeight = 25;
	f.lfWeight = FW_THIN;
	settextstyle(&f);
	_stprintf_s(s, _T("POWERED BY EASYX"));
	drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	r = { 100, 200,600, 220 };
	f.lfHeight = 25;
	f.lfWeight = FW_THIN;
	settextstyle(&f);
	_stprintf_s(s, _T("DESIGNED BY KLB IN TONGJI UNIVERSITY"));
	drawtext(s, &r, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(WHITE);
	fillrectangle(580, 420, 620, 460);
	r = { 580, 420, 620, 460 };
	settextcolor(BLACK);
	f.lfHeight = 40;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("←"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(0xD3D3D3);
	fillrectangle(640 + 20, 25, 640 + 180, 125);
	fillrectangle(640 + 20, 135, 640 + 180, 235);

	r = { 640 + 20, 25,640 + 180, 125 };
	settextcolor(BLACK);
	f.lfHeight = 30;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("创建房间"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 135,640 + 180 ,235 };
	_stprintf_s(s, _T("加入房间"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();
	while (true)
	{
		BeginBatchDraw();
		mouse = GetMouseMsg();
		if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 25 && mouse.y <= 125)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 25, 640 + 180, 125);

			settextcolor(WHITE);
			r = { 640 + 20, 25, 640 + 180, 125 };
			_stprintf_s(s, _T("创建房间"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return HOST;
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 135 && mouse.y <= 235)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 135, 640 + 180, 235);

			settextcolor(WHITE);
			r = { 640 + 20, 135, 640 + 180 ,235 };
			_stprintf_s(s, _T("加入房间"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return SLAVE;
			}
		}
		else if (mouse.x >= 580 && mouse.x <= 620 && mouse.y >= 420 && mouse.y <= 460)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(580, 420, 620, 460);

			settextcolor(WHITE);
			r = { 580, 420, 620, 460 };
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				FlushMouseMsgBuffer();
				return MAINMENU;
			}
		}
		else
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(WHITE);
			fillrectangle(580, 420, 620, 460);
			r = { 580, 420, 620, 460 };
			settextcolor(BLACK);
			f.lfHeight = 40;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(0xD3D3D3);
			fillrectangle(640 + 20, 25, 640 + 180, 125);
			fillrectangle(640 + 20, 135, 640 + 180, 235);

			r = { 640 + 20, 25,640 + 180, 125 };
			settextcolor(BLACK);
			f.lfHeight = 30;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("创建房间"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 135,640 + 180 ,235 };
			_stprintf_s(s, _T("加入房间"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		FlushBatchDraw();
	}
}

void  Control::showlist()
{
	File file;

	wchar_t grade[NAME_SIZE];
	wchar_t name[NAME_SIZE];
	wchar_t no[NAME_SIZE];

	int grades[LIST_SIZE] = { 0 };
	wchar_t names[LIST_SIZE][NAME_SIZE] = { 0 };
	int scores[LIST_SIZE] = { 0 };
	file.readMeg(grades, names, scores, LIST_SIZE);

	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	clearrectangle(0, 0, getwidth() - 200, getheight());
	setbkcolor(0xD3D3D3);
	clearrectangle(getwidth() - 200, 0, getwidth(), getheight());

	MOUSEMSG mouse;
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
	_stprintf_s(s, _T("排\t行\t榜"));
	settextstyle(&f);
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);

	f.lfHeight = 20;
	settextstyle(&f);
	for (int i = 0; i < 15; i++)
	{
		r = { 20,60 + 25 * i,570,80 + 25 * i };
		if (grades[i] == 0)
			_stprintf_s(s, _T("\tNo%d\t入门\t用户名:%s\t分数:%d"), (i + 1), names[i], scores[i]);
		else if (grades[i] == 1)
			_stprintf_s(s, _T("\tNo%d\t进阶\t用户名:%s\t分数:%d"), (i + 1), names[i], scores[i]);
		else
			_stprintf_s(s, _T("\tNo%d\t大师\t用户名:%s\t分数:%d"), (i + 1), names[i], scores[i]);
		drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
	}

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(WHITE);
	fillrectangle(580, 420, 620, 460);
	r = { 580, 420, 620, 460 };
	settextcolor(BLACK);
	f.lfHeight = 40;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("←"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(0xD3D3D3);
	fillrectangle(640 + 20, 25, 640 + 180, 125);
	fillrectangle(640 + 20, 135, 640 + 180, 235);
	fillrectangle(640 + 20, 245, 640 + 180, 345);

	r = { 640 + 20, 25,640 + 180, 125 };
	settextcolor(BLACK);
	f.lfHeight = 30;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("筛选名字"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 135,640 + 180 ,235 };
	_stprintf_s(s, _T("筛选难度"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	r = { 640 + 20, 245, 640 + 180, 345 };
	_stprintf_s(s, _T("修改序号"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();
	while (true)
	{
		BeginBatchDraw();
		mouse = GetMouseMsg();
		if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 25 && mouse.y <= 125)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 25, 640 + 180, 125);

			settextcolor(WHITE);
			r = { 640 + 20, 25, 640 + 180, 125 };
			_stprintf_s(s, _T("筛选名字"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				setbkcolor(WHITE);
				clearrectangle(0, 0, 570, getheight());
				settextcolor(BLACK);
				setbkcolor(0xD3D3D3);
				InputBox(name, NAME_SIZE, _T("请输入玩家名字"), _T("筛选名字"), _T("Root"));
				f.lfHeight = 20;
				settextstyle(&f);
				int i = 0, j = 0;
				while (j < LIST_SIZE && i < 15)
				{
					if (!wcscmp(names[j], name))
					{
						r = { 20,60 + 25 * i,570,80 + 25 * i };
						if (grades[j] == 0)
							_stprintf_s(s, _T("\t入门\t用户名:%s\t分数:%d"), names[j], scores[j]);
						else if (grades[j] == 1)
							_stprintf_s(s, _T("\t进阶\t用户名:%s\t分数:%d"), names[j], scores[j]);
						else
							_stprintf_s(s, _T("\t大师\t用户名:%s\t分数:%d"), names[j], scores[j]);
						drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
						i++;
					}
					j++;
				}
				FlushMouseMsgBuffer();
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 135 && mouse.y <= 235)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 135, 640 + 180, 235);

			settextcolor(WHITE);
			r = { 640 + 20, 135, 640 + 180 ,235 };
			_stprintf_s(s, _T("筛选难度"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				setbkcolor(WHITE);
				clearrectangle(0, 0, 570, getheight());
				settextcolor(BLACK);
				setbkcolor(0xD3D3D3);
				InputBox(grade, NAME_SIZE, _T("请输入难度"), _T("筛选难度"), _T("入门"));
				int g = -1;
				if (!wcscmp(grade, _T("入门")))
					g = 0;
				else if (!wcscmp(grade, _T("进阶")))
					g = 1;
				else if (!wcscmp(grade, _T("大师")))
					g = 2;
				f.lfHeight = 20;
				settextstyle(&f);
				int i = 0, j = 0;
				while (j < LIST_SIZE && i < 15)
				{
					if (grades[j] == g)
					{
						r = { 20,60 + 25 * i,570,80 + 25 * i };
						if (grades[j] == 0)
							_stprintf_s(s, _T("\t入门\t用户名:%s\t分数:%d"), names[j], scores[j]);
						else if (grades[j] == 1)
							_stprintf_s(s, _T("\t进阶\t用户名:%s\t分数:%d"), names[j], scores[j]);
						else
							_stprintf_s(s, _T("\t大师\t用户名:%s\t分数:%d"), names[j], scores[j]);
						drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
						i++;
					}
					j++;
				}
				FlushMouseMsgBuffer();
			}
		}
		else if (mouse.x >= 640 + 20 && mouse.x <= 640 + 180 && mouse.y >= 245 && mouse.y <= 345)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(640 + 20, 245, 640 + 180, 345);

			settextcolor(WHITE);
			r = { 640 + 20, 245, 640 + 180, 345 };
			_stprintf_s(s, _T("修改序号"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			if (mouse.mkLButton)
			{
				setbkcolor(WHITE);
				clearrectangle(0, 0, 570, getheight());
				settextcolor(BLACK);
				setbkcolor(0xD3D3D3);
				InputBox(no, 3, _T("请输入修改条目的序号"), _T("修改条目"), _T("1"));
				int num = file.wchars2int(no);
				num = num - 1;
				if (num < 0)
				{
					FlushMouseMsgBuffer();
					continue;
				}
				f.lfHeight = 20;
				settextstyle(&f);
				r = { 20,60,570,80 };
				if (grades[num] == 0)
					_stprintf_s(s, _T("\t入门\t用户名:%s\t分数:%d"), names[num], scores[num]);
				else if (grades[num] == 1)
					_stprintf_s(s, _T("\t进阶\t用户名:%s\t分数:%d"), names[num], scores[num]);
				else
					_stprintf_s(s, _T("\t大师\t用户名:%s\t分数:%d"), names[num], scores[num]);
				drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);
				FlushBatchDraw();

				InputBox(no, NAME_SIZE, _T("将这个条目的分数修改为"), _T("修改条目"), _T("0"));
				int score = file.wchars2int(no);
				scores[num] = score;

				r = { 20,60 + 25,570,80 + 25 };
				if (grades[num] == 0)
					_stprintf_s(s, _T("\t入门\t用户名:%s\t分数:%d"), names[num], scores[num]);
				else if (grades[num] == 1)
					_stprintf_s(s, _T("\t进阶\t用户名:%s\t分数:%d"), names[num], scores[num]);
				else
					_stprintf_s(s, _T("\t大师\t用户名:%s\t分数:%d"), names[num], scores[num]);
				drawtext(s, &r, DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);

				file.sortMeg(grades, names, scores, LIST_SIZE);
				file.writeMeg(grades, names, scores, LIST_SIZE);

				FlushMouseMsgBuffer();
			}
		}

		else if (mouse.x >= 580 && mouse.x <= 620 && mouse.y >= 420 && mouse.y <= 460)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(580, 420, 620, 460);

			settextcolor(WHITE);
			r = { 580, 420, 620, 460 };
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				return;
				FlushMouseMsgBuffer();
			}
		}

		else
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(WHITE);
			fillrectangle(580, 420, 620, 460);
			r = { 580, 420, 620, 460 };
			settextcolor(BLACK);
			f.lfHeight = 40;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(0xD3D3D3);
			fillrectangle(640 + 20, 25, 640 + 180, 125);
			fillrectangle(640 + 20, 135, 640 + 180, 235);
			fillrectangle(640 + 20, 245, 640 + 180, 345);

			r = { 640 + 20, 25,640 + 180, 125 };
			settextcolor(BLACK);
			f.lfHeight = 30;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("筛选名字"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 135,640 + 180 ,235 };
			_stprintf_s(s, _T("筛选难度"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			r = { 640 + 20, 245, 640 + 180, 345 };
			_stprintf_s(s, _T("修改序号"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		FlushBatchDraw();
	}
}

void Control::showMeg()
{
	setbkcolor(WHITE);
	setbkmode(TRANSPARENT);
	clearrectangle(0, 0, getwidth() - 200, getheight());
	setbkcolor(0xD3D3D3);
	clearrectangle(getwidth() - 200, 0, getwidth(), getheight());

	MOUSEMSG mouse;
	TCHAR s[1000];
	RECT r = { 20,0,620,60 };
	LOGFONT f;
	settextcolor(BLACK);
	gettextstyle(&f);
	f.lfHeight = 40;
	f.lfWidth = 0;
	f.lfQuality = ANTIALIASED_QUALITY;
	f.lfWeight = FW_NORMAL;
	_tcscpy_s(f.lfFaceName, _T("楷体"));
	_stprintf_s(s, _T("说\t\t明"));
	settextstyle(&f);
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_EXPANDTABS);

	f.lfHeight = 18;
	f.lfWeight = FW_THIN;
	r = { 20,60,620,480 };
	settextstyle(&f);
	_stprintf_s(s, _T("【操作说明】\n双人游戏P1由WASD控制方向P2由方向键控制方向\n\
其他模式用WASD或方向键控制方向均可\n\
【地图说明】\n红方块为硬墙，撞到后贪吃蛇长度损失一半\n\
蓝方块为软墙，撞到后贪吃蛇长度损失2\n\
黑圆点为尸体，撞到后贪吃蛇长度损失2\n\
绿地块为加速区，速度提高一倍；红地块为减速区，速度减慢一倍\n\
【食物说明】\n*金果*<稀有>加命加20分1600能5长\n*蓝果*加8分800能2长\n*绿果*加5分400能1长\n*白果*加2分100能\n*紫果*<有毒>致死\n\
【能量条说明】\n一般情况能量条为黑色随时间而减少，能量消耗完后蛇将死亡。\n能量溢出时能量条将变红\n\
【胜利条件】\n你什么都不用做，当你的蛇撞到墙你就赢了\nPS：这句话是别的蛇告诉你的(向PVZ致敬)\n\
【开发信息】同济大学 18级计算机系 孔令百 于 2020年5月16日"));
	drawtext(s, &r, DT_LEFT | DT_EXPANDTABS);

	setcolor(BLACK);
	setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
	setfillcolor(WHITE);
	fillrectangle(580, 420, 620, 460);
	r = { 580, 420, 620, 460 };
	settextcolor(BLACK);
	f.lfHeight = 40;
	f.lfWeight = FW_HEAVY;
	_tcscpy_s(f.lfFaceName, _T("宋体"));
	settextstyle(&f);
	_stprintf_s(s, _T("←"));
	drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	FlushBatchDraw();
	while (true)
	{
		BeginBatchDraw();
		mouse = GetMouseMsg();
		if (mouse.x >= 580 && mouse.x <= 620 && mouse.y >= 420 && mouse.y <= 460)
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(BLACK);
			fillrectangle(580, 420, 620, 460);

			settextcolor(WHITE);
			r = { 580, 420, 620, 460 };
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			if (mouse.mkLButton)
			{
				return;
				FlushMouseMsgBuffer();
			}
		}
		else
		{
			setcolor(BLACK);
			setlinestyle(PS_SOLID | PS_ENDCAP_ROUND, 5);
			setfillcolor(WHITE);
			fillrectangle(580, 420, 620, 460);
			r = { 580, 420, 620, 460 };
			settextcolor(BLACK);
			f.lfHeight = 40;
			f.lfWeight = FW_HEAVY;
			_tcscpy_s(f.lfFaceName, _T("宋体"));
			settextstyle(&f);
			_stprintf_s(s, _T("←"));
			drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		FlushBatchDraw();
	}
}

void Control::solo(int grade)
{
	File file;
	JuniorGame JG;
	wchar_t username[NAME_SIZE] = { 0 };
	int score = 0;
	int grades[LIST_SIZE] = { 0 };
	wchar_t names[LIST_SIZE][NAME_SIZE] = { 0 };
	int scores[LIST_SIZE] = { 0 };
	setbkcolor(WHITE);
	file.readMeg(grades, names, scores, LIST_SIZE);
	InputBox(username, NAME_SIZE, _T("请输入玩家名字"), _T("新玩家"), _T("Root"));
	JG.juniorGame(username, scores[0], score, Gmode(grade));
	for (int i = 0; i < LIST_SIZE; i++)
	{
		if (scores[i] <= score)
		{
			for (int j = LIST_SIZE - 1; j > i; j--)
			{
				grades[j] = grades[j - 1];
				wcscpy_s(names[j], names[j - 1]);
				scores[j] = scores[j - 1];
			}
			grades[i] = grade;
			wcscpy_s(names[i], username);
			scores[i] = score;
			break;
		}
	}
	file.writeMeg(grades, names, scores, LIST_SIZE);
}

void Control::control()
{
	int state = MAINMENU;
	while (true)
	{
		switch (state)
		{
		case MAINMENU:
			state = mainmenu();
			Sleep(100);
			FlushMouseMsgBuffer();
			break;
		case SOLO:
			state = solomenu();
			Sleep(100);
			FlushMouseMsgBuffer();
			break;
		case G1:
			solo(0);
			state = SOLO;
			break;
		case G2:
			solo(1);
			state = SOLO;
			break;
		case G3:
			solo(2);
			state = SOLO;
			break;
		case G4:
			state = SOLO;
			break;
		case LIST:
			showlist();
			state = SOLO;
			break;
		case PVP:
			Pvp pvpG;
			initgraph(2 * (WINDOW_W - 200), WINDOW_H + 150);
			pvpG.pvp();
			initgraph(WINDOW_W, WINDOW_H);
			state = MAINMENU;
			break;
		case PVE:
			Pve pveG;
			pveG.pve();
			state = MAINMENU;
			break;
		case ONLINE:
			state = onlinemenu();
			Sleep(100);
			FlushMouseMsgBuffer();
			break;
		case HOST:
			EndBatchDraw();
			Host h;
			h.hostOnline();
			state = ONLINE;
			break;
		case SLAVE:
			EndBatchDraw();
			Slave s;
			s.slaveOnline();
			state = ONLINE;
			break;
		case MEG:
			showMeg();
			state = MAINMENU;
			break;
		default:break;
		}
	}
}
