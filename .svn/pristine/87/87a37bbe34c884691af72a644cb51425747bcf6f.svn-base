// TaskTabView.cpp : 实现文件
//

#include "stdafx.h"
#include "BackStageTabView.h"
#include "../../MainFrm.h"
#include "CUserMangerView.h"
#include "CProductLineView.h"
#include "RolePowerView.h"
#include "WorkTypeView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDepartTabView

IMPLEMENT_DYNCREATE(CBackStageTabView, CTabView)

CBackStageTabView::CBackStageTabView()
{
	m_selPrjId = 0;
	m_selTaskStatusIndex = -1;
}

CBackStageTabView::~CBackStageTabView()
{
	if ( ::IsWindow( m_wndTabs.GetSafeHwnd() )  )
	{
		m_wndTabs.RemoveAllTabs();
	}
}

BEGIN_MESSAGE_MAP(CBackStageTabView, CTabView)
	ON_WM_CREATE()

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBackStageTabView 诊断

#ifdef _DEBUG
void CBackStageTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CBackStageTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBackStageTabView 消息处理程序
void CBackStageTabView::ActiveTab(int index)
{
	SetActiveView( index );
	//SetActiveTab(index);
}
void CBackStageTabView::UpdateTabLabel(Tab_Index index)
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
CString  CBackStageTabView::GetTabString( Tab_Index index ) const
{
	CString str;
	switch( index)
	{
	case userMangerTabInex:
		str = _T("用户管理");
		break;
	case  productLineTabIndex:
		str = _T("产品线管理");
		break;
	case  rolePowerTabIndex:
		str = _T("权限管理");
		break;	
	case  workTimeType:
		str = _T("工时类型");
		break;
	}
	return str;
}
int  CBackStageTabView::OnCreateTab()
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
	if ( AddView( RUNTIME_CLASS(CUserMangerView), GetTabString(userMangerTabInex),count ) < 0 )
	{
		TRACE("用户管理页面失败\n");
		return -1;
	}

	m_tabIndex.clear();
	m_tabIndex[userMangerTabInex] = count++;
	/* 创建产品线 */
	if ( AddView( RUNTIME_CLASS(CProductLineView), GetTabString(productLineTabIndex),count ) < 0 )
	{
		TRACE("产品线页面失败\n");
		return -1;
	}
	m_tabIndex[productLineTabIndex] = count++;

	/* 创建角色权限 */
	if ( AddView( RUNTIME_CLASS(CRolePowerView), GetTabString(rolePowerTabIndex),count ) < 0 )
	{
		TRACE("部门管理页面失败\n");
		return -1;
	}
	m_tabIndex[rolePowerTabIndex] = count++;
	/* 创建 */
	if ( AddView( RUNTIME_CLASS(CWorkTimeTypeView), GetTabString(workTimeType),count ) < 0 )
	{
		TRACE("工时类型失败\n");
		return -1;
	}
	m_tabIndex[workTimeType] = count++;

	///* 改变被激活选项卡背景色和文字颜色 */
	m_wndTabs.SetActiveTabColor(RGB(137,213,228));
	m_wndTabs.SetActiveTabTextColor(RGB(146,78,235));

	m_wndTabs.SetActiveTab( setDefIndex );
	GetActiveView()->SendMessage(WM_INITIALUPDATE,0,0);
	/* 允许选项卡交换 */
	m_wndTabs.EnableTabSwap(FALSE);
	
	return 0;
}

void CBackStageTabView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}

int CBackStageTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	OnCreateTab();
	
	return 0;
}



