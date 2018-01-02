
// GLdetectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "GLdetection.h"
#include "GLdetectionDlg.h"
#include "afxdialogex.h"
#include "ReadWriteConfig\ReadWriteConfig.h"
#include "logrecord\LogRecord.h"
#include "public\publicFun.h"
#include "Resource.h"
#include "LayoutManager.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLdetectionDlg 对话框

/*启动美化的宏*/
#define  SHOW_MOUDLE


CGLdetectionDlg::CGLdetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GLDETECTION_DIALOG, pParent)
	, m_strBTL(_T(""))
	, m_strSJW(_T(""))
	, m_strJYW(_T(""))
	, m_strTZW(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CGLdetectionDlg::~CGLdetectionDlg()
{
	m_SkinWndHelper.Detach();
}

void CGLdetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSCOMM, m_mscomm_ocx);
	DDX_Control(pDX, IDC_COMBO_COM, m_combox_com);

	DDX_Text(pDX, IDC_STATIC_BTL_TEXT, m_strBTL);
	DDX_Text(pDX, IDC_STATIC_SJW_TEXT, m_strSJW);
	DDX_Text(pDX, IDC_STATIC_JYW_TEXT, m_strJYW);
	DDX_Text(pDX, IDC_STATIC_TZW_TEXT, m_strTZW);
}

BEGIN_MESSAGE_MAP(CGLdetectionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGLdetectionDlg::OnBnClickedButtonStart)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
END_MESSAGE_MAP()


// CGLdetectionDlg 消息处理程序

/*list 控件中需要显示的数据*/
TCHAR* chListShow[] = {
	_TEXT("二氧化硫(ppm)"),
	_TEXT("一氧化氮(ppm)"),
	_TEXT("一氧化碳(ppm)"),
	_TEXT("二氧化氮(ppm)"),
	_TEXT("氧气(%)"),
	_TEXT("二氧化碳(%)"),
	NULL
};

// {0x0B, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},			//O2
// { 0x0B, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 },			//CO
// { 0x0B, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 },			//NO
// { 0x0B, 0x10, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00 },			//SO2
// { 0x0B, 0x10, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 },			//NOX
//																		{0x0B, 0x10, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00},		// H2
// { 0x0B, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }			//CO2
TCHAR* chCOMInstruct[] = {
	_TEXT("0B10070000000000"),
	_TEXT("0B10040000000000"),
	_TEXT("0B10030000000000"),
	_TEXT("0B10060000000000"),
	_TEXT("0B10000000000000"),
	_TEXT("0B10010000000000"),
	NULL
};

/*static 控件ID*/
static int controlID[] = { IDC_STATIC_SO2,IDC_STATIC_NO,IDC_STATIC_CO,IDC_STATIC_NO2
,IDC_STATIC_O2,IDC_STATIC_CO2,-1 };
static int controlIDT[] = { IDC_STATIC_SO2_T,IDC_STATIC_NO_T,IDC_STATIC_CO_T,IDC_STATIC_NO2_T
,IDC_STATIC_O2_T,IDC_STATIC_CO2_T,-1 };
static int staticID[] = { IDC_STATIC_CK,IDC_STATIC_BTL,IDC_STATIC_SJW,IDC_STATIC_JYW
,IDC_STATIC_TZW,-1 };
static int staticIDT[] = { -1,IDC_STATIC_BTL_TEXT,IDC_STATIC_SJW_TEXT,IDC_STATIC_JYW_TEXT
,IDC_STATIC_TZW_TEXT,-1 };
static CStringArray strAry;


BOOL CGLdetectionDlg::OnInitDialog()
{
// 	int end = 128, begin = 2;
// 	int result = ((end << 8) | begin)&(~0x8000);
// 	long l = (end << 8);
// 	l = (end << 8) | begin;
// 	l = l & (~0x8000);
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	//SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	CRect rect;
	GetClientRect(&rect);     //取客户区大小    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	////CFont font;
	//font.CreatePointFont(260,_T("楷体"));//LPCTSTR
	//GetDlgItem(IDC_STATIC_TEST)->SetFont(&font);
	//
	//


	/*界面初始化*/
	InitFace();
	InitFaceData();
	//禁止边框ModifyStyle(0, ); 
	ModifyStyle(m_hWnd, WS_THICKFRAME , 0, 0);
	ModifyStyle(0, WS_MINIMIZEBOX);

	m_SkinWndHelper.Attach(m_hWnd);
	ShowWindow(SW_MAXIMIZE);
	m_strText = _T("开始检测");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
#define  BKCOLOR RGB(0xea, 0xec, 0xf1)
void CGLdetectionDlg::OnPaint()
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

		CRect rect;
		CPaintDC dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect, RGB(255,255,255));  //颜色可以自己随便设置
		
		CClientDC *pDC = new CClientDC(GetDlgItem(IDC_STATIC_LOG));
		CRect rect2;
		GetDlgItem(IDC_STATIC_LOG)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(CLogRecord::GetAppPath()+_T("//picture//logo.png")); // Construct an image

		graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
		delete pDC;
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CGLdetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CGLdetectionDlg, CDialogEx)
	ON_EVENT(CGLdetectionDlg, IDC_MSCOMM, 1, CGLdetectionDlg::OnCommMscomm, VTS_NONE)
END_EVENTSINK_MAP()



//函数说明:com监听 事件 
void CGLdetectionDlg::OnCommMscomm()
{
	// TODO: 在此处添加消息处理程序代码

	CString str;// 保存数据

				/*
				@** VARIANT 类型测试
				*/
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len, k;
	BYTE rxdata[2048] = { 0 };
	//设置BYTE数组 An 8-bit integerthat is not signed.

	CString strtemp, strRXData;

	if (m_mscomm_ocx.get_CommEvent() == 2)//事件值为2表示接收缓冲区内有字符
	{
		
		variant_inp = m_mscomm_ocx.get_Input();    //读串口输入缓冲区
												 //将VARAIANT变量赋值给COleSafeArray类的实例
		safearray_inp = variant_inp;
			//使用COleSafeArray类的成员函数获取数据长度
			len = safearray_inp.GetOneDimSize();
			//使用COleSafeArray类的成员函数将数据写入数组
			for (k = 0; k < len; k++)
			{
				safearray_inp.GetElement(&k, &rxdata[k]);
				strtemp.Format(_T("%02X"), rxdata[k]);
				strRXData += strtemp;
			}
		
	}

	/*解析数据填充*/
	CLogRecord::WriteRecordToFile(_T("___接受数据为:")+strRXData+_T("______开始做数据解析与填充!___"));

	int lenIndex = -1;
	for ( int index = 0; chCOMInstruct[index] != NULL; ++index)
	{
		lenIndex = strRXData.Find(chCOMInstruct[index]);
		if (lenIndex >= 0) //如果接受到的 数据中 有指定的 指令协议 标识
		{
			lenIndex = index;  //记录 是哪一条指令
			break;
		}
	}
	CString strTemp;
	strtemp.Format(_T("%d"), lenIndex);
	if (m_strAryJTData.GetAt(0).IsEmpty())
	{
		if (lenIndex >= 0 ) //表示接受道的数据中 是 发送的指令之一 并且 没有前一条数据
		{
			m_strAryJTData.SetAt(0, strtemp);
		}
		return;
	}
	else
	{
		if (lenIndex >= 0) ////表示收到的两条都 是 发送的指令
 		{
			m_strAryJTData.SetAt(0, strtemp);
			return;
		}
	}
	
	//表示这条 数据感觉是 返回的检测数据

	int begIndex = strRXData.Find(_T("0B10"));
	if (begIndex < 0) //数据有误
	{
		return;
	}

	strRXData = strRXData.Mid(begIndex, 2 * 8);
	if (strRXData.GetLength() < 16) //数据有误
	{
		return;
	}
	strRXData = strRXData.Mid(4, 4);
	//数据检测 0000  FFFF 
	if (_T("0000") == strRXData || _T("FFFF") == strRXData)
	{
		return;
	}
	
	int begin = 0, end = 0;
	sscanf_s(CpublicFun::UnicodeToAsc(strRXData.Left(2)), "%x", &begin);
	sscanf_s(CpublicFun::UnicodeToAsc(strRXData.Right(2)), "%x", &end);
	
	if (LISTSHOW_O2 == lenIndex
		|| LISTSHOW_CO2 == lenIndex)
	{
	
	}

	int result = ((end << 8) | begin)&(~0x8000);
	
	strtemp.Format(_T("%d"), result);
	m_strAryJTData.SetAt(1, strtemp);
	ChangeListData(m_strAryJTData);

	m_strAryJTData.RemoveAll();  // 清空 数据
	m_strAryJTData.SetSize(2);
	CLogRecord::WriteRecordToFile(_T("___一次数据接受结束!___"));
}


void CGLdetectionDlg::OnBnClickedButtonStart()
{


	// TODO: 在此添加控件通知处理程序代码
	CString temp, strCom;
	
	m_combox_com.GetWindowText(strCom);
	
	int index = strCom.Find(_T("COM"));
	if (index < 0)
	{
		return;
	}
	strCom = strCom.Right(strCom.GetLength() - index - 3); 

	int size = 10 * 1024;
	//temp.Format("9600,n,8,1");
// 	CString m_strBTL;
// 	CString m_strSJW;
// 	CString m_strJYW;
// 	CString m_strTZW;
	temp.Format(_T("%s,%s,%s,%s"), m_strBTL, m_strJYW, m_strSJW, m_strTZW);
	
	if (m_mscomm_ocx.get_PortOpen())
	{ 
		m_mscomm_ocx.put_PortOpen(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("开始监听!"));
		m_strText = _T("开始检测");
		return;
	}
		

	m_mscomm_ocx.put_CommPort(_ttoi(strCom)); //设置串口
	m_mscomm_ocx.put_Settings(temp); //设置波特率等参数
	m_mscomm_ocx.put_RThreshold(1); //设置收到多少个字符后触发OnComm事件
	m_mscomm_ocx.put_InBufferSize(size);
	m_mscomm_ocx.put_InputLen(0);
	//m_mscomm_ocx.put_OutBufferSize(size);
	//m_mscomm_ocx.put_OutBufferCount(0);

	//int mode = m_mscomm_ocx.get_InputMode();
	//文本模式
	//m_mscomm_ocx.put_InputMode(0);

	//二进制模式
	m_mscomm_ocx.put_InputMode(1);

	if (!m_mscomm_ocx.get_PortOpen())
	{
		m_mscomm_ocx.put_PortOpen(TRUE); //打开串口
		((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("结束监听!"));
		m_strText = _T("结束监听");
		CReadWriteConfig::ConfigSetValue(_T("COM"), _T("COM_CUR"), strCom);
	}
}


void CGLdetectionDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: 在此处添加消息处理程序代码
}


void CGLdetectionDlg::OnSize(UINT nType, int cx, int cy)
{
	
// 	CWnd *pWnd;
// 
// 	pWnd = GetDlgItem(IDC_STATIC_SO2_T);//获取控件句柄
// 
// 	if (pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
// 
// 	{
// 		CRect rect;//获取控件变化前大小
// 		pWnd->GetWindowRect(&rect);
// 		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
// 							  //　cx/m_rect.Width()为对话框在横向的变化比例
// 		rect.left = rect.left*cx / m_rect.Width();/////调整控件大小
// 		rect.right = rect.right*cx / m_rect.Width();
// 		rect.top = rect.top*cy / m_rect.Height();
// 		rect.bottom = rect.bottom*cy / m_rect.Height();
// 		pWnd->MoveWindow(rect);//设置控件大小
// 	}
// 	GetClientRect(&m_rect);//将变化后的对话框大小设为旧大小
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED) //
	{
		ReSize();
		Invalidate(TRUE);
	}
	CDialogEx::OnSize(nType, cx, cy);
}

/*实时显示数据到界面上*/
BOOL CGLdetectionDlg::ChangeListData(CStringArray const& strAry)
{
// 	m_listctrl_show.SetRedraw(FALSE);
// 	//更新内容
// 	
// 	int curItem = m_listctrl_show.GetItemCount();
// 	for (int index = 0; index < strAry.GetSize();++index)
// 	{
// 		m_listctrl_show.SetItemText(curItem+1, index+1, strAry.GetAt(index));
// 	}
// 	m_listctrl_show.SetRedraw(TRUE);
// 	m_listctrl_show.Invalidate();
// 	m_listctrl_show.UpdateWindow();
	if (strAry.GetSize() < 2)
	{
		return FALSE;
	}
	int index = _ttoi(strAry.GetAt(0));
	CString strTemp = strAry.GetAt(1);

	if (LISTSHOW_CO2 == index
		|| LISTSHOW_O2 == index)
	{
		strTemp.Format(_T("%.02f"),_ttoi(strTemp)/100.0);
	}
	
		if (controlID[index] != -1)
		{
			((CButton*)GetDlgItem(controlID[index]))->SetWindowText(strTemp);
		}

	
	return  TRUE;
}



/*控件的颜色*/
#ifdef SHOW_MOUDLE
#define  STATICSO2 RGB(0xf1, 0xb7, 0x06)
#define  STATICNO RGB(0x20, 0x97, 0xf1)
#define  STATICCO RGB(0xcc, 0xdc, 0x38)
#define  STATICNO2 RGB(0x3e, 0x51, 0xb3)
#define  STATICO2 RGB(0xfe, 0x90, 0x00)
#define  STATICCO2 RGB(0xe8, 0x1e, 0x62)

#define  STATICSO2_BMP RGB(0xe5, 0xaf, 0x06)
#define  STATICNO_BMP RGB(0x1d, 0x8e, 0xe5)
#define  STATICCO_BMP RGB(0xc2, 0xd0, 0x35)
#define  STATICNO2_BMP RGB(0x3e, 0x51, 0xb3)
#define  STATICO2_BMP RGB(0xf1, 0x90, 0x00)
#define  STATICCO2_BMP RGB(0xe8, 0x1e, 0x62)
#define	 BUTTONJC	RGB(67,78,82)
#ifdef SHOW_MOUDLE

#define  STATICSO2_T RGB(0xf1, 0xb7, 0x06)
#define  STATICNO_T RGB(0x20, 0x97, 0xf1)
#define  STATICCO_T RGB(0xcc, 0xdc, 0x38)
#define  STATICNO2_T RGB(0x3e, 0x51, 0xb3)
#define  STATICO2_T RGB(0xf1, 0x90, 0x00)
#define  STATICCO2_T RGB(0xe8, 0x1e, 0x62)


#else
#define  STATICSO2_T RGB(0xff, 0xff, 0xff)
#define  STATICNO_T RGB(0xff, 0xff, 0xff)
#define  STATICCO_T RGB(0xff, 0xff, 0xff)
#define  STATICNO2_T RGB(0xff, 0xff, 0xff)
#define  STATICO2_T RGB(0xff, 0xff, 0xff)
#define  STATICCO2_T RGB(0xff, 0xff, 0xff)
#endif
#endif

HBRUSH CGLdetectionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	CFont font;
	// TODO:  在此更改 DC 的任何特性
// 	if (pWnd->GetDlgCtrlID() == )
// 	{
// 		pDC->SetBkMode(TRANSPARENT); //你可以修改字体背景模式
// 									 //pDC-> SetBkColor(m_bkcolor); //你可以修改背景颜色
// 									 //hbr=CreateSolidBrush(m_bkcolor); //你可以修改画刷
// 		//pDC->SetTextColor(RGB(255, 0, 0)); //你当然可以修改字体颜色
// 		
// 		HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
// 		return b;//这句好有用
// 	}
	switch (pWnd->GetDlgCtrlID())
	{
	
#if 1  //显示的6中气体的 static 控件
	case IDC_STATIC_SO2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICSO2);
		return hbr;//
		break;
	case IDC_STATIC_NO:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICNO);
		return hbr;//
		break;
	case IDC_STATIC_CO:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICCO);
		return hbr;//
		break;
	case IDC_STATIC_NO2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICNO2);
		return hbr;//
		break;
	case IDC_STATIC_O2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICO2);
		return hbr;//
		break;
	case IDC_STATIC_CO2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICCO2);
		return hbr;//
		break;


	case IDC_STATIC_SO2_T:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICSO2_T);
		return hbr;//
		break;
	case IDC_STATIC_NO_T:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICNO_T);
		return hbr;//
		break;
	case IDC_STATIC_CO_T:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICCO_T);
		return hbr;//
		break;
	case IDC_STATIC_NO2_T:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICNO2_T);
		return hbr;//
		break;
	case IDC_STATIC_O2_T:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICO2);
		return hbr;//
		break;
	case IDC_STATIC_CO2_T:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICCO2_T);
		return hbr;//
		break;

	case IDC_STATIC_Bit_SO2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICSO2_T);
		return hbr;//
		break;
	case IDC_STATIC_Bit_NO:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICNO_T);
		return hbr;//
		break;
	case IDC_STATIC_Bit_CO:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICCO_T);
		return hbr;//
		break;
	case IDC_STATIC_Bit_NO2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICNO2_T);
		return hbr;//
		break;
	case IDC_STATIC_Bit_O2:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICO2_T);
		return hbr;//
		break;
	case IDC_STATIC_Bit_CO2: 
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(STATICCO2_T);
		return hbr;//
		break;

#endif
	case IDC_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(255, 255, 255));
		return hbr;//
		break;

	case IDC_STATIC_LOG:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(255,255,255));
		return hbr;//
		break;
	default:
		pDC->SetBkMode(TRANSPARENT);
		hbr = CreateSolidBrush(RGB(255, 255, 255));
		return hbr;//
		break;
	}
	
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CGLdetectionDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	CDC dc;
// 	dc.Attach(lpDrawItemStruct->hDC);
// 	ASSERT(lpDrawItemStruct->CtlType == ODT_BUTTON);
// 
// 	CString strText;
// 	((CButton *)GetDlgItem(nIDCtl))->GetWindowText(strText);
// 
// 	SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
// 	if (lpDrawItemStruct->itemState&ODS_SELECTED)
// 	{
// 		CBrush brush(RGB(230, 227, 232));
// 		dc.FillRect(&(lpDrawItemStruct->rcItem), &brush);
// 		DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(),
// 			&lpDrawItemStruct->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
// 		SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
// 	}
// 	dc.Detach();
	CString strSO2Path = CLogRecord::GetAppPath() + _T("//picture//so2.png");
	CString strCO2Path = CLogRecord::GetAppPath() + _T("//picture//co2.png");
	CString strO2Path = CLogRecord::GetAppPath() + _T("//picture//o2.png");
	CString strNO2Path = CLogRecord::GetAppPath() + _T("//picture//no2.png");
	CString strCOPath = CLogRecord::GetAppPath() + _T("//picture//co.png");
	CString strNOPath = CLogRecord::GetAppPath() + _T("//picture//no.png");
	CString strLogoPath = CLogRecord::GetAppPath() + _T("//picture//logo.png");
	static unsigned int gIn = 20;

	/*logo*/
	
	if (nIDCtl == IDC_STATIC_LOG)
	{
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, RGB(255,255,255));


		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strLogoPath); // Construct an image

		graphics.DrawImage(&image,0, 0, image.GetWidth(), image.GetHeight());
		delete pDC;
	}
#if 1 //6种气体 显示的static 控件
	if (nIDCtl == IDC_STATIC_Bit_SO2)
	{
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICSO2_BMP);

		
		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strSO2Path); // Construct an image
		graphics.DrawImage(&image, lpDrawItemStruct->rcItem.right/4, lpDrawItemStruct->rcItem.bottom/4);
		//graphics.DrawImage(&image, 0,0,image.GetWidth(),image.GetHeight());
		delete pDC;
	}

	if (nIDCtl == IDC_STATIC_Bit_CO2)
	{
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICCO2_BMP);


		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strCO2Path); // Construct an image
		graphics.DrawImage(&image, lpDrawItemStruct->rcItem.right / 4, lpDrawItemStruct->rcItem.bottom / 4);
		delete pDC;
	}
	if (nIDCtl == IDC_STATIC_Bit_O2)
	{

		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICO2_BMP);


		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strO2Path); // Construct an image
		graphics.DrawImage(&image, lpDrawItemStruct->rcItem.right / 4, lpDrawItemStruct->rcItem.bottom / 4);
		delete pDC;
	}
	if (nIDCtl == IDC_STATIC_Bit_NO2)
	{
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICNO2_BMP);


		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strNO2Path); // Construct an image
		graphics.DrawImage(&image, lpDrawItemStruct->rcItem.right / 4, lpDrawItemStruct->rcItem.bottom / 4);
		delete pDC;
	}
	if (nIDCtl == IDC_STATIC_Bit_CO)
	{
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICCO_BMP);


		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strCOPath); // Construct an image
		graphics.DrawImage(&image, lpDrawItemStruct->rcItem.right / 4, lpDrawItemStruct->rcItem.bottom / 4);
		delete pDC;
	}
	if (nIDCtl == IDC_STATIC_Bit_NO)
	{
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICNO_BMP);


		CClientDC *pDC = new CClientDC(GetDlgItem(nIDCtl));
		CRect rect2;
		GetDlgItem(nIDCtl)->GetWindowRect(&rect2);
		Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
		Image image(strNOPath); // Construct an image
		graphics.DrawImage(&image, lpDrawItemStruct->rcItem.right / 4, lpDrawItemStruct->rcItem.bottom / 4);
		delete pDC;
	}

#endif
	if(nIDCtl == IDC_BUTTON_START)
	{
#define  BUTTONCOLOR RGB(67,78,82)
		SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);   //透明
		CDC *pD = CDC::FromHandle(lpDrawItemStruct->hDC);

		pD->FillSolidRect(0, 0, lpDrawItemStruct->rcItem.right
			, lpDrawItemStruct->rcItem.bottom, STATICNO_BMP);


		CFont font;

		font.CreateFont(
			24, // nHeight
			0, // nWidth
			0, // nEscapement
			0, // nOrientation
			FW_NORMAL, // nWeight
			FALSE, // bItalic
			FALSE, // bUnderline
			0, // cStrikeOut
			ANSI_CHARSET, // nCharSet
			OUT_DEFAULT_PRECIS, // nOutPrecision
			CLIP_DEFAULT_PRECIS, // nClipPrecision
			DEFAULT_QUALITY, // nQuality
			DEFAULT_PITCH | FF_SWISS,
			_T("微软雅黑") // nPitchAndFamily Arial
		);
		pD->SelectObject(&font);
		pD->SetTextColor(RGB(255, 255, 255));
		CString strBegin = m_strText;
		DrawText(lpDrawItemStruct->hDC, strBegin, strBegin.GetLength(), &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		if (lpDrawItemStruct->itemState &ODS_SELECTED)  //当按下按钮时的处理
		{//
		 //重绘整个控制
			CBrush brush(BUTTONCOLOR);
			pD->FillRect(&(lpDrawItemStruct->rcItem), &brush);//

															 //因为这里进行了重绘,所以文字也要重绘
			DrawText(lpDrawItemStruct->hDC, strBegin, strBegin.GetLength(),
				&lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_TABSTOP);
			SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		}
		else                       //当按钮不操作或者弹起时
		{
			CBrush brush(BUTTONCOLOR);
			pD->FillRect(&(lpDrawItemStruct->rcItem), &brush);//

															 //同上,进行重绘文字
			DrawText(lpDrawItemStruct->hDC, strBegin, strBegin.GetLength(), &lpDrawItemStruct->rcItem, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_TABSTOP);
			SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		}

		if ((lpDrawItemStruct->itemState &ODS_SELECTED) && (lpDrawItemStruct->itemAction &(ODA_SELECT | ODA_DRAWENTIRE)))
		{   //选中了本控件,高亮边框
			COLORREF fc = RGB(255 - GetRValue(BUTTONCOLOR), 255 - GetGValue(BUTTONCOLOR), 255 - GetBValue(BUTTONCOLOR));//
			CBrush brush(fc);//
			pD->FrameRect(&(lpDrawItemStruct->rcItem), &brush);//

		}
		if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && (lpDrawItemStruct->itemAction & ODA_SELECT))
		{
			//控制的选中状态结束,去掉边框
			CBrush brush(BUTTONCOLOR);
			pD->FrameRect(&lpDrawItemStruct->rcItem, &brush);//

		}
	}
		
		
		
		
		
	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CGLdetectionDlg::initLayout(void)
{

	// 初始化布局
	CRect rectWnd;
	GetClientRect(&rectWnd);
	CRect rectShow;
	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->GetClientRect(&rectShow);

	m_layout.SetType(Win32Layout::HORIZONTAL); //从左到右 布局

	Win32Layout::Box* boxLeft = m_layout.AddHBox(rectWnd.right - rectShow.right, 0, Win32Layout::PIXEL, Win32Layout::FILL);



	Win32Layout::Box* boxRight = m_layout.AddHBox(0, 0, Win32Layout::FILL, Win32Layout::FILL);

	boxRight->SetType(Win32Layout::VERTICAL); //右边box 从上到下 布局

	CRect rectShowSO2, rectTempB;
	((CButton*)GetDlgItem(IDC_STATIC_SO2))->GetClientRect(&rectShowSO2);
	((CButton*)GetDlgItem(IDC_STATIC_BOXTOP1))->GetClientRect(&rectTempB);

	  (*boxRight).AddHBox(0, rectTempB.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	Win32Layout::Box* boxRightTop = (*boxRight).AddHBox(0, rectShowSO2.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->GetClientRect(&rectTempB); //布局box 间隔

	float fTemp = (rectTempB.right - 4 * rectShowSO2.right) / 5.0;

	Win32Layout::Box* testBox = (*boxRightTop).AddVBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	testBox->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_SO2), 0, 0, Win32Layout::FILL, Win32Layout::FILL);


	(*boxRightTop).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_SO2), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_NO), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CO), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_NO2), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);


	((CButton*)GetDlgItem(IDC_STATIC_SO2_T))->GetClientRect(&rectShowSO2);
	
	(*boxRight).AddHBox(0, rectShowSO2.bottom, Win32Layout::FILL, Win32Layout::PIXEL); //间隔box

	Win32Layout::Box* boxRightTop2 = (*boxRight).AddHBox(0, rectShowSO2.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	
	(*boxRightTop2).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);  //例如:二氧化硫的 字
	boxRightTop2->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_SO2_T), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop2).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop2->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_NO_T), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop2).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop2->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CO_T), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop2).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop2->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_NO2_T), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop2).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);



	((CButton*)GetDlgItem(IDC_STATIC_O2))->GetClientRect(&rectShowSO2);  //右边 box总大小 减去 控件的大小 剩下的距离 建
	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->GetClientRect(&rectTempB);  //间隔box

	fTemp = (rectTempB.right - 2 * rectShowSO2.right) / 3.0;
	((CButton*)GetDlgItem(IDC_STATIC_O2_T))->GetClientRect(&rectTempB);  //间隔box
	(*boxRight).AddHBox(0, rectTempB.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	Win32Layout::Box* boxRightTop3 = (*boxRight).AddHBox(0, rectShowSO2.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	(*boxRightTop3).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop3->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_O2), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop3).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop3->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CO2), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop3).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);


	((CButton*)GetDlgItem(IDC_STATIC_O2_T))->GetClientRect(&rectTempB);  //间隔box
	((CButton*)GetDlgItem(IDC_STATIC_SO2))->GetClientRect(&rectShowSO2);  
	(*boxRight).AddHBox(0, rectTempB.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	Win32Layout::Box* boxRightTop4 = (*boxRight).AddHBox(0, rectShowSO2.bottom, Win32Layout::FILL, Win32Layout::PIXEL);

	(*boxRightTop4).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop4->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_O2_T), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop4).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);
	boxRightTop4->AddWindow(::GetDlgItem(m_hWnd, IDC_STATIC_CO2_T), 0, 0, Win32Layout::FILL, Win32Layout::FILL);
	(*boxRightTop4).AddHBox(fTemp, 0, Win32Layout::PIXEL, Win32Layout::FILL);


}


BOOL CGLdetectionDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreCreateWindow(cs);
}


void CGLdetectionDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);
	

	lpMMI->ptMinTrackSize.x = 1366;
	lpMMI->ptMinTrackSize.y = 768;

	//lpMMI->ptMinTrackSize = point;
	//lpMMI->ptMaxTrackSize = point;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

/*界面初始化*/
/*
pDC =GetDC();
Graphics graphics( pDC->m_hDC);
Image image(L"walkman.png", FALSE);
graphics.DrawImage(&image, 10,10);
*/
void CGLdetectionDlg::InitFace(void)
{

#ifndef SHOW_MOUDLE
	return;
#endif
	static unsigned int uWidth = 208, uHeight = 55
		, uLeft = 100, uTop = 100, uBmpWidth = 90, uInterval = 10
		, uWidthNO2 = 218, uHeightNO2 = 92
		, uWidthO2 = 346, uHeightO2 = 60
		,uLeftText = 26,uTopText=22,uIntervalText = 14
		,uLogoHeight = 230,uLogoInterval=40
		,uButtonTop = 514,uButtonH = 46,uButtonW = 210
		,uSticSLeft = 238; //右边窗口 到左边的 距离
	CRect rectShow,rectTemp;

	((CButton*)GetDlgItem(IDC_STATIC_LEFT))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->ShowWindow(SW_HIDE);


	/*button 开始检测*/
	GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowPos(NULL
		, rectTemp.left + uLogoInterval, rectTemp.top + uButtonTop+ uTopText
		, uButtonW, uButtonH, SWP_SHOWWINDOW);

	/*logo*/
	((CButton*)GetDlgItem(IDC_STATIC_LEFT))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_LOG))->SetWindowPos(NULL, rectTemp.left+ uLogoInterval, rectTemp.top+ uLogoInterval, uSticSLeft, uLogoHeight, SWP_SHOWWINDOW);
#if 1 //6种气体显示的static控件 设置
	//so2
	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->GetWindowRect(rectShow);
	ScreenToClient(&rectShow);

	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->SetWindowPos(NULL, uSticSLeft
		, rectShow.top ,0, 0,  SWP_NOSIZE);

	((CButton*)GetDlgItem(IDC_STATIC_SHOW))->GetWindowRect(rectShow);
	ScreenToClient(&rectShow);



	::SetWindowPos(((CButton*)GetDlgItem(IDC_STATIC_SO2_T))->m_hWnd, HWND_TOP, rectShow.left + uLeft, rectShow.top + uTop, uWidth, uHeight, SWP_SHOWWINDOW );

	((CButton*)GetDlgItem(IDC_STATIC_SO2_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_SO2))->SetWindowPos(NULL, rectTemp.left, rectTemp.bottom, uWidth, uHeight, SWP_SHOWWINDOW);

	((CButton*)GetDlgItem(IDC_STATIC_SO2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_Bit_SO2))->SetWindowPos(NULL, rectTemp.right, rectShow.top + uTop, uBmpWidth, 2*uHeight, SWP_SHOWWINDOW);
	
	//布局
// 	CRect rectTest;
// 	((CButton*)GetDlgItem(IDC_STATIC_SO2_T))->GetWindowRect(rectTest);
// 	ScreenToClient(&rectTest);
// 	((CButton*)GetDlgItem(IDC_STATIC_SO2))->GetWindowRect(rectTest);
// 	ScreenToClient(&rectTest);
// 	((CButton*)GetDlgItem(IDC_STATIC_Bit_SO2))->GetWindowRect(rectTest);
// 	ScreenToClient(&rectTest);


	//NO
	((CButton*)GetDlgItem(IDC_STATIC_Bit_SO2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);


	((CButton*)GetDlgItem(IDC_STATIC_NO_T))->SetWindowPos(NULL, rectTemp.right + uInterval, rectShow.top + uTop, uWidth, uHeight, SWP_SHOWWINDOW );

	((CButton*)GetDlgItem(IDC_STATIC_NO_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_NO))->SetWindowPos(NULL, rectTemp.left, rectTemp.bottom, uWidth, uHeight, SWP_SHOWWINDOW);

	((CButton*)GetDlgItem(IDC_STATIC_NO))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_Bit_NO))->SetWindowPos(NULL, rectTemp.right, rectShow.top + uTop, uBmpWidth, 2*uHeight, SWP_SHOWWINDOW);

	//CO
	((CButton*)GetDlgItem(IDC_STATIC_Bit_NO))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);

	((CButton*)GetDlgItem(IDC_STATIC_CO_T))->SetWindowPos(NULL, rectTemp.right + uInterval, rectShow.top + uTop, uWidth, uHeight, SWP_SHOWWINDOW );

	((CButton*)GetDlgItem(IDC_STATIC_CO_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_CO))->SetWindowPos(NULL, rectTemp.left, rectTemp.bottom, uWidth, uHeight, SWP_SHOWWINDOW);

	((CButton*)GetDlgItem(IDC_STATIC_CO))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_Bit_CO))->SetWindowPos(NULL, rectTemp.right, rectShow.top + uTop, uBmpWidth, 2*uHeight, SWP_SHOWWINDOW);

	//NO2
	((CButton*)GetDlgItem(IDC_STATIC_SO2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);

	((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->SetWindowPos(NULL, rectTemp.left, rectTemp.bottom + uInterval, uWidthNO2, uHeightNO2, SWP_SHOWWINDOW);
	((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	
	((CButton*)GetDlgItem(IDC_STATIC_NO2))->SetWindowPos(NULL, rectTemp.left , rectTemp.bottom , uWidthNO2-uBmpWidth, uHeightNO2, SWP_SHOWWINDOW);
	((CButton*)GetDlgItem(IDC_STATIC_NO2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_Bit_NO2))->SetWindowPos(NULL, rectTemp.right, rectTemp.top, uBmpWidth, uHeightNO2, SWP_SHOWWINDOW);

	//CO2
	((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);

	((CButton*)GetDlgItem(IDC_STATIC_CO2_T))->SetWindowPos(NULL, rectTemp.right+uInterval, rectTemp.top, uWidthNO2, uHeightNO2, SWP_SHOWWINDOW );
	((CButton*)GetDlgItem(IDC_STATIC_CO2_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);

	((CButton*)GetDlgItem(IDC_STATIC_CO2))->SetWindowPos(NULL, rectTemp.left, rectTemp.bottom, uWidthNO2-uBmpWidth, uHeightNO2, SWP_SHOWWINDOW);

	((CButton*)GetDlgItem(IDC_STATIC_CO2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_Bit_CO2))->SetWindowPos(NULL, rectTemp.right, rectTemp.top, uBmpWidth, uHeightNO2, SWP_SHOWWINDOW);

	//O2
	((CButton*)GetDlgItem(IDC_STATIC_NO2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);

	((CButton*)GetDlgItem(IDC_STATIC_Bit_O2))->SetWindowPos(NULL, rectTemp.left
		, rectTemp.bottom+uInterval, uBmpWidth+uInterval, 2*uHeightO2, SWP_SHOWWINDOW);
	((CButton*)GetDlgItem(IDC_STATIC_Bit_O2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);

	((CButton*)GetDlgItem(IDC_STATIC_O2_T))->SetWindowPos(NULL, rectTemp.right , rectTemp.top
		, uWidthO2, uHeightO2, SWP_SHOWWINDOW );
	((CButton*)GetDlgItem(IDC_STATIC_O2_T))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
	((CButton*)GetDlgItem(IDC_STATIC_O2))->SetWindowPos(NULL, rectTemp.left, rectTemp.bottom 
		, uWidthO2, uHeightO2, SWP_SHOWWINDOW);
	((CButton*)GetDlgItem(IDC_STATIC_O2))->GetWindowRect(rectTemp);
	ScreenToClient(&rectTemp);
#endif

	GetDlgItem(IDC_STATIC_Bit_SO2)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_STATIC_Bit_NO)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_STATIC_Bit_CO)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_STATIC_Bit_NO2)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_STATIC_Bit_O2)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_STATIC_Bit_CO2)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_STATIC_LOG)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	//GetDlgItem(IDC_BUTTON_START)->ModifyStyle(SS_TYPEMASK, SS_OWNERDRAW);
	GetDlgItem(IDC_BUTTON_START)->ModifyStyle(0, BS_OWNERDRAW, 0);


	/*修改字体*/
	m_fontText.CreatePointFont(180, _T("微软雅黑"));
	m_fontShow.CreatePointFont(100, _T("微软雅黑"));
	
	for (int index = 0; controlID[index] != -1 && controlIDT[index] != -1; ++index)
	{
		GetDlgItem(controlID[index])->SetFont(&m_fontText);
		GetDlgItem(controlIDT[index])->SetFont(&m_fontText);
	}

	for (int index = 0; staticID[index] != -1 ;++index)
	{
		GetDlgItem(staticID[index])->SetFont(&m_fontShow);
		if (staticIDT[index] != -1)
		{
			GetDlgItem(staticIDT[index])->SetFont(&m_fontShow);
		}
	}
}


BOOL CGLdetectionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CGLdetectionDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
// 	CRect rect;
// 	GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// 	lpncsp->rgrc[0].top += 60 - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYSMCAPTION);
// 	lpncsp->rgrc[0].left += rect.left - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME);
// 	lpncsp->rgrc[0].right -= rect.right - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME);
// 	lpncsp->rgrc[0].bottom -= rect.bottom - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYSIZEFRAME);
	CDialogEx::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CGLdetectionDlg::InitFaceData(void)
{
	/*初始化 数据包, 0索引 表示指令 1索引表示 返回的数据*/
	m_strAryJTData.RemoveAll();
	m_strAryJTData.SetSize(2);

	/*初始化窗口 控件*/
	//1.初始化COM口的COMBOX控件
	m_strBTL = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_BTL"));
	m_strSJW = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_SJW"));
	m_strJYW = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_JYW"));
	m_strTZW = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_TZW"));
	int maxNum = CReadWriteConfig::ConfigGetInt(_T("COM"), _T("COM_MAX"));
	int curNum = CReadWriteConfig::ConfigGetInt(_T("COM"), _T("COM_CUR"));
	if (m_strBTL.IsEmpty())
	{
		m_strBTL = _T("9600");
	}
	if (m_strSJW.IsEmpty())
	{
		m_strSJW = _T("8");
	}
	if (m_strJYW.IsEmpty())
	{
		m_strJYW = _T("N");
	}
	if (m_strTZW.IsEmpty())
	{
		m_strTZW = _T("1");
	}
	if (maxNum <= 0)
	{
		maxNum = 10;
	}
	if (curNum <= 0)
	{
		curNum = 1;
	}
	CString strTemp;
	for (int index = 1; index <= maxNum; ++index)
	{
		strTemp.Format(_T("COM%d"), index);
		m_combox_com.AddString(strTemp);
	}
	m_combox_com.SetCurSel(curNum - 1);

	UpdateData(FALSE);
}

void CGLdetectionDlg::ReSize()
{
	float fsp[2];
	POINT Newp; //获取现在对话框的大小  
	CRect recta;
	GetClientRect(&recta);     //取客户区大小    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //左上角  
	CPoint OldBRPoint, BRPoint; //右下角  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //列出所有控件    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//取得ID  
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}

LRESULT CGLdetectionDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::WindowProc(message, wParam, lParam);
}
