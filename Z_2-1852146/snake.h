#pragma once
#ifndef SNAKE_H
#define SNAKE_H
#include<iostream>
#include<ctime>
#include<cstdlib>
#include"wall.h"
#include"general.h"
struct sNode {
	int x;
	int y;
	sNode* next;
};
class Canvas;
class Snake {
private:
	sNode* head;
	int mapX;
	int mapY;
	void addHead();
	void delHead();
	void delTail();
public:
	int len;
	int dlen;
	bool alive;
	Opt direct;
	Snake(Walls& w, int X = 64, int Y = 48, int l = 4);
	void resetSnake(Walls& w, int l = 4);
	~Snake();
	void setState(Opt d = STAY);
	void move();
	void retHead(int& hx, int& hy);
	void retTail(int& tx, int& ty);
	sNode* retSnake();
	bool isBody(int x, int y);
	void dieToWall(Walls& w);
};

#endif // !SNAKE_H