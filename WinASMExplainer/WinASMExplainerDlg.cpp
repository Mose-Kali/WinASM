
// WinASMExplainerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WinASMExplainer.h"
#include "WinASMExplainerDlg.h"
#include "afxdialogex.h"
#include "CVedioDlg.h"
#include<windows.h>
#include<Mmsystem.h>
#include <vector>
#pragma comment(lib,"winmm.lib")
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
int Screen_Width;
int Screen_Height;
DWORD WINAPI WindowThread(LPVOID p)
{
	Message* Msg = (Message*)p;
	Message m;
	int Screen_Width = GetSystemMetrics(SM_CXSCREEN);
	int Screen_Height = GetSystemMetrics(SM_CYSCREEN);
	m.caption = Msg->caption;
	m.Msg = Msg->Msg;
	m.Timeout = Msg->Timeout;
	m.Type = Msg->Type;
	m.x = Msg->x;
	m.y = Msg->y;
	Msg = &m;
	/*if (Msg->x<0 || Msg->y<0 || Msg->x>Screen_Width || Msg->y>Screen_Height)
	{
		Msg->x = rand() % Screen_Width;
		Msg->y = rand() % Screen_Height;
	}*/
	//TRACE(L"%s %s %d %d %d %d \n", Msg->caption.c_str(), Msg->Msg.c_str(), Msg->x, Msg->y, (MT_Type)Msg->Type, Msg->Timeout);
	ShowMessage(Msg->caption.c_str(), Msg->Msg.c_str(), Msg->x, Msg->y, (MT_Type)Msg->Type, Msg->Timeout);
	return 0;
}

string LeftRotateString(string str, int n) {
	if (n <= 0 || str.size() == 0)
		return str;
	int len = str.size();
	n = n % len;
	str += str;
	return str.substr(n, len);
}
wstring LeftRotateString(wstring str, int n) {
	if (n <= 0 || str.size() == 0)
		return str;
	int len = str.size();
	n = n % len;
	str += str;
	return str.substr(n, len);
}
DWORD WINAPI ImageThread(LPVOID p)
{
	ImageParam* Msg = (ImageParam*)p;
	//ImageParam m;
	//int Screen_Width = GetSystemMetrics(SM_CXSCREEN);
	//int Screen_Height = GetSystemMetrics(SM_CYSCREEN);
	////m.Path = Msg->Path;
	//m.x = Msg->x;
	//m.y = Msg->y;
	ShowImageDlg(LeftRotateString(Msg->Path, 1).c_str(), Msg->x, Msg->y, Msg->Timeout);
	return 0;
}
DWORD WINAPI VedioThread(LPVOID p)
{
	VedioParam* Msg = (VedioParam*)p;
	//ImageParam m;
	//int Screen_Width = GetSystemMetrics(SM_CXSCREEN);
	//int Screen_Height = GetSystemMetrics(SM_CYSCREEN);
	////m.Path = Msg->Path;
	//m.x = Msg->x;
	//m.y = Msg->y;
	//ShowVedioDlg(LeftRotateString(Msg->Path, 1).c_str(), Msg->x, Msg->y, Msg->cx, Msg->cy, Msg->Timeout);
	CVedioDlg imgdlg;
	imgdlg.cpath = LeftRotateString(Msg->Path, 1).c_str();
	imgdlg.x = Msg->x;
	imgdlg.y = Msg->y;
	imgdlg.cx = Msg->cx;
	imgdlg.cy = Msg->cy;
	imgdlg.Timeout = Msg->Timeout;
	imgdlg.DoModal();
	return 0;
}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinASMExplainerDlg 对话框



CWinASMExplainerDlg::CWinASMExplainerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WINASMEXPLAINER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CWinASMExplainerDlg::~CWinASMExplainerDlg()
{
}

void CWinASMExplainerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CWinASMExplainerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWinASMExplainerDlg 消息处理程序
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}
BOOL CWinASMExplainerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			//
	SetIcon(m_hIcon, FALSE);		//


	// TODO: 在此添加额外的初始化代码
	CWSMEncoder Encoder;
	//WSMPath = L"E:\\Devloper Files\\WinASM\\Debug\\Script.txt";
	std::wstring ws(WSMPath);
	std::string s;
	s = WChar2Ansi(ws.c_str());
	Encoder.LoadWSM(s);
	Encoder.SetCmdQueue(&Cmd_Queue);
	Screen_Width = GetSystemMetrics(SM_CXSCREEN);
	Screen_Height = GetSystemMetrics(SM_CYSCREEN);
	Encoder.Prepare();
	SetTimer(0, 1, NULL);
	SetTimer(2, 1, NULL);
	ShowWindow(SW_MINIMIZE);
	return TRUE;  //
}

void CWinASMExplainerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWinASMExplainerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWinASMExplainerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


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
void CWinASMExplainerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0)
	{
		if (Cmd_Queue.empty())
		{
			KillTimer(0);
			//MessageBox(L"ERROR!!!\r\nCannot Repair!!!\r\nComplated!", L"ERROR", MB_OK | MB_ICONERROR);
			//exit(0);
			ShowWindow(SW_MINIMIZE);
			return;
		}
		Command cmd = Cmd_Queue.front();
		Cmd_Queue.pop();
		Message Msg;
		Slep slep = cmd.Slp;
		Audio ado = cmd.Ado;
		CString sPath = WSMPath;
		Cmd_Param cp;
		wstring strl;
		int index;
		string str;
		ImageParam ip;
		VedioParam vp;
		CVedioDlg imgdlg;
		switch (cmd.cType)
		{
		case MESG:
			Msg = cmd.Msg;
			CreateThread(NULL, NULL, WindowThread, &Msg, NULL, NULL);
			Sleep(3);
			break;
		case ADO:
			//sPath.Format(L"%s",ado.Path.c_str());
			index = sPath.ReverseFind(L'\\');
			strl = sPath.Left(index + 1);
			strl += LeftRotateString(ado.Path, 1);
			sPath.Format(L"%s", strl.c_str());
			//TRACE(L"Audio %s\n", strl.c_str());
			PlaySoundW(NULL, NULL, SND_PURGE);
			PlaySoundW(sPath, NULL, SND_FILENAME | SND_ASYNC);
			break;
		case SLEP:
			SetTimer(1, slep.MillionSec, NULL);
			//TRACE(L"Sleep %d\n", slep.MillionSec);
			KillTimer(0);
			break;
		case CMD:
			//SetTimer(1, slep.MillionSec, NULL);
			//TRACE(L"Sleep %d\n", slep.MillionSec);
			//KillTimer(0);
			cp = cmd.cp;
			str = wstring2string(cp.ExecusePath);
			str = LeftRotateString(str, 1);
			WinExec(str.c_str(), SW_SHOW);
			break;
		case IMG:
			ip = cmd.ip;
			CreateThread(NULL, NULL, ImageThread, &ip, NULL, NULL);
			Sleep(3);
			break;
		case MDI:
			vp = cmd.vp;
			//CreateThread(NULL, NULL, VedioThread, &vp, NULL, NULL);
			imgdlg.cpath = LeftRotateString(vp.Path, 1).c_str();
			imgdlg.x = vp.x;
			imgdlg.y = vp.y;
			imgdlg.cx = vp.cx;
			imgdlg.cy = vp.cy;
			imgdlg.Timeout = vp.Timeout;
			imgdlg.DoModal();
			Sleep(3);
			break;
		default:
			break;
		}
	}
	if (nIDEvent == 1)
	{
		KillTimer(1);
		//ShowMessage(L"AAA", L"AAA", 1500, 100, MTP_INFO, 1000);
		SetTimer(0, 1, NULL);
	}
	if (nIDEvent == 2)
	{
		KillTimer(2);
		ShowWindow(SW_HIDE);
		//ShowMessage(L"AAA", L"AAA", 1500, 500, MTP_INFO, 1000);
	}
	CDialogEx::OnTimer(nIDEvent);
}
