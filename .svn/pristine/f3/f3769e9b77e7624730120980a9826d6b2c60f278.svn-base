XAccessible
#include "stdafx.h"
#include "CDepartView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/DlgNewDepart.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CDepartView, CBaseFormView)

CDepartView::CDepartView()
	: CBaseFormView(CDepartView::IDD)
{
	 
}

CDepartView::~CDepartView()
{
}

void CDepartView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDepartView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()

	ON_BN_CLICKED(IDC_BTN_FRESH, &CDepartView::OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_PROJECT, OnGridEndEidt )
	ON_NOTIFY( NM_RCLICK, ID_GRID_PROJECT, OnGridRClick )
	ON_NOTIFY( NM_CLICK, ID_GRID_PROJECT, OnGridClick )

	ON_COMMAND( IDM_DEPART_MODIY_MANGER, OnChangeOwner)
	ON_COMMAND( IDM_DEPART_MODIY_BAKMNAGER, OnChangeBakOwner)
	ON_COMMAND( IDM_DEL_DEPART, OnDeleteDepart ) 
	//ON_NOTIFY(GVN_SELCHANGED,	ID_GRID_PUBLISH,	OnGridSelChanged)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEW_DEPART, &CDepartView::OnBnClickedBtnNewDepart)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CDepartView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CDepartView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CDepartView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		OnInitCtrl();
	}
	m_taskGrid.GetFocus();
}
void CDepartView::OnInitCtrl()
{
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	if ( pActionFactory->GetLoginServer()->FindLoginPower( backgroundManger ) )
	{
		GetDlgItem( IDC_BTN_NEW_DEPART )->ShowWindow( SW_SHOW );

	}else
	{
		GetDlgItem( IDC_BTN_NEW_DEPART )->ShowWindow( SW_HIDE );
	}
}
int  CDepartView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
int CDepartView::CreateCtrl()
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
	ASSERT( NULL != pActionFactory );
	m_gridManger.reset( new CDepartViewManger( pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}

void CDepartView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
}





void CDepartView::OnBnClickedBtnFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gridManger->RefreshDepartment();
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
void CDepartView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CDepartView::OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}
void CDepartView::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridClick( pNMHDR, pResult );
	}
}
void CDepartView::OnGridSelChanged( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		
	}
}
void CDepartView::OnChangeOwner( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnChangeOwner();
	}
}
void CDepartView::OnChangeBakOwner( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnChangeDepartBakManger(  );
	}
}
void CDepartView::OnDeleteDepart( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnDeleteDepart();
	}
}
void CDepartView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseFormView::OnPaint()
	CBaseFormView::OnPaint();
}


void CDepartView::OnBnClickedBtnNewDepart()
{
	CDlgNewDepart dlg;
	if ( IDOK == dlg.DoModal() )
	{
		if ( m_gridManger.get() ) 
		{
			m_gridManger->RefreshDepartment();
		}
	}

}
