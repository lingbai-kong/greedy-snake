#include"main.h"
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;
int main()
{
	Control t;
	initgraph(WINDOW_W, WINDOW_H);
	PlaySound(TEXT("BGM.wav"), NULL, SND_FILENAME | SND_ASYNC| SND_LOOP);
	t.control();
	closegraph();
	return 0;
}