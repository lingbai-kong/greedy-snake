#pragma once
#ifndef FILE_H
#define FILE_H
#include<iostream>
#include<Windows.h>
#include<fstream>
#include <graphics.h>
#include"general.h"
class File {
private:
	void char2wchar(const char* cchar, wchar_t wchar[]);
	void wchar2char(const wchar_t* wchar, char* cchar);
public:
	void writeMeg(int grade[], wchar_t wname[][NAME_SIZE], int score[], int size);
	void readMeg(int grade[], wchar_t wname[][NAME_SIZE], int score[], int size);
	int wchars2int(wchar_t* p);
	void sortMeg(int grades[], wchar_t names[][NAME_SIZE], int scores[], int size);
};
#endif // !FILE_H
