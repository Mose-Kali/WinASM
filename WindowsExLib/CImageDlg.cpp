// CImageDlg.cpp: 实现文件
//

#include "pch.h"
#include "WindowsExLib.h"
#include "CImageDlg.h"
#include "afxdialogex.h"


// CImageDlg 对话框

IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CImageDlg, pParent)
{

}

CImageDlg::~CImageDlg()
{
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CImageDlg 消息处理程序


BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(0, 1, NULL);
	SetTimer(1, Timeout, NULL);
	//HBITMAP hbmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),
	//	m_StrImgFilePath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	//if (hbmp == NULL)
	//	return FALSE;
	/////////////////////////该断程序用来取得加载的BMP的信息////////////////////////
	//m_bmp.Attach(hbmp);
	//DIBSECTION ds;
	//BITMAPINFOHEADER &bminfo = ds.dsBmih;
	//m_bmp.GetObject(sizeof(ds), &ds);
	//int cx = bminfo.biWidth; //得到图像宽度
	//int cy = bminfo.biHeight; //得到图像高度
	///////////////////// ////////////////////////////////
	///////////////得到了图像的宽度和高度后,我们就可以对图像大小进行适应,即调整控件的大小,让它正好显示一张图片///////////////////////////
	//CRect rect;
	//GetWindowRect(&rect);
	//ScreenToClient(&rect);
	//MoveWindow(x, y, cx, cy, true);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CImageDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 0)
	{
		KillTimer(0);
		HBITMAP hbmp = NULL;

		hbmp = (HBITMAP)LoadImage(NULL,
			m_StrImgFilePath,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		float cx, cy, dx, dy, k, t;
		CRect   rect;
		CImage cimage;
		//cimage.Load(_T("D:\\wer.bmp"));
		cimage.Attach(hbmp);

		cx = cimage.GetWidth();
		cy = cimage.GetHeight();
		k = cy / cx;
		::SetWindowPos(this->GetSafeHwnd(), HWND_TOP, x, y, cimage.GetWidth(), cimage.GetHeight(), SWP_NOZORDER);
		CWnd *pWnd = NULL;
		pWnd = this;
		pWnd->GetClientRect(&rect);
		dx = rect.Width();
		dy = rect.Height();
		t = dy / dx;
		if (k >= t)
		{
			rect.right = floor(rect.bottom / k);
			rect.left = (dx - rect.right) / 2;
			rect.right = floor(rect.bottom / k) + (dx - rect.right) / 2;
		}
		else
		{
			rect.bottom = floor(k*rect.right);
			rect.top = (dy - rect.bottom) / 2;
			rect.bottom = floor(k*rect.right) + (dy - rect.bottom) / 2;
		}
		CDC *pDc = NULL;
		pDc = pWnd->GetDC();
		int ModeOld = SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);

		this->ShowWindow(FALSE);
		this->ShowWindow(TRUE);
		cimage.StretchBlt(pDc->m_hDC, rect, SRCCOPY);
		SetStretchBltMode(pDc->m_hDC, ModeOld);
		ReleaseDC(pDc);
		DeleteObject(hbmp);
		hbmp = NULL;
		//CDC* pDC = GetDlgItem(IDC_STATIC1)->GetDC();  //m_ctrlPic是Picture控件的变量
		//Gdiplus::Graphics graphics(pDC->GetSafeHdc());
		//Gdiplus::Image image(m_StrImgFilePath);//若路径不对，则无法实现在客户区里绘图
		//graphics.DrawImage(&image, 0, 0);
	}
	if (nIDEvent == 1)
	{
		OnCancel();
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CImageDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
}
