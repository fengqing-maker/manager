// AskForLeaveDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AskForLeaveDlg.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CAskForLeaveDlg �Ի���

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


// CAskForLeaveDlg ��Ϣ�������
BOOL CAskForLeaveDlg::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	CString str;
	str.Format( _T("����:       %s"),  m_setAppForDate.Format(_T("%Y-%m-%d")) );
	GetDlgItem( IDC_EDIT_DATE )->SetWindowText( str );
	
	str.Format(_T("������ʱ%0.1f"), m_taskHour );
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
	//�������ܺ�
	CString str;
	str.Format(_T("������ʱ%0.1f"), total );
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox(_T("�ܹ�ʱ������7.5,���������÷�����ʱ����ٹ�ʱ") );
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
	{//������
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
	{//��ӷ�����ʱ
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CComonDlg::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString str;
	m_editNorWorkHour.GetWindowText( str );
	int selIndex = m_timeTypeBox.GetCurSel();
	map<int,double>::iterator itor = m_norTaskHours.begin();
	advance( itor, selIndex);
	if ( itor != m_norTaskHours.end() )
	{
		double value = 0.0;
		if( !CUtilityTool::GetObj()->ConvertStringToDouble( str, value ) || value < 0)
		{//ת�����ɹ�
			AfxMessageBox(_T("���������зǷ���ֵ"));
			str.Format(_T("0.1f"), itor->second );
			m_editNorWorkHour.SetWindowText( str );
			return;
		}
	    itor->second = value;
	}
	SetNorWorkHourTotal();
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CAskForLeaveDlg::OnEnChangeEditWorkhourHoliday()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CComonDlg::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString str;
	m_eidtHoliday.GetWindowText( str );
	if( !CUtilityTool::GetObj()->ConvertStringToDouble( str, m_holidayHour ) )
	{
		double value = 0.0;
		if( !CUtilityTool::GetObj()->ConvertStringToDouble( str, value ) || value < 0)
		{//ת�����ɹ�
			AfxMessageBox(_T("���������зǷ���ֵ"));
			str.Format(_T("0.1f"), m_holidayHour );
			m_eidtHoliday.SetWindowText( str );
			return;
		}
		m_holidayHour = value;
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
