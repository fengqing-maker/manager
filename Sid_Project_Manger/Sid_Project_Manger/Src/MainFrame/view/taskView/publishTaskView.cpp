#include "stdafx.h"
#include "publishTaskView.h"
#include "../../MainFrm.h"
#include "../../messageDef.h"
#include "Src/action/factory/actionServerFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CPublishTaskView, CBaseFormView)

CPublishTaskView::CPublishTaskView()
	: CBaseFormView(CPublishTaskView::IDD)
{

}

CPublishTaskView::~CPublishTaskView()
{
}

void CPublishTaskView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND, m_searchTask);
}

BEGIN_MESSAGE_MAP(CPublishTaskView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()

	ON_CBN_SELCHANGE(IDC_COMBO_FIND, &CPublishTaskView::OnCbnSelchangeComboFind)
	ON_BN_CLICKED(IDC_BTN_FRESH, &CPublishTaskView::OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_PUBLISH, OnGridEndEidt )
	ON_NOTIFY( GVN_SELCHANGED, ID_GRID_PUBLISH, OnGridSelChange )
	ON_NOTIFY(NM_CLICK,	ID_GRID_PUBLISH,	OnClickGrid)
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CPublishTaskView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CPublishTaskView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CPublishTaskView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		m_searchTask.InsertString( 0,_T("所有待审核任务") );
		m_searchTask.InsertString( 1, _T("部门任务") );
		m_searchTask.InsertString( 2, _T("项目任务") );
		m_searchTask.SetCurSel( 0 );
	}
	m_taskGrid.GetFocus();
}
int CPublishTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}

int CPublishTaskView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );
	if (!m_taskGrid.Create(gridRect,this,ID_GRID_PUBLISH ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
    CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	m_gridManger.reset( new CPublishTaskManger(pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}

void CPublishTaskView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid);
}


void CPublishTaskView::OnCbnSelchangeComboFind()
{
	// TODO: 在此添加控件通知处理程序代码
	int culIndex = m_searchTask.GetCurSel();
	switch(culIndex)
	{
	case 0:
		m_gridManger->UpdateAllTask();
		break;
	case 1:
		m_gridManger->UpdateDepartTask();
		break;
	case 2:
		m_gridManger->UpdateProjectTask();
		break;
	}
    m_taskGrid.Refresh();
}


void CPublishTaskView::OnBnClickedBtnFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_gridManger->UpdateDbData();
}
//网格选择改变变化
void CPublishTaskView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//网格文本框编辑结束
void CPublishTaskView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CPublishTaskView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}
LRESULT CPublishTaskView::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateDbData();
		m_gridManger->SelectTaskLine( lp );
	}
	return 0;
}
LRESULT CPublishTaskView::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateDbData();
	}
	return 0;
}
LRESULT CPublishTaskView::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateTaskContent( lp );
	}
	return 0;
}
LRESULT CPublishTaskView::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->SelectTaskLine( lp );
	}
	return 0;
}