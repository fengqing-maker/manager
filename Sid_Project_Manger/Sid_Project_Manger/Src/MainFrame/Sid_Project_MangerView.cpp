
// Sid_Project_MangerView.cpp : CSid_Project_MangerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Sid_Project_Manger.h"
#endif

#include "Sid_Project_MangerDoc.h"
#include "Sid_Project_MangerView.h"
#include "MainFrm.h"
#include "../Tools/UtilityTool/UtilityTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CSid_Project_MangerView

IMPLEMENT_DYNCREATE(CSid_Project_MangerView, CView)

BEGIN_MESSAGE_MAP(CSid_Project_MangerView, CView)
	// ��׼��ӡ����
	/*ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSid_Project_MangerView::OnFilePrintPreview)*/
	ON_BN_CLICKED(IDC_BTN_LOADIN, OnLoadInSystem)
	ON_BN_CLICKED(IDC_CBOX_SAVELOADINF, OnSaveLoadInf)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CSid_Project_MangerView ����/����

CSid_Project_MangerView::CSid_Project_MangerView()
{
	m_nTitlePointSize = WELCOME_TITLE_FONTSIZE;
	m_nInfoPointSize  = LOAD_INFO_FONTSIZE;
	m_nLineSpace = LINE_SPACE_HSIZE;
	m_fontInfo.CreatePointFont(m_nInfoPointSize, _T("����"));
	m_fontTitle.CreatePointFont(m_nTitlePointSize, _T("����"));
	m_isMd5Pwd = false;
	//
	m_nInfoYPos = 0;
	m_nInfoXPos = 0;
	//
	m_bkColor = RGB( 221, 231, 245 );
}

CSid_Project_MangerView::~CSid_Project_MangerView()
{
}

BOOL CSid_Project_MangerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CSid_Project_MangerView ����

void CSid_Project_MangerView::OnDraw(CDC* /*pDC*/)
{
	CSid_Project_MangerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


void CSid_Project_MangerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSid_Project_MangerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSid_Project_MangerView ���

#ifdef _DEBUG
void CSid_Project_MangerView::AssertValid() const
{
	CView::AssertValid();
}

void CSid_Project_MangerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSid_Project_MangerDoc* CSid_Project_MangerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSid_Project_MangerDoc)));
	return (CSid_Project_MangerDoc*)m_pDocument;
}
#endif //_DEBUG


// CSid_Project_MangerView ��Ϣ�������


int CSid_Project_MangerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������
	GetParentFrame()->RecalcLayout();

	OnInitLoginFromFile();
	//���Ʊ���
	DrawTitleString();
	//��ʼ�����
	OnInitControlObj();

	return 0;
}
void CSid_Project_MangerView::OnInitLoginFromFile()
{
	CFileFind fileFinder;
	BOOL bFindResult=fileFinder.FindFile(STR_FIL_LOADINI);
	if( bFindResult )
	{//��ȡ����
		m_bSaveLoadInf= ::GetPrivateProfileInt(_T("LoadInf"), _T("bLoadSaveInf"),0,STR_FIL_LOADINI);
		//workID
		::GetPrivateProfileString(_T("worker"), _T("workerID"), _T(""), m_edStrLoginName.GetBuffer(MAX_PATH), MAX_PATH, STR_FIL_LOADINI);
		//workPwd
		::GetPrivateProfileString(_T("worker"), _T("workerPwd"),_T(""), m_edStrPwd.GetBuffer(250), 250, STR_FIL_LOADINI);
		m_readStrLog = m_edStrPwd;
		m_isMd5Pwd = true;
	}
}
//��ʼ���ؼ�
void CSid_Project_MangerView::OnInitControlObj()
{//
	//��ȡ����߶�
	//LOGFONT lf;
	//m_fontInfo.GetLogFont( &lf );
	CDC * pDC = GetDC();
	CFont *pFont = pDC->SelectObject(&m_fontTitle);
	CSize szTemp;
	szTemp = pDC->GetTextExtent( _T("A") );

	int hight =  szTemp.cy;
	int x = m_nInfoXPos ;
	int y = m_nInfoYPos ;
	int wide = szTemp.cx * 12;
	//
	CRect rect( x, y, x + wide , y + hight );
	DWORD edStyle= WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|WS_BORDER|ES_LEFT;
	m_edCtlLoginName.Create(edStyle, rect, this, IDC_ED_WORKNUM);
	m_edCtlLoginName.SetFont(&m_fontInfo);
	m_edCtlLoginName.SetWindowTextW( m_edStrLoginName );
	//
	y += hight + m_nLineSpace*2/3;
	rect.SetRect(x, y, x + wide, y + hight );
	edStyle = ES_PASSWORD|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP|WS_BORDER|ES_LEFT;
	m_edCtlPwd.Create( edStyle, rect, this, IDC_ED_PWD);
	m_edCtlPwd.SetFont( &m_fontInfo);
	m_edCtlPwd.SetWindowTextW(m_edStrPwd);
	//��ס��¼:
	y += hight + m_nLineSpace*2/3;
	CString str=_T("��ס");
	szTemp = pDC->GetTextExtent( str );
	int recordX = x + wide - szTemp.cx;
	CRect rctChBoxSavLoadInf( recordX, y, recordX + szTemp.cx , y + szTemp.cy);
	DWORD btnStyle = BS_AUTOCHECKBOX|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP; 
	m_chBoxSaveLoadInf.Create( str, btnStyle, rctChBoxSavLoadInf, this, IDC_CBOX_SAVELOADINF);
	m_chBoxSaveLoadInf.SetFont(&m_fontInfo);
	if(m_bSaveLoadInf)
	{
		m_chBoxSaveLoadInf.SetCheck(BST_CHECKED);
	}	
	//��¼��ť��ť
	int sizeOneStr = szTemp.cx/str.GetLength();
	str = _T("=>:��¼:<=");
	szTemp = pDC->GetTextExtent( str );
	recordX =  recordX - sizeOneStr - szTemp.cx;
	CRect rctBtnLoadIn( recordX, y, recordX + szTemp.cx, y + szTemp.cy );
	btnStyle = BS_DEFPUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_GROUP; //ENTER, �Զ���¼
	m_btnLoadIn.Create(str, btnStyle, rctBtnLoadIn, this, IDC_BTN_LOADIN);
	m_btnLoadIn.SetFont(&m_fontInfo);

	pDC->SelectObject( pFont );
}
//���Ƶ�½����
void CSid_Project_MangerView::DrawTitleString()
{
	CDC * pDC = GetDC();
	CRect rectClient;
	CSize szTemp;
	GetClientRect(&rectClient);
	pDC->SetBkColor( m_bkColor );
	//��ӭ����:
	int x = 0, y = 0;
	CString strTitle = _T("���ڹ��������������ϵͳ");
	pDC->SelectObject(&m_fontTitle);
	szTemp = pDC->GetTextExtent( strTitle );
	x = ( rectClient.right - szTemp.cx)/2;
	y = rectClient.top + rectClient.Height()/4;//200;
	m_ptWelcomTitlePos.x = x ;
	m_ptWelcomTitlePos.y = y;
	pDC->TextOutW( x, y, strTitle);
	//Ա�����
	pDC->SelectObject( &m_fontInfo );
	strTitle = _T("�û�����");
	x = m_ptWelcomTitlePos.x + 50;
	szTemp = pDC->GetTextExtent( strTitle );
	y = m_ptWelcomTitlePos.y + m_nTitlePointSize/10 + m_nLineSpace + 40;
	m_nInfoYPos = y - 20;
	pDC->TextOutW( x, y, strTitle );
	//��½����
	strTitle=_T("����: ");
	y += szTemp.cy + m_nLineSpace;
	pDC->TextOutW( x, y, strTitle );
	//
	m_nInfoXPos = x + szTemp.cx + 20;
}

void CSid_Project_MangerView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
    DrawTitleString();
}


BOOL CSid_Project_MangerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rc;
	GetClientRect( &rc );
	pDC->FillSolidRect( rc, m_bkColor );
	return TRUE/*CView::OnEraseBkgnd(pDC)*/;
}
void CSid_Project_MangerView::OnSaveLoadInf( void )
{
}
//��½��ť��Ӧ
void CSid_Project_MangerView::OnLoadInSystem(void)
{
	UpdateData( TRUE );

	m_edCtlLoginName.GetWindowText( m_edStrLoginName );
	m_edCtlPwd.GetWindowText( m_edStrPwd );
	
	if ( m_edStrPwd != m_readStrLog || false == m_isMd5Pwd )
	{//���м��ܴ���
		m_edStrPwd = CUtilityTool::GetObj()->GetMd5String( m_edStrPwd ); 
	}
    CMainFrame *pMainFrm = (CMainFrame *)GetParentFrame();
	CActionServerFactory *pFactory = pMainFrm->GetSeverFactory();
	BOOL ret = pFactory->GetLoginServer()->LoginSystem( m_edStrLoginName, m_edStrPwd );
	if ( userExcOk != ret  )
	{
	   if ( paraError != ret )
	   {
		   AfxMessageBox( pFactory->GetErrorString(ret) );
	   }
	}else
	{	
		//��¼�û���¼��Ϣ
		::WritePrivateProfileString(_T("LoadInf"), _T("bLoadSaveInf"),_T("1"), STR_FIL_LOADINI);
		//workID
		::WritePrivateProfileString(_T("worker"), _T("workerID"), m_edStrLoginName, STR_FIL_LOADINI);
		//workPwd
		::WritePrivateProfileString(_T("worker"), _T("workerPwd"),m_edStrPwd, STR_FIL_LOADINI);
		//��½�ɹ�����ҳ�����
		pMainFrm->LoginLoading();
	}

}