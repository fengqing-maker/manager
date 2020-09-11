// TaskTabView.cpp : 实现文件
//

#include "stdafx.h"
#include "TaskTabView.h"
#include "../../MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CTaskTabView

IMPLEMENT_DYNCREATE(CTaskTabView, CTabView)

CTaskTabView::CTaskTabView()
{

}

CTaskTabView::~CTaskTabView()
{
	if ( ::IsWindow( m_wndTabs.GetSafeHwnd() )  )
	{
		m_wndTabs.RemoveAllTabs();
	}
}

BEGIN_MESSAGE_MAP(CTaskTabView, CTabView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_MESSAGE( MSG_UPDATE_TITLE_CNT, OnUpdateTabName )
	ON_MESSAGE( GRID_UPDATE_WORKHOUR, OnUpdateWorkHourTab )
END_MESSAGE_MAP()


// CTaskTabView 诊断

#ifdef _DEBUG
void CTaskTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CTaskTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG

//选项卡名称索引  、 待处理任务数
void CTaskTabView::UpdateTabLabel(Tab_Index index, int cnt )
{
	CString tabLabel;
	TabIndxeMap_Itor itor = m_tabIndex.find( index );
	if ( itor == m_tabIndex.end() )
	{
		return;
	}
	int tabIndex = (*itor).second;
	CString str;
	str = GetTabString(  index, cnt );
	
	m_wndTabs.SetTabLabel( tabIndex, str );
}
CString  CTaskTabView::GetTabString( Tab_Index index, int cnt ) const
{
	CString str, cntStr;
	switch( index)
	{
	case MyTask:
		str = _T("我的任务");
		break;
	case  TaskPublishReview:
		str = _T("任务发布及完成审核");
		break;
	case taskScoreReview:
		str =  _T("任务评分");
		break;
	//case ScheduleReview:
	//	str = _T("任务进度评审");
	//	break;
	case ChangeReview:
		str = _T("任务变更评审");
		break;
	case  TabWorkHourView:
		str = _T("工时更新");
		break;
	case  TabForecastView:
		str = _T("预测工作饱和度");
		break;
	}
	if ( cnt >= 0 )
	{
		cntStr.Format(_T("[%d]"), cnt ); //待处理任务数
	}
	str += cntStr;
	return str;
}
LRESULT CTaskTabView::OnUpdateTabName( WPARAM wp, LPARAM lp )
{
	Tab_Index index;
	switch( wp )
	{
		case HandleMyTask:			//新增我要处理任务
			index = MyTask;			//我的任务
			break;
		case PublishTask:			//新增待发布任务
			index = TaskPublishReview;//任务发布及完成审核
			break;
		case ScoreTask:				//新增待评分任务
			index = taskScoreReview;	//任务评分
			break;
		case ReviewChange:			//新增待评审变更任务
			index = ChangeReview;	//任务变更审核
			break;
		default:
			return 0;
	}
	UpdateTabLabel( index, (int)lp );
	return 0;
}
int  CTaskTabView::OnCreateTab()
{
	//获取数据服务基类指针
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	/* 通过改变VS自动默认字体改变标签页的字体，必须置于标签控件创建之前 */
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	lf.lfHeight = 15;
	lstrcpy(lf.lfFaceName,_T("宋体"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);
	CRect rect;
	rect.SetRectEmpty();
	/* 创建我的任务视图 */												//把一个视图作为一个子项添加
	int count = 0;
	CCreateContext context;
	context.m_pNewViewClass = RUNTIME_CLASS(CMyTaskView);
	context.m_pCurrentDoc = GetDocument();
	int setDefIndex = 0;
	//获取账户下人物数目的数量
	int handCnt = m_pSever->GetTaskSever()->GetOwernTask().size();
	//
	if ( AddView( RUNTIME_CLASS(CMyTaskView), GetTabString(MyTask, handCnt ),count, &context ) < 0 )		//我的任务
	{
		TRACE("我的任务Table创建失败.\n");
		return -1;
	}
	m_tabIndex.clear();
	m_tabIndex[MyTask] = count++;
	/* 创建进度评分视图 */
	//BOOL enable = m_pSever->GetLoginServer()->FindLoginPower( taskAgree );
	//BOOL enable2 = m_pSever->GetLoginServer()->FindLoginPower( projectReview );
	//m_scheduleView.reset(NULL);
	//if( enable || enable2 )
	//{
	//	context.m_pNewViewClass = RUNTIME_CLASS(CScheduleReviewView);
	//	if ( AddView( RUNTIME_CLASS( CScheduleReviewView ), GetTabString(ScheduleReview),count,&context) < 0 )
	//	{
	//		TRACE("发布进度评分视图创建失败.\n");
	//		return -1;
	//	}
	//	if ( m_pSever->GetTaskSever()->GetReviewSchedule().size() > 0 )
	//	{
	//		setDefIndex = count;
	//	}
	//	m_tabIndex[ScheduleReview] = count++;
	//}
	/* 创建任务发布视图 */
	//任务下发和进度评审、项目任务完成审核（就是查看用户有没有这两个权限）
	BOOL enable = m_pSever->GetLoginServer()->FindLoginPower( taskAgree ) || m_pSever->GetLoginServer()->FindLoginPower( ReviewProjectTaskFinish );
	//公共任务视图清空
	m_publishTaskView.reset(NULL);
	//任务发布与评审
	if( enable )
	{
		//任务发布及任务审核
		handCnt = m_pSever->GetTaskSever()->GetReviewTask().size();
		context.m_pNewViewClass = RUNTIME_CLASS(CPublishTaskView);
		if ( AddView( RUNTIME_CLASS( CPublishTaskView ), GetTabString( TaskPublishReview, handCnt ),count, &context ) < 0)
		{
			TRACE("发布Table创建失败.\n");
			return -1;
		}
		int reviewCnt = m_pSever->GetTaskSever()->GetReviewTask().size();
		if ( m_pSever->GetTaskSever()->GetReviewTask().size() > 0 )
		{
			setDefIndex = count;
			if ( reviewCnt != handCnt )
			{//更新tab名
			    CString tabNameStr = GetTabString( TaskPublishReview, reviewCnt );
				m_wndTabs.SetTabLabel( count, tabNameStr);
			}
		}
		
		m_tabIndex[TaskPublishReview] = count++;
	}
	/* 创建任务评分视图 */
	enable = m_pSever->GetLoginServer()->FindLoginPower( taskScore );
	m_scoreTaskView.reset(NULL);
	//任务评分
	if( enable )
	{
		handCnt = m_pSever->GetTaskSever()->GetScoreTask().size();
		context.m_pNewViewClass = RUNTIME_CLASS(CScoreTaskView);
		if ( AddView( RUNTIME_CLASS(CScoreTaskView), GetTabString(taskScoreReview, handCnt ), count, &context ) < 0)
		{
			TRACE("发布任务评分视图创建失败.\n");
			return -1;
		}
		m_tabIndex[taskScoreReview] = count;
		if ( m_pSever->GetTaskSever()->GetScoreTask().size() > 0 )
		{
			UpdateTabLabel( taskScoreReview, m_pSever->GetTaskSever()->GetScoreTask().size() );
			setDefIndex = count;
		}
		count++;
	}

	/* 创建任务变更审核视图 *///分为1级和2级 
	enable = m_pSever->GetLoginServer()->FindLoginPower( changeLevelAgree1 );
	BOOL enable2 = m_pSever->GetLoginServer()->FindLoginPower( changeLevelAgree2 );
	m_publishTaskView.reset(NULL);
	if( enable || enable2 )
	{
		context.m_pNewViewClass = RUNTIME_CLASS(CChangeTaskView);
		bool isShowfirst = false;
		size_t ownCount = m_pSever->GetTaskSever()->GetOwneChangeCount();
		size_t reviewChgCnt = m_pSever->GetTaskSever()->GetReviewChange().size();
		//如果接收的数目大于自己的数目
		if ( reviewChgCnt > ownCount )
		{
			const ListChange &lstCh = m_pSever->GetTaskSever()->GetReviewChange();
			reviewChgCnt = 0;
			int loginUserId = m_pSever->GetLoginServer()->GetLoginUser().GetId();
			for ( ListChangeConstItor itor = lstCh.begin(); itor != lstCh.end(); itor++ )
			{
				CTaskObj obj;
				CProject prj;
				if ( itor->IsFinishChange() )
				{
					continue;
				}
				m_pSever->GetTaskSever()->GetTaskByTaskId( itor->GetTaskId(), obj );
				m_pSever->GetProjectSever()->GetProjectById( obj.GetProjctId(), prj );
				if ( loginUserId == itor->GetAuditorId() )
				{
					if ( up_apper == itor->GetStatus() )
					{
						reviewChgCnt++;
					}
				}
				if ( loginUserId == itor->GetGeneralAuditorId() )
				{
					if ( departAgree == itor->GetStatus()  )
					{
						reviewChgCnt++;
					}
				}
				if ( obj.GetProjctId() > 0 && prj.GetMangerId() == loginUserId )
				{
					if ( prj_remark_on == itor->GetStatus() )
					{
						reviewChgCnt++;
					}
				}
			}	
		}
		else
		{
            reviewChgCnt = 0;
		}
		//添加任务变更评审
		if ( AddView( RUNTIME_CLASS(CChangeTaskView), GetTabString(ChangeReview, reviewChgCnt ) ) < 0 )
		{
			TRACE("发布任务视图创建失败.\n");
			return -1;
		}
		if ( reviewChgCnt > 0 )
		{
			setDefIndex = count;
		}
		m_tabIndex[ChangeReview] = count++;
	}
	/* 创建工时填写 */
	m_workHourView.reset(NULL);
	context.m_pNewViewClass = RUNTIME_CLASS(CWorkHourView);
	if ( AddView( RUNTIME_CLASS( CWorkHourView ), GetTabString(TabWorkHourView, -1 ),count,&context) < 0 )
	{
		TRACE("发布进度评分视图创建失败.\n");
		return -1;
	}
	m_tabIndex[TabWorkHourView] = count++;
	/* 创建预测员工工作饱和度 */
	//m_forecastWorkHourView.reset(NULL);
	context.m_pNewViewClass = RUNTIME_CLASS( CForecastWorkHourView );
	if ( AddView( RUNTIME_CLASS( CForecastWorkHourView ), GetTabString(TabForecastView, -1 ),count,&context) < 0 )
	{
		TRACE("发布进度评分视图创建失败.\n");
		return -1;
	}
	m_tabIndex[TabForecastView] = count++;

	///* 改变被激活选项卡背景色和文字颜色 */
	m_wndTabs.SetActiveTabColor(RGB(137,213,228));
	m_wndTabs.SetActiveTabTextColor(RGB(146,78,235));

	m_wndTabs.SetActiveTab( setDefIndex );
	GetActiveView()->SendMessage(WM_INITIALUPDATE,0,0);
	m_wndTabs.EnableTabSwap(TRUE);

	return 0;
}

void CTaskTabView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(m_myTaskView.get() && m_myTaskView->GetSafeHwnd() )
	{
		m_myTaskView->MoveWindow( 0, 0, cx, cy );
	}
}

int CTaskTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	OnCreateTab();
	return 0;
}


void CTaskTabView::OnDestroy()
{
	CTabView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}


void CTaskTabView::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTabView::OnPaint()
	CTabView::OnPaint();
}


void CTaskTabView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 在此添加专用代码和/或调用基类

	CTabView::OnActivateView(pActivateView);
}

//切换活动选项卡
BOOL  CTaskTabView::SetActiveViewByTabFlg( int flg )
{
	if ( flg < 0  )
	{
		for ( TabIndxeMap_Itor itor = m_tabIndex.begin(); itor != m_tabIndex.end(); itor++ )
		{
			if ( itor->second == flg )
			{
				SetActiveView( itor->first );
				return TRUE;
			}
		}
	}
	return FALSE;
}

//获取当前选中选项卡的索引
int          CTaskTabView::GetActiveViewTabFlg( )
{
	int flg = FindTab( GetActiveView()->GetSafeHwnd() );
	TabIndxeMap_Itor itor = m_tabIndex.find(flg);
	if ( m_tabIndex.end() != m_tabIndex.find(flg) )
	{
		flg = m_tabIndex[flg];
	}
	return flg;
}


//应该是工时发生变化时出发的消息
LRESULT  CTaskTabView::OnUpdateWorkHourTab( WPARAM wp, LPARAM lp )
{
	TabIndxeMap::const_iterator itor = m_tabIndex.find( TabWorkHourView );
	if ( itor != m_tabIndex.end()  )
	{
		CWorkHourView *pWnd = NULL;
		pWnd = (CWorkHourView *)GetTabControl().GetTabWnd( itor->second );
		pWnd->RefreshShow();
	}
    return 0;
}

