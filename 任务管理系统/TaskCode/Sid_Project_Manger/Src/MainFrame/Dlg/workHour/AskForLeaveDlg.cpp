// AskForLeaveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AskForLeaveDlg.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CAskForLeaveDlg 对话框

IMPLEMENT_DYNAMIC(CAskForLeaveDlg, CDialogEx)

CAskForLeaveDlg::CAskForLeaveDlg(const COleDateTime &date, double taskWorkHour, CWnd* pParent /*=NULL*/)
	: CComonDlg(CAskForLeaveDlg::IDD, pParent),m_setAppForDate( date )
{
	m_taskHour = taskWorkHour;
	/*m_holidayHour = 7.5 - taskWorkHour;*/
	m_pWorkHourSever = m_pSever->GetWorkHourSever();
	m_loginUserId = m_pSever->GetLoginServer()->GetLoginUser().GetId();
}

CAskForLeaveDlg::~CAskForLeaveDlg()
{
}

void CAskForLeaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CComonDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TIME_TYPE, m_timeTypeBox );
	//DDX_Text(pDX, IDC_EDIT_CONENT, m_strContent);
	//DDX_Text( pDX, IDC_EDIT_WORHOUR_TIME, m_norTaskHour);
	//DDX_Text( pDX, IDC_EDIT_WORKHOUR_HOLIDAY, m_holidayHour );

	DDX_Control(pDX, IDC_EDIT_WORHOUR_TIME, m_editNorWorkHour);
	DDX_Control(pDX, IDC_EDIT_WORKHOUR_HOLIDAY, m_eidtHoliday);
}


BEGIN_MESSAGE_MAP(CAskForLeaveDlg, CComonDlg)
	ON_BN_CLICKED(IDOK, &CAskForLeaveDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TIME_TYPE, &CAskForLeaveDlg::OnCbnSelchangeComboTimeType)
	ON_EN_CHANGE(IDC_EDIT_WORHOUR_TIME, &CAskForLeaveDlg::OnEnChangeEditWorhourTime)
	ON_EN_CHANGE(IDC_EDIT_WORKHOUR_HOLIDAY, &CAskForLeaveDlg::OnEnChangeEditWorkhourHoliday)
END_MESSAGE_MAP()


// CAskForLeaveDlg 消息处理程序
BOOL CAskForLeaveDlg::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	CString str;
	str.Format( _T("日期:       %s"),  m_setAppForDate.Format(_T("%Y-%m-%d")) );
	GetDlgItem( IDC_EDIT_DATE )->SetWindowText( str );
	
	str.Format(_T("总任务工时%0.1f"), m_taskHour );
	GetDlgItem(IDC_STATIC_TASK_WORKHOUR)->SetWindowText( str );
	
	//
	CWorkHourData obj;
	m_pWorkHourSever->GetHolidayHour( m_loginUserId, m_setAppForDate, obj );
	m_holidayHour = obj.GetNorTaskWorkHour(); //

	OnInitWorkTimeType();
	SetNorWorkHourTotal();
	
	str.Format(_T("%0.1f"), m_holidayHour );
	m_eidtHoliday.SetWindowText( str );

	UpdateData( FALSE );
	return TRUE;
}
void CAskForLeaveDlg::SetNorWorkHourTotal()
{
	double total = 0.0;
	map<int,double>::iterator itor = m_norTaskHours.begin();
	for ( ; itor != m_norTaskHours.end(); itor++ )
	{
		total += itor->second; 
	}
	total += m_holidayHour;
	//非任务总合
	CString str;
	str.Format(_T("非任务工时%0.1f"), total );
	GetDlgItem(IDC_STATIC__NOR_TASK)->SetWindowText( str );
}
void CAskForLeaveDlg::OnInitWorkTimeType()
{
	m_timeTypeBox.ResetContent();
	const TaskIdStringMap &workHourType = m_pSever->GetTaskSever()->GetWorkHourString();
	int cnt = 0;
	double workHour = 0.0;
	m_norTaskHours.clear();
	for ( TaskIdStringMapConstItor itor = workHourType.begin(); itor != workHourType.end(); itor++ )
	{
		m_timeTypeBox.InsertString( cnt++, itor->second );
		CWorkHourData data;
		m_pWorkHourSever->GetWorkTimeData( itor->first, m_loginUserId, m_setAppForDate, data );
		m_norTaskHours.insert( make_pair( itor->first, data.GetNorTaskWorkHour()) );
	}
}
void CAskForLeaveDlg::OnCbnSelchangeComboTimeType()
{
	int selIndex = m_timeTypeBox.GetCurSel();
	map<int,double>::iterator itor = m_norTaskHours.begin();
	advance( itor, selIndex );
	if ( itor != m_norTaskHours.end() )
	{
		CString str;
		str.Format(_T("%0.1f"), itor->second );
		m_editNorWorkHour.SetWindowText( str );
	}
}

void CAskForLeaveDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( TRUE );
	double totalWorkHour = 0.0;
	map<int,double>::iterator itor = m_norTaskHours.begin();
	for ( ; itor != m_norTaskHours.end(); itor++ )
	{
	     totalWorkHour += itor->second;
	}
	double value = totalWorkHour+m_taskHour+m_holidayHour- 7.5f;
	long bRet = CUtilityTool::GetObj()->DoubleCmp( value );
    if ( 0 != bRet )
    {
		AfxMessageBox(_T("总工时不等于7.5,请重新设置非任务工时或请假工时") );
		return;
    }
	BOOL hasHoliday = FALSE;
	BOOL hasNorTaskHour = FALSE;
	if (  totalWorkHour > 0)
	{//
		hasNorTaskHour = TRUE;
	}
	if ( m_holidayHour > 0 )
	{
		hasHoliday = TRUE;
	}
	BOOL ret = paraError;
	CWorkHourData data;
	data.SetDate( m_setAppForDate );
	data.SetUserId( m_loginUserId );
	if ( hasHoliday )
	{//添加请假
		CWorkHourData data;
		//data.SetTaskWorkHour( m_holidayHour );
		data.SetNorTaskWorkHour( m_holidayHour );
		data.SetTimeType( 0 );
		data.SetUserId( m_loginUserId );
		data.SetDate( m_setAppForDate );
		ret = m_pWorkHourSever->AddOrUpdateWorkHour( data );
		if ( userExcOk == ret )
		{//2015-9-22 
			m_pWorkHourSever->DeleteUnUpdateDateByUserId( m_loginUserId, data.GetDate() );
		}
	}
	if ( hasNorTaskHour )
	{//添加非任务工时
		itor = m_norTaskHours.begin();
		for ( ; itor != m_norTaskHours.end(); itor++ )
		{
			data.SetTimeType( itor->first );
			m_pWorkHourSever->GetWorkTimeData( itor->first, m_loginUserId, m_setAppForDate, data );
			data.SetNorTaskWorkHour( itor->second );
			ret = m_pWorkHourSever->AddOrUpdateWorkHour( data );
			if ( userExcOk != ret )
			{
				break;
			}
			m_pWorkHourSever->DeleteUnUpdateDateByUserId( m_loginUserId, data.GetDate() );
		}
	}
	if ( ret == userExcOk  )
	{
		CComonDlg::OnOK();
	}
	if ( ret != paraError  )
	{
		AfxMessageBox( m_pSever->GetErrorString( ret ) );
	}
}

void CAskForLeaveDlg::OnEnChangeEditWorhourTime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CComonDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString str;
	m_editNorWorkHour.GetWindowText( str );
	int selIndex = m_timeTypeBox.GetCurSel();
	map<int,double>::iterator itor = m_norTaskHours.begin();
	advance( itor, selIndex);
	if ( itor != m_norTaskHours.end() )
	{
		double value = 0.0;
		if( !CUtilityTool::GetObj()->ConvertStringToDouble( str, value ) || value < 0)
		{//转换不成功
			AfxMessageBox(_T("输入数据有非法数值"));
			str.Format(_T("0.1f"), itor->second );
			m_editNorWorkHour.SetWindowText( str );
			return;
		}
	    itor->second = value;
	}
	SetNorWorkHourTotal();
	// TODO:  在此添加控件通知处理程序代码
}


void CAskForLeaveDlg::OnEnChangeEditWorkhourHoliday()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CComonDlg::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	CString str;
	m_eidtHoliday.GetWindowText( str );
	if( !CUtilityTool::GetObj()->ConvertStringToDouble( str, m_holidayHour ) )
	{
		double value = 0.0;
		if( !CUtilityTool::GetObj()->ConvertStringToDouble( str, value ) || value < 0)
		{//转换不成功
			AfxMessageBox(_T("输入数据有非法数值"));
			str.Format(_T("0.1f"), m_holidayHour );
			m_eidtHoliday.SetWindowText( str );
			return;
		}
		m_holidayHour = value;
	}
	// TODO:  在此添加控件通知处理程序代码
}
