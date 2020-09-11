
// Sid_Project_MangerView.h : CSid_Project_MangerView 类的接口
//

#pragma once

#define  STR_FIL_LOADINI             _T(".\\sid_prj_sysboot.ini")
//字体与间隔
#define  WELCOME_TITLE_FONTSIZE       250  // 200/10 = 20 磅
#define  LOAD_INFO_FONTSIZE           120  // 150/10 = 15 磅
#define  LINE_SPACE_HSIZE             30   
//控件Id
#define IDC_ED_WORKNUM	    	3001	//编辑框/工号	 
#define IDC_ED_PWD	    	    3002	//密码框/工号	 
#define IDC_BTN_LOADIN	    	3004	//
#define IDC_CBOX_SAVELOADINF	3005	//



class CSid_Project_MangerView : public CView
{
protected: // 仅从序列化创建
	CSid_Project_MangerView();
	DECLARE_DYNCREATE(CSid_Project_MangerView)

// 特性
public:
	CSid_Project_MangerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
	afx_msg void OnLoadInSystem(void);
	afx_msg void OnSaveLoadInf(void);
	void         OnInitLoginFromFile();
	void         DrawTitleString();
	void         OnInitControlObj();

public:
	virtual ~CSid_Project_MangerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	//afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	//控件
	CEdit	       m_edCtlLoginName;      //编辑控件工号
	CEdit          m_edCtlPwd;          //编辑控件密码
	CButton        m_btnLoadIn;         //一个登录按钮,代表设定结束,执行登录流程
	CButton        m_chBoxSaveLoadInf;  //记住登录
	//CIPAddressCtrl m_ipAddrCtlDataBase; //SID-PLAD数据库系统IP地址
	//字体设置
	CFont	  m_fontTitle;         //标题文字字体
	int       m_nTitlePointSize;   //欢迎文字的文字大小
	int		  m_nInfoPointSize;    //工号等信息的文字大小
	int       m_nLineSpace;        //行距 
	int       m_nInfoXPos;          //登陆编辑框x像素
	int       m_nInfoYPos;         //登陆信息框开始的y轴像素
	CFont     m_fontInfo;          //除标题外其他字体
	//跟控件关联的登陆信息
	BOOL           m_bSaveLoadInf;      //是否记住登录
	CString        m_edStrLoginName;      //编辑框下的工号 
	CString        m_edStrPwd;          //编辑框下的密码，经过加密的
	CString        m_readStrLog;        //从日志读取的密码
	bool           m_isMd5Pwd;          //是否为加密密码。默认flase，在读日志中出来的密码：true   
	
	// 绘制位置
	CPoint         m_ptWelcomTitlePos;          //欢迎...的起始点
	COLORREF       m_bkColor;
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // Sid_Project_MangerView.cpp 中的调试版本
inline CSid_Project_MangerDoc* CSid_Project_MangerView::GetDocument() const
   { return reinterpret_cast<CSid_Project_MangerDoc*>(m_pDocument); }
#endif

