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
	m_dbName = _T("tasksystem101"); //版本v1.01 数据库
	m_dbName = _T("tasksystem102");//版本v1.02 数据库
	m_dbName = _T("tasksystem103");//版本v1.03 数据库
	m_dbName = _T("tasksystem104");//版本v1.04 数据库
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