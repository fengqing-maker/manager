#include "StdAfx.h"
#include "BluFontBotton.h"

BEGIN_MESSAGE_MAP(CBluFontBotton, CButton)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

CBluFontBotton::CBluFontBotton(void)
{
	m_isShowBlu = false;
}
CBluFontBotton::~CBluFontBotton(void)
{
}
HBRUSH CBluFontBotton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CButton::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if ( m_isShowBlu )
	{
		pDC->SetTextColor( RGB(0, 162, 232) );
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}