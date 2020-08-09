#pragma once


// CImageDlg 对话框

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CImageDlg();
	CString m_StrImgFilePath;
	int x, y, Timeout;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CImageDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CBitmap m_bmp;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};
