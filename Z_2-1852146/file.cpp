#include"file.h"
using namespace std;


void File::wchar2char(const wchar_t* wchar, char* cchar)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), cchar, len, NULL, NULL);
	cchar[len] = '\0';
}

void File::char2wchar(const char* cchar, wchar_t wchar[])
{
	int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), wchar, len);
	wchar[len] = '\0';
}

int File::wchars2int(wchar_t* p)
{
	char s[NAME_SIZE] = { 0 };
	wchar2char(p, s);
	int num = 0;
	for (int i = 0; i < strlen(s); i++)
	{
		num *= 10;
		num += s[i] - '0';
	}
	return num;
}

void File::writeMeg(int grade[], wchar_t wname[][NAME_SIZE], int score[], int size)
{
	char outname[NAME_SIZE] = "record.txt";
	char name[NAME_SIZE];
	fstream out(outname, ios::out);
	if (!out)
	{
		cerr << "error:open file failed\n";
		exit(FILE_ERROR);
	}
	for (int i = 0; i < size; i++)
	{
		wchar2char(wname[i], name);
		switch (grade[i])
		{
		case 0:out << "�Ѷ�:���� "; break;
		case 1:out << "�Ѷ�:���� "; break;
		case 2:out << "�Ѷ�:��ʦ "; break;
		default:out << "�Ѷ�:error "; break;
		}
		if (strlen(name) == 0)
			out << "�û���:" << "<��>" << ' ';
		else
			out << "�û���:" << name << ' ';
		out << "�÷�:" << score[i] << '\n';
	}
	out.close();
}
void File::readMeg(int grade[], wchar_t wname[][NAME_SIZE], int score[], int size)
{
	char inname[NAME_SIZE] = "record.txt";
	char name[NAME_SIZE];
	char mode[NAME_SIZE] = { 0 };
	fstream in(inname, ios::in);
	if (in)
	{
		for (int i = 0; i < size; i++)
		{
			in.ignore(100, ':');
			in >> mode;
			in.ignore(100, ':');
			in >> name;
			char2wchar(name, wname[i]);
			in.ignore(100, ':');
			in >> score[i];
			if (!strcmp(mode, "����"))
				grade[i] = 0;
			else if (!strcmp(mode, "����"))
				grade[i] = 1;
			else
				grade[i] = 2;
			in.ignore(100, '\n');
		}
	}
	in.close();
}

void File::sortMeg(int grades[], wchar_t names[][NAME_SIZE], int scores[], int size)
{
	wchar_t tempname[NAME_SIZE] = { 0 };
	int tempgrade = 0;
	int tempscore = 0;
	int maxno = 0;
	for (int i = 0; i < size - 1; i++)
	{
		maxno = i;
		for (int j = i + 1; j < size; j++)
		{
			if (scores[j] > scores[maxno])
				maxno = j;
		}
		wcscpy_s(tempname, names[maxno]);
		tempgrade = grades[maxno];
		tempscore = scores[maxno];

		wcscpy_s(names[maxno], names[i]);
		grades[maxno] = grades[i];
		scores[maxno] = scores[i];

		wcscpy_s(names[i], tempname);
		grades[i] = tempgrade;
		scores[i] = tempscore;
	}
}