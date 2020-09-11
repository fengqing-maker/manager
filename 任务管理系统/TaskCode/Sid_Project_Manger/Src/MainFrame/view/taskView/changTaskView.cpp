#include "stdafx.h"
#include "changTaskView.h"
#include "../../MainFrm.h"
#include "../../messageDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CChangeTaskView, CBaseFormView)

CChangeTaskView::CChangeTaskView()
	: CBaseFormView(CChangeTaskView::IDD)
{

}

CChangeTaskView::~CChangeTaskView()
{
}

void CChangeTaskView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND, m_searchTask);
}

BEGIN_MESSAGE_MAP(CChangeTaskView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_CREATE()

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_CHANGE, OnGridEndEidt )
	ON_NOTIFY( GVN_SELCHANGED, ID_GRID_CHANGE, OnGridSelChange )
	ON_NOTIFY(NM_CLICK,	ID_GRID_CHANGE, OnClickGrid)
	
	ON_CBN_SELCHANGE(IDC_COMBO_FIND, &CChangeTaskView::OnCbnSelchangeComboFind)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CChangeTaskView::OnBnClickedBtnFresh)
	//
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CChangeTaskView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CChangeTaskView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CChangeTaskView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		OnInitCtrl();
	}
}
int CChangeTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
int CChangeTaskView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );

	CRect gridRect( 0, 0, rect.Width(), rect.Height() );
	if (!m_taskGrid.Create(rect,this,ID_GRID_CHANGE ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	m_gridManger.reset( new CChangeReviewManger(pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}

	return 0;
}
void CChangeTaskView::OnInitCtrl()
{
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	size_t ownCount = pActionFactory->GetTaskSever()->GetOwneChangeCount();
	int selIndex = 0;
	//if ( pActionFactory->GetTaskSever()->GetReviewChange().size() > ownCount )
	//{
		selIndex = 1;
	/*}*/
	m_searchTask.InsertString( 0, _T("所有变更") );
	m_searchTask.InsertString( 1, _T("所有待审核变更") );
	m_searchTask.InsertString( 2, _T("所有部门任务变更") );
	m_searchTask.InsertString( 3, _T("所有项目任务变更") );
	m_searchTask.InsertString( 4, _T("所有我申请的变更") );

	m_searchTask.SetCurSel( selIndex );
	//if ( selIndex != 1 )
	//{//
	//	OnCbnSelchangeComboFind();
	//}
}
//网格选择改变变化
void CChangeTaskView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//网格文本框编辑结束
void CChangeTaskView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CChangeTaskView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid );
}

void CChangeTaskView::OnCbnSelchangeComboFind()
{
	int index = m_searchTask.GetCurSel();
	switch( index )
	{
	case 0:
		m_gridManger->UpdateShowAllChange();
		break;
	case 1:
		m_gridManger->UpdateShowAllReviewChange();//需要评审的所有变更
		break;
	case 2:
		m_gridManger->UpdateShowDepartChange();
		break;
	case 3:
		m_gridManger->UpdateShowProjectChange();
		break;
	case 4:
		m_gridManger->UpdateOwnerChange();//所有我申请的变更
	}
}


void CChangeTaskView::OnBnClickedBtnFresh()
{
	m_gridManger->UpdateDbData();
	OnCbnSelchangeComboFind();
}
void CChangeTaskView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}

LRESULT CChangeTaskView::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_searchTask.SetCurSel( 1 );
		m_gridManger->UpdateShowAllReviewChange();
		m_gridManger->SelectChangeLine( lp );
	}
	return 0;
}
LRESULT CChangeTaskView::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_searchTask.SetCurSel( 1 );
		m_gridManger->UpdateShowAllReviewChange();
	}
	return 0;
}
LRESULT CChangeTaskView::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateChangeContent( lp );
	}
	return 0;
}
LRESULT CChangeTaskView::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->SelectChangeLine( lp );
	}
	return 0;
}