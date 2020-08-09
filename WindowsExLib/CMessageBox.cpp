// CMessageBox.cpp: 实现文件
//

#include "pch.h"
#include "WindowsExLib.h"
#include "CMessageBox.h"
#include "afxdialogex.h"

#define _DLL_EXPORTS
// CMessageBox 对话框

IMPLEMENT_DYNAMIC(CMessageBox, CDialogEx)

CMessageBox::CMessageBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CMessageBox, pParent)
{

	Caption = _T("");
	Text = _T("");
	x = 0;
	y = 0;
	Timewait = 0;
}

CMessageBox::~CMessageBox()
{
}

void CMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ICON, m_Icon);
	//DDX_Control(pDX, IDC_TEXT, m_Text);
}


BEGIN_MESSAGE_MAP(CMessageBox, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMessageBox 消息处理程序


//void CMessageBox::MessageOut(CString Caption, CString Text, int x, int y, MT_Type Type, int Timeout)
//{
//	// TODO: 在此处添加实现代码.
//	
//}


BOOL CMessageBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowTextW(Caption);
	GetDlgItem(IDC_TEXT)->SetWindowTextW(Text);
	switch (Type)
	{
	case MTP_ERROR:
		((CStatic*)GetDlgItem(IDC_ICON))->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_ERROR));
		break;
	case MTP_ATTENTION:
		((CStatic*)GetDlgItem(IDC_ICON))->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_WARNING));
		break;
	case MTP_INFO:
		((CStatic*)GetDlgItem(IDC_ICON))->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_INFORMATION));
		break;/*
	case MT_ATTENTION:
		m_Icon.SetIcon(AfxGetApp()->LoadStandardIcon(IDI_EXCLAMATION));
		break;*/
	case MTP_QUESTION:
		((CStatic*)GetDlgItem(IDC_ICON))->SetIcon(AfxGetApp()->LoadStandardIcon(IDI_QUESTION));
		break;
	default:
		break;
	}
	::SetWindowPos(GetSafeHwnd(),HWND_TOP, x, y, 0, 0, SWP_NOSIZE );
	SetTimer(0, Timewait, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CMessageBox::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0)
	{
		KillTimer(0);
		OnCancel();
	}
	CDialogEx::OnTimer(nIDEvent);
}
