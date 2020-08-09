// Lrc2WSM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <string>
#include <cstring>
#include "pch.h"
#pragma warning(disable:4996)
using namespace std;

string wstring2string(wstring wstr)
{
	string result;
	//获取缓冲区大小，并申请空间，缓冲区大小事按字节计算的  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//宽字节编码转换成多字节编码  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//删除缓冲区并返回值  
	result.append(buffer);
	delete[] buffer;
	return result;
}
int _tmain(int argc,TCHAR* argv[])
{
	wchar_t str[100000];
	wcscpy(str, argv[1]);
	freopen(wstring2string(str).c_str(), "r", stdin);
	string spath = wstring2string(str);
	int index = spath.rfind("\\");
	int size = spath.length();
	string s = spath.substr(index + 1, size - index - 4);
	s += "wsm";
	freopen(s.c_str(), "w", stdout);
	char input[200000];
	int last = 0;
	while (gets_s(input) != NULL)
	{
		char min[3], sec[3], ml[3];
		min[0] = input[1];
		min[1] = input[2];
		sec[0] = input[4];
		sec[1] = input[5];
		ml[0] = input[7];
		ml[1] = input[8];
		int mn = atoi(min);
		int se = atoi(sec);
		int mi = atoi(ml);
		se += mn * 60;
		mi += se * 1000;
		char msg[200000];
		memset(msg, 0, sizeof(msg));
		int i;
		for (i = 10; i < strlen(input); i++)
		{
			msg[i - 10] = input[i];
		}
		cout << "SLEP " << mi - last << endl;
		cout << "MSG " << msg << " Lyirc " << 1920 / 2 << " " << 1080 / 2 << " 2 10000" << endl;
		last = mi;
		memset(input, 0, sizeof(input));
	}
	printf("END");
	return 0;
}

