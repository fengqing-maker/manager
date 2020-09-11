#include "stdafx.h"
#include "DbSev.h"
#include "inaddr.h"
#include "Src/MainFrame/Output/ClientOutputManager.h"
#include "Src/Tools/BaseThread/SmartLock.h"
//CString CBaseDbSev::m_strConIP;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
CMySqlBaseImp::CMySqlBaseImp()
{

}

CMySqlBaseImp::~CMySqlBaseImp()
{
	CloseConn();
}
/************************************************************************
*																		
*  NAME:			AdoConn                                                 
*  FUNCTION:		连接数据库
*                  
*  PARAMETER:															
*					Input:	ip:数据库服务所在的主机IP 
*                   strDbName:连接的数据库名称
*																	
*	RETURN:																
*					TRUE：成功			            				
*					FALSE:失败
*	CODER:			lhz													
*	DATE:			2013.5.31													   
*																								
************************************************************************/
BOOL CMySqlBaseImp::OpenConn(DWORD ip, const CString &strDbName)
{
	//_T("Driver=MySQL ODBC 5.1 Driver;SERVER=%d;UID=root;PWD=123;DATABASE=%s;PORT=3306;COLUMN_SIZE_S32=1")
	CString strIP;
	struct  in_addr addr; 
	addr.S_un.S_addr = htonl(ip);
	strIP = inet_ntoa(addr);
	//SetdwIP(ip);
	//其实就是把网络字节序的IP重新换回主机字节序
	SetStrIP(strIP); //Driver=MySQL ODBC 5.2 Unicode Driver;SERVER=192.0.0.28;UID=root;DATABASE=sid_plad;PORT=3306
	//m_strCon.Format(_T("Driver={MySQL ODBC 5.1 Driver};SERVER=%s;UID=root;DATABASE=%s;PORT=3306;COLUMN_SIZE_S32=1"), strIP, strDbName);
	//m_strCon.Format(_T("Driver={MySQL ODBC 5.2 Unicode Driver};SERVER=127.0.0.1;UID=root;DATABASE=%s;PORT=3306;COLUMN_SIZE_S32=1"), strDbName);
    //m_strCon.Format(_T("Driver={MySQL ODBC 5.2 Unicode Driver};SERVER=%s;UID=myuser;PWD=123456;DATABASE=%s;PORT=3305;"), strIP, strDbName);
	m_strCon.Format(_T("Driver={MySQL ODBC 8.0 Unicode Driver};SERVER=%s;UID=root;PWD=szglzn778756837;DATABASE=%s;PORT=3306;"), strIP, strDbName);
	//m_strCon.Format(_T("Driver={MySQL ODBC 5.2 Unicode Driver};SERVER=%s;UID=root;PWD=szglzn778756837;DATABASE=%s;PORT=3306;"), strIP, _T("tasktest"));
	//test
	//
	//m_strCon.Format(_T("Driver={MySQL ODBC 5.2 Unicode Driver};SERVER=172.16.2.188;UID=myuser;PWD=123456;DATABASE=%s;PORT=3305;"),  _T("testsystem2") );
	//m_strCon.Format(_T("Driver={MySQL ODBC 5.2 Unicode Driver};SERVER=172.16.2.29;UID=myuser;PWD=123456;DATABASE=%s;PORT=3306;"),  _T("tasksystem") );
	//m_strCon.Format(_T("Driver={MySQL ODBC 5.2 Unicode Driver};SERVER=127.0.0.1;UID=root;DATABASE=%s;PORT=3306;"),  _T("testsystem2") );
	HRESULT   hr   =   S_OK;  
	_CatalogPtr   pCatalog   =   NULL;
	try
	{
		//创建数据库连接实例
		hr   =   pCatalog.CreateInstance(__uuidof(Catalog));
		if(FAILED(hr))
		{
			_com_issue_error(hr);
		}
		else
		{
			//pCatalog->Create(_bstr_t(m_strCon));   //Create   MDB
			//((_ConnectionPtr )pCatalog->GetActiveConnection())->Close();
		}
		if (m_adoDatabase.IsOpen())
		{
			m_adoDatabase.Close();
		}
		//打开数据库
		if( !m_adoDatabase.Open(m_strCon,adModeUnknown))
		{
			return FALSE;
		}
	}
	catch(_com_error &e)
	{
		CString strErr= e.Description();
		TRACE(strErr);
		ClientOutputManager::GetObj()->OutputMsg(_T("SQL Exception:") + strErr );
		AfxMessageBox( strErr ); //11111
		return FALSE;
	}
	catch (...)
	{
		AfxMessageBox( _T("未知错误") ); //1111111
		return FALSE;
	}
	return TRUE;
}
/************************************************************************
*																		
*  NAME:			CloseConn                                                 
*  FUNCTION:		关闭连接数据库
*                  
*  PARAMETER:															
*																	
*	RETURN:			无													
*	CODER:			lhz													
*	DATE:			2013.5.31													   
*																								
************************************************************************/
void CMySqlBaseImp::CloseConn()
{
	try
	{
		if (m_adoDatabase.IsOpen())
		{
			m_adoDatabase.Close();
		}
	}
	catch (...)
	{
		CString error = _T("close dataBase failure");
		ClientOutputManager::GetObj()->OutputMsg(_T("SQL Exception:") + error );
		TRACE( error );
	}
}
/************************************************************************
*																		
*  NAME:			Execute                                                 
*  FUNCTION:		执行sql语句，一般用于无结果值处理的语句
*                  
*  PARAMETER:		strSql:Sql创建语句
*																	
*	RETURN:			无													
*	CODER:			lhz													
*	DATE:			2013.5.31													   
*																								
************************************************************************/
BOOL CMySqlBaseImp::Execute(const CString &strSql)
{
	SmartCSLock lock( &m_dataCS );
	try
	{
		if (!m_adoDatabase.IsOpen())
		{//数据库未连接			
			return FALSE;
		}
		m_adoDatabase.Execute(strSql);
		return TRUE;
	}
	catch(_com_error &e)
	{
		CString str = e.Description();
		TRACE(str);
		ClientOutputManager::GetObj()->OutputMsg(_T("SQL Exception:") + str );
		return FALSE;
	}
	catch (...)
	{
		return FALSE;
	}

}
/************************************************************************
*																		
*  NAME:			Execute                                                 
*  FUNCTION:		执行sql语句，一般用于有结果值处理的语句
*                  
*  PARAMETER:		strSql:Sql查询语句等													
*																	
*	RETURN:			无													
*	CODER:			lhz													
*	DATE:			2013.5.31													   
*																								
************************************************************************/
BOOL CMySqlBaseImp::ExecuteSql(const CString &strSql)
{
	SmartCSLock lock( &m_dataCS );
	try
	{
		if (!m_adoDatabase.IsOpen())
		{//数据库未连接
			return FALSE;
		}
		if( m_recordset.IsOpen() )
		{
			m_recordset.Close();
		}
		return m_recordset.Open(strSql, m_adoDatabase, adLockOptimistic);
	}
	catch(_com_error &e)
	{
		CString strErr= e.Description();
		ClientOutputManager::GetObj()->OutputMsg(_T("SQL Exception:") + strErr );
		TRACE(strErr);
		return FALSE;
	}
	catch (...)
	{
		CString eror = _T("excute sql string failure");
		ClientOutputManager::GetObj()->OutputMsg(_T("SQL Exception:") + eror);
		TRACE( eror );
		return FALSE;
	}

}


void CMySqlBaseImp::SetStrIP( const CString & strIP )
{
	m_strIP=strIP;
	char DbStrIP[32]={0};
	WideCharToMultiByte(CP_ACP, 0, strIP,-1, DbStrIP, 32, NULL, NULL);
	m_dwIP=inet_addr(DbStrIP);
	m_dwIP=ntohl(m_dwIP);  //换回主机字节序
}

void CMySqlBaseImp::SetdwIP( const DWORD dwIP )
{
	m_dwIP=dwIP;
	in_addr inaddrIP;
	inaddrIP.S_un.S_addr = htonl(m_dwIP);
	m_strIP=inet_ntoa(inaddrIP);
}