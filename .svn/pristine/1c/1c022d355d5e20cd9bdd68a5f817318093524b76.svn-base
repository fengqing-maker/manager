#include "stdafx.h"
#include "CProjectView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/NewProjectDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CProjectView, CBaseFormView)

CProjectView::CProjectView()
	: CBaseFormView(CProjectView::IDD)
{
	 
}

CProjectView::~CProjectView()
{
}

void CProjectView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND, m_searchTask);
}

BEGIN_MESSAGE_MAP(CProjectView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_FIND, &CProjectView::OnCbnSelchangeComboFind)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CProjectView::OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_PROJECT, OnGridEndEidt )
	ON_NOTIFY( NM_RCLICK, ID_GRID_PROJECT, OnGridRClick )
	ON_NOTIFY( NM_CLICK, ID_GRID_PROJECT, OnGridClick )

	ON_COMMAND( IDM_PROJECT_MODIY_OWENR, OnChangeOwner)
	ON_COMMAND( IDM_PROJECT_MODIY_MEMBER, OnChangePrjOwner)
	ON_COMMAND( IDM_PROJECT_DEL_PROJECT, OnDeleteProject )
	//ON_NOTIFY(GVN_SELCHANGED,	ID_GRID_PUBLISH,	OnGridSelChanged)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEW_PROJECT, &CProjectView::OnBnClickedBtnNewProject)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CProjectView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CProjectView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CProjectView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		//CreatCtrl();
		InitPrjLine();
		m_taskGrid.GetFocus();
	}
}
void CProjectView::InitPrjLine()
{
	int lastSel = m_searchTask.GetCurSel();
	m_searchTask.ResetContent();
	m_searchTask.InsertString( 0,_T("所有产品线") );
	ASSERT( m_gridManger.get() );
	const ListProjectLine& list = m_gridManger->GetAllProjectLine();
	ListProjectLine::const_iterator itor = list.begin();
	int count = 1;
	while( itor != list.end() )
	{
		m_searchTask.InsertString( count++, itor->GetName() );
		itor++;
	}
	if ( lastSel>= 0 && lastSel < count )
	{
		m_searchTask.SetCurSel( lastSel );
	}else
	{
		m_searchTask.SetCurSel( 0 );
	}

	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	if ( pActionFactory->GetLoginServer()->FindLoginPower( backgroundManger ) )
	{
		GetDlgItem( IDC_BTN_NEW_PROJECT )->ShowWindow( SW_SHOW );

	}else
	{
		GetDlgItem( IDC_BTN_NEW_PROJECT )->ShowWindow( SW_HIDE );
	}
}
int CProjectView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );

	if (!m_taskGrid.Create(rect,this,ID_GRID_PROJECT ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	m_gridManger.reset( new CProjectViewManger( pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}
int CProjectView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
void CProjectView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
}


void CProjectView::OnCbnSelchangeComboFind()
{
	int selIndex = m_searchTask.GetCurSel();
	if ( m_gridManger.get() && selIndex >= 0 )
	{
		m_gridManger->ChangeSelProjectLine( selIndex );
	}
}


void CProjectView::OnBnClickedBtnFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gridManger->RefreshProjectDataFromDb();
	InitPrjLine();
    RedrawWindow();
}
//网格选择改变变化
//void CPublishTaskView::OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult )
//{
//	if ( m_gridManger.get() )
//	{
//		m_gridManger->OnGridSelChanged( pNMHDR, pResult );
//	}
//}
////网格文本框编辑结束
void CProjectView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CProjectView::OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}
void CProjectView::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridClick( pNMHDR, pResult );
	}
}
void CProjectView::OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult )
{
}
void CProjectView::OnChangeOwner( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnChangeOwner();
	}
}
void CProjectView::OnChangePrjOwner( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnChangePrjMember(  );
	}
}
void CProjectView::OnDeleteProject( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnDeleteProject();
	}
}
void CProjectView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseFormView::OnPaint()
	CBaseFormView::OnPaint();
}


void CProjectView::OnBnClickedBtnNewProject()
{
	NewProjectDlg dlg;
	if ( IDOK == dlg.DoModal() )
	{
		if ( m_gridManger.get() ) 
		{
			m_gridManger->RefreshProject();
		}
	}
	
}
