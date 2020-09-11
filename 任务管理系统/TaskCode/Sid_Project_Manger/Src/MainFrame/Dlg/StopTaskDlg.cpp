// StopTaskDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "StopTaskDlg.h"
#include "afxdialogex.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

// CStopTaskDlg 对话框
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CStopTaskDlg, CDialogEx)

CStopTaskDlg::CStopTaskDlg(CTaskObj &task,CWnd* pParent /*=NULL*/)
	: CDialogEx(CStopTaskDlg::IDD, pParent), m_taskObj( task )
{

}

CStopTaskDlg::~CStopTaskDlg()
{
}

void CStopTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStopTaskDlg, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SET_SCORE, &CStopTaskDlg::OnBnClickedBtnSetScore)
END_MESSAGE_MAP()


// CStopTaskDlg 消息处理程序


BOOL CStopTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem( IDC_EDIT_TASK_NAME )->SetWindowText( m_taskObj.GetName() );
	(( CDateTimeCtrl *)GetDlgItem( IDC_DATE_PLAN_START ))->SetTime( m_taskObj.GetPlanStartTime() );
	(( CDateTimeCtrl *)GetDlgItem( IDC_DATE_PLAN_END ))->SetTime( m_taskObj.GetPlanEndTime() );
	CString str;
	str.Format(_T("%d"), m_taskObj.GetForecastScore() );
	GetDlgItem( IDC_EDIT_SYS_SCORE )->SetWindowText( str );

	return TRUE; 

}


void CStopTaskDlg::OnBnClickedBtnSetScore()
{
	CString str;
	GetDlgItem( IDC_EDIT_SCORE )->GetWindowText( str );

	int value = 0;
	if (FALSE == CUtilityTool::GetObj()->ConvertStringToInt( str, value) )
	{
	    AfxMessageBox(_T("输入非法字符"));
		return;
	}
	if ( value < 0 )
	{
		AfxMessageBox( _T("评分分数不能为负"));
		return;
	}
	m_taskObj.SetFactEndTime( COleDateTime::GetCurrentTime() );

	m_taskObj.SetScore( value );
	m_taskObj.SetScoreTime( COleDateTime::GetCurrentTime());

	CDialogEx::OnOK();
}
