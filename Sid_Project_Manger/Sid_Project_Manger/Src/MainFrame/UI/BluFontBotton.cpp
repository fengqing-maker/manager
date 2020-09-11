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

	// TODO:  �ڴ˸��� DC ���κ�����

	if ( m_isShowBlu )
	{
		pDC->SetTextColor( RGB(0, 162, 232) );
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}