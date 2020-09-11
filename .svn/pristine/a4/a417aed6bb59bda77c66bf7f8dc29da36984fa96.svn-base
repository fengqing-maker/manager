#pragma once

#include "../../action/factory/actionServerFactory.h"

class CComonDlg:public CDialogEx
{
	DECLARE_DYNAMIC(CComonDlg)
public:
	CComonDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CComonDlg();

	//BOOL  IsHasPower( int userId, int power );

protected:
	DECLARE_MESSAGE_MAP()
protected:
	CActionServerFactory *m_pSever;
	CTaskSeverInc        *m_pTaskSever;
	CLoginActionInc      *m_pLoginAction;
};