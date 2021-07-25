#pragma once
#ifndef CONTROL_H
#define CONTROL_H
#include"host.h"
#include"slave.h"
#include<iostream>
#include"junior.h"
#include"PVP.h"
#include"PVE.h"
#include"file.h"
#include"general.h"
class Control
{
private:
	int mainmenu();
	int solomenu();
	int onlinemenu();
	void solo(int);
	void showlist();
	void showMeg();
public:
	void control();
};

#endif // !CONTROL_H
