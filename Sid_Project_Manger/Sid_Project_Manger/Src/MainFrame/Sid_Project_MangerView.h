
// Sid_Project_MangerView.h : CSid_Project_MangerView ��Ľӿ�
//

#pragma once

#define  STR_FIL_LOADINI             _T(".\\sid_prj_sysboot.ini")
//��������
#define  WELCOME_TITLE_FONTSIZE       250  // 200/10 = 20 ��
#define  LOAD_INFO_FONTSIZE           120  // 150/10 = 15 ��
#define  LINE_SPACE_HSIZE             30   
//�ؼ�Id
#define IDC_ED_WORKNUM	    	3001	//�༭��/����	 
#define IDC_ED_PWD	    	    3002	//�����/����	 
#define IDC_BTN_LOADIN	    	3004	//
#define IDC_CBOX_SAVELOADINF	3005	//



class CSid_Project_MangerView : public CView
{
protected: // �������л�����
	CSid_Project_MangerView();
	DECLARE_DYNCREATE(CSid_Project_MangerView)

// ����
public:
	CSid_Project_MangerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	//virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
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

// ���ɵ���Ϣӳ�亯��
protected:
	//afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	//�ؼ�
	CEdit	       m_edCtlLoginName;      //�༭�ؼ�����
	CEdit          m_edCtlPwd;          //�༭�ؼ�����
	CButton        m_btnLoadIn;         //һ����¼��ť,�����趨����,ִ�е�¼����
	CButton        m_chBoxSaveLoadInf;  //��ס��¼
	//CIPAddressCtrl m_ipAddrCtlDataBase; //SID-PLAD���ݿ�ϵͳIP��ַ
	//��������
	CFont	  m_fontTitle;         //������������
	int       m_nTitlePointSize;   //��ӭ���ֵ����ִ�С
	int		  m_nInfoPointSize;    //���ŵ���Ϣ�����ִ�С
	int       m_nLineSpace;        //�о� 
	int       m_nInfoXPos;          //��½�༭��x����
	int       m_nInfoYPos;         //��½��Ϣ��ʼ��y������
	CFont     m_fontInfo;          //����������������
	//���ؼ������ĵ�½��Ϣ
	BOOL           m_bSaveLoadInf;      //�Ƿ��ס��¼
	CString        m_edStrLoginName;      //�༭���µĹ��� 
	CString        m_edStrPwd;          //�༭���µ����룬�������ܵ�
	CString        m_readStrLog;        //����־��ȡ������
	bool           m_isMd5Pwd;          //�Ƿ�Ϊ�������롣Ĭ��flase���ڶ���־�г��������룺true   
	
	// ����λ��
	CPoint         m_ptWelcomTitlePos;          //��ӭ...����ʼ��
	COLORREF       m_bkColor;
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // Sid_Project_MangerView.cpp �еĵ��԰汾
inline CSid_Project_MangerDoc* CSid_Project_MangerView::GetDocument() const
   { return reinterpret_cast<CSid_Project_MangerDoc*>(m_pDocument); }
#endif

