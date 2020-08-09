// CVedioDlg.cpp: 实现文件
//

#include "pch.h"
#include "WindowsExLib.h"
#include "CVedioDlg.h"
#include "afxdialogex.h"
#include "CWMPControls.h"


// CVedioDlg 对话框

IMPLEMENT_DYNAMIC(CVedioDlg, CDialogEx)

CVedioDlg::CVedioDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CVedioDlg, pParent)
{

}

CVedioDlg::~CVedioDlg()
{
}

void CVedioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX1, m_player);
}


BEGIN_MESSAGE_MAP(CVedioDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CVedioDlg 消息处理程序


BOOL CVedioDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_player.put_uiMode(L"none");
	if (cx == -1)
	{
		cx = GetSystemMetrics(SM_CXSCREEN);
	}
	if (cy == -1)
	{
		cy = GetSystemMetrics(SM_CYSCREEN);
	}
	MoveWindow(x, y, cx, cy, true);
	CRect r;
	GetClientRect(r);
	m_player.MoveWindow(r);
	m_player.put_URL(cpath);
	SetTimer(0, Timeout, NULL);
	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CVedioDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0)
	{
		OnCancel();
	}
	CDialogEx::OnTimer(nIDEvent);
}
