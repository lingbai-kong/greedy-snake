#pragma once
#ifndef AI_H
#define AI_H
#include<iostream>
#include"snake.h"
#include"wall.h"
#include"food.h"

class SearchNode {
private:
	SearchNode* leftchild;
	SearchNode* midchild;
	SearchNode* rightchile;
public:
	int x, y;
	int state;
	bool ischeck;
	int fn;
	int gn;
	int hn;
	SearchNode* father;
	SearchNode();
	void addchild(SearchNode* child);
};
class SearchList {
private:
	struct ListNode {
		SearchNode* node;
		ListNode* next;
	}*head;
public:
	SearchList();
	~SearchList();
	bool isEmpty();
	void addNode(SearchNode* s);
	void popMin(SearchNode*& s);
};
class AI {
private:
	int mapX, mapY;
	int h(int x, int y, int tx, int ty);
	Opt greed(int no, int pnum, Snake s[], Walls& w, Foods& f);
	Opt A_star(int no, int pnum, Snake s[], Walls& w, Foods& f);
	bool reachtail(int no, int pnum, Snake s[], Walls& w, int nhx, int nhy);
public:
	AI(int x, int y) :mapX(x), mapY(y) {}
	Opt fun(int no, int pnum, Snake s[], Walls& w, Foods& f, int type = 0);
};

#endif // !AI_H
