// CProjectProcessReviewDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectProcessReviewDlg.h"
#include "afxdialogex.h"


// CProjectProcessReviewDlg 对话框

IMPLEMENT_DYNAMIC(CProjectProcessReviewDlg, CComonDlg)

CProjectProcessReviewDlg::CProjectProcessReviewDlg(const CTaskObj &obj, const CString &newSchedul,CWnd* pParent /*=NULL*/)
	: CComonDlg(CProjectProcessReviewDlg::IDD, pParent),m_setTask(obj), m_strSchedul(newSchedul)
	,m_processReview(_T(""))
{
	m_pScheduleSever = m_pSever->GetScheduleSever();
}

CProjectProcessReviewDlg::~CProjectProcessReviewDlg()
{
}

void CProjectProcessReviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DEPART_REVIEW, m_processReview);
}


BEGIN_MESSAGE_MAP(CProjectProcessReviewDlg, CComonDlg)
	ON_BN_CLICKED(IDOK, &CProjectProcessReviewDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CProjectProcessReviewDlg 消息处理程序
BOOL CProjectProcessReviewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString taskDc;
	taskDc.Format(_T("任务名:%s\r\n最新进度:%s\r\n计划:%s-%s %s-%s"), 
		m_setTask.GetName(), m_strSchedul.IsEmpty()?_T("无"):m_strSchedul,
		m_setTask.GetPanStartDateShowStr(), m_setTask.GetPanEndDateShowStr(),
		m_setTask.GetFactStartDateShowStr(), m_setTask.GetFactEndDateShowStr() );
	GetDlgItem( IDC_EDIT_TASK_DEC )->SetWindowText( taskDc );
	GetDlgItem( IDC_STATIC_SCORE_DEC )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_EDIT_DEPART_SCORE )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_SCORE_RANGE )->ShowWindow( SW_HIDE );
	
	SetWindowText( _T("项目过程点评") );
	UpdateData( FALSE );
	return TRUE;
}

void CProjectProcessReviewDlg::OnBnClickedOk()
{
	UpdateData( TRUE );
	BOOL ret = m_pScheduleSever->OnPrjReviewSchedule( m_setTask, m_pLoginAction->GetLoginUser().GetUserName_R(),m_processReview );
	if ( ret == userExcOk )
	{
		CDialogEx::OnOK();
		return;
	}
	if ( ret != paraError )
	{
		AfxMessageBox( m_pSever->GetErrorString(ret) );
	}
}
