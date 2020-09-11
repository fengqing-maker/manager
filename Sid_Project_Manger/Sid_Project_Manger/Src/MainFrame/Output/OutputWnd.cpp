
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

	// ����ѡ�����:
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ������ѡ�����\n");
		return -1;      // δ�ܴ���
	}

	// �����������:
	DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
	const DWORD mulitColumnStyle = WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
		                          LBS_USETABSTOPS|/*LBS_STANDARD|*/LBS_OWNERDRAWVARIABLE | LBS_HASSTRINGS  ;
	/*const DWORD mulitColumnStyle = WS_CHILD |LBS_OWNERDRAWVARIABLE | 
		LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_VSCROLL | 
		WS_TABSTOP;*/
	if (!m_wndTaskInfo.Create(mulitColumnStyle, rectDummy, &m_wndTabs, 10) ||
		!m_wndLoginInfo.Create(dwStyle, rectDummy, &m_wndTabs, 3) )
	{
		TRACE0("δ�ܴ����������\n");
		return -1;      // δ�ܴ���
	}
	// �����б�:
	dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT |WS_HSCROLL | WS_VSCROLL
		| LVS_ALIGNLEFT | LVS_AUTOARRANGE | LVS_SHOWSELALWAYS;
	dwStyle = WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_ALIGNLEFT | 
		LVS_AUTOARRANGE | LVS_SHOWSELALWAYS;
	//dwStyle = WS_OVERLAPPED|WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_AUTOARRANGE|LVS_SHOWSELALWAYS|LVS_EDITLABELS;
	//dwStyle = WS_VSCROLL|WS_CHILD| WS_VISIBLE | WS_HSCROLL |LVS_ALIGNLEFT | LVS_REPORT;
	if (!m_svrOutputList.Create( dwStyle, rectDummy, &m_wndTabs,ID_LIST_SERVER_OUTPUT))
	{
		TRACE0("δ�ܴ���ϵͳ�������.\n");
		return -1;      // δ�ܴ���
	}
	DWORD  dwNewStyle =	LVS_EX_FULLROWSELECT |		/* ����ѡȡ */
		LVS_EX_HEADERDRAGDROP |		/* ��ͷ��ק */
		LVS_EX_INFOTIP;				/* tooltip��ʾ */
	m_svrOutputList.SetExtendedStyle(dwNewStyle);
	SvrOutputManager::GetObj()->RegisterAsOutputList( &m_svrOutputList );
	SvrOutputManager::GetObj()->InitOutputList();
	AdjustLayout();

	//m_wndTaskInfo.ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	
	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// ���б��ڸ��ӵ�ѡ�:
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

	// ѡ��ؼ�Ӧ��������������:
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
			CString str(_T("δ�����������ڣ�"));
			for ( ListDateItor itor = lstDate.begin(); itor != lstDate.end(); itor++ )
			{
				str += itor->Format( _T("%Y-%m-%d") );
				str += _T("��");
			}
			m_wndTaskInfo.AppendString( str, RGB(255, 255, 255), RGB(255, 0, 0) );
		}
		CString tmpStr;
		m_pTaskSever->GetWorkHourString();
		m_pTaskSever->GetWorkTypeObj().GetWorkTimeTypeById( obj.GetWorkTypeId() , tmpStr );
		str.Format(_T("������:%s  \r\n������ʱ:%s ������:%03d"), obj.GetName(),  tmpStr, obj.GetId() );
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
		
		str.Format(_T("\r\n������Ϣ:%s,%s,������%s, %s��%s����, �Ѷ�ϵ��%0.1f"), statusStr, prjName, ownerName, tempUser.GetUserName_R(), obj.GetCreateTimeStr(),
			          obj.GetDifficultyParam() );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		//�ƻ�
		str.Format(_T("�ƻ�ʱ��:%s~%s ��ʱ:%0.1f ʵ��ʱ��:%s~%s ��ʱ:%0.1f"),
			obj.GetPanStartDateShowStr(), obj.GetPanEndDateShowStr(), obj.GetPlanWorkHour(), 
			obj.GetFactStartDateShowStr(), obj.GetFactEndDateShowStr(), obj.GetFactWorkHour() );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		//����ע
		str = _T("����ע:\r\n") + obj.GetDec();
		//m_wndTaskInfo.AddString(  str );
		m_wndTaskInfo.AppendString( str );

		//����
		int st = obj.GetTaskStatus();
		m_pSeverFactory->GetLoginServer()->GetUserById( obj.GetMakeScoreUserId(), tempUser );
		if ( task_close == st || midStop == st )
		{/*ϵͳԤ���:%d*/
			str.Format(_T("\r\n ��������:%d ���֣�%d ������:%s ����ʱ��:%s ����������%s"), obj.GetForecastScore() ,obj.GetScore(), tempUser.GetUserName_R(),
				         obj.GetScoreTimeStr(), obj.GetScoreDec() );
			//m_wndTaskInfo.AddString( str );
			m_wndTaskInfo.AppendString( str );
		}else
		{/*ϵͳԤ���:%d*/
			str.Format(_T("\r\n ��������:%d ���֣��� ������:%s ����ʱ��:�� ������������"), obj.GetForecastScore(), tempUser.GetUserName_R() );
			m_wndTaskInfo.AppendString( str );
		}
		//����
		//if ( !obj.GetReview().IsEmpty() )
		//{
	 //       str.Format( _T("���Ź��̵���:%s "), obj.GetReview() );
		//	m_wndTaskInfo.AppendString( str );
		//}
		if ( !obj.GetPrjReview().IsEmpty() )
		{
			str.Format( _T("��Ŀ���̵���:%s "), obj.GetPrjReview() );
			m_wndTaskInfo.AppendString( str );
		}
		//����
		str = _T("\r\n����:\r\n");
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
		//���:
		str = _T("\r\n���:\r\n");
		ListChange lst;
		m_pSeverFactory->GetChangeSever()->GetChangeByTaskId( obj.GetId(),  lst );
		//m_wndTaskInfo.AddString( str );
		m_wndTaskInfo.AppendString( str );
		for ( ListChange::iterator itor = lst.begin(); itor != lst.end(); itor++ )
		{
			int st = itor->GetStatus();
			tempUser.SetUserName_R(_T(""));
			m_pSeverFactory->GetLoginServer()->GetUserById( itor->GetAuditorId(), tempUser );
			tmpStr.Format(_T("���ʱ�䣺%s  ������ݣ�%s ���ԭ��%s%s ��������%s \r\n   ������:%s "), itor->GetChangeDateTimeStr(), 
				     itor->GetChangeContent(), itor->GetResonSelsOptionString(),itor->GetChangeReason(), itor->GetChangeStatusString( st, true ),
					 tempUser.GetUserName_R() );
		
			if ( st == departAgree || st == departUnAgree || st == genernalUnAgree || st == genernalAgree )
			{
				statusStr.Format( _T(" ��������%s"), itor->GetReviewTimeStr() );
				tmpStr += statusStr;
			}
			tempUser.SetUserName_R(_T(""));
			m_pSeverFactory->GetLoginServer()->GetUserById( itor->GetGeneralAuditorId(), tempUser );
			tmpStr += _T(" ����������:")+tempUser.GetUserName_R();
			if (  st == genernalUnAgree || st == genernalAgree  )
			{
				statusStr.Format( _T(" ��������%s"), itor->GetGeneralReviewTimeStr() );
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
  str.Format( _T("����:%s"), obj.GetUserName_R() );
  m_wndLoginInfo.AddString( str );
  
  CRole role;
  m_pSeverFactory->GetLoginServer()->GetRoleById( obj.GetRolId(), role );
  str.Format(_T("��ɫ��%s"), role.GetRoleName() );
  m_wndLoginInfo.AddString( str );
  //
  str.Format(_T("���ڲ��ţ�%s "),m_pSeverFactory->GetLoginServer()->GetOrganizationString()) ;
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
// COutputList ��Ϣ�������

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
	MessageBox(_T("�������"));
}

//void COutputList::OnEditClear()
//{
//	MessageBox(_T("������"));
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

