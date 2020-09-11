// EventView.cpp : 实现文件
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
	ON_COMMAND(IDM_DELETE_TASK, OnDeleteTask ) //删除任务
	ON_COMMAND(IDM_CHANGE_TASK, OnApplyForChangeTask ) //申请任务变更
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


// CEventView 诊断

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


// CEventView 消息处理程序
void CMyTaskView::OnInitialUpdate() 
{
	CBaseFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	m_searchTextBox.InsertString( 0, _T("所有任务") );
	m_searchTextBox.InsertString( 1, _T("需要处理的任务") );
	m_searchTextBox.InsertString( 2, _T("新建中的任务"));
	m_searchTextBox.InsertString( 3, _T("进行中的任务") );
	m_searchTextBox.InsertString( 4, _T("已关闭的任务") );
	m_searchTextBox.InsertString( 5, _T("未开始的任务") );
	m_searchTextBox.InsertString( 6, _T("待审核的任务") );
	m_searchTextBox.SetCurSel( 1 );

	m_editSerarchYear = COleDateTime::GetCurrentTime().GetYear();
	m_comboQuarter.InsertString( 0, _T("无") );
	m_comboQuarter.InsertString( 1, _T("第一季度") );
	m_comboQuarter.InsertString( 2, _T("第二季度") );
	m_comboQuarter.InsertString( 3, _T("第三季度") );
	m_comboQuarter.InsertString( 4, _T("第四季度") );
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
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	m_gridManger.reset( new CTaskGridManager(m_pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_myTaskGrid );			//注册表格视图	=  其实就是把表格加载到对应的类中
		m_gridManger->CreateGridView();								//创建表格属性
	}
	return 0;
}


int CMyTaskView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBaseFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
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
	// TODO: 在此添加控件通知处理程序代码
	int findIndex = m_searchTextBox.GetCurSel();
	GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( FALSE );
	int selQuart = m_comboQuarter.GetCurSel();
	if ( selQuart < 0 )
	{
		selQuart = 0;
	}
	switch( findIndex )
	{
	case 0://_T("所有任务")
		
		m_gridManger->UpdateShowAllTask( m_editSerarchYear, selQuart );
		break;
	case 1://_T("需要处理的任务")
		m_gridManger->UpdateOwnerTask();
		GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( TRUE );
		break;
	case 2://_T("新建中的任务")
		m_gridManger->UpdateNewingTask();
		break;
	case 3://_T("进行中的任务")
		m_gridManger->UpdateOngoingTask();
		GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( TRUE );
		break;
	case 4://_T("已完成的任务")
		m_gridManger->UpdateFinishTask();
		break;
	case 5://_T("未开始的任务")
		m_gridManger->UpdateWaitStartTask();
		break;
	case 6://_T("待审核的任务")
		m_gridManger->UpdateWaitReveiwTask();
		break;
	}
	this->RedrawWindow();
}

//新建任务
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
	// TODO: 在此添加专用代码和/或调用基类
	CBaseFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
//网格选择改变变化
void CMyTaskView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//网格文本框编辑结束
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CBaseFormView::OnPaint()
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
//导出工时
void CMyTaskView::OnBnClickedBtnExportHour()
{
	ExportWorkHourDlg dlg;
	dlg.DoModal();
}

//更新进度
void CMyTaskView::OnBnClickedBtnUpdate()
{
	if ( m_gridManger.get() && m_comboUpdateDate.GetCurSel() > 0 )
	{
		m_gridManger->OnUpdateSchdule();
		ResetSelDateList();
	}
}

//日期的改变、进度更新到数据库
void CMyTaskView::ResetSelDateList()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	int selPos = m_comboUpdateDate.GetCurSel();
	CString selDateStr;
	//获取更新日期的文本数据
	if ( selPos >= 0 )
	{
		m_comboUpdateDate.GetLBText( selPos, selDateStr );
	}
	m_comboUpdateDate.ResetContent();			//删除列表框内所有数据行
	const ListDate &selDateList = m_gridManger->GetSelListDate();
	m_comboUpdateDate.InsertString( 0, _T("") );
	int cnt = 1;
	CString strTmp;
	selPos = 0;
	BOOL isFind = FALSE;
	//获取未更新任务的时间信息放入
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
	//保证更新以后选中的还是更新前选中的
	m_comboUpdateDate.SetCurSel( selPos );
	if (  FALSE == isFind && selDateList.size() > 0 )
	{
		m_comboUpdateDate.GetLBText( selPos, strTmp );
		m_gridManger->SetUpdateDate( strTmp );
	}
	GetDlgItem( IDC_BTN_UPDATE )->EnableWindow( m_comboUpdateDate.GetCount() > 0 );
}

//时间列表框的变化
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
		AfxMessageBox(_T("请选择更新进度的日期"));
	}
	
}

