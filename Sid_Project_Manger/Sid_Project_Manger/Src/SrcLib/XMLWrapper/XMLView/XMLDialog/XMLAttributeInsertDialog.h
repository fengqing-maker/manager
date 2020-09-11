#pragma once


// CXMLAttributeInsertDialog 对话框

class CXMLAttributeInsertDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CXMLAttributeInsertDialog)

public:
	CXMLAttributeInsertDialog(CString defAttrName,CString defAttrNamespace,CString defAttrValue,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CXMLAttributeInsertDialog();

// 对话框数据
	enum { IDD = IDD_SCLFILE_ATTRIBUTE_INSERT_DIALOG };

	/** @brief 返回用户选择,确定：IDOK，取消：IDCANCEL */
	int		GetChoice();
	/** @brief 返回属性名称 */
	CString GetAttributeName();
	/** @brief 返回属性名称空间 */
	CString GetAttributeNamespaceURI();
	/** @brief 返回属性值 */
	CString GetAttributeValue();

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	afx_msg void OnOK();
	afx_msg void OnCancel();

	DECLARE_MESSAGE_MAP()

private:

	/** @brief 记录用户选择 */
	int		m_userChoice;
	/** @brief 记录用户输入的属性名称 */
	CString m_attributeName;
	/** @brief 记录用户输入的属性名称空间 */
	CString m_attributeNamespace;
	/** @brief 记录用户输入的属性值 */
	CString m_attributeValue;
};
