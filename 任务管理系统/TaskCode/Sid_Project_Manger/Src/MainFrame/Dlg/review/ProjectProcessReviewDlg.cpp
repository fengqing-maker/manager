// CProjectProcessReviewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProjectProcessReviewDlg.h"
#include "afxdialogex.h"


// CProjectProcessReviewDlg �Ի���

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


// CProjectProcessReviewDlg ��Ϣ�������
BOOL CProjectProcessReviewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CString taskDc;
	taskDc.Format(_T("������:%s\r\n���½���:%s\r\n�ƻ�:%s-%s %s-%s"), 
		m_setTask.GetName(), m_strSchedul.IsEmpty()?_T("��"):m_strSchedul,
		m_setTask.GetPanStartDateShowStr(), m_setTask.GetPanEndDateShowStr(),
		m_setTask.GetFactStartDateShowStr(), m_setTask.GetFactEndDateShowStr() );
	GetDlgItem( IDC_EDIT_TASK_DEC )->SetWindowText( taskDc );
	GetDlgItem( IDC_STATIC_SCORE_DEC )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_EDIT_DEPART_SCORE )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_SCORE_RANGE )->ShowWindow( SW_HIDE );
	
	SetWindowText( _T("��Ŀ���̵���") );
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
