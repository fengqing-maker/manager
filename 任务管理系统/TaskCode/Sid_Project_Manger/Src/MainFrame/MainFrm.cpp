
// MainFrm.cpp : CMainFrame ���ʵ��
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
	this->SetWindowText( _T("�������ϵͳ") );
	BOOL bNameValid;
	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
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

	// �����û�����Ĺ���������:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//����ط�����������߼��������еĸ��Ӷ�
	//��Excel��MySql���ݿ�
	if ( FALSE == m_actionFactory.GetConnectSever()->Connect()|| FALSE == m_watchFactory.GetConnectSever()->Connect())
	{
		//m_progressThread.CloseMsgDlg();
		return -1;
	}
	//��ʾ���ȿ�����ʱ�е���
	m_progressThread.create();		//����һ�����ڴ��������Ϣ���߳�
	m_progressThread.addObserver( this->GetSafeHwnd() );
	m_progressThread.ShowMsgDlg( MSG_CONNECT_SEVER );
	m_progressThread.CloseMsgDlg();
	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);
	
	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput,AFX_IDW_DOCKBAR_BOTTOM);

	// ���ù�������ͣ�����ڲ˵��滻
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// �����û�����Ĺ�����ͼ��
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
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
	//2015-3-24 lhz ��½ǰ������
	m_wndMenuBar.ShowPane( FALSE, FALSE, FALSE );		//�˵���
	m_wndToolBar.ShowPane( FALSE, FALSE, FALSE );		//������
	m_wndOutput.ShowPane(FALSE,FALSE,FALSE);			//���Ҵ���
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
//	cs.style &= ~(WS_MAXIMIZEBOX|WS_THICKFRAME); //2015-6-15 moidy
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	// �����������
	BOOL bNameValid;
	CRect rect;
	GetClientRect(&rect);

	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, rect.Width(), BOTTOM_VIEW_HEIGHT), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM| CBRS_FLOAT_MULTI/*, AFX_CBRS_AUTOHIDE | AFX_CBRS_AUTOHIDE*/))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}
	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
}

// CMainFrame ���

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
	// TODO: �ڴ���������������
	BOOL bVisiable=m_wndOutput.IsVisible();
	BOOL bHide = m_wndOutput.IsAutoHideMode();
	//������岻�ɼ��������ص�
	if (!bVisiable || bHide)
	{//���ɼ����ɼ�
	  
		ShowOutput( TRUE );
	}
	else //�ɼ������ɼ�
	{
		ShowOutput( FALSE );
	}
}

//�������Ҵ�����Զ���������ʾ
void CMainFrame::ShowOutput( BOOL isShow )
{
	if ( isShow )
	{//���ɼ����ɼ�
		m_wndOutput.ShowPane(TRUE, FALSE, TRUE);
		//CpaneDivider�ֿ���������
		CPaneDivider* pDefaultSlider = m_wndOutput.GetDefaultPaneDivider();		//�����б����еķָ���
		if (NULL != pDefaultSlider)
		{
			//pDefaultSlider->GetCurrentAlignment()���ش���Ķ��䷽ʽ
			//�л�����ɼ����Զ�����ģʽ֮��ĶԽ�
			m_wndOutput.SetAutoHideMode(FALSE, pDefaultSlider->GetCurrentAlignment(), NULL, FALSE);
		}

	}
	else //�ɼ������ɼ�
	{
		m_wndOutput.ShowPane(FALSE,FALSE,FALSE);
	}
}
// CMainFrame ��Ϣ�������

//����滻�б���е�һ����������ť�� ���� ҳ�����ʹ���Դ����¹���������ʾ �Զ����� �Ի���
void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
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
	//�Զ���
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
	// ���ཫִ�������Ĺ���

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// Ϊ�����û������������Զ��尴ť
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
//���ٴ����е��Ӵ���
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
		//��ȡ���������
		pfather = pWnd->GetParent();
		//pWnd->DestroyWindow();
		if ( pfather == this )
		{
			pWnd->DestroyWindow();
		}
		DestroyAllWindow( pfather );
	}
}

//��ͼ�ػ�
BOOL     CMainFrame::ReplaceView(CRuntimeClass *pViewClass)
{
	CCreateContext context;
	CDocument *pDoc = this->GetActiveDocument();
	CView *pOldActiveView = this->GetActiveView();
	pDoc->m_bAutoDelete = FALSE; //���ñ�־����������ͼ����ʱ����ɾ���ĵ�
	CWnd *pFather = pOldActiveView->GetParent();
	pOldActiveView->DestroyWindow();
	DestroyAllWindow( pFather );

	pDoc->m_bAutoDelete = TRUE; //���Ĭ�ϵı�־
	//��������ͼ
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
	//��½ϵͳUI
	CLoginActionInc*  pLoginAction = m_actionFactory.GetLoginServer();
	int removeCount = 0;
	//���ҡ����̡��Ƿ������ϵͳ���ݿ����Ƿ����
    if( FALSE == pLoginAction->FindLoginPower( projctTaskManger ) )				//�Ƿ�ӵ����ĿȨ��
	{
		removeCount++;
		m_wndMenuBar.RemoveButton( 1 );
	}
	//�������Ų���
	if( FALSE == pLoginAction->FindLoginPower( departTaskManger ) )				//�Ƿ�ӵ�в��Ź���Ȩ��
	{
		m_wndMenuBar.RemoveButton( 2-removeCount );
		removeCount++;
	}
	//��̨����
	if( FALSE == pLoginAction->FindLoginPower( backgroundManger ) )				//�Ƿ�ӵ�к�̨����Ȩ��
	{
		m_wndMenuBar.RemoveButton( 3-removeCount );
	}
	m_wndMenuBar.ShowPane( TRUE, FALSE, FALSE );
}



//��½�ɹ��󣬼���ҳ��
void CMainFrame::LoginLoading()
{
	//��ʾ�ַ�������½�ɹ������ڼ������ݡ�
	m_progressThread.ShowMsgDlg( MSG_INITE_LOADING );
	//
	BOOL ret = m_actionFactory.GetLoginServer()->Loading();
	if ( userExcOk == ret )
	{
		//CUser��������û���Ϣ
		const CUser &obj = m_actionFactory.GetLoginServer()->GetLoginUser();			//���û���Ϣ��������(��ȡ�û����û����е���Ϣ)
		m_actionFactory.GetTaskSever()->SetLoginUser( obj );							//��ȡһ��ѵ�������Ϣ
		m_watchFactory.GetTaskSever()->SetLoginUser( obj );
		CString temp;
		temp.Format( _T("�������ϵͳ-%s"), obj.GetUserName_R() );
		this->SetWindowText( temp );
		//
		m_progressThread.CloseMsgDlg();
		//ˢ���������ͼ
		ReplaceView(  RUNTIME_CLASS(CTaskTabView) );									//����Ҫˢ����ͼ��
		//��ʾ�˵���
		ShowMenu();
		//�ѽ��������ߴ���ʾ
		ShowWindow( SW_SHOWMAXIMIZED );
		m_wndOutput.SetActionFactory( &m_actionFactory );								//����һ������
		m_wndOutput.FillLoginWindow();
		//�ײ�����Ϣ����Ҳ������ʾ
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
		str.Format( _T("��������ʧ��[%s,������%d]�������µ�¼"), m_actionFactory.GetErrorString(ret), ret );
		AfxMessageBox( str );
		m_actionFactory.GetLoginServer()->QuitLogin();
	}
}
void CMainFrame::ExitApp()
{
	//ֹͣ�߳�
	m_watchTaskThread.StopCheckTask();
	m_actionFactory.GetLoginServer()->QuitLogin();
	m_watchTaskThread.StopCheckTask();
}


//��Ŀ�����ܵ�ʱ��
void CMainFrame::OnChangePrjMemger()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CProjectTabView ) )|| 
		   (NULL == pFather && !pFather->IsKindOf( RUNTIME_CLASS( CProjectTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CProjectTabView ) );
	}
}

//��ʾ������ͼ
void CMainFrame::OnChangeMyTaskView()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CTaskTabView ) ) || 
		   (NULL == pFather && !pFather->IsKindOf( RUNTIME_CLASS( CTaskTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CTaskTabView ) );
	}
}

//������ͼ
void CMainFrame::OnChangeDepartView()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CDepartTabView ) ) ||
		  (NULL != pFather && !pFather->IsKindOf( RUNTIME_CLASS( CDepartTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CDepartTabView ) );
	}
}


//��̨������ͼ
void CMainFrame::OnChangeBackMangerView()
{
	CWnd *pFather = GetActiveView()->GetParent();
	if ( !GetActiveView()->IsKindOf( RUNTIME_CLASS( CBackStageTabView ) ) ||
		 (NULL != pFather && !pFather->IsKindOf( RUNTIME_CLASS( CBackStageTabView ))) )
	{
		ReplaceView(  RUNTIME_CLASS( CBackStageTabView ) );
	}
}

//�޸�����
void CMainFrame::OnChangePwd()
{
	ChangePswDlg dlg( m_actionFactory.GetLoginServer() );
	dlg.DoModal();
}
//�߳�
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
	// TODO: �ڴ˴������Ϣ����������
	ExitApp();
}

//�Եײ���������������
LRESULT CMainFrame::OnFillTaskOutput( WPARAM wp, LPARAM lp )
{
	m_wndOutput.FillTaskWindow( wp );
	return 0;
}
CWnd* CMainFrame::OnSvOutputMsgComon( int taskType )
{
	//��ȡ��ǰ�������ͼ�������ָ��
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
		case HandleMyTask:		//������Ҫ���������
			activeViewId = pTabView->MyTask;
		     break;
		case  PublishTask://��������������
			activeViewId = pTabView->TaskPublishReview;
			break;
		//case ReviewScheduleTask:
		//	activeViewId = pTabView->ScheduleReview;
		//	break;//�������ȵ�������
		case ScoreTask:		//��������������
			activeViewId = pTabView->taskScoreReview;
			break;//��������������
		case ReviewChange: //����������������
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
//��������
LRESULT CMainFrame::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	CWnd* pWnd= OnSvOutputMsgComon( wp );
	if ( pWnd )
	{
		::PostMessage( pWnd->GetSafeHwnd(), MSG_ADD_CNT_CHANGE_UPDATE, wp, lp );
	}
	return 0;
}
//��������
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

