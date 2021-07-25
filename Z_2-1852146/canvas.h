#pragma once
#ifndef CANVAS_H
#define CANVAS_H
#include <graphics.h>
#include"general.h"
#include"antialias.h"
#include "snake.h"
#include "wall.h"
#include "food.h"
class Canvas {
private:
	int mapX;
	int mapY;
	int smapX;
	int smapY;
	Antialias fun;
	int showX;
	int showY;
public:
	Canvas(int X = 64, int Y = 48);
	void drawSnake(Snake& s, Color color = { 0,0x64,0 });
	void drawWalls(Walls& w);
	void drawFoods(Foods& f);
	void showMeg(int score, int hightest_score, int len, int life, int left_food, int grade, LPTSTR name, double time, int energy);
	void showMeg(int score1, int score2, int life1, int life2, int energy1, int energy2, int left_food, double time);
	void showMeg(int pnum, Player p[], int left_food, double time);
	void defineShow(int x, int y);
	void setorigin_p(int x, int y);
	void drawMap(char map[]);
};

#endif // !CANVAS_H
