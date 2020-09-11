// SetRolePower.cpp : 实现文件
//

#include "stdafx.h"
#include "Sid_Project_Manger.h"
#include "SetRolePower.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CSetRolePower 对话框

IMPLEMENT_DYNAMIC(CSetRolePower, CDialogEx)

CSetRolePower::CSetRolePower(CWnd* pParent /*=NULL*/)
	: CComonDlg(CSetRolePower::IDD, pParent)
{

}

CSetRolePower::~CSetRolePower()
{
}

void CSetRolePower::DoDataExchange(CDataExchange* pDX)
{
	CComonDlg::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetRolePower, CDialogEx)
END_MESSAGE_MAP()


// CSetRolePower 消息处理程序
BOOL CSetRolePower::OnInitDialog()
{
	CComonDlg::OnInitDialog();
}