#include "stdafx.h"
#include "RolePowerView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/BackMangerDlg/SetRolePower.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CEventView


IMPLEMENT_DYNCREATE(CRolePowerView, CBaseFormView)

CRolePowerView::CRolePowerView()
	: CBaseFormView(CRolePowerView::IDD)
{

}

CRolePowerView::~CRolePowerView()
{
}

void CRolePowerView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRolePowerView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//ON_BN_CLICKED(IDC_BTN_FRESH, &CRolePowerView::OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_ROLE_POWER, OnGridEndEidt )
	ON_NOTIFY( NM_RCLICK, ID_GRID_ROLE_POWER, OnGridRClick )

	ON_COMMAND( IDM_BACK_EDIT, OnEditChangePower)
	ON_COMMAND( IDM_DELTE_OBJ, OnDelete ) 

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEW , OnBnClickedBtnNew)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CRolePowerView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CRolePowerView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CRolePowerView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	m_taskGrid.GetFocus();
}

int CRolePowerView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );

	if (!m_taskGrid.Create(rect,this,ID_GRID_ROLE_POWER ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );

	m_gridManger.reset( new CRolePowerGridManger( pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}
int CRolePowerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
void CRolePowerView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
}

////网格文本框编辑结束
void CRolePowerView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CRolePowerView::OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}

void CRolePowerView::OnDelete( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnDeleteRole();
	}
}

void CRolePowerView::OnEditChangePower( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnChangePowers();
	}
}

void CRolePowerView::OnBnClickedBtnNew()
{
	CDlgSetRolePower dlg(  NULL, TRUE);
	if ( IDOK == dlg.DoModal()  )
	{
		if ( m_gridManger.get() )
		{
			m_gridManger->UpdateGrid();
		}
	}

}



