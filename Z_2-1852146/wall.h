#pragma once
#ifndef WALL_H
#define WALL_H
#include<iostream>
#include<ctime>
#include<cstdlib>
#include"general.h"
enum wType { NONE, SOFT, HARD, DBODY, SUP, SDOWN };
const int pHARD = 30;
class Canvas;
class Walls {
private:
	wType** wall;
	int mapX;
	int mapY;
public:
	Walls(int X = 64, int Y = 48);
	~Walls();
	bool isWall(int x, int y, wType& wkind);
	wType** retWall();
};
#endif // !WALL_H
