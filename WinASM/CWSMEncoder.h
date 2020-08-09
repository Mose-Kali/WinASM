#pragma once
#include <cmath>
#include <cstring>
#include <queue>
#include "pch.h"

#ifdef _DLL_EXPORTS
#define DLL_API _declspec(dllexport)
#else
#define DLL_API _declspec(dllimport)
#endif
using namespace std;

enum CMD_Type
{
	MESG,
	FUNC,
	MDI,
	ADO,
	IMG,
	SLEP,
	NOP,
};
enum MSG_Type {
	MT_OK,
	MT_ERROR,
	MT_INFO,
	MT_ATTENTION,
	MT_QUESTION,
};
enum EDR_Statu {
	STU_STOPPED,
	STU_READY,
	STU_PAUSED,
	STU_RUNNING,
	STU_RELEASED
};
enum CMD_Statu {
	CMD_READY,
	CMD_NONE,
};
struct Message {
	wstring Msg;
	wstring caption;
	int x;
	int y;
	MSG_Type Type;
	int Timeout;
	void operator =(Message msg)
	{
		Msg = msg.Msg;
		caption = msg.caption;
		x = msg.x;
		y = msg.y;
		Type = msg.Type;
		Timeout = msg.Timeout;
	}
};
struct Function {
	int x_4;
	int x_3;
	int x_2;
	int x_1;
	int c;
	int RangeMin;
	int RangeMax;
	int Offset;
	int Timewait;
	Message Msg;
};
struct Audio {
	wstring Path;
};
struct ImageParam {
	wstring Path;
	int x;
	int y;
};
struct Slep {
	int MillionSec;
};
struct Command {
	CMD_Type cType;
	Message Msg;
	Audio Ado;
	Function Func;
	Slep Slp;
	void operator =(Command cmd) {
		cType = cmd.cType;
		Msg = Msg;
	}
};

class CWSMEncoder
{
protected:
	queue<Command> CMD_Paused;

	EDR_Statu c_Status = STU_RELEASED;
	//DWORD WINAPI Worker(LPVOID param);
	BOOL HasReady();
	queue<Command> CMD_Queue;
	queue<Command>* RMT_Queue;
public:
	int Screen_Width;
	int Screen_Height;
	void LoadWSM(string Path);
	void Prepare();
	void Run();
	void Pause();
	void Suspend();
	void Stop();
	void Release();
	Command GetCurrentCmd();
	void PushCmd(Command cmd);
	void SetStatus(EDR_Statu c_Statu);
	EDR_Statu GetStatus();
	void SetCmdQueue(queue<Command>* p_Queue);
};

