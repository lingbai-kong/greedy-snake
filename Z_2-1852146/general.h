#pragma once
#ifndef GENERAL_H
#define GENERAL_H
#define NAME_SIZE 20
#define WINDOW_W 840
#define WINDOW_H 480
#define MEMORY_ERROR 1 
#define SNAKE_ERROR 2
#define GMODE_ERROR 3
#define FILE_ERROR 4
#define ONLINE_ERR 5
#define LIST_SIZE 15

#define MAINMENU 30

#define SOLO 50
#define PVP 51
#define PVE 52
#define ONLINE 53
#define MEG 54

#define G1 60
#define G2 61
#define G3 62
#define G4 64
#define LIST 65

#define SLAVE 70
#define HOST 71

#define BLANK 1000
#define FAST 1001
#define SLOW 1002
#define SKIP 1003
#define FOOD 1004

enum Opt { UP, DOWN, LEFT, RIGHT, STAY, QUIT };

struct Player {
	int score;
	int nowdelay_num;
	int delay_count;
	int life;
	int energy;
	Opt op;
	int hx, hy;
	int len;
};
#endif // !GENERAL_H
