#pragma once
#include "CWMPPlayer4.h"


// CVedioDlg 对话框

class CVedioDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CVedioDlg)

public:
	CVedioDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CVedioDlg();
	int x, y, cx, cy;
	CString cpath;
	int Timeout;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CVedioDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CWMPPlayer4 m_player;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
