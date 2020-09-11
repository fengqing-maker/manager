// ExportWorkHourDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExportWorkHourDlg.h"
#include "afxdialogex.h"
#include "Src/action/ExcelAction/CTaskExcelAction.h"

// ExportWorkHourDlg �Ի���
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(ExportWorkHourDlg, CComonDlg)

ExportWorkHourDlg::ExportWorkHourDlg(CWnd* pParent /*=NULL*/)
	: CComonDlg(ExportWorkHourDlg::IDD, pParent)
{

}

ExportWorkHourDlg::~ExportWorkHourDlg()
{
}

void ExportWorkHourDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATE_BEGIN, m_ctrlDateBegin);
	DDX_Control(pDX, IDC_DATE_END, m_ctrlDateEnd);
}


BEGIN_MESSAGE_MAP(ExportWorkHourDlg, CComonDlg)
	ON_BN_CLICKED(IDOK, &ExportWorkHourDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_DIR, &ExportWorkHourDlg::OnBnClickedBtnDir)
END_MESSAGE_MAP()


// ExportWorkHourDlg ��Ϣ�������


BOOL ExportWorkHourDlg::OnInitDialog()
{
	CComonDlg::OnInitDialog();

	COleDateTime time = COleDateTime::GetCurrentTime();
	time.SetDateTime( time.GetYear(), time.GetMonth(), time.GetDay(),
		              0, 0, 0 );
	m_ctrlDateBegin.SetTime(  time );
	time.SetDateTime( time.GetYear(), time.GetMonth(), time.GetDay(), 
		               23, 59, 59 );
	m_ctrlDateEnd.SetTime( time );
	//
	CString dir;
	dir.Format( _T("D://%s��ʱ.xls"), m_pLoginAction->GetLoginUser().GetUserName_R() );
	GetDlgItem( IDC_EDIT_DIR )->SetWindowText( dir ); 
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void ExportWorkHourDlg::OnBnClickedOk()
{
	CString dirName;
	GetDlgItem(IDC_EDIT_DIR)->GetWindowText( dirName );
	CTaskExcelAction action( m_pSever );
	COleDateTime beginDate;
	m_ctrlDateBegin.GetTime( beginDate );
	COleDateTime endDate;
	m_ctrlDateEnd.GetTime( endDate );

	BOOL ret = action.ExportWorkHour( dirName,beginDate, endDate );
	if ( ret == userExcOk )
	{
		AfxMessageBox(_T("�����ɹ�"));
		CComonDlg::OnOK();
	}else if ( ret != paraError )
	{
		AfxMessageBox( m_pSever->GetErrorString( ret ) ); 
	}
}


void ExportWorkHourDlg::OnBnClickedBtnDir()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog saveFile = CFileDialog(FALSE, _T(".xls"), _T(""), OFN_CREATEPROMPT | OFN_PATHMUSTEXIST ,_T( "(*.xls)|*xls" ));  
    if( saveFile.DoModal() == IDOK )
	{//
		GetDlgItem(IDC_EDIT_DIR)->SetWindowText( saveFile.GetPathName() );
	}
}
