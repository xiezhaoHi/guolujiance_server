
// GLdetectionDlg.h : 头文件
//

#pragma once
#include "mscomm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "LayoutManager.h"
// CGLdetectionDlg 对话框
#include "skin/SkinWndHelper.h"

class CGLdetectionDlg : public CDialogEx
{
	
// 构造
public:
	CGLdetectionDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CGLdetectionDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLDETECTION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	/*static 字体*/
	CFont m_fontText;
	CFont m_fontShow;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// //com 通信控件
	CMscomm m_mscomm_ocx;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm();
	afx_msg void OnBnClickedButtonStart();

	//CString		m_strComBTL;// 记录 串口通信时的波特率
	

							// //COM口 设置
	CComboBox m_combox_com;
	CListCtrl m_listctrl_show; 
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//填充数据到listctrl 
	BOOL  ChangeListData(CStringArray const&);
	CStringArray	m_strAryPrevious;// 保持前一次 收到的数据(解析过的)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	Win32Layout::Box m_layout;
	void initLayout(void); //初始化布局

	/*监听 数据包*/
	CStringArray	m_strAryJTData;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	/*界面初始化*/
	void InitFace(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	/*非客户区绘制标题栏*/
	CSkinWndHelper m_SkinWndHelper;
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	/*初始化界面数据*/
	void InitFaceData(void);

	CString		m_strText;
	void ReSize();
	POINT old;

	LONG m_nWidth;
	LONG m_nHeight;
	CString m_strBTL;
	CString m_strSJW;
	CString m_strJYW;
	CString m_strTZW;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
