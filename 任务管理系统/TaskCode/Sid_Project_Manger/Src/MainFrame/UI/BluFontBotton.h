#pragma once
#include "afxwin.h"
class CBluFontBotton :
	public CButton
{
public:
	CBluFontBotton(void);
	~CBluFontBotton(void);
	void SetShowColor( bool bValue ){ m_isShowBlu = bValue;}
protected:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

	bool m_isShowBlu;
};

