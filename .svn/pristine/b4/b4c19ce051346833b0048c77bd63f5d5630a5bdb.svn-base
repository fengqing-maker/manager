#pragma once
#include "afxwin.h"
class CNewEdit :
	public CEdit
{
public:
	CNewEdit(void);
	~CNewEdit(void);
	void SetEnableEdit( BOOL isEnable ) { m_isEnableEdit = isEnable;}
protected:
	DECLARE_MESSAGE_MAP()

	//
	BOOL m_isEnableEdit;
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};
