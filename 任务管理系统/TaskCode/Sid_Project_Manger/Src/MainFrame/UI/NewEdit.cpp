#include "stdafx.h"
#include "NewEdit.h"

BEGIN_MESSAGE_MAP(CNewEdit, CEdit)
	
	ON_WM_CHAR()
END_MESSAGE_MAP()

CNewEdit::CNewEdit(void)
{
	 m_isEnableEdit = true;
}


CNewEdit::~CNewEdit(void)
{
}


void CNewEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( m_isEnableEdit || !nChar )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
