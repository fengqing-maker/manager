#pragma once
#include "resource.h"
#include "../CComonDlg.h"
// CNewWorkType �Ի���

class CDlgNewWorkType : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgNewWorkType)

public:
	CDlgNewWorkType(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgNewWorkType();

// �Ի�������
	enum { IDD = IDD_DLG_NEW_WORKTYPE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
