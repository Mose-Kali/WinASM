#pragma once

#ifdef MATUREAPPROACH_EXPORTS
#define MATUREAPPROACH_API __declspec(dllexport)
#else
#define MATUREAPPROACH_API __declspec(dllimport)
#endif
// CMessageBox 对话框
enum MT_Type {
	MTP_OK,
	MTP_ERROR,
	MTP_INFO,
	MTP_ATTENTION,
	MTP_QUESTION,
};
__declspec(dllexport) class CMessageBox : public CDialogEx
{
	DECLARE_DYNAMIC(CMessageBox)

public:
	CMessageBox(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMessageBox();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CMessageBox };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Icon;
	CStatic m_Text;
//	static void MessageOut(CString Caption, CString Text, int x, int y, MT_Type Type, int Timeout);
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CString Caption;
	CString Text;
	int x;
	int y;
	int Timewait;
	MT_Type Type;
};
