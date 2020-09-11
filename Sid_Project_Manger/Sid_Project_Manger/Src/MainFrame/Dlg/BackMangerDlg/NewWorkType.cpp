// NewWorkType.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "NewWorkType.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CNewWorkType �Ի���

IMPLEMENT_DYNAMIC(CDlgNewWorkType, CComonDlg)

CDlgNewWorkType::CDlgNewWorkType(CWnd* pParent /*=NULL*/)
	: CComonDlg(CDlgNewWorkType::IDD, pParent)
{

}

CDlgNewWorkType::~CDlgNewWorkType()
{
}

void CDlgNewWorkType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNewWorkType, CComonDlg)
	ON_BN_CLICKED(IDOK, &CDlgNewWorkType::OnBnClickedOk)
END_MESSAGE_MAP()
// CNewWorkType ��Ϣ�������


void CDlgNewWorkType::OnBnClickedOk()
{
	CString text;
	GetDlgItem( IDC_EDIT_NAME )->GetWindowText( text );
	if ( text.IsEmpty() )
	{
		AfxMessageBox(_T("��ʱ����������Ϊ��") );
		return;
	}
	BOOL ret = m_pSever->GetBackStageSver()->InsertWorkType( text );
	if ( ret == userExcOk )
	{
		CDialogEx::OnOK();
	}else
	{
		AfxMessageBox( m_pSever->GetErrorString(ret) );
	}
}
