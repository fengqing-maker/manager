#include "stdafx.h"
#include "connectActionImp.h"

CConectDbImp::CConectDbImp(CDaoSupportInterface *pDao):CConnectDbServerInc(pDao)
{
	ASSERT( pDao != NULL );
	//m_ip = inet_addr("127.0.0.1");
	//m_ip = inet_addr("172.16.2.188");
	m_ip = inet_addr("172.16.2.102");
	//m_ip = inet_addr("172.16.3.160");
	m_ip = htonl(m_ip);
	//m_dbName = _T("tasksystem");
	m_dbName = _T("tasksystem101"); //�汾v1.01 ���ݿ�
	m_dbName = _T("tasksystem102");//�汾v1.02 ���ݿ�
	m_dbName = _T("tasksystem103");//�汾v1.03 ���ݿ�
	m_dbName = _T("tasksystem104");//�汾v1.04 ���ݿ�
	//m_dbName = _T("tasksystem_gongyi");
}

BOOL CConectDbImp::IsConect()
{
	return m_pDaoBase->IsOpen();
}
BOOL CConectDbImp::Connect()
{
	 return m_pDaoBase->OpenConn( m_ip, m_dbName );
}
void CConectDbImp::CloseConnect() 
{
	 return m_pDaoBase->CloseConn();
}