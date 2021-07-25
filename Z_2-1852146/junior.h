#pragma once
#ifndef JUNIOR_H
#define JUNIOR_H
#include<iostream>
#include<conio.h>
#include<Windows.h>
#include<climits>
#include"general.h"
#include"snake.h"
#include"wall.h"
#include"food.h"
#include"canvas.h"
enum Gmode { INTRODUCTION, ADVANCED, MASTER, RPG };
class JuniorGame {
private:
	Opt input();
	void initPlayers(int num, Player p[], int life, int delaynum);
	void flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num);
	void flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy);
	bool isOver(int num, Player p[], Snake s[]);
public:
	void juniorGame(LPTSTR username, int highestscore, int& score, Gmode mode = INTRODUCTION);
};
#endif // !JUNIOR_H
