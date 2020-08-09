// WindowsExLib.h: WindowsExLib DLL 的主标头文件
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// 主符号
#include "CMessageBox.h"

// CWindowsExLibApp
// 有关此类实现的信息，请参阅 WindowsExLib.cpp
//
void ShowMessage(CString Cap, CString Text, int x, int y, MT_Type Type, int Timeout);
void ShowImageDlg(CString Path, int x, int y, int Timeout);
void ShowVedioDlg(CString Path, int x, int y, int cx, int cy, int Timeout);
class CWindowsExLibApp : public CWinApp
{
public:
	CWindowsExLibApp();

	// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	//void ShowMessage(CString Cap, CString Text, int x, int y, MT_Type Type, int Timeout);
};
