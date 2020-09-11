#include "stdafx.h"
#include "scheduleReviewView.h"
#include "../../MainFrm.h"
#include "../../messageDef.h"
#include "Src/action/factory/actionServerFactory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView
IMPLEMENT_DYNCREATE(CScheduleReviewView, CBaseFormView)

CScheduleReviewView::CScheduleReviewView()
	: CBaseFormView(CScheduleReviewView::IDD)
{

}

CScheduleReviewView::~CScheduleReviewView()
{
}

void CScheduleReviewView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND_SCHE, m_findTextBox);
}

BEGIN_MESSAGE_MAP(CScheduleReviewView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_SCHEDULE, OnGridEndEidt )
	ON_NOTIFY(NM_CLICK,	ID_GRID_SCHEDULE,	OnClickGrid)
	ON_NOTIFY(GVN_SELCHANGED,	ID_GRID_SCHEDULE,	OnGridSelChange)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_FRESH_SCH, &CScheduleReviewView::OnBnClickedBtnFresh)
	ON_CBN_SELCHANGE(IDC_COMBO_FIND_SCHE, &CScheduleReviewView::OnCbnEditchangeComboFind)

	//
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CScheduleReviewView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CScheduleReviewView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CScheduleReviewView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		InitSearchCondition( TRUE );
	}

}
int CScheduleReviewView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
int CScheduleReviewView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );
	if (!m_taskGrid.Create(gridRect,this,ID_GRID_SCHEDULE ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	m_pActionFactory = pActionFactory;
	m_gridManger.reset( new CTaskScheduleManger(pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}
//void CScheduleReviewView::OnBnClickedBtnFresh()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//m_gridManger->UpdateDbData();
//}
//网格选择改变变化
void CScheduleReviewView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//网格文本框编辑结束
void CScheduleReviewView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}


void CScheduleReviewView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid );

	// TODO: 在此处添加消息处理程序代码
}
void CScheduleReviewView::InitSearchCondition( BOOL isInit )
{
	int lastSel = m_findTextBox.GetCurSel();
	m_findTextBox.ResetContent();
	m_findTextBox.InsertString( 0, _T("所有待点评的任务") );
	m_findTextBox.InsertString( 1, _T("待点评的部门任务") );
	m_findTextBox.InsertString( 2, _T("待点评的项目任务") );
	
	m_prjConditionPos = 3;
	if ( m_pActionFactory )
	{
		CTaskSeverInc *pTaskSever = m_pActionFactory->GetTaskSever();
		const MapTaskSchedule  &remarkSchedule = pTaskSever->GetReviewSchedule();
		m_prjConditionMap.clear();
	    MapTaskSchedule::const_iterator itor = remarkSchedule.begin();
		BOOL ret;
		int beginPos = m_prjConditionPos;
		while( itor != remarkSchedule.end() )
		{ 
			CTaskObj obj;
			ret = pTaskSever->GetTaskByTaskId( itor->first, obj );
			int prjId = obj.GetProjctId();
			if ( userExcOk == ret && TASKTYPE_PRJSTR == obj.GetType() && obj.GetProjctId() > 0 )
			{
				if ( m_prjConditionMap.end() == m_prjConditionMap.find( prjId ) )
				{
					CProject prj;
					ret = m_pActionFactory->GetProjectSever()->GetProjectById( prjId, prj );
					if ( userExcOk == ret )
					{
						m_findTextBox.InsertString( beginPos++, prj.GetShortName() );
					}
				}
				
			}
			itor++;
		}
		if ( isInit )
		{
			m_findTextBox.SetCurSel( 0 );
		}else if ( lastSel >= 0 && lastSel < beginPos )
		{
			m_findTextBox.SetCurSel( lastSel );
		}else
		{
			m_findTextBox.SetCurSel( 0 );
		}

	}

}

void CScheduleReviewView::OnBnClickedBtnFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pActionFactory->GetTaskSever()->UpdateAllReviewSchedule();
	InitSearchCondition( FALSE );
	OnCbnEditchangeComboFind();
}


void CScheduleReviewView::OnCbnEditchangeComboFind()
{
	int curSel = m_findTextBox.GetCurSel();
	if ( curSel < 0 )
	{
		return;
	}
	if( 0 == curSel ) 
	{
		m_gridManger->UpdateShowAllSchedul();
	}else if ( 1 == curSel )
	{
		m_gridManger->UpdateShowDepartSchedul();
	}else if ( 2 == curSel )
	{
		m_gridManger->UpdateShowAllPrjSchedul();
	}else
	{
		int dataPos = curSel - m_prjConditionPos;
		if ( m_prjConditionMap.size() > 0 && dataPos >= 0 )
		{
			ProjectMap::const_iterator itor = m_prjConditionMap.begin();
			advance( itor, dataPos );
			if ( itor == m_prjConditionMap.end())
			{
				return;
			}
			//
			m_gridManger->UpdateShowPrjSchedul( itor->second );
		}
	}
	m_taskGrid.Refresh();
}
void CScheduleReviewView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}
LRESULT CScheduleReviewView::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_findTextBox.SetCurSel( 0 );
		m_gridManger->UpdateDbDataMapAndShowAll();
		m_gridManger->SelectScheuleLine( lp );
	}
	return 0;
}
LRESULT CScheduleReviewView::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_findTextBox.SetCurSel( 0 );
		m_gridManger->UpdateDbDataMapAndShowAll();
	}
	return 0;
}
LRESULT CScheduleReviewView::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateScheulContent( lp );
	}
	return 0;
}
LRESULT CScheduleReviewView::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateScheulContent( lp );
	}
	return 0;
}
