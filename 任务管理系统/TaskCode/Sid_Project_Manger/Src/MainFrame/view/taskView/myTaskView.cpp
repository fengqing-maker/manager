// EventView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "myTaskView.h"
#include "../../MainFrm.h"
#include "../../Dlg/NewTaskDlg.h"
#include "../../messageDef.h"
#include "Src/MainFrame/Dlg/workHour/ExportWorkHourDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView


IMPLEMENT_DYNCREATE(CMyTaskView, CBaseFormView)

CMyTaskView::CMyTaskView()
	: CBaseFormView(CMyTaskView::IDD)
{
	m_editSerarchYear = 0;
	m_pActionFactory = NULL;
	//	m_pTaskSever = NULL;
}

CMyTaskView::~CMyTaskView()
{
	m_pActionFactory = NULL;
	//	m_pTaskSever = NULL;
}

void CMyTaskView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO1, m_searchTextBox);
	DDX_Control(pDX, IDC_COMBO_UPDATED_ATE, m_comboUpdateDate);
	DDX_Text(pDX, IDC_EDIT_SEARCH_YEAR2, m_editSerarchYear);
	DDX_Control(pDX, IDC_COMBO_QUARTER2, m_comboQuarter);
}

BEGIN_MESSAGE_MAP(CMyTaskView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CMyTaskView::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_NEW_TASK, &CMyTaskView::OnBnClickedBtnNewTask)

	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_MYTASK, OnGridEndEidt )
	ON_NOTIFY( GVN_SELCHANGED, ID_GRID_MYTASK, OnGridSelChange)

	ON_NOTIFY(NM_CLICK,	ID_GRID_MYTASK,	OnClickGrid)
	ON_NOTIFY(NM_RCLICK, ID_GRID_MYTASK,	OnRclickGrid)
	ON_COMMAND(IDM_DELETE_TASK, OnDeleteTask ) //ɾ������
	ON_COMMAND(IDM_CHANGE_TASK, OnApplyForChangeTask ) //����������
	//
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
	ON_MESSAGE( GRID_MYTASK_UPDATE_DATE, OnUpdateSelDateList )

	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_EXPORT_HOUR, &CMyTaskView::OnBnClickedBtnExportHour)
	ON_BN_CLICKED(IDC_BTN_UPDATE, &CMyTaskView::OnBnClickedBtnUpdate)
	ON_CBN_SELCHANGE(IDC_COMBO_UPDATED_ATE, &CMyTaskView::OnCbnSelchangeComboUpdatedAte)
END_MESSAGE_MAP()


// CEventView ���

#ifdef _DEBUG
void CMyTaskView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CMyTaskView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView ��Ϣ�������
void CMyTaskView::OnInitialUpdate() 
{
	CBaseFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_searchTextBox.InsertString( 0, _T("��������") );
	m_searchTextBox.InsertString( 1, _T("��Ҫ���������") );
	m_searchTextBox.InsertString( 2, _T("�½��е�����"));
	m_searchTextBox.InsertString( 3, _T("�����е�����") );
	m_searchTextBox.InsertString( 4, _T("�ѹرյ�����") );
	m_searchTextBox.InsertString( 5, _T("δ��ʼ������") );
	m_searchTextBox.InsertString( 6, _T("����˵�����") );
	m_searchTextBox.SetCurSel( 1 );

	m_editSerarchYear = COleDateTime::GetCurrentTime().GetYear();
	m_comboQuarter.InsertString( 0, _T("��") );
	m_comboQuarter.InsertString( 1, _T("��һ����") );
	m_comboQuarter.InsertString( 2, _T("�ڶ�����") );
	m_comboQuarter.InsertString( 3, _T("��������") );
	m_comboQuarter.InsertString( 4, _T("���ļ���") );
	int selQuarter = 0/*(COleDateTime::GetCurrentTime().GetMonth()-1)/3 + 1*/;
	m_comboQuarter.SetCurSel( selQuarter );
	ResetSelDateList();
	UpdateData( FALSE );
}

//
BOOL CMyTaskView::CreateCtrl()
{
	CRect rect;
	rect.SetRectEmpty();
	if (!m_myTaskGrid.Create(rect,this,ID_GRID_MYTASK ))
	{//��������
		TRACE("Զ����������ϢTable����ʧ��.\n");
		return -1;
	}
	m_gridManger.reset( new CTaskGridManager(m_pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_myTaskGrid );			//ע������ͼ	=  ��ʵ���ǰѱ����ص���Ӧ������
		m_gridManger->CreateGridView();								//�����������
	}
	return 0;
}


int CMyTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������
	m_pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != m_pActionFactory );
	return CreateCtrl();
}


void CMyTaskView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if ( m_myTaskGrid.GetSafeHwnd() )
	{
		m_myTaskGrid.MoveWindow( 0, 100, cx, cy-100);
	}

}


void CMyTaskView::OnBnClickedBtnSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int findIndex = m_searchTextBox.GetCurSel();
	GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( FALSE );
	int selQuart = m_comboQuarter.GetCurSel();
	if ( selQuart < 0 )
	{
		selQuart = 0;
	}
	switch( findIndex )
	{
	case 0://_T("��������")
		
		m_gridManger->UpdateShowAllTask( m_editSerarchYear, selQuart );
		break;
	case 1://_T("��Ҫ���������")
		m_gridManger->UpdateOwnerTask();
		GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( TRUE );
		break;
	case 2://_T("�½��е�����")
		m_gridManger->UpdateNewingTask();
		break;
	case 3://_T("�����е�����")
		m_gridManger->UpdateOngoingTask();
		GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( TRUE );
		break;
	case 4://_T("����ɵ�����")
		m_gridManger->UpdateFinishTask();
		break;
	case 5://_T("δ��ʼ������")
		m_gridManger->UpdateWaitStartTask();
		break;
	case 6://_T("����˵�����")
		m_gridManger->UpdateWaitReveiwTask();
		break;
	}
	this->RedrawWindow();
}

//�½�����
void CMyTaskView::OnBnClickedBtnNewTask()
{
	CNewTaskDlg dlg( NULL, NULL );
	if ( IDOK == dlg.DoModal() )
	{
		OnBnClickedBtnSearch();
	}
}

void CMyTaskView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: �ڴ����ר�ô����/����û���
	CBaseFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
//����ѡ��ı�仯
void CMyTaskView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//�����ı���༭����
void CMyTaskView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}
void CMyTaskView::OnRclickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridRclick( pNMHDR, pResult );
	}
}
void CMyTaskView::OnDeleteTask()
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnDeleteTask( );
		OnBnClickedBtnSearch();
	}
}
void CMyTaskView::OnApplyForChangeTask()
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnApplyForChangeTask( );
	}
}

void CMyTaskView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CBaseFormView::OnPaint()
	 CBaseFormView::OnPaint();
}
void CMyTaskView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridSelChange( pNMHDR, pResult );
	}
}
LRESULT CMyTaskView::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_searchTextBox.SetCurSel( 1 );
		m_gridManger->UpdateOwnerTask();
		m_gridManger->SelectTaskLine( lp );
	}
	return 0;
}
LRESULT CMyTaskView::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_searchTextBox.SetCurSel( 1 );
		m_gridManger->UpdateOwnerTask();
	}
	return 0;
}
LRESULT CMyTaskView::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->UpdateTaskContent( lp );
	}
	return 0;
}
LRESULT CMyTaskView::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->SelectTaskLine( lp );
	}
	return 0;
}
LRESULT CMyTaskView::OnUpdateSelDateList( WPARAM wp, LPARAM lp )
{//
	ResetSelDateList();
	return 0;
}
//������ʱ
void CMyTaskView::OnBnClickedBtnExportHour()
{
	ExportWorkHourDlg dlg;
	dlg.DoModal();
}

//���½���
void CMyTaskView::OnBnClickedBtnUpdate()
{
	if ( m_gridManger.get() && m_comboUpdateDate.GetCurSel() > 0 )
	{
		m_gridManger->OnUpdateSchdule();
		ResetSelDateList();
	}
}

//���ڵĸı䡢���ȸ��µ����ݿ�
void CMyTaskView::ResetSelDateList()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	int selPos = m_comboUpdateDate.GetCurSel();
	CString selDateStr;
	//��ȡ�������ڵ��ı�����
	if ( selPos >= 0 )
	{
		m_comboUpdateDate.GetLBText( selPos, selDateStr );
	}
	m_comboUpdateDate.ResetContent();			//ɾ���б��������������
	const ListDate &selDateList = m_gridManger->GetSelListDate();
	m_comboUpdateDate.InsertString( 0, _T("") );
	int cnt = 1;
	CString strTmp;
	selPos = 0;
	BOOL isFind = FALSE;
	//��ȡδ���������ʱ����Ϣ����
	for ( ListDateConstItor itor = selDateList.begin(); itor != selDateList.end(); itor++ )
	{
		strTmp =  (*itor).Format(_T("%Y-%m-%d") );
		 m_comboUpdateDate.InsertString( cnt, strTmp );
		if ( selDateStr == strTmp )
		{
			selPos = cnt;
			isFind = TRUE;
		}
	    cnt++;
	}
	//��֤�����Ժ�ѡ�еĻ��Ǹ���ǰѡ�е�
	m_comboUpdateDate.SetCurSel( selPos );
	if (  FALSE == isFind && selDateList.size() > 0 )
	{
		m_comboUpdateDate.GetLBText( selPos, strTmp );
		m_gridManger->SetUpdateDate( strTmp );
	}
	GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( m_comboUpdateDate.GetCount() > 0 );
}

//ʱ���б��ı仯
void CMyTaskView::OnCbnSelchangeComboUpdatedAte()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	int selPos = m_comboUpdateDate.GetCurSel();
	CString selDateStr;
	if ( selPos > 0  )
	{
		m_comboUpdateDate.GetLBText( selPos, selDateStr );
		m_gridManger->SetUpdateDate( selDateStr );
	}
	else
	{
		AfxMessageBox(_T("��ѡ����½��ȵ�����"));
	}
	
}

