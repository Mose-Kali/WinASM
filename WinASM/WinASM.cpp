// WinASM.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <queue>
#include "pch.h"
#include "CWSMEncoder.h"
#include "WindowsExLib.h"


queue<Command> q;
HWND hwnd;
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	if (nTimerid == 0)
	{
		if (q.empty())
		{
			exit(0);
		}
		Command cmd = q.front();
		Message Msg;
		Slep slep = cmd.Slp;
		switch (cmd.cType)
		{
		case MESG:
			Msg = cmd.Msg;
			//ShowMessage(Msg.caption.c_str(), Msg.Msg.c_str(), Msg.x, Msg.y, (MT_Type)Msg.Type, Msg.Timeout);
			printf("%s %s %d %d %d %d \n", Msg.caption.c_str(), Msg.Msg.c_str(), Msg.x, Msg.y, (MT_Type)Msg.Type, Msg.Timeout);
			break;
		case ADO:
			break;
		case SLEP:
			SetTimer(hwnd, 1, slep.MillionSec, TimerProc);
			KillTimer(hwnd,0);
			break;
		default:
			break;
		}
		q.pop();
	}
	if (nTimerid == 1)
	{
		KillTimer(hwnd, 1);
		SetTimer(hwnd, 0, 10, TimerProc);
	}
}

int _tmain(int argv,TCHAR* argc[])
{
	system("pause");
	hwnd = FindWindow(L"ConsoleWindowClass", NULL);	
	//if (hwnd)
	//{
	//	ShowWindow(hwnd, SW_HIDE);				
	//}
	ShowMessage(L"AAA", L"AAA", 1500, 500, MTP_INFO, 1000);
	CWSMEncoder Encoder;
	Encoder.LoadWSM("E:\\Devloper Files\\WinASM\\Debug\\Script.txt");
	Encoder.SetCmdQueue(&q);
	Encoder.Prepare();
	/*Encoder.Run();
	Command cmd = Encoder.GetCurrentCmd();;
	while (cmd.cType == NOP)
	{
		cmd = Encoder.GetCurrentCmd();
		Sleep(500);
	}*/
	/*Message Msg = q.front().Msg;
	_tprintf(Msg.Msg.c_str());*/
	SetTimer(hwnd, 0, 10, TimerProc);
	MSG   msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_TIMER)
		{
			DispatchMessage(&msg);
		}
	}
    std::cout << "Hello World!\n";
	system("pause");
	return 0;
}
