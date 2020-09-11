// TaskTabView.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectTabView.h"
#include "../../MainFrm.h"
#include "CProjectView.h"
#include "CProjectTaskView.h"
#include "CProjectScheduleView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CProjectTabView

IMPLEMENT_DYNCREATE(CProjectTabView, CTabView)

CProjectTabView::CProjectTabView()
{
	m_selPrjId = 0;
	m_selTaskStatusIndex = -1;
}

//当视图退出的时候删除所有的Tab表
CProjectTabView::~CProjectTabView()
{
	if ( ::IsWindow( m_wndTabs.GetSafeHwnd() )  )
	{
		m_wndTabs.RemoveAllTabs();
	}
}

BEGIN_MESSAGE_MAP(CProjectTabView, CTabView)
	ON_WM_CREATE()
	ON_MESSAGE( PRJ_MANGER_MSG_CHANGE,  OnChangePrjTaskView)
	ON_MESSAGE( PRJ_VIEW_MSG_RECODE_PRJID,  OnRecordShowPrjIs)
	ON_MESSAGE( PRJ_VIEW_MSG_RECODE_TASK_INDEX,  OnRecordTaskStSelIndex)
	ON_MESSAGE( PRJ_VIEW_MSG_CHANGE_TASKLIST, OnActivteTaskPictrue)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CProjectTabView 诊断

#ifdef _DEBUG
void CProjectTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CProjectTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProjectTabView 消息处理程序
void CProjectTabView::ActiveTab(int index)
{
	SetActiveView( index );
	//SetActiveTab(index);
}

//给选项卡中的类中相关联的选项卡控件添加一个个的标签，就是添加选项
void CProjectTabView::UpdateTabLabel(Tab_Index index)
{
	CString tabLabel;
	//map<int,int>的迭代器
	TabIndxeMap_Itor itor = m_tabIndex.find( index );			//容器中查找选项卡号
	if ( itor == m_tabIndex.end() )
	{
		return;
	}
	int tabIndex = (*itor).second;	
	CString str;
	//为选项卡添加标题
	str = GetTabString(  index );
	m_wndTabs.SetTabLabel(tabIndex,str);		//
}
CString  CProjectTabView::GetTabString( Tab_Index index ) const
{
	CString str;
	switch( index)
	{
		case projectTabIndex:
			str = _T("项目列表");
			break;
		case  taskTabIndex:
			str = _T("项目任务");
			break;
		case  schedulePictrue:
			str = _T("任务甘特图");
			break;
	}
	return str;
}

//给选项卡类添加一个个的选项卡
int  CProjectTabView::OnCreateTab()				
{
	//
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	/* 通过改变VS自动默认字体改变标签页的字体，必须置于标签控件创建之前 */
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	//删除系统字体，重新设置系统字体
	lf.lfHeight = 15;
	lstrcpy(lf.lfFaceName,_T("宋体"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);
	CRect rect;
	rect.SetRectEmpty();
	/* 创建项目列表 */
	int count = 0;
	int setDefIndex = 0;
	//初始化项目管理页面，任务列表视图
	if ( AddView( RUNTIME_CLASS(CProjectView), GetTabString(projectTabIndex),count ) < 0 )
	{
		TRACE("项目管理页面失败\n");
		return -1;
	}
	m_tabIndex.clear();
	//不太明白
	m_tabIndex[projectTabIndex] = count++;
	/* 创建任务甘特图 */
	if ( AddView( RUNTIME_CLASS(CProjectScheduleView), GetTabString(schedulePictrue), schedulePictrue ) < 0 )
	{
		TRACE("项目任务页面失败\n");
		return -1;
	}
	m_tabIndex[schedulePictrue] = count++;

	/* 创建项目任务 */
	if ( AddView( RUNTIME_CLASS(CProjectTaskView), GetTabString(taskTabIndex), taskTabIndex ) < 0 )
	{
		TRACE("项目任务页面失败\n");
		return -1;
	}
	m_tabIndex[taskTabIndex] = count++;


	///* 改变被激活选项卡背景色和文字颜色 */
	m_wndTabs.SetActiveTabColor(RGB(137,213,228));
	m_wndTabs.SetActiveTabTextColor(RGB(146,78,235));

	m_wndTabs.SetActiveTab( setDefIndex );
	GetActiveView()->SendMessage(WM_INITIALUPDATE,0,0);
	/* 允许选项卡交换 */
	m_wndTabs.EnableTabSwap(FALSE);		//不允许选项卡交换
	
	return 0;
}

void CProjectTabView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

int CProjectTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	OnCreateTab();
	
	return 0;
}

//点击到  项目任务视图
LRESULT CProjectTabView::OnChangePrjTaskView( WPARAM wPara, LPARAM lPara )
{
	//查找选项卡是否已经添加了项目任务的视图
	TabIndxeMap::const_iterator itor = m_tabIndex.find( taskTabIndex );
	if ( itor != m_tabIndex.end() )
	{
		SetActiveView( taskTabIndex );
	    CView *pView = GetActiveView();
		m_selPrjId = wPara;
		if ( pView->IsKindOf( RUNTIME_CLASS(CProjectTaskView) ) )			//显示项目任务视图必须是在选项卡视图存在的情况下
		{
			( (CProjectTaskView *)pView )->SetSelProjectByPrjId( m_selPrjId );
			OnActivteTaskPictrue( (WPARAM)( ((CProjectTaskView *)pView )->GetTaskListPoint() ), NULL );
		}
	}
	return TRUE;
}
LRESULT CProjectTabView::OnRecordShowPrjIs( WPARAM wPara, LPARAM lPara )
{
	m_selPrjId = wPara;
	return TRUE;
}
LRESULT CProjectTabView::OnRecordTaskStSelIndex( WPARAM wPara, LPARAM lPara )
{
	m_selTaskStatusIndex = wPara;
	return TRUE;
}
void CProjectTabView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CTabView::OnPaint()
	CTabView::OnPaint();
}

void CProjectTabView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CTabView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
}

//转到甘特图选项卡
LRESULT CProjectTabView::OnActivteTaskPictrue( WPARAM wPara, LPARAM lPara )
{
	TabIndxeMap::const_iterator itor = m_tabIndex.find( schedulePictrue );
	if ( itor != m_tabIndex.end()  )
	{
		CProjectScheduleView *pWnd = NULL;
		if ( m_tabIndex.size() <= schedulePictrue )
		{
			pWnd = (CProjectScheduleView *)GetTabControl().GetTabWnd( itor->second );
		}
		else
		{
			pWnd = (CProjectScheduleView *)GetTabControl().GetTabWnd( schedulePictrue ); //111111111111
		}
		if ( wPara )
		{
			pWnd->SetTaskListPoint( (ListTask*)wPara );
		}	
		
	}
	return 0;
}