#pragma once
#ifndef HOST_H
#define HOST_H
#include "Client.h"
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <conio.h>
#include"general.h"
#include"snake.h"
#include"wall.h"
#include"food.h"
#include"canvas.h"

class Host {
private:
	Opt input();
	void initPlayers(int num, Player p[], int life, int delaynum);
	void flushPlayers1(int num, Player p[], Snake s[], Walls& w, Foods& f, int delay_num);
	void flushPlayers2(int num, Player p[], Snake s[], Walls& w, int denergy);
	bool isOver(int num, Player p[], Snake s[]);
	void creatMap(int mapX, int mapY, char map[], int num, Snake s[], Walls& w, Foods& f);
	void getName(char name[]);
	int retNameNo(char nameList[][40], int pnum, const char name[40]);
	bool hostRoom(NetworkHelper::ClientHelper& _host, uint32_t& roomId, int& pnum, int mapX, int mapY, char nameList[][40]);
	void searveGame(NetworkHelper::ClientHelper& _host, int pnum, int mapX, int mapY, char nameList[][40]);
public:
	void hostOnline();
};

#endif // !HOST_H
