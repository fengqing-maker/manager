#include "stdafx.h"
#include "CProductLineView.h"
#include "../../MainFrm.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/BackMangerDlg/DlgNewProductLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CProductLineView, CBaseFormView)

CProductLineView::CProductLineView()
	: CBaseFormView(CProductLineView::IDD)
{

}

CProductLineView::~CProductLineView()
{
}

void CProductLineView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)	
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CProductLineView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	//ON_BN_CLICKED(IDC_BTN_FRESH, &CProductLineView::OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_PRODCT_LINE, OnGridEndEidt )
	ON_NOTIFY( NM_RCLICK, ID_GRID_PRODCT_LINE, OnGridRClick )

	ON_COMMAND( IDM_BACK_EDIT, OnEditOwner)
	ON_COMMAND( IDM_DELTE_OBJ, OnDeleteProductLine ) 

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_NEW , OnBnClickedBtnNew)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CProductLineView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CProductLineView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CProductLineView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	m_taskGrid.GetFocus();
}

int CProductLineView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );

	if (!m_taskGrid.Create(rect,this,ID_GRID_PRODCT_LINE ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );

	m_gridManger.reset( new CProductLineGridManger( pActionFactory ));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}
int CProductLineView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
void CProductLineView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
}

////网格文本框编辑结束
void CProductLineView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CProductLineView::OnGridRClick( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}

void CProductLineView::OnDeleteProductLine( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnDeleteProductLine();
	}
}

void CProductLineView::OnEditOwner( void )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnChangeOwner();
	}
}

void CProductLineView::OnBnClickedBtnNew()
{
	CDlgNewProductLine dlg;
	if ( IDOK == dlg.DoModal()  )
	{
		if ( m_gridManger.get() )
		{
			m_gridManger->UpdateGrid();
		}
	}

}



