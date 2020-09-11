
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "Sid_Project_Manger.h"
#include "MainFrm.h"
#include "messageDef.h"
#include "view/taskView/TaskTabView.h"
#include "view/prjView/ProjectTabView.h"
#include "view/DepartView/DepartTabView.h"
#include "view/BackView/BackStageTabView.h"
#include "./Dlg/hangePswDlg.h"

// CMainFrame
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND( ID_PRJ_MANGER, OnChangePrjMemger )	
	ON_COMMAND( ID_CHANGE_MYTASK, OnChangeMyTaskView )
	ON_COMMAND( ID_DEPART_MANGE, OnChangeDepartView )
	ON_COMMAND( ID_MENUE_BACK_MANGER, OnChangeBackMangerView)
    ON_COMMAND( ID_CHANGE_PWD, OnChangePwd )
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_MESSAGE( MAINFRAME_MSG_TASKID, OnFillTaskOutput )
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
	ON_MESSAGE( MSG_WORKHOUR_CHECK, OnCheckWorkHour )
	ON_MESSAGE( MSG_UPDATE_TITLE_CNT, OnUpdateTabName )
    ON_MESSAGE( MSG_CLOSE_APP, OnCloseApp )
	ON_REGISTERED_MESSAGE( AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_isLogin =false;
}

CMainFrame::~CMainFrame()
{
	m_progressThread.destroy();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	this->SetWindowText( _T("任务管理系统") );
	BOOL bNameValid;
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}
	//richEdit
	m_richEditTmp.Create( WS_CHILD, CRect(0, 0, 0, 0), this, IDC_RICHEDIT_TEMP );
	//
	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//这个地方很巧妙，开发者减少了运行的复杂度
	//打开Excel与MySql数据库
	if ( FALSE == m_actionFactory.GetConnectSever()->Connect()|| FALSE == m_watchFactory.GetConnectSever()->Connect())
	{
		//m_progressThread.CloseMsgDlg();
		return -1;
	}
	//提示进度框，连接时有点慢
	m_progressThread.create();		//这是一条用于处理进度消息的线程
	m_progressThread.addObserver( this->GetSafeHwnd() );
	m_progressThread.ShowMsgDlg( MSG_CONNECT_SEVER );
	m_progressThread.CloseMsgDlg();
	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	
	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput,AFX_IDW_DOCKBAR_BOTTOM);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);
	//2015-3-24 lhz 登陆前的隐藏
	m_wndMenuBar.ShowPane( FALSE, FALSE, FALSE );		//菜单栏
	m_wndToolBar.ShowPane( FALSE, FALSE, FALSE );		//工具栏
	m_wndOutput.ShowPane(FALSE,FALSE,FALSE);			//悬挂窗体
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
//	cs.style &= ~(WS_MAXIMIZEBOX|WS_THICKFRAME); //2015-6-15 moidy
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	// 创建输出窗口
	BOOL bNameValid;
	CRect rect;
	GetClientRect(&rect);

	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, rect.Width(), BOTTOM_VIEW_HEIGHT), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM| CBRS_FLOAT_MULTI/*, AFX_CBRS_AUTOHIDE | AFX_CBRS_AUTOHIDE*/))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::OnShowInfoView()
{
	// TODO: 在此添加命令处理程序代码
	BOOL bVisiable=m_wndOutput.IsVisible();
	BOOL bHide = m_wndOutput.IsAutoHideMode();
	//如果窗体不可见或者隐藏的
	if (!bVisiable || bHide)
	{//不可见到可见
	  
		ShowOutput( TRUE );
	}
	else //可见到不可见
	{
		ShowOutput( FALSE );
	}
}

//关于悬挂窗体的自动隐藏与显示
void CMainFrame::ShowOutput( BOOL isShow )
{
	if ( isShow )
	{//不可见到可见
		m_wndOutput.ShowPane(TRUE, FALSE, TRUE);
		//CpaneDivider分开两个窗格
		CPaneDivider* pDefaultSlider = m_wndOutput.GetDefaultPaneDivider();		//返回列表窗格中的分割线
		if (NULL != pDefaultSlider)
		{
			//pDefaultSlider->GetCurrentAlignment()返回窗格的对其方式
			//切换窗体可见，自动隐藏模式之间的对接
			m_wndOutput.SetAutoHideMode(FALSE, pDefaultSlider->GetCurrentAlignment(), NULL, FALSE);
		}

	}
	else //可见到不可见
	{
		m_wndOutput.ShowPane(FALSE,FALSE,FALSE);
	}
}
// CMainFrame 消息处理程序

//如何替换列表框中的一个工具栏按钮在 命令 页的命令，使可以创建新工具栏和显示 自定义项 对话框。
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
	
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	//自定义
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
}
//销毁窗体中的子窗体
void     CMainFrame::DestroyAllWindow( CWnd *pWnd )
{
	CWnd *pfather = NULL; 
	//if ( pWnd && pWnd != this ) //1111111111
	//{
	//	pfather = pWnd->GetParent();
	//	pWnd->DestroyWindow();
	//	DestroyAllWindow( pfather );
	//}
	if ( pWnd && pWnd != this ) //1111111111
	{
		//获取窗体的属主
		pfather = pWnd->GetParent();
		//pWnd->DestroyWindow();
		if ( pfather == this )
		{
			pWnd->DestroyWindow();
		}
		DestroyAllWindow( pfather );
	}
}

//视图重绘
BOOL     CMainFrame::ReplaceView(CRuntimeClass *pViewClass)
{
	CCreateContext context;
	CDocument *pDoc = this->GetActiveDocument();
	CView *pOldActiveView = this->GetActiveView();
	pDoc->m_bAutoDelete = FALSE; //设置标志，这样当视图销毁时不会删除文档
	CWnd *pFather = pOldActiveView->GetParent();
	pOldActiveView->DestroyWindow();
	DestroyAllWindow( pFather );

	pDoc->m_bAutoDelete = TRUE; //设回默认的标志
	//创建新视图
	context.m_pNewViewClass=pViewClass;
	context.m_pCurrentDoc = pDoc;
	context.m_pNewDocTemplate = NULL;
	context.m_pLastView = NULL;
	context.m_pCurrentFrame = NULL;
	CView *pActiveView=(CView *)CreateView(&context);
	SetActiveView(pActiveView);
	RecalcLayout();

	//pActiveView->GetParent()->SetFocus();
	
	return TRUE;
}
void CMainFrame::ShowMenu()
{
	//登陆系统UI
	CLoginActionInc*  pLoginAction = m_actionFactory.GetLoginServer();
	int removeCount = 0;
	//查找“工程”是否存在于系统数据库中是否存在
    if( FALSE == pLoginAction->FindLoginPower( projctTaskManger ) )				//是否拥有项目权限
	{
		removeCount++;
		m_wndMenuBar.RemoveButton( 1 );
	}
	//遍历部门部门
	if( FALSE == pLoginAction->FindLoginPower( departTaskManger ) )				//是否拥有部门管理权限
	{
		m_wndMenuBar.RemoveButton( 2-removeCount );
		removeCount++;
	}
	//后台管理
	if( FALSE == pLoginAction->FindLoginPower( backgroundManger ) )				//是否拥有后台管理权限
	{
		m_wndMenuBar.RemoveButton( 3-removeCount );
	}
	m_wndMenuBar.ShowPane( TRUE, FALSE, FALSE );
}



//登陆成功后，加载页面
void CMainFrame::LoginLoading()
{
	//显示字符串“登陆成功，正在加载数据”
	m_progressThread.ShowMsgDlg( MSG_INITE_LOADING );
	//
	BOOL ret = m_actionFactory.GetLoginServer()->Loading();
	if ( userExcOk == ret )
	{
		//CUser好像包含用户信息
		const CUser &obj = m_actionFactory.GetLoginServer()->GetLoginUser();			//把用户信息拷贝过来(获取用户在用户表中的信息)
		m_actionFactory.GetTaskSever()->SetLoginUser( obj );							//获取一大堆的任务信息
		m_watchFactory.GetTaskSever()->SetLoginUser( obj );
		CString temp;
		temp.Format( _T("任务管理系统-%s"), obj.GetUserName_R() );
		this->SetWindowText( temp );
		//
		m_progressThread.CloseMsgDlg();
		//刷出任务表视图
		ReplaceView(  RUNTIME_CLASS(CTaskTabView) );									//终于要刷入视图了
		//显示菜单栏
		ShowMenu();
		//把界面以最大尺寸显示
		ShowWindow( SW_SHOWMAXIMIZED );
		m_wndOutput.SetActionFactory( &m_actionFactory );								//传入一个参数
		m_wndOutput.FillLoginWindow();
		//底部的信息窗体也进行显示
		ShowOutput( TRUE );
		UpdateWindow();

        m_watchTaskThread.RegistActionSever( m_watchFactory );
		m_watchTaskThread.RegistOrginSever( m_actionFactory );
		m_watchTaskThread.addObserver( m_wndOutput.GetSafeHwnd() );
		m_watchTaskThread.addObserver( this->GetSafeHwnd() );
		m_watchTaskThread.create();
		m_watchTaskThread.StartCheckTask();
	}
	else
	{
		m_progressThread.CloseMsgDlg();
		CString str;
		str.Format( _T("加载数据失败[%s,错误编号%d]，请重新登录"), m_actionFactory.GetErrorString(ret), ret );
		AfxMessageBox( str );
		m_actionFactory.GetLoginServer()->QuitLogin();
	}
}
void CMainFrame::ExitApp()
{
	//停止线程
	m_watchTaskThread.StopCheckTask();
	m_actionFactory.GetLoginServer()->QuitLogin();
	m_watchTaskThread.StopCheckTask();
}


//项目管理功能的时候
void CMainFrame::OnChangePrjMemger()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CProjectTabView ) )|| 
		   (NULL == pFather && !pFather->IsKindOf( RUNTIME_CLASS( CProjectTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CProjectTabView ) );
	}
}

//显示任务视图
void CMainFrame::OnChangeMyTaskView()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CTaskTabView ) ) || 
		   (NULL == pFather && !pFather->IsKindOf( RUNTIME_CLASS( CTaskTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CTaskTabView ) );
	}
}

//部门视图
void CMainFrame::OnChangeDepartView()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CDepartTabView ) ) ||
		  (NULL != pFather && !pFather->IsKindOf( RUNTIME_CLASS( CDepartTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CDepartTabView ) );
	}
}


//后台管理视图
void CMainFrame::OnChangeBackMangerView()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CBackStageTabView ) ) ||
		 (NULL != pFather && !pFather->IsKindOf( RUNTIME_CLASS( CBackStageTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CBackStageTabView ) );
	}
}

//修改密码
void CMainFrame::OnChangePwd()
{
	ChangePswDlg dlg( m_actionFactory.GetLoginServer() );
	dlg.DoModal();
}
//线程
void     CMainFrame::ShowProgressMsg( int infoId, CString ext   )
{
	m_progressThread.ShowMsgDlg( infoId );
}
void     CMainFrame::CloseMsgDlg()
{
	m_progressThread.CloseMsgDlg();
}

void CMainFrame::OnDestroy()
{
	CFrameWndEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	ExitApp();
}

//对底部窗体进行数据填充
LRESULT CMainFrame::OnFillTaskOutput( WPARAM wp, LPARAM lp )
{
	m_wndOutput.FillTaskWindow( wp );
	return 0;
}
CWnd* CMainFrame::OnSvOutputMsgComon( int taskType )
{
	//获取当前窗体的视图、窗体的指针
	CView *pActiew = GetActiveView();
	CWnd *pFather = pActiew->GetParent();
	CTaskTabView *pTabView = NULL; 
	if ( pActiew->IsKindOf( RUNTIME_CLASS( CTaskTabView ) ) )
	{
		pTabView = (CTaskTabView *)pActiew;
	}
	else if ( NULL != pFather && pFather->IsKindOf( RUNTIME_CLASS( CTaskTabView )) )
	{
		pTabView = (CTaskTabView *)pFather;
	}
	else if( pFather->GetParent() && pFather->GetParent()->IsKindOf( RUNTIME_CLASS( CTaskTabView ) ) )
	{
		pTabView = (CTaskTabView *)pFather->GetParent();
	}
	else
	{
		ReplaceView( RUNTIME_CLASS( CTaskTabView ) );
		pTabView =  (CTaskTabView *)GetActiveView();
	}
	if ( pTabView )
	{
		int activeViewId = 0;
		switch( taskType )
		{
		case HandleMyTask:		//新增我要处理的任务
			activeViewId = pTabView->MyTask;
		     break;
		case  PublishTask://新增待发布任务
			activeViewId = pTabView->TaskPublishReview;
			break;
		//case ReviewScheduleTask:
		//	activeViewId = pTabView->ScheduleReview;
		//	break;//新增进度点评任务
		case ScoreTask:		//新增待评分任务
			activeViewId = pTabView->taskScoreReview;
			break;//新增待评分任务
		case ReviewChange: //新增待评审变更任务
			activeViewId = pTabView->ChangeReview;
			break;
		case  WorkHourView:
			activeViewId = pTabView->TabWorkHourView;
			break;
		default:
			return NULL;
		}
		
		if ( activeViewId != pTabView->GetActiveViewTabFlg( ) )
		{
			pTabView->SetActiveViewByTabFlg( activeViewId );
		}
		return pTabView->GetActiveView();
	}
	return NULL;
}
//增加任务
LRESULT CMainFrame::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	CWnd* pWnd= OnSvOutputMsgComon( wp );
	if ( pWnd )
	{
		::PostMessage( pWnd->GetSafeHwnd(), MSG_ADD_CNT_CHANGE_UPDATE, wp, lp );
	}
	return 0;
}
//减少任务
LRESULT CMainFrame::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	CWnd* pWnd = OnSvOutputMsgComon( wp );
	if ( pWnd )
	{
		::PostMessage( pWnd->GetSafeHwnd(), MSG_DECREASE_CNT_CHANGE_UPDATE, wp, lp );
	}
	return 0;
}
//
LRESULT CMainFrame::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	CWnd* pWnd = OnSvOutputMsgComon( wp );
	if ( pWnd )
	{
		::PostMessage( pWnd->GetSafeHwnd(), MSG_CONTENT_CHANGE_UPDAE, wp, lp );
	}
	return 0;
}
LRESULT CMainFrame::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	CWnd* pWnd = OnSvOutputMsgComon( wp );
	if ( pWnd )
	{
		::PostMessage( pWnd->GetSafeHwnd(), MSG_SELECT_TASK_ITEM, wp, lp );
	}
	return 0;
}
LRESULT CMainFrame::OnCheckWorkHour( WPARAM wp, LPARAM lp )
{
	CWnd* pWnd= OnSvOutputMsgComon( wp );
	if ( pWnd )
	{
		::PostMessage( pWnd->GetSafeHwnd(), MSG_WORKHOUR_CHECK, wp, lp );
	}
	return 0;
}
LRESULT  CMainFrame::OnCloseApp( WPARAM , LPARAM )
{
	this->CloseWindow();
	return 0;
}
LRESULT CMainFrame::OnUpdateTabName( WPARAM wp, LPARAM lp )
{
	CView *pActiew = GetActiveView();
	CWnd *pFather = pActiew->GetParent();
	CTaskTabView *pTabView = NULL; 
	if ( pActiew->IsKindOf( RUNTIME_CLASS( CTaskTabView ) ) )
	{
		pTabView = (CTaskTabView *)pActiew;
	}else if ( NULL != pFather && pFather->IsKindOf( RUNTIME_CLASS( CTaskTabView )) )
	{
		pTabView = (CTaskTabView *)pFather;
	}else if( pFather->GetParent() && pFather->GetParent()->IsKindOf( RUNTIME_CLASS( CTaskTabView ) ) )
	{
		pTabView = (CTaskTabView *)pFather->GetParent();
	}
	else
	{
		return 0;
	}
	::PostMessage( pTabView->GetSafeHwnd(), MSG_UPDATE_TITLE_CNT, wp, lp );
	return 0;
}

