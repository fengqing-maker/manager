// TaskTabView.cpp : 实现文件
//

#include "stdafx.h"
#include "DepartTabView.h"
#include "../../MainFrm.h"
#include "CDepartView.h"
#include "CDepartTaskView.h"
#include "../prjView/CProjectScheduleView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDepartTabView

IMPLEMENT_DYNCREATE(CDepartTabView, CTabView)

CDepartTabView::CDepartTabView()
{
	m_selPrjId = 0;
	m_selTaskStatusIndex = -1;
}

CDepartTabView::~CDepartTabView()
{
	if ( ::IsWindow( m_wndTabs.GetSafeHwnd() )  )
	{
		m_wndTabs.RemoveAllTabs();
	}
}

BEGIN_MESSAGE_MAP(CDepartTabView, CTabView)
	ON_WM_CREATE()
	ON_MESSAGE( DEPART_MANGER_MSG_CHANGE,  OnChangeDepartTaskView)
	//ON_MESSAGE( PRJ_VIEW_MSG_RECODE_PRJID,  OnRecordShowPrjIs)
	//ON_MESSAGE( PRJ_VIEW_MSG_RECODE_TASK_INDEX,  OnRecordTaskStSelIndex)
	ON_MESSAGE( PRJ_VIEW_MSG_CHANGE_TASKLIST, OnActivteTaskPictrue)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDepartTabView 诊断

#ifdef _DEBUG
void CDepartTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CDepartTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDepartTabView 消息处理程序
void CDepartTabView::ActiveTab(int index)
{
	SetActiveView( index );
	//SetActiveTab(index);
}
void CDepartTabView::UpdateTabLabel(Tab_Index index)
{
	CString tabLabel;
	TabIndxeMap_Itor itor = m_tabIndex.find( index );
	if ( itor == m_tabIndex.end() )
	{
		return;
	}
	int tabIndex = (*itor).second;
	CString str;
	str = GetTabString(  index );
	
	m_wndTabs.SetTabLabel(tabIndex,str);
}
CString  CDepartTabView::GetTabString( Tab_Index index ) const
{
	CString str;
	switch( index)
	{
	case departTabIndex:
		str = _T("部门列表");
		break;
	case  departTaskTabIndex:
		str = _T("部门任务");
		break;
	case  schedulePictrue:
		str = _T("任务甘特图");
		break;
	}
	return str;
}
int  CDepartTabView::OnCreateTab()
{
	//
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
	/* 创建项目列表 */
	int count = 0;
	int setDefIndex = 0;
	if ( AddView( RUNTIME_CLASS(CDepartView), GetTabString(departTabIndex),count ) < 0 )
	{
		TRACE("部门管理页面失败\n");
		return -1;
	}

	m_tabIndex.clear();
	m_tabIndex[departTabIndex] = count++;
	/* 创建任务甘特图 */
	if ( AddView( RUNTIME_CLASS(CProjectScheduleView), GetTabString(schedulePictrue), schedulePictrue ) < 0 )
	{
		TRACE("项目任务页面失败\n");
		return -1;
	}
	m_tabIndex[schedulePictrue] = count++;
	/* 创建项目任务 */
	if ( AddView( RUNTIME_CLASS(CDepartTaskView), GetTabString(departTaskTabIndex), departTaskTabIndex ) < 0 )
	{
		TRACE("部门管理页面失败\n");
		return -1;
	}
	m_tabIndex[departTaskTabIndex] = count++;

	///* 改变被激活选项卡背景色和文字颜色 */
	m_wndTabs.SetActiveTabColor(RGB(137,213,228));
	m_wndTabs.SetActiveTabTextColor(RGB(146,78,235));

	m_wndTabs.SetActiveTab( setDefIndex );
	GetActiveView()->SendMessage(WM_INITIALUPDATE,0,0);
	/* 允许选项卡交换 */
	m_wndTabs.EnableTabSwap(FALSE);
	
	return 0;
}

void CDepartTabView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

int CDepartTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	OnCreateTab();
	
	return 0;
}
LRESULT CDepartTabView::OnChangeDepartTaskView( WPARAM wPara, LPARAM lPara )
{
	TabIndxeMap::const_iterator itor = m_tabIndex.find( departTaskTabIndex );
	if ( itor != m_tabIndex.end() )
	{
		SetActiveView( departTaskTabIndex );
	    CView *pView = GetActiveView();
		m_selPrjId = wPara;
		if ( pView->IsKindOf( RUNTIME_CLASS(CDepartTaskView) ) )
		{
			( (CDepartTaskView *)pView )->SetSelDepartById( m_selPrjId/*, ongoing*/ );
			OnActivteTaskPictrue( (WPARAM)( ((CDepartTaskView *)pView )->GetTaskListPoint() ), NULL );
			//this->UpdateWindow();
		}
	}
	return TRUE;
}
//LRESULT CDepartTabView::OnRecordShowPrjIs( WPARAM wPara, LPARAM lPara )
//{
//	m_selPrjId = wPara;
//	return TRUE;
//}
//LRESULT CDepartTabView::OnRecordTaskStSelIndex( WPARAM wPara, LPARAM lPara )
//{
//	m_selTaskStatusIndex = wPara;
//	return TRUE;
//}

LRESULT CDepartTabView::OnActivteTaskPictrue( WPARAM wPara, LPARAM lPara )
{
	TabIndxeMap::const_iterator itor = m_tabIndex.find( schedulePictrue );
	if ( itor != m_tabIndex.end()  )
	{
		CProjectScheduleView *pWnd = NULL;
		if ( m_tabIndex.size() <= schedulePictrue )
		{
			pWnd = (CProjectScheduleView *)GetTabControl().GetTabWnd( itor->second );
		}else
		{
			pWnd = (CProjectScheduleView *)GetTabControl().GetTabWnd( schedulePictrue );
		}
		if ( wPara )
		{
			ListTask* pTaskList = (ListTask*)wPara;
			if (  pTaskList && pTaskList->size() > 0 )
			{
				int userId = pTaskList->begin()->GetOwnerId();
				CUser userObj;
				m_pSever->GetLoginServer()->GetUserById( userId, userObj );
				CDepartment departObj;
				m_pSever->GetDepartSever()->GetDepartById( userObj.GetDepartId(), departObj );
				pWnd->SetTiltleString( departObj.GetName() );
			}
			pWnd->SetTaskListPoint( (ListTask*)wPara );
		}	

	}
	return 0;
}
