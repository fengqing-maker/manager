#ifndef PRO_PROGERSS_MSG_THERAD_H
#define  PRO_PROGERSS_MSG_THERAD_H

#include "../../Tools/BaseThread/BaseThread.h"
#include "../ui/CProgressDialog/ProgressDialog.h"
#include <boost/scoped_ptr.hpp>


class CProgressMsgThread:public CBaseThread
{
	DECLARE_DYNCREATE(CProgressMsgThread)
public:
	CProgressMsgThread();
	virtual ~CProgressMsgThread();
	void     ShowMsgDlg( WPARAM msgId );
	void     CloseMsgDlg();
protected:
	void    ShowDlgFromMsg( WPARAM wParam, LPARAM lParm);
	DECLARE_MESSAGE_MAP()

	void    ShowProgessDlg( CString str );
	void    CloseProgessDlg( WPARAM wParam, LPARAM lParm );
protected:
	boost::scoped_ptr<CProgressDialog>		m_progressDialog;	/* 进度条对话框 */

private:
};

#endif