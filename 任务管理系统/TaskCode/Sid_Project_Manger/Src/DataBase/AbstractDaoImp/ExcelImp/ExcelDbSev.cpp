#include "stdafx.h"
#include "ExcelDbSev.h"
#include "inaddr.h"
#include "Src/MainFrame/Output/ClientOutputManager.h"
//CString CBaseDbSev::m_strConIP;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExcelBaseImp::CExcelBaseImp()
{

}

CExcelBaseImp::~CExcelBaseImp()
{
	CloseConn();
}
/************************************************************************
*																		
*  NAME:			AdoConn                                                 
*  FUNCTION:		连接数据库
*                  
*  PARAMETER:															
*					Input:	strFileName:excel文件的																
*	RETURN:																
*					TRUE：成功			            				
*					FALSE:失败
*	CODER:			lhz													
*	DATE:			2013.5.31													   
*																								
************************************************************************/
BOOL CExcelBaseImp::OpenConn(const CString &strFileName)
{

	m_strCon.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=0\";"),strFileName);
	//m_strCon.Format(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s;Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=1\";"),strFileName);
	
	HRESULT   hr   =   S_OK;  
	_CatalogPtr   pCatalog   =   NULL;

	try
	{
		//创建数据库
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
		if( !m_adoDatabase.Open(m_strCon,adModeUnknown))
		{
			return FALSE;
		}
	}
	catch(_com_error &e)
	{
		CString strErr= e.Description();
		ClientOutputManager::GetObj()->OutputMsg(_T("Excel Exception:") + strErr );
		TRACE(strErr);
		return FALSE;
	}
	catch (...)
	{

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
void CExcelBaseImp::CloseConn()
{
	try
	{
		if (m_adoDatabase.IsOpen())
		{
			m_adoDatabase.Close();
		}
	}catch (...)
	{
		CString strErr = _T("close dataBase failure");
		TRACE( strErr );
		ClientOutputManager::GetObj()->OutputMsg(_T("Excel Exception:") + strErr );
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
BOOL CExcelBaseImp::Execute(const CString &strSql)
{
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
		CString strErr = e.ErrorMessage();
		TRACE(strErr);
		ClientOutputManager::GetObj()->OutputMsg(_T("Excel Exception:") + strErr );
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
BOOL CExcelBaseImp::ExecuteSql(const CString &strSql)
{
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
		TRACE(strErr);
		ClientOutputManager::GetObj()->OutputMsg(_T("Excel Exception:") + strErr );
		return FALSE;
	}
	catch (...)
	{
		TRACE(_T("excute sql string failure"));
		return FALSE;
	}

}


