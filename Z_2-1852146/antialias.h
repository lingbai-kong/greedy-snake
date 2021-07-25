#pragma once
#ifndef ANTIALIAS_H
#define ANTIALIAS_H
#include <iostream>
#include <graphics.h>
#include <math.h>
#include"general.h"
#define NORMAL 1//正常方式
#define GRADUAL 2//渐变方式
struct Point {
	long long x;
	long long y;
};
struct Color {
	BYTE R, G, B;
};
struct Line {
	Point p1;
	Point p2;
	Color L_RGB;
	BYTE width;
};
struct Circle {
	Point P;
	BYTE R;
	Color C_RGB;
	BYTE width;
};
class Antialias {
private:
	int windowH;
	int	windowW;
	int originX;
	int originY;
public:
	Antialias(int windowX = 0, int windowY = 0, int x = 0, int y = 0) :windowH(windowY), windowW(windowX), originX(x), originY(y) {}
	void set(int windowX, int windowY);
	void setorigin_p(int x, int y);
	void drawLine(Line L);
	void drawCircle(Circle C);
	void floodCircle(Circle C, BYTE mode = NORMAL);
};

#endif // !ANTIALIAS_H
