// NewProjectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewProjectDlg.h"
#include "afxdialogex.h"
#include "DlgUserChose.h"

// NewProjectDlg 对话框

IMPLEMENT_DYNAMIC(NewProjectDlg, CComonDlg)

NewProjectDlg::NewProjectDlg(CWnd* pParent /*=NULL*/)
	: CComonDlg(NewProjectDlg::IDD, pParent)
{
   m_projectSever =	m_pSever->GetProjectSever();
}

NewProjectDlg::~NewProjectDlg()
{
}

void NewProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PRJ_NAME, m_eiditPrjName);
	DDX_Control(pDX, IDC_EDIT_LONG_NAME, m_editPrjLongName);
	DDX_Control(pDX, IDC_COMBO_PRODUCT_LINE, m_comboPrjLine);
	DDX_Control(pDX, IDC_EDIT_OWNER, m_editOwner);
	DDX_Control(pDX, IDC_EDIT_MEMBER, m_eidtPrjMember);
}


BEGIN_MESSAGE_MAP(NewProjectDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CHOCE_OWNER, &NewProjectDlg::OnBnClickedBtnChooseOwner)
	ON_BN_CLICKED(IDC_BTN_CHOCE_MEMBER, &NewProjectDlg::OnBnClickedBtnChooseMember)
	ON_BN_CLICKED(IDOK, &NewProjectDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// NewProjectDlg 消息处理程序


BOOL NewProjectDlg::OnInitDialog()
{
	CComonDlg::OnInitDialog();

	if ( FALSE == OnInitProductLine() )
	{
		OnCancel();
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL NewProjectDlg::OnInitProductLine()
{
	m_lstProductLine.clear();
	BOOL ret = m_projectSever->GetAllProjectLine( m_lstProductLine );
	if ( ret != userExcOk )
	{
		AfxMessageBox( m_pSever->GetErrorString(ret) );
		return FALSE;
	}
	size_t lineCount = m_lstProductLine.size();
	if ( 0 == lineCount   )
	{
		AfxMessageBox( _T("产品线为空，请先新建产品线") );
		return FALSE;
	}
	int cnt = 0;
	for ( ListProjectLine::iterator itor = m_lstProductLine.begin(); itor != m_lstProductLine.end(); itor++ )
	{
		m_comboPrjLine.InsertString( cnt, itor->GetName() );
		cnt++;
	}
    m_comboPrjLine.SetCurSel( 0 );
	return TRUE;
}

void NewProjectDlg::OnBnClickedBtnChooseOwner()
{
	CDlgUserChose dlg( FALSE );
	if ( m_newPrjObj.GetMangerId() > 0 )
	{
		dlg.SetDelSelUser( m_newPrjObj.GetMangerId() );
	}
	if ( IDOK == dlg.DoModal() )
	{
		const CPtrArray* pUserArray = dlg.GetSelArrary();
		if ( 1 == pUserArray->GetCount() )
		{
			const CUser *pUser = (const CUser *)pUserArray->GetAt( 0 );
			m_newPrjObj.SetMangerId( pUser->GetId() );
			m_editOwner.SetWindowText( pUser->GetUserName_R() );
		}
	}
}

void NewProjectDlg::OnBnClickedBtnChooseMember()
{
	CDlgUserChose dlg( TRUE );
	if ( m_prjMember.size() > 0 )
	{
		dlg.SetDelSelUser( m_prjMember );
	}
	if ( IDOK == dlg.DoModal() )
	{
		const CPtrArray* pUserArray = dlg.GetSelArrary();
		int selCount = pUserArray->GetCount();
		if ( selCount > 0 )
		{
			m_prjMember.clear();
			CString str;
			for ( int i = 0; i < selCount; i++ )
			{
				const CUser *pUser = (const CUser *)pUserArray->GetAt( i );
				if ( pUser )
				{
					m_prjMember.push_back( *pUser );
					str += pUser->GetUserName_R();
                    if (i > 0 && 0 == i%5 )
                    {
						str += _T("\r\n");
                    }
				}
			}
			m_eidtPrjMember.SetWindowText( str );
		}
	}
}


void NewProjectDlg::OnBnClickedOk()
{
	//责任人
	if ( m_newPrjObj.GetMangerId() <= 0 )
	{
		AfxMessageBox( _T("项目经理不能为空，请选择项目经理") );
		return;
	}

	CString str;
	m_eiditPrjName.GetWindowText( str );
	if ( str.IsEmpty() )
	{
		AfxMessageBox( _T("项目名不能为空") );
		return;
	}
	m_newPrjObj.SetShortName( str );
	m_editPrjLongName.GetWindowText( str );
	if ( str.IsEmpty() )
	{
		AfxMessageBox( _T("项目描述不能为空") );
		return;
	}
	m_newPrjObj.SetLongName( str );
	int selIndex = m_comboPrjLine.GetCurSel();
	if ( m_lstProductLine.size() <= 0 || selIndex < 0 )
	{
		AfxMessageBox( _T("产品线选择错误") );
		return;
	}
	ListProjectLine::iterator itor = m_lstProductLine.begin();
	advance( itor, selIndex );
	if ( itor == m_lstProductLine.end() )
	{
		AfxMessageBox( _T("产品线选择错误") );
		return;
	}
	m_newPrjObj.SetLineId( itor->GetId() );
	if ( m_newPrjObj.GetMangerId() <= 0 )
	{
		AfxMessageBox( _T("项目负责人不能为空") );
		return;
	}
	BOOL ret = m_projectSever->NewProject( m_newPrjObj, m_prjMember );
	if ( userExcOk == ret )
	{
		CDialogEx::OnOK();
		return;
	}
	AfxMessageBox( m_pSever->GetErrorString( ret) );

}
