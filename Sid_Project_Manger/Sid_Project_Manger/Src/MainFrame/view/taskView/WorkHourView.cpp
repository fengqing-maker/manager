#include "stdafx.h"
#include "WorkHourView.h"
#include "../../MainFrm.h"
#include "../../messageDef.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/MainFrame/Dlg/DlgUserChose.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CEventView
IMPLEMENT_DYNCREATE(CWorkHourView, CBaseFormView)

CWorkHourView::CWorkHourView()
	: CBaseFormView(CWorkHourView::IDD)
{
	m_currDate = COleDateTime::GetCurrentTime();
	m_assginWorkHour = FALSE;
	m_selYear = m_currDate.GetYear();
}

CWorkHourView::~CWorkHourView()
{
}

void CWorkHourView::DoDataExchange(CDataExchange* pDX)
{
	CBaseFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventView)
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_FIND_SCHE, m_findTextBox);
	DDX_Text(pDX, IDC_EDIT_YEAR, m_selYear);
	DDX_Control( pDX, IDC_COMBO_MONTH, m_monthBox );
}

BEGIN_MESSAGE_MAP(CWorkHourView, CBaseFormView)
	//{{AFX_MSG_MAP(CEventView)
	ON_NOTIFY( GVN_ENDLABELEDIT, ID_GRID_SCHEDULE, OnGridEndEidt )
	ON_NOTIFY(NM_CLICK,	ID_GRID_SCHEDULE,	OnClickGrid)
	ON_NOTIFY(GVN_SELCHANGED,	ID_GRID_SCHEDULE,	OnGridSelChange)
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_FRESH_SCH, &CWorkHourView::OnBnClickedBtnFresh)
	ON_CBN_SELCHANGE(IDC_COMBO_FIND_SCHE, &CWorkHourView::OnCbnEditchangeComboFind)

	//
	ON_MESSAGE( MSG_ADD_CNT_CHANGE_UPDATE, OnAddCntChangeUpdate )
	ON_MESSAGE( MSG_DECREASE_CNT_CHANGE_UPDATE, OnDecreateCntChangeUpdate )
	ON_MESSAGE( MSG_CONTENT_CHANGE_UPDAE, OnContentChangeUpdate )
	ON_MESSAGE( MSG_WORKHOUR_CHECK, OnCheckWorkHour )
	ON_MESSAGE( MSG_SELECT_TASK_ITEM, OnSelGridLineView )
	ON_BN_CLICKED(IDC_BTN_SELECT, &CWorkHourView::OnBnClickedBtnSelect)
	ON_BN_CLICKED(IDC_BTN_CHECK, &CWorkHourView::OnBnClickedBtnCheck)
	ON_CBN_SELCHANGE(IDC_COMBO_YEAR, &CWorkHourView::OnCbnSelchangeComboYear)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, &CWorkHourView::OnCbnSelchangeComboMonth)
	ON_BN_CLICKED(IDC_BTN_EXPORT_HOUR, &CWorkHourView::OnBnClickedBtnExportHour)

	ON_BN_CLICKED(IDC_SET_DEF_DATE, &CWorkHourView::OnBnClickedSetDefDate)
END_MESSAGE_MAP()


// CEventView 诊断

#ifdef _DEBUG
void CWorkHourView::AssertValid() const
{
	CBaseFormView::AssertValid();
}
#ifndef _WIN32_WCE
void CWorkHourView::Dump(CDumpContext& dc) const
{
	CBaseFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventView 消息处理程序
void CWorkHourView::OnInitialUpdate() 
{
	BOOL isChange = m_bFirstActive;
	CBaseFormView::OnInitialUpdate();
	if ( isChange )
	{
		m_assginWorkHour = m_pActionFactory->GetLoginServer()->FindLoginPower( allocateWorkHour );
		OnInitMonth();
		InitSearchCondition();
		 OnInitUserName();
		if ( m_assginWorkHour )
		{
			m_monthBox.EnableWindow( TRUE );
			GetDlgItem( IDC_EDIT_YEAR )->EnableWindow( TRUE );
			GetDlgItem( IDC_BTN_SELECT )->ShowWindow( SW_SHOW ) ;
			GetDlgItem( IDC_BTN_EXPORT_HOUR )->ShowWindow( SW_SHOW ) ;
		}else
		{
			GetDlgItem( IDC_EDIT_YEAR )->EnableWindow( FALSE );
			GetDlgItem( IDC_SET_DEF_DATE )->ShowWindow( SW_HIDE );
		}

	}
}

int CWorkHourView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	return CreateCtrl();
}
int CWorkHourView::CreateCtrl()
{
	CRect rect;
	this->GetWindowRect( rect );
	ClientToScreen( rect );
	
	CRect gridRect( 0, 80, rect.Width(), rect.Height() );
	if (!m_taskGrid.Create(gridRect,this,ID_GRID_SCHEDULE ))
	{//带滚动条
		TRACE("远方服务器信息Table创建失败.\n");
		return -1;
	}
	CActionServerFactory* pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != pActionFactory );
	m_pActionFactory = pActionFactory;
	m_showUser = m_pActionFactory->GetLoginServer()->GetLoginUser();
	m_gridManger.reset( new CWorkHourManger(pActionFactory));
	if ( m_gridManger.get() )
	{
		m_gridManger->RegisterAsGridView( &m_taskGrid );
		m_gridManger->CreateGridView();
	}
	return 0;
}

//void CWorkHourView::OnBnClickedBtnFresh()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	//m_gridManger->UpdateDbData();
//}
//网格选择改变变化
void CWorkHourView::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnClickGrid( pNMHDR, pResult );
	}
}
//网格文本框编辑结束
void CWorkHourView::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->OnGridEndEidt( pNMHDR, pResult );
	}
}


void CWorkHourView::OnSize(UINT nType, int cx, int cy)
{
	CBaseFormView::OnSize(nType, cx, cy, m_taskGrid );

	// TODO: 在此处添加消息处理程序代码
}

void CWorkHourView::OnInitMonth()
{
	m_monthBox.InsertString( 0, _T("1月") );
	m_monthBox.InsertString( 1, _T("2月") );
	m_monthBox.InsertString( 2, _T("3月") );
	m_monthBox.InsertString( 3, _T("4月") );
	m_monthBox.InsertString( 4, _T("5月") );
	m_monthBox.InsertString( 5, _T("6月") );
	m_monthBox.InsertString( 6, _T("7月") );
	m_monthBox.InsertString( 7, _T("8月") );
	m_monthBox.InsertString( 8, _T("9月") );
	m_monthBox.InsertString( 9, _T("10月") );
	m_monthBox.InsertString( 10, _T("11月") );
	m_monthBox.InsertString( 11, _T("12月") );
	m_monthBox.SetCurSel( m_currDate.GetMonth() - 1 );
}
void CWorkHourView::OnInitUserName()
{
	CString str;
	str.Format(_T("用户：%s"), m_showUser.GetUserName_R());
	GetDlgItem( IDC_STATIC_USER_NAME )->SetWindowText( str );
}

void CWorkHourView::InitSearchCondition()
{
	UpdateData( TRUE );
	int lastSel = m_findTextBox.GetCurSel();
	m_findTextBox.ResetContent();
	int culSel = m_monthBox.GetCurSel();
	m_fristWeekPos = 0;
    int month = m_currDate.GetMonth();
	int year = m_selYear;
	if ( year == m_currDate.GetYear() && culSel+1 == month )
	{//本月
		m_findTextBox.InsertString(0, _T("本周") );
		m_findTextBox.InsertString(1, _T("上周") );
		m_fristWeekPos = 2;
	}
	month = culSel+1;
	if ( m_assginWorkHour )
	{
		COleDateTime beginDate( year, month, 1, 0, 0, 0);
		CString temp = beginDate.Format( _T("%Y-%m-%d") );
		int tempMonth = beginDate.GetMonth();
		int pos = m_fristWeekPos;
		CString str;
		COleDateTimeSpan  tmpSpan( 7, 0, 0, 0);
		while( beginDate.GetMonth() == month )
		{
			str.Format(_T("第%d周"), pos - m_fristWeekPos+1 );
			m_findTextBox.InsertString( pos++, str );
			beginDate += tmpSpan;
		}
	}
	m_findTextBox.SetCurSel( 0 );
}

void CWorkHourView::OnBnClickedBtnFresh()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pActionFactory->GetTaskSever()->UpdateAllReviewSchedule();
	//InitSearchCondition( FALSE );
	//OnCbnEditchangeComboFind();
}

void CWorkHourView::OnCbnEditchangeComboFind()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	OnSelDateChange();
}
void CWorkHourView::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
	//if ( m_gridManger.get() )
	//{
	//	m_gridManger->OnGridSelChange( pNMHDR, pResult );
	//}
}
LRESULT CWorkHourView::OnAddCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	//if ( m_gridManger.get() )
	//{
	//	m_findTextBox.SetCurSel( 0 );
	//	m_gridManger->UpdateDbDataMapAndShowAll();
	//	m_gridManger->SelectScheuleLine( lp );
	//}
	return 0;
}
LRESULT CWorkHourView::OnCheckWorkHour( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
	     m_pActionFactory->GetLoginServer()->GetUserById( lp, m_showUser );
		 OnInitUserName();
		 m_gridManger->SetSelUser( m_showUser.GetId() );
		 m_gridManger->OnCheckSelUser();
	}
	return 0;
}

LRESULT CWorkHourView::OnDecreateCntChangeUpdate( WPARAM wp, LPARAM lp )
{
	//if ( m_gridManger.get() )
	//{
	//	m_findTextBox.SetCurSel( 0 );
	//	m_gridManger->UpdateDbDataMapAndShowAll();
	//}
	return 0;
}
LRESULT CWorkHourView::OnContentChangeUpdate( WPARAM wp, LPARAM lp )
{
	if ( m_gridManger.get() )
	{
		m_gridManger->SetSelUser( m_showUser.GetId() );
	}
	return 0;
}

LRESULT CWorkHourView::OnSelGridLineView( WPARAM wp, LPARAM lp )
{
	//if ( m_gridManger.get() )
	//{
	//	m_gridManger->UpdateScheulContent( lp );
	//}
	return 0;
}

void CWorkHourView::OnBnClickedBtnSelect()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	CDlgUserChose dlg( FALSE );
	if ( IDOK == dlg.DoModal() )
	{
		const CPtrArray* pUser = dlg.GetSelArrary();
		if ( pUser->GetCount() == 1 )
		{
			const CUser *pSelUserObj = ( CUser *)pUser->GetAt( 0 );
			if ( pSelUserObj->GetId() > 0 )
			{
				m_showUser = *pSelUserObj;
				OnInitUserName( );
				m_gridManger->SetSelUser( m_showUser.GetId() );
			}
		}
	}
}


void CWorkHourView::OnBnClickedBtnCheck()
{
	if ( m_gridManger.get() )
	{
		if ( m_assginWorkHour )
		{
			m_gridManger->OnCheckAllUser();
		}else
		{
			m_gridManger->OnCheckSelUser();
		}
	}
}

void CWorkHourView::OnSelDateChange()
{
	UpdateData( TRUE );
	int month = m_monthBox.GetCurSel() + 1;
	int weekSelect = m_findTextBox.GetCurSel();
	COleDateTime date = m_currDate/*(GetSelYear(), month, )*/;
	int year = m_selYear;
	if ( weekSelect < m_fristWeekPos )
	{
		month = date.GetMonth();
		year = date.GetYear();
		m_monthBox.SetCurSel( date.GetMonth() - 1 );
		m_selYear = year;
	}
	if ( year == m_currDate.GetYear() && month == m_currDate.GetMonth() )
	{
		if ( m_fristWeekPos > 0 && weekSelect < m_fristWeekPos )
		{//
			if ( weekSelect != 0 )
			{//上一周
				int weekDay = CUtilityTool::GetObj()->GetDayOfWeek( date );
				COleDateTimeSpan span(  weekDay, 0, 0, 0 ); //回到上周星期天
				int dataCnt = weekDay; //星期五返回6
				COleDateTime tmpDate;
				tmpDate = date - span  ;
				BOOL isAllHoliday = TRUE;
				span.SetDateTimeSpan( 1, 0, 0, 0 );
				BOOL reslut = FALSE;
				while( isAllHoliday )
				{
					for ( int i = 0; i < 7; i++ )
					{
						m_pActionFactory->GetSystemCfgSever()->IsHoliday( tmpDate, reslut );
						if ( FALSE == reslut )
						{
							isAllHoliday = FALSE;
							break;
						}
						tmpDate -=span;
					}
				}
				date = tmpDate;
			}else if ( weekSelect > 0 )
			{
				weekSelect -= m_fristWeekPos;
				date.SetDate( year, month, 1+weekSelect*7 );
			}
		}else
		{//设置日期
			int weekOfMonth = weekSelect - m_fristWeekPos;
			date.SetDate( year, month, 1+weekOfMonth*7 );
		}
	}else
	{
		date.SetDate( year, month, 1+weekSelect*7 );
	}
	if ( date.GetYear() != year )
	{
		m_selYear = year;
	}
	if ( date.GetMonth() != month )
	{
		m_monthBox.SetCurSel( date.GetMonth() - 1 );
	}
	m_gridManger->OnUpdateShowBeginDate( date );
	UpdateData( FALSE );
}
void CWorkHourView::OnCbnSelchangeComboYear()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	InitSearchCondition();
	OnSelDateChange();
}


void CWorkHourView::OnCbnSelchangeComboMonth()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	InitSearchCondition();
	OnSelDateChange();
}

void CWorkHourView::OnBnClickedBtnExportHour()
{
	if ( !m_gridManger.get() )
	{
		return;
	}
	UpdateData( TRUE );
	int month = m_monthBox.GetCurSel() + 1;
	if ( month > 0 && m_selYear)
	{
		m_gridManger->OnExportAllWorkHour( m_selYear, month );
	}else
	{
		AfxMessageBox(_T("选择年月错误"));
	}
	
}

//设置当月默认假日
void CWorkHourView::OnBnClickedSetDefDate()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( !m_gridManger.get() )
	{
		return;
	}
	UpdateData( TRUE );
	int month = m_monthBox.GetCurSel() + 1;
	if ( month > 0 && m_selYear)
	{
		m_gridManger->OnSetDefHoliday( m_selYear, month );
	}else
	{
		AfxMessageBox(_T("选择年月错误"));
	}
	
}
void    CWorkHourView::RefreshShow()
{
	if ( m_gridManger.get() )
	{
		m_gridManger->RefreshShowData();
	}
}
