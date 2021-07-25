#pragma once
#ifndef PVP_H
#define PVP_H
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<climits>
#include"general.h"
#include"snake.h"
#include"wall.h"
#include"food.h"
#include"canvas.h"
class Pvp {
private:
	void input(Opt& P1, Opt& P2);
	void initPlayers(int num, Player p[], int life, int delaynum);
	void flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num);
	void flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy);
	bool isOver(int num, Player p[], Snake s[]);
public:
	void pvp();
};
#endif // !PVP_H
