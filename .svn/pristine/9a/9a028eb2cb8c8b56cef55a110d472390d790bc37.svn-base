popupMenuID_ClearContent
// InfoPane.cpp : 实现文件
//

#include "stdafx.h"
#include "SvrOutputList.h"
#include "resource.h"

#ifdef _DEBUG                      	/* 判断是否定义_DEBUG */
#undef THIS_FILE                   	/* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;  	/* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              	/* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */

// CSvrOutputList

IMPLEMENT_DYNAMIC(CSvrOutputList, COutputListCtrl)

CSvrOutputList::CSvrOutputList()
{
	installComponent();
}

CSvrOutputList::~CSvrOutputList()
{
	uninstallComponent();
}


BEGIN_MESSAGE_MAP(CSvrOutputList, COutputListCtrl)
	/* 标准Windows消息 */
	ON_WM_CREATE()
	//ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	//ON_WM_PAINT()
	//ON_WM_SETFOCUS()

	//ON_NOTIFY(NM_DBLCLK, ID_LIST_SERVER_OUTPUT, OnDblClick)  //ON_LBN_SELCHANGE 
	//ON_NOTIFY(NM_CLICK, ID_LIST_SERVER_OUTPUT, OnClick)
	//ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblClick)
	//ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT_EX(NM_DBLCLK, OnDblClick)
	ON_NOTIFY_REFLECT_EX(NM_CLICK, OnClick)

	ON_MESSAGE(WM_OUTPUT_UPDATE,	OnUpdateOutputList)
	/* 右键弹出菜单消息响应 */
	ON_COMMAND_RANGE(WM_OUTPUT_POPUP_MENUID_MIN,WM_OUTPUT_POPUP_MENUID_MAX,OnRespPopupMenu)
	ON_UPDATE_COMMAND_UI_RANGE(WM_OUTPUT_POPUP_MENUID_MIN,WM_OUTPUT_POPUP_MENUID_MAX,OnUpdatePopupMenu)

END_MESSAGE_MAP()

//void CSvrOutputList::AdjustLayout()
//{
//	if (GetSafeHwnd() == NULL)
//	{
//		return;
//	}
//	CRect rectClient;
//	GetClientRect(rectClient);
//	SvrOutputManager::GetObj()->ResetColumnWidth();
//}


// CSvrOutputList 消息处理程序
int CSvrOutputList::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COutputListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}

//void CSvrOutputList::OnSize(UINT nType, int cx, int cy)
//{
//	COutputListCtrl::OnSize(nType, cx, cy);
//	//AdjustLayout();
//}

void CSvrOutputList::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//CSvrOutputList* pWndList = (CSvrOutputList*)&m_svrOutputList;
	//ASSERT_VALID(pWndList);

	//if (pWnd != pWndList)
	//{
		//COutputListCtrl::OnContextMenu(pWnd, point);
		//return;
	//}
	//else
	//{
		SvrOutputManager::GetObj()->ShowOutputContextMenu(point);
	//}
}

//void CSvrOutputList::OnPaint()
//{
	//CPaintDC dc(this); // 用于绘制的设备上下文

	//CRect rectList;
	///*m_svrOutputList.*/GetWindowRect(rectList);
	//ScreenToClient(rectList);

	//rectList.InflateRect(1, 1);
	//dc.Draw3dRect(rectList, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
//}



void CSvrOutputList::OnRespPopupMenu(UINT menuID)
{
	if (menuID >= WM_OUTPUT_POPUP_MENUID_MIN
		&& menuID <= WM_OUTPUT_POPUP_MENUID_MAX)
	{
		SvrOutputManager::GetObj()->RespOutputContextMenu(menuID);
	}
}
void CSvrOutputList::OnUpdatePopupMenu(CCmdUI *pCmdUI)
{
	if (pCmdUI->m_nID >= WM_OUTPUT_POPUP_MENUID_MIN
		&& pCmdUI->m_nID <= WM_OUTPUT_POPUP_MENUID_MAX)
	{
		pCmdUI->Enable(TRUE);
	}
}
/******************************private function****************************/
void CSvrOutputList::installComponent()
{
	if (SvrOutputManager::GetObj() == 0)
	{
		TRACE("创建系统输出窗口管理器失败.\n");
	}
}
void CSvrOutputList::uninstallComponent()
{
	SvrOutputManager::ReleaseObj();
}
// CSvrOutputList 消息处理程序
LRESULT CSvrOutputList::OnUpdateOutputList(WPARAM wParam,LPARAM lParam)
{
	SvrOutputManager::GetObj()->UpdateContent();
	return 0;
}
BOOL CSvrOutputList::OnDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	//COutputListCtrl::OnDblClick(pNMHDR, pResult); 
	SvrOutputManager::GetObj()->OnDblClick( pNMHDR, pResult );
	return TRUE;
}
BOOL CSvrOutputList::OnClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	//COutputListCtrl::OnClick(pNMHDR, pResult);
	SvrOutputManager::GetObj()->OnClick( pNMHDR, pResult );
	return TRUE;
}
