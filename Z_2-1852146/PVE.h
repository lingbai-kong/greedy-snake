#pragma once
#ifndef PVE_H
#define PVE_H
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<climits>
#include"general.h"
#include"snake.h"
#include"wall.h"
#include"food.h"
#include"canvas.h"
#include"AI.h"
class Pve {
private:
	Opt input();
	void initPlayers(int num, Player p[], int life, int delaynum);
	void flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num);
	void flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy);
	bool isOver(int num, Player p[], Snake s[]);
public:
	void pve();
};
#endif // !PVE_H
