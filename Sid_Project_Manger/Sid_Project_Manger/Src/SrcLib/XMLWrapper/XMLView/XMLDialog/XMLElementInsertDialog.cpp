// XMLElementInsertDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EP61850Client.h"
#include "XMLElementInsertDialog.h"
#include "afxdialogex.h"


// CXMLElementInsertDialog 对话框

IMPLEMENT_DYNAMIC(CXMLElementInsertDialog, CDialogEx)

CXMLElementInsertDialog::CXMLElementInsertDialog
(CString defElemName,CString defElemNamespace,CWnd* pParent /*=NULL*/)
: CDialogEx(CXMLElementInsertDialog::IDD, pParent)
,m_elementName(defElemName)
,m_elementNamespace(defElemNamespace)
{

}

CXMLElementInsertDialog::~CXMLElementInsertDialog()
{
}

void CXMLElementInsertDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_ELEMENT_NAME,		m_elementName);
	DDX_Text(pDX,IDC_EDIT_ELEMENT_NAMESPACE,m_elementNamespace);
}


BEGIN_MESSAGE_MAP(CXMLElementInsertDialog, CDialogEx)
END_MESSAGE_MAP()

BOOL CXMLElementInsertDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);	/* 调用UpdateData把默认值显示到控件中 */

	return TRUE;
}

int CXMLElementInsertDialog::GetChoice()
{
	return m_userChoice;
}
CString CXMLElementInsertDialog::GetElementName()
{
	return m_elementName;
}
CString CXMLElementInsertDialog::GetElementNamespaceURI()
{
	return m_elementNamespace;
}

// CXMLElementInsertDialog 消息处理程序
void CXMLElementInsertDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_elementName.IsEmpty())
	{
		HWND hWnd = this->GetSafeHwnd();
		::MessageBox(hWnd,TRANS_T("元素名称不能为空!"),
			_T("Error!"),MB_OK|MB_ICONERROR);
		return;
	}
	m_userChoice = IDOK;
	CDialogEx::OnOK();
}
void CXMLElementInsertDialog::OnCancel()
{
	UpdateData(TRUE);
	m_userChoice = IDCANCEL;
	CDialogEx::OnCancel();
}