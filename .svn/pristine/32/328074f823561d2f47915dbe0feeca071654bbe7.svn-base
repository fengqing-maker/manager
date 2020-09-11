
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "../MainFrm.h"
#include "../view/baseGridManger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
	m_pSeverFactory = NULL;
	m_showTaskId = -1;
}

COutputWnd::~COutputWnd()
{
	m_wndTaskInfo.RealseData();
    m_wndLoginInfo.DestroyWindow();
	m_wndTaskInfo.DestroyWindow();
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()

	ON_MESSAGE( MSG_OUTPUT_SEL_SYS, OnSelectSystemOutput )
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建选项卡窗口:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("未能创建输出选项卡窗口\n");
		return -1;      // 未能创建
	}

	// 创建输出窗格:
	DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	const DWORD mulitColumnStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
		                          LBS_USETABSTOPS|/*LBS_STANDARD|*/LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS  ;
	/*const DWORD mulitColumnStyle = WS_CHILD |LBS_OWNERDRAWVARIABLE | 
		LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | 
		WS_TABSTOP;*/
	if (!m_wndTaskInfo.Create(mulitColumnStyle, rectDummy, &m_wndTabs, 10) ||
		!m_wndLoginInfo.Create(dwStyle, rectDummy, &m_wndTabs, 3) )
	{
		TRACE0("未能创建输出窗口\n");
		return -1;      // 未能创建
	}
	// 创建列表:
	dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT |WS_HSCROLL | WS_VSCROLL
		| LVS_ALIGNLEFT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS;
	dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_ALIGNLEFT | 
		LVS_AUTOARRANGE | LVS_SHOWSELALWAYS;
	//dwStyle = WS_OVERLAPPED|WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_AUTOARRANGE|LVS_SHOWSELALWAYS|LVS_EDITLABELS;
	//dwStyle = WS_VSCROLL|WS_CHILD| WS_VISIBLE | WS_HSCROLL |LVS_ALIGNLEFT | LVS_REPORT;
	if (!m_svrOutputList.Create( dwStyle, rectDummy, &m_wndTabs,ID_LIST_SERVER_OUTPUT))
	{
		TRACE0("未能创建系统输出窗口.\n");
		return -1;      // 未能创建
	}
	DWORD  dwNewStyle =	LVS_EX_FULLROWSELECT |		/* 整行选取 */
		LVS_EX_HEADERDRAGDROP |		/* 表头拖拽 */
		LVS_EX_INFOTIP;				/* tooltip提示 */
	m_svrOutputList.SetExtendedStyle(dwNewStyle);
	SvrOutputManager::GetObj()->RegisterAsOutputList( &m_svrOutputList );
	SvrOutputManager::GetObj()->InitOutputList();
	AdjustLayout();

	//m_wndTaskInfo.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	
	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// 将列表窗口附加到选项卡:
	bNameValid = strTabName.LoadString(IDS_TASK_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndTaskInfo, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString( IDS_STR_SYSMST );
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_svrOutputList, strTabName, (UINT)1);
	bNameValid = strTabName.LoadString(IDS_STR_LOGIN_INFO);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndLoginInfo, strTabName, (UINT)2);

	m_wndLoginInfo.SetHorizontalExtent( 1000 );

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// 选项卡控件应覆盖整个工作区:
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	//m_svrOutputList.RedrawWindow();
	AdjustLayout();
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillTaskWindow( int taskId )
{
	ASSERT( m_pSeverFactory );
	m_wndTaskInfo.RemoveAllString();

	if ( taskId <= 0 )
	{
		return;
	}
	CTaskSeverInc *m_pTaskSever = m_pSeverFactory->GetTaskSever();
	CTaskObj obj;
	m_pTaskSever->GetTaskByTaskId( taskId, obj );
	
	if ( obj.GetId() > 0 )
	{
		CString str;
		ListDate lstDate;
		m_pSeverFactory->GetWorkHourSever()->GetUnUpdateDate( obj.GetId(), lstDate );
		if ( lstDate.size() > 0 )
		{
			CString str(_T("未连续更新日期："));
			for ( ListDateItor itor = lstDate.begin(); itor != lstDate.end(); itor++ )
			{
				str += itor->Format( _T("%Y-%m-%d") );
				str += _T("、");
			}
			m_wndTaskInfo.AppendString( str, RGB(255, 255, 255), RGB(255, 0, 0) );
		}
		CString tmpStr;
		m_pTaskSever->GetWorkHourString();
		m_pTaskSever->GetWorkTypeObj().GetWorkTimeTypeById( obj.GetWorkTypeId() , tmpStr );
		str.Format(_T("任务名:%s  \r\n所属工时:%s 任务编号:%03d"), obj.GetName(),  tmpStr, obj.GetId() );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		CString prjName;
		if ( obj.GetProjctId() <= 0 )
		{
			prjName = TASKTYPE_DEPARTSTR;
		}else
		{
			CProject prjObj;
			m_pSeverFactory->GetProjectSever()->GetProjectById( obj.GetProjctId(), prjObj );
			prjName = prjObj.GetShortName();
		}
		//
		CString statusStr;
		m_pTaskSever->GetTaskStausMap().GetStrStausById(obj.GetTaskStatus(), statusStr );
		CUser tempUser;
		m_pSeverFactory->GetLoginServer()->GetUserById( obj.GetOwnerId(), tempUser );
		CString ownerName = tempUser.GetUserName_R();
		BOOL ret = m_pSeverFactory->GetLoginServer()->GetUserById( obj.GetCreateId(), tempUser );
		
		str.Format(_T("\r\n任务信息:%s,%s,责任人%s, %s在%s创建, 难度系数%0.1f"), statusStr, prjName, ownerName, tempUser.GetUserName_R(), obj.GetCreateTimeStr(),
			          obj.GetDifficultyParam() );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		//计划
		str.Format(_T("计划时间:%s~%s 耗时:%0.1f 实际时间:%s~%s 耗时:%0.1f"),
			obj.GetPanStartDateShowStr(), obj.GetPanEndDateShowStr(), obj.GetPlanWorkHour(), 
			obj.GetFactStartDateShowStr(), obj.GetFactEndDateShowStr(), obj.GetFactWorkHour() );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		//任务备注
		str = _T("任务备注:\r\n") + obj.GetDec();
		//m_wndTaskInfo.AddString(  str );
		m_wndTaskInfo.AppendString( str );

		//评分
		int st = obj.GetTaskStatus();
		m_pSeverFactory->GetLoginServer()->GetUserById( obj.GetMakeScoreUserId(), tempUser );
		if ( task_close == st || midStop == st )
		{/*系统预测分:%d*/
			str.Format(_T("\r\n 经理评分:%d 评分：%d 评分人:%s 评分时间:%s 评分描述：%s"), obj.GetForecastScore() ,obj.GetScore(), tempUser.GetUserName_R(),
				         obj.GetScoreTimeStr(), obj.GetScoreDec() );
			//m_wndTaskInfo.AddString( str );
			m_wndTaskInfo.AppendString( str );
		}else
		{/*系统预测分:%d*/
			str.Format(_T("\r\n 经理评分:%d 评分：无 评分人:%s 评分时间:无 评分描述：无"), obj.GetForecastScore(), tempUser.GetUserName_R() );
			m_wndTaskInfo.AppendString( str );
		}
		//点评
		//if ( !obj.GetReview().IsEmpty() )
		//{
	 //       str.Format( _T("部门过程点评:%s "), obj.GetReview() );
		//	m_wndTaskInfo.AppendString( str );
		//}
		if ( !obj.GetPrjReview().IsEmpty() )
		{
			str.Format( _T("项目过程点评:%s "), obj.GetPrjReview() );
			m_wndTaskInfo.AppendString( str );
		}
		//进度
		str = _T("\r\n进度:\r\n");
		m_wndTaskInfo.AppendString( str );
		std::list<CString> schArray;
		CBaseGridManger::GetHistoryString( m_pTaskSever, obj.GetId(), schArray );
		for ( std::list<CString>::iterator itor = schArray.begin(); itor != schArray.end(); itor++ )
		{
			m_wndTaskInfo.AppendString( *itor );
		}
		//tmpStr = _T("");
		//CBaseGridManger::GetHistoryString( m_pTaskSever, obj.GetId(), tmpStr );
		//str += tmpStr;
		//m_wndTaskInfo.AppendString( str );
		//
		//变更:
		str = _T("\r\n变更:\r\n");
		ListChange lst;
		m_pSeverFactory->GetChangeSever()->GetChangeByTaskId( obj.GetId(),  lst );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		for ( ListChange::iterator itor = lst.begin(); itor != lst.end(); itor++ )
		{
			int st = itor->GetStatus();
			tempUser.SetUserName_R(_T(""));
			m_pSeverFactory->GetLoginServer()->GetUserById( itor->GetAuditorId(), tempUser );
			tmpStr.Format(_T("变更时间：%s  变更内容：%s 变更原因：%s%s 变更结果：%s \r\n   评审人:%s "), itor->GetChangeDateTimeStr(), 
				     itor->GetChangeContent(), itor->GetResonSelsOptionString(),itor->GetChangeReason(), itor->GetChangeStatusString( st, true ),
					 tempUser.GetUserName_R() );
		
			if ( st == departAgree || st == departUnAgree || st == genernalUnAgree || st == genernalAgree )
			{
				statusStr.Format( _T(" 评审日期%s"), itor->GetReviewTimeStr() );
				tmpStr += statusStr;
			}
			tempUser.SetUserName_R(_T(""));
			m_pSeverFactory->GetLoginServer()->GetUserById( itor->GetGeneralAuditorId(), tempUser );
			tmpStr += _T(" 二级评审人:")+tempUser.GetUserName_R();
			if (  st == genernalUnAgree || st == genernalAgree  )
			{
				statusStr.Format( _T(" 评审日期%s"), itor->GetGeneralReviewTimeStr() );
				tmpStr += statusStr;
			}
			//m_wndTaskInfo.AddString( tmpStr );
			m_wndTaskInfo.AppendString( tmpStr );
			m_wndTaskInfo.AppendString(_T("................................................................................."));
			//m_wndTaskInfo.AddString(_T("................................................................................."));
		}
		
	}
	//this->RedrawWindow();
}

void COutputWnd::FillLoginWindow()
{
  const CUser &obj = m_pSeverFactory->GetLoginServer()->GetLoginUser();
  CString str;
  str.Format( _T("姓名:%s"), obj.GetUserName_R() );
  m_wndLoginInfo.AddString( str );
  
  CRole role;
  m_pSeverFactory->GetLoginServer()->GetRoleById( obj.GetRolId(), role );
  str.Format(_T("角色：%s"), role.GetRoleName() );
  m_wndLoginInfo.AddString( str );
  //
  str.Format(_T("所在部门：%s "),m_pSeverFactory->GetLoginServer()->GetOrganizationString()) ;
  m_wndLoginInfo.AddString( str );
}

void COutputWnd::UpdateFonts()
{
	//m_wndOutputBuild.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputDebug.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputFind.SetFont(&afxGlobalData.fontRegular);
}
void COutputWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}
	CRect rectClient;
	GetClientRect(rectClient);
	SvrOutputManager::GetObj()->ResetColumnWidth();
}
LRESULT COutputWnd::OnSelectSystemOutput( WPARAM wpara, LPARAM lpara )
{
	//this->SetActiveWindow()
	m_wndTabs.SetActiveTab( 1 );
	//this->SetDlgCtrlID( ID_LIST_SERVER_OUTPUT );
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	//ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList 消息处理程序

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("复制输出"));
}

//void COutputList::OnEditClear()
//{
//	MessageBox(_T("清除输出"));
//}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();
	}
}

