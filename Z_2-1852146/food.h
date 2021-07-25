#pragma once
#ifndef FOOD_H
#define FOOD_H
#include<iostream>
#include<ctime>
#include<cstdlib>
#include"general.h"
#include"snake.h"
#include"wall.h"
enum fType { S, A, B, C, D };
const int pS = 3;
const int pA = 12 + pS;
const int pB = 50 + pA;
const int pC = 20 + pB;
const int pD = 15 + pC;
struct fNode {
	int x;
	int y;
	fType kind;
	bool exist;
};
class Canvas;
class Foods {
private:
	fNode* food;
	int max_num;
	int mapX;
	int mapY;
public:
	Foods(int n = 5, int X = 64, int Y = 48);
	~Foods();
	bool isFood(int x, int y);
	bool isFood(int x, int y, fType& fkind);
	bool eatFood(int x, int y, fType& fkind);
	void produceFood(int pnum, Snake s[], Walls& w);
	int num;
	int left_num;
	fNode* retFood();
	int retFoodnum();
};
#endif // !FOOD_H
