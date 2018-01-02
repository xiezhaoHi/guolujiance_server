
// GLdetectionDlg.h : ͷ�ļ�
//

#pragma once
#include "mscomm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "LayoutManager.h"
// CGLdetectionDlg �Ի���
#include "skin/SkinWndHelper.h"

class CGLdetectionDlg : public CDialogEx
{
	
// ����
public:
	CGLdetectionDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CGLdetectionDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLDETECTION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	/*static ����*/
	CFont m_fontText;
	CFont m_fontShow;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// //com ͨ�ſؼ�
	CMscomm m_mscomm_ocx;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm();
	afx_msg void OnBnClickedButtonStart();

	//CString		m_strComBTL;// ��¼ ����ͨ��ʱ�Ĳ�����
	

							// //COM�� ����
	CComboBox m_combox_com;
	CListCtrl m_listctrl_show; 
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//������ݵ�listctrl 
	BOOL  ChangeListData(CStringArray const&);
	CStringArray	m_strAryPrevious;// ����ǰһ�� �յ�������(��������)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	Win32Layout::Box m_layout;
	void initLayout(void); //��ʼ������

	/*���� ���ݰ�*/
	CStringArray	m_strAryJTData;

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	/*�����ʼ��*/
	void InitFace(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	/*�ǿͻ������Ʊ�����*/
	CSkinWndHelper m_SkinWndHelper;
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	/*��ʼ����������*/
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
