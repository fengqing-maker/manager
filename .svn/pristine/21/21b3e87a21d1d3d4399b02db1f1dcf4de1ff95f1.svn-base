// DlgNewUser.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgNewUser.h"
#include "afxdialogex.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDlgNewUser 对话框

IMPLEMENT_DYNAMIC(CDlgNewUser, CDialogEx)

CDlgNewUser::CDlgNewUser(CWnd* pParent /*=NULL*/)
	: CComonDlg(CDlgNewUser::IDD, pParent)
{
	m_pBackActionInc = m_pSever->GetBackStageSver();
	m_pSever->GetDepartSever()->GetAllDepart( m_allDepart );
	m_pBackActionInc->GetAllRole( m_allRole );

}

CDlgNewUser::~CDlgNewUser()
{
}

void CDlgNewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ROLE, m_ctrlRole);
	DDX_Control(pDX, IDC_COMBO_DEPART , m_ctrldepart);
}


BEGIN_MESSAGE_MAP(CDlgNewUser, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgNewUser::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgNewUser 消息处理程序


BOOL CDlgNewUser::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	
	if ( FALSE == OnInitDepart() )
	{
		return FALSE;
	}
	if ( FALSE == OnInitRoleList() )
	{
		return FALSE;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
BOOL CDlgNewUser::OnInitDepart()
{
	m_ctrldepart.ResetContent();
	int cnt = 0;
	for ( ListDepartConstItor itor = m_allDepart.begin(); itor != m_allDepart.end(); itor++ )
	{
		m_ctrldepart.InsertString( cnt++, itor->GetName() );
	}
	if ( cnt == 0 )
	{
		AfxMessageBox( _T("初始化部门列表失败，请先创建部门"));
		return FALSE;
	}
	m_ctrldepart.SetCurSel( 0 );
	return TRUE;
}
BOOL CDlgNewUser::OnInitRoleList()
{
	m_ctrlRole.ResetContent();
	int cnt = 0;
	for ( ListRoleConstItor itor = m_allRole.begin(); itor != m_allRole.end(); itor++ )
	{
		m_ctrlRole.InsertString( cnt++, itor->GetRoleName() );
	}
	if ( cnt == 0 )
	{
		AfxMessageBox( _T("初始化角色列表失败，请先创建角色"));
		return FALSE;
	}
	m_ctrlRole.SetCurSel( 0 );
	return TRUE;
}


void CDlgNewUser::OnBnClickedOk()
{
	CString str;
	GetDlgItem( IDC_EDIT_NAME )->GetWindowText( str );
	if ( str.IsEmpty() )
	{
		AfxMessageBox(_T("用户姓名不能为空"));
		return;
	} 
	CUser obj;
	CUser newObj;
	BOOL ret = m_pLoginAction->GetUserByName(str, obj );
	if ( ret != notfind )
	{
		if ( userExcOk == ret )
		{
			AfxMessageBox(_T("此用户姓名已存在"));
		}else
		{
			AfxMessageBox( m_pSever->GetErrorString( ret) );
		}
		return;
	}
	newObj.SetUserName_R( str );

	str.ReleaseBuffer();
	GetDlgItem( IDC_EDIT_LOGIN_NAME )->GetWindowText( str );
	if ( str.IsEmpty() )
	{
		AfxMessageBox(_T("用户姓名不能为空"));
		return;
	} 
	ret = m_pLoginAction->GetUserByLoginName( str, obj );
	if ( ret != notfind )
	{
		if ( userExcOk == ret )
		{
			AfxMessageBox(_T("用户登录名已存在"));
		}else
		{
			AfxMessageBox( m_pSever->GetErrorString( ret) );
		}
		return;
	}
	newObj.SetLoginName( str );
	//登陆密码
	str.ReleaseBuffer();
	GetDlgItem( IDC_EDIT_PWD  )->GetWindowText( str );
	CString pwd;
	if ( !str.IsEmpty() )
	{
		pwd = CUtilityTool::GetObj()->GetMd5String( str );
	}
	newObj.SetPwd( pwd );
	//所属部门
	int curSel = m_ctrldepart.GetCurSel();
	if ( m_allDepart.size() <= 0 || curSel < 0 )
	{
		AfxMessageBox(_T("部门列表未初始化获取未选择部门"));
		return;
	}
	ListDepartConstItor itor = m_allDepart.begin();
	advance( itor, curSel );
	if ( itor == m_allDepart.end() )
	{
		AfxMessageBox(_T("查找部门失败"));
		return;
	}
	newObj.SetDepartId( itor->GetId() );
	//角色
	 curSel = m_ctrlRole.GetCurSel();
	if ( m_allRole.size() <= 0 || curSel < 0 )
	{
		AfxMessageBox(_T("角色列表未初始化或者未选择角色"));
		return;
	}
	ListRoleConstItor itorRole = m_allRole.begin();
	advance( itorRole, curSel );
	if ( itorRole == m_allRole.end() )
	{
		AfxMessageBox(_T("查找部门失败"));
		return;
	}
	newObj.SetRolId( itor->GetId() );
   //
	ret = m_pBackActionInc->InsertUser( newObj );
	if ( userExcOk == ret )
	{
		CDialogEx::OnOK();
		return;
	}
	AfxMessageBox( m_pSever->GetErrorString(ret) );
}
