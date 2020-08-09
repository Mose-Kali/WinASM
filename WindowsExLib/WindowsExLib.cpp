// WindowsExLib.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "WindowsExLib.h"
#include "CMessageBox.h"
#include "CImageDlg.h"
#include "CWMPPlayer4.h"
#include "CVedioDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CWindowsExLibApp

BEGIN_MESSAGE_MAP(CWindowsExLibApp, CWinApp)
END_MESSAGE_MAP()


// CWindowsExLibApp 构造

CWindowsExLibApp::CWindowsExLibApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的 CWindowsExLibApp 对象

CWindowsExLibApp theApp;


// CWindowsExLibApp 初始化

BOOL CWindowsExLibApp::InitInstance()
{
	CWinApp::InitInstance();
	//AfxEnableControlContainer();
	//if (!AfxOleInit())
	//{
	//	return FALSE;
	//}
	////初始化控件环境
	//AfxEnableControlContainer();
	//CoInitialize(NULL);
	AfxEnableControlContainer();
	return TRUE;
}


void ShowMessage(CString Cap, CString Text, int x, int y, MT_Type Type, int Timeout)
{
	// TODO: 在此处添加实现代码.
	try {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		CMessageBox Box;
		Box.Caption = Cap;
		Box.Text = Text;
		Box.Type = Type;
		Box.x = x;
		Box.y = y;
		Box.Timewait = Timeout;
		Box.DoModal();
	}
	catch (CMemoryException* e)
	{
		return;
	}
}

void ShowImageDlg(CString Path, int x, int y, int Timeout)
{
	try {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
		ULONG_PTR m_pGdiToken;
		Gdiplus::GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
		CImageDlg imgdlg;
		imgdlg.m_StrImgFilePath = Path;
		imgdlg.x = x;
		imgdlg.y = y;
		imgdlg.Timeout = Timeout;
		imgdlg.DoModal();
		Gdiplus::GdiplusShutdown(m_pGdiToken);
	}
	catch (CMemoryException* e)
	{
		return;
	}
}

void ShowVedioDlg(CString Path, int x, int y, int cx, int cy, int Timeout)
{
	try {
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		/*AfxEnableControlContainer();*/
		CVedioDlg imgdlg;
		imgdlg.cpath = Path;
		imgdlg.x = x;
		imgdlg.y = y;
		imgdlg.cx = cx;
		imgdlg.cy = cy;
		imgdlg.Timeout = Timeout;
		imgdlg.DoModal();
	}
	catch (CMemoryException* e)
	{
		return;
	}
}