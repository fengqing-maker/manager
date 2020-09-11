// FinisTaskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../MainFrm.h"
#include "FinisTaskDlg.h"
#include "afxdialogex.h"


// CFinisTaskDlg �Ի���

IMPLEMENT_DYNAMIC(CFinisTaskDlg, CDialogEx)

CFinisTaskDlg::CFinisTaskDlg(const CTaskObj &obj,const COleDateTime &FinishTime, CWnd* pParent /*=NULL*/)
	: CDialogEx(CFinisTaskDlg::IDD, pParent)
{
	m_handTask = obj;
    m_pTaskSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory()->GetTaskSever();
	m_finishDate = FinishTime; 
}

CFinisTaskDlg::~CFinisTaskDlg()
{
}

void CFinisTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_FACT_END, m_endDateCtrl);
}


BEGIN_MESSAGE_MAP(CFinisTaskDlg, CDialogEx)
	ON_BN_CLICKED(ID_BTN_END, &CFinisTaskDlg::OnBnClickedBtnTaskFinish)
END_MESSAGE_MAP()


// CFinisTaskDlg ��Ϣ�������
BOOL CFinisTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	VERIFY(m_font.CreateFont(
		30,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Cambria")));        
	m_endDateCtrl.SetFont( &m_font );

	GetDlgItem( IDC_EDIT_TASK_NAME )->SetWindowText( m_handTask.GetName() );
	CDateTimeCtrl *pTemp = ( CDateTimeCtrl *)GetDlgItem( IDC_DATE_PLAN_START );
	if ( pTemp )
	{
		(pTemp)->SetTime( m_handTask.GetPlanStartTime() );
	}
	
	(( CDateTimeCtrl *)GetDlgItem( IDC_DATE_PLAN_END ))->SetTime( m_handTask.GetPlanEndTime() );

	m_endDateCtrl.SetTime( m_finishDate );
	return TRUE;
}
//
void CFinisTaskDlg::OnBnClickedBtnTaskFinish()
{
	COleDateTime time;
	m_endDateCtrl.GetTime( time );
	time.SetDateTime( m_finishDate.GetYear(), m_finishDate.GetMonth(),m_finishDate.GetDay(),
		               0, 0, 0);
	COleDateTime time2 = m_handTask.GetFactStartTime();
	if ( time < m_handTask.GetFactStartTime() )
	{
		AfxMessageBox(_T("ʵ�����ʱ���ʵ������ʱ����") );
		return;
	}
	time2 = m_handTask.GetPlanEndTime();
	if ( time > time2 && IDNO == AfxMessageBox(_T("ʵ�����ʱ��ȼƻ����ʱ�����Ƿ�ȷ���ύ���"), MB_YESNO ) )
	{
		return;
	}
	COleDateTimeSpan span = time - m_handTask.GetFactStartTime(); 
	int datas = span.GetDays();
	double factDatas = m_handTask.GetFactWorkHour()/7.5 + 1;
	//if ( factDatas > datas )
	//{
	//	CString str;
	//	str.Format( _T("ʵ�����������ʵ�ʿ�ʼ���ڵ�����Ϊ%d����ʱ�乤ʱ%0.1fת��Ϊ����%d��"),  datas, m_handTask.GetFactWorkHour(),(int)factDatas);
	//	AfxMessageBox( str );
	//	return;
	//}
	//
    m_handTask.SetFactEndTime( time );
	if ( m_handTask.GetProjctId() > 0 )
	{
		m_handTask.SetTaskStatus( finishReviewWait );
	}else
	{
		m_handTask.SetTaskStatus( waitScore );
	}
	CString record = m_handTask.GetDec(), tmp;
	COleDateTime currTime = COleDateTime::GetCurrentTime();
    tmp.Format( _T("%s�ύ���"), currTime.Format( _T("%Y-%m-%d %H:%M:%S") ) );
	record += tmp;
	m_handTask.SetDec( tmp );
	//
	BOOL ret = m_pTaskSever->TaskFinishWaitScore( m_handTask );
	if ( userExcOk != ret )
	{
		AfxMessageBox( CActionServerFactory::GetErrorString( ret ) );
	}else
	{
		return CDialogEx::OnOK();
	}
} 