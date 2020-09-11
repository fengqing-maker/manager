#include "stdafx.h"
#include "PopProgressMsgTheard.h"
#include "../messageDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CProgressMsgThread,CBaseThread)

BEGIN_MESSAGE_MAP( CProgressMsgThread,CBaseThread )
	ON_THREAD_MESSAGE( WM_SHOW_DLG,	ShowDlgFromMsg)
	ON_THREAD_MESSAGE( WM_CLOSE_DLG, CloseProgessDlg)
END_MESSAGE_MAP()

CProgressMsgThread::CProgressMsgThread()
{
}
CProgressMsgThread::~CProgressMsgThread()
{

}
void     CProgressMsgThread::ShowMsgDlg( WPARAM msgId )
{
	PostThreadMessage(  WM_SHOW_DLG, msgId, NULL );
}
void     CProgressMsgThread::CloseMsgDlg()
{
	PostThreadMessage( WM_CLOSE_DLG, NULL, NULL);
}
void CProgressMsgThread::ShowDlgFromMsg( WPARAM wParam, LPARAM lParm )
{  
	switch ( wParam )
	{
	case MSG_CONNECT_SEVER:
		ShowProgessDlg(  _T("�������ӷ�����,���Ե�") );
		break;
	case MSG_INITE_LOADING:
		ShowProgessDlg(  _T("��½�ɹ������ڼ�������") );
		break;
	case MSG_READ_TASK_DATA:
		ShowProgessDlg(  _T("���ڼ�����������") );
		break;
	case MSG_READ_DATA:
		ShowProgessDlg(  _T("���ڼ�������") );
		break;
	}
}
void CProgressMsgThread::ShowProgessDlg( CString str )
{
	PD_CONTENT content;
	content.text = str;
	content.bCancelButtonVisible = FALSE;
	m_progressDialog.reset( new CProgressDialog( &content,TRUE ) );	/* ʹ��ģ̬�Ի���ķ�ʽ���ÿ��ʹ���������´��� */
	m_progressDialog->show( true );	/* Ӧ��ʹ��ģ̬�������Ի��� */
}
void CProgressMsgThread::CloseProgessDlg( WPARAM wParam, LPARAM lParm )
{
	if (m_progressDialog != NULL)
	{
		if ( IsWindow( m_progressDialog->GetSafeHwnd())  )
		{
			m_progressDialog->close();
		}
		
	}
}