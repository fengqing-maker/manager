#pragma once


// CXMLElementInsertDialog 对话框

class CXMLElementInsertDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CXMLElementInsertDialog)

public:
	CXMLElementInsertDialog(CString defElemName,CString defElemNamespace,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXMLElementInsertDialog();

// 对话框数据
	enum { IDD = IDD_SCLFILE_ELEMENT_INSERT_DIALOG };

	/** @brief 返回用户选择,确定：IDOK，取消：IDCANCEL */
	int		GetChoice();
	/** @brief 返回元素名称 */
	CString GetElementName();
	/** @brief 返回元素名称空间 */
	CString GetElementNamespaceURI();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

private:

	/** @brief 记录用户选择 */
	int		m_userChoice;
	/** @brief 记录用户输入的元素名称 */
	CString m_elementName;
	/** @brief 记录用户输入的元素名称空间 */
	CString m_elementNamespace;
};
