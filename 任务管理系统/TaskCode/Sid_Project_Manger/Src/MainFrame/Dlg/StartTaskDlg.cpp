#include "stdafx.h"
#include "StartTaskDlg.h"
#include "../MainFrm.h"

IMPLEMENT_DYNAMIC(CStartTaskDlg, CDialogEx)

CStartTaskDlg::CStartTaskDlg(const CTaskObj &obj, const COleDateTime &startTime, CWnd* pParent /*= NULL*/ )
        :CDialogEx(CStartTaskDlg::IDD, pParent) //2015-4-8 lhz 
{
	if ( obj.GetTaskStatus() != waitStart )
	{
		AfxMessageBox( _T("任务状态不能进行启动"));
		return;
	}
	m_handTask = obj;
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	m_pTaskSever = m_pSever->GetTaskSever();
	if ( startTime.m_dt <= 0 || startTime.GetStatus() != COleDateTime::valid )
	{
		m_startDate = COleDateTime::GetCurrentTime();
	}else
	{
		m_startDate = startTime;
	}

}
CStartTaskDlg::~CStartTaskDlg()
{

}
BEGIN_MESSAGE_MAP(CStartTaskDlg, CDialogEx)
	ON_BN_CLICKED(ID_BTN_START, &CStartTaskDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()

void CStartTaskDlg::DoDataExchange(CDataExchange* pDX)
{

	DDX_Control(pDX, IDC_DATE_FACT_START, m_startFactTime);
}

BOOL CStartTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//修改时间框的字体

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
	m_startFactTime.SetFont( &m_font );

	GetDlgItem( IDC_EDIT_TASK_NAME )->SetWindowText( m_handTask.GetName() );
	(( CDateTimeCtrl *)GetDlgItem( IDC_DATE_PLAN_START ))->SetTime( m_handTask.GetPlanStartTime() );
	(( CDateTimeCtrl *)GetDlgItem( IDC_DATE_PLAN_END ))->SetTime( m_handTask.GetPlanEndTime() );

	
	m_startFactTime.SetTime( m_startDate/* COleDateTime::GetCurrentTime()*/ );
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//启动任务
void CStartTaskDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码
	COleDateTime time;
	m_startFactTime.GetTime( time );
	COleDateTime panStart = m_handTask.GetPlanStartTime();
	//if ( time <  panStart )
	//{
	//	AfxMessageBox( _T("实际开始时间比计划启动日期早") );
	//	return;
	//}
	time.SetDateTime( time.GetYear(), time.GetMonth(),time.GetDay(),
		                0, 0, 0);
	if ( time > m_handTask.GetPlanEndTime() )
	{
		AfxMessageBox( _T("实际开始时间比计划完成日期晚") );
		return;
	}
   m_handTask.SetTaskStatus( ongoing );
   m_handTask.SetFactStartTime( time );
   BOOL ret = m_pTaskSever->UpdateFactStartTime( m_handTask );
   if ( userExcOk != ret )
   {
	   AfxMessageBox(  CActionServerFactory::GetErrorString( ret ) );
   }else
   {
	   m_pSever->GetScheduleSever()->InsertEmptySchdule( m_handTask.GetId(), time );
	  AfxMessageBox( _T("启动任务成功，可以编辑进度"));
	  CDialogEx::OnOK();
   }
}
