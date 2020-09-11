#include "stdafx.h"
#include "scoreTaskView.h"
#include "../../MainFrm.h"
#include "../../messageDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView
IMPLEMENT_DYNCREATE(CScoreTaskView, CBaseFormView)

CScoreTaskView::CScoreTaskView()
	: CBaseFormView(CScoreTaskView::IDD)
{

}

CScoreTaskView::~CScoreTaskView()
{
}

void CScoreTaskView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	DDX_Control(pDX, IDC_COMBO_FIND , m_searchTask);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScoreTaskView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_FIND, &OnCbnSelchangeComboFind)
	ON_BN_CLICKED(IDC_BTN_FRESH, &OnBnClickedBtnFresh)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_SCORE, OnGridEndEidt )
	ON_NOTIFY( GVN_SELCHANGED, ID_GRID_SCORE, OnGridSelChange )
	ON_NOTIFY(NM_CLICK,	ID_GRID_SCORE, OnClickGrid)
	//
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CScoreTaskView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CScoreTaskView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CScoreTaskView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		m_searchTask.InsertString( 0, _T("所有待评分任务") );
		m_searchTask.InsertString( 1, _T("待评分部门任务") );
		m_searchTask.InsertString( 2, _T("待评分项目任务") );
		m_searchTask.SetCurSel( 0 );
	}
	m_taskGrid.GetFocus();
}
int CScoreTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
int CScoreTaskView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );

	CRect gridRect( 0, 0, rect.Width(), rect.Height() );
	if (!m_taskGrid.Create(rect,this,ID_GRID_SCORE ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	m_gridManger.reset( new CScoreTaskManger(pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}
void CScoreTaskView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid );
}
//网格选择改变变化
void CScoreTaskView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//网格文本框编辑结束
void CScoreTaskView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CScoreTaskView::OnCbnSelchangeComboFind()
{
	int index = m_searchTask.GetCurSel();
    switch( index )
	{
	case 0:
		m_gridManger->UpdateShowAllTask();
		break;
	case 1:
		m_gridManger->UpdateShowDepartTask();
		break;
	case 2:
		m_gridManger->UpdateShowProjectTask();
		break;
	}
}
void CScoreTaskView::OnBnClickedBtnFresh()
{
	m_gridManger->RefreshTaskList();
	OnCbnSelchangeComboFind();
}
void CScoreTaskView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}
LRESULT CScoreTaskView::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_searchTask.SetCurSel( 0 );
		m_gridManger->UpdateDb();
		m_gridManger->UpdateShowAllTask();
		m_gridManger->SelectTaskLine( lp );
	}
	return 0;
}
LRESULT CScoreTaskView::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_searchTask.SetCurSel( 0 );
		m_gridManger->UpdateDb();
		m_gridManger->UpdateShowAllTask();
	}
	return 0;
}
LRESULT CScoreTaskView::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateTaskContent( lp );
	}
	return 0;
}
LRESULT CScoreTaskView::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->SelectTaskLine( lp );
	}
	return 0;
}
