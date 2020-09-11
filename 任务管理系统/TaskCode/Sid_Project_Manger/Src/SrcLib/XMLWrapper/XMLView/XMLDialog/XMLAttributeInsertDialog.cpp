// XMLAttributeInsertDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EP61850Client.h"
#include "XMLAttributeInsertDialog.h"
#include "afxdialogex.h"


// CXMLAttributeInsertDialog 对话框

IMPLEMENT_DYNAMIC(CXMLAttributeInsertDialog, CDialogEx)

CXMLAttributeInsertDialog::CXMLAttributeInsertDialog
(CString defAttrName,CString defAttrNamespace,CString defAttrValue,CWnd* pParent /*=NULL*/)
: CDialogEx(CXMLAttributeInsertDialog::IDD, pParent)
,m_attributeName(defAttrName)
,m_attributeNamespace(defAttrNamespace)
,m_attributeValue(defAttrValue)
{

}

CXMLAttributeInsertDialog::~CXMLAttributeInsertDialog()
{
}

void CXMLAttributeInsertDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_ATTRIBUTE_NAME,		m_attributeName);
	DDX_Text(pDX,IDC_EDIT_ATTRIBUTE_NAMESPACE,	m_attributeNamespace);
	DDX_Text(pDX,IDC_EDIT_ATTRIBUTE_VALUE,		m_attributeValue);
}


BEGIN_MESSAGE_MAP(CXMLAttributeInsertDialog, CDialogEx)
END_MESSAGE_MAP()

BOOL CXMLAttributeInsertDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);	/* 将构造函数中控件默认值显示到控件中 */

	return TRUE;
}

int CXMLAttributeInsertDialog::GetChoice()
{
	return m_userChoice;
}
CString CXMLAttributeInsertDialog::GetAttributeName()
{
	return m_attributeName;
}
CString CXMLAttributeInsertDialog::GetAttributeNamespaceURI()
{
	return m_attributeNamespace;
}
CString CXMLAttributeInsertDialog::GetAttributeValue()
{
	return m_attributeValue;
}

// CXMLAttributeInsertDialog 消息处理程序
void CXMLAttributeInsertDialog::OnOK()
{
	UpdateData(TRUE);
	if (m_attributeName.IsEmpty())
	{
		HWND hWnd = this->GetSafeHwnd();
		::MessageBox(hWnd,TRANS_T("属性名称不能为空!"),
			_T("Error!"),MB_OK|MB_ICONERROR);
		return;
	}
	m_userChoice = IDOK;
	CDialogEx::OnOK();
}
void CXMLAttributeInsertDialog::OnCancel()
{
	UpdateData(TRUE);
	m_userChoice = IDCANCEL;
	CDialogEx::OnCancel();
}