// DlgTaskPictrueView.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgTaskPictrueView.h"
#include "afxdialogex.h"


// CDlgTaskPictrueView 对话框

IMPLEMENT_DYNAMIC(CDlgTaskPictrueView, CDialogEx)

CDlgTaskPictrueView::CDlgTaskPictrueView( ListTask *pTaskList, const CTaskChange &chObj, const CTaskObj &chAfterObj,CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgTaskPictrueView::IDD, pParent)
{
	 m_pListTask = pTaskList;
	 m_chObj = chObj;
	 m_changeAfterTask = chAfterObj;

	 m_pctrlView = NULL;
	 m_pFrame = NULL;
}

CDlgTaskPictrueView::~CDlgTaskPictrueView()
{
}

void CDlgTaskPictrueView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTaskPictrueView, CDialogEx)
		ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgTaskPictrueView 消息处理程序
BOOL CDlgTaskPictrueView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//111111111111111
     //创建
	 //方法一 
	//BOOL bSuccess = m_ctrlView.CreateFormCtrl(IDC_STATIC_PICTRUE, this);
	//m_ctrlView.m_nMapMode = MM_TEXT;
	//m_ctrlView.OnInitialUpdate();

	/* 方法二
	m_ctrlView = (CProjectScheduleView *)RUNTIME_CLASS( CProjectScheduleView )->CreateObject();
	if ( NULL == m_ctrlView )
	{
		AfxMessageBox( _T("创建显示视图失败") );
		return FALSE;
	}
	*/

	CRect  rect;
	//方法二
	//m_ctrlView->Create( NULL, NULL, AFX_WS_DEFAULT_VIEW, rect, this, IDC_STATIC_PICTRUE );
	//m_ctrlView->OnInitialUpdate();
	
	//方法三
	GetClientRect( &rect );
	m_pFrame = new CFrameWnd();
	m_pFrame->Create( NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this );
	m_pctrlView = (CProjectScheduleView *)RUNTIME_CLASS( CProjectScheduleView )->CreateObject();
	rect.right -= 5;
	rect.bottom -= 5;
	m_pctrlView->Create( NULL, NULL, WS_VISIBLE | WS_CHILD, rect, m_pFrame, 153 );
	//m_pFrame->SetWindowPos( NULL, 0, 0, rect1.Width(), rect1.Height(), SWP_NOZORDER );
	//m_pctrlView->SetWindowPos( NULL, 0, 0, rect1.Width(), rect1.Height(), SWP_NOZORDER );
	m_pctrlView->SetTaskChangePoint( &m_chObj, m_changeAfterTask );
	m_pctrlView->SetTaskListPoint( m_pListTask );
	m_pctrlView->OnInitialUpdate();
	m_pctrlView->OnUpdate();
    //方法一
	//m_ctrlView.MoveWindow( rect/*rect1*/ );
	//m_ctrlView.SetTaskChangePoint( &m_chObj, m_changeAfterTask );
	//m_ctrlView.SetTaskListPoint( m_pListTask );
	//m_ctrlView.OnUpdate();
	return 0;
}
void CDlgTaskPictrueView::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize( nType, cx, cy );
	//if ( IsWindow( m_ctrlView.GetSafeHwnd() ) )
	//{
	//	//m_ctrlView.MoveWindow( 0, 0, cx, cy);
	//	m_ctrlView.SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER );
	//	m_ctrlView.OnUpdate();
	//}
	if ( m_pFrame || m_pctrlView )
	{
		m_pFrame->SetWindowPos( NULL, 0, 0, cx, cy, SWP_NOZORDER );
		m_pctrlView->SetWindowPos( NULL, 0, 0, cx - 5, cy - 5, SWP_NOZORDER );
	}
	/*if ( m_ctrlView && IsWindow( m_ctrlView->GetSafeHwnd() ))
	{
		m_ctrlView->MoveWindow( 0, 0, cx, cy);
	}*/
	
}