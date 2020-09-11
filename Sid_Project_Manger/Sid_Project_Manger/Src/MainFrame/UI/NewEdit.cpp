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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_isEnableEdit || !nChar )
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}
