#pragma once
#ifndef SLAVE_H
#define SLAVE_H
#include "Client.h"
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <conio.h>
#include"general.h"
#include"canvas.h"
#include"file.h"
class Slave
{
private:
	Opt input();
	void getName(char name[]);
	int retNameNo(char nameList[][40], int pnum, const char name[]);
	bool connectRoom(NetworkHelper::ClientHelper& _client, uint32_t& roomId, int& pnum, int& mapX, int& mapY, char nameList[][40]);
	void joinGame(NetworkHelper::ClientHelper& _client, int pnum, int mapX, int mapY, char nameList[][40], const char name[40]);
public:
	void slaveOnline();
};
#endif // !SLAVE_H
