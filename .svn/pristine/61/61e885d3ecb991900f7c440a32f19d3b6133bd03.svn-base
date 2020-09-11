#ifndef  CEXCEL_BASE_INTERFAC_345
#define  CEXCEL_BASE_INTERFAC_345

#include "../../AbstractDaoImp/base/AdoDatabase.h"
#include "../../AbstractDaoImp/base/AdoRecordset.h"


class CExcelBaseInc
{
public:
	CExcelBaseInc(){};
	virtual ~CExcelBaseInc(){};

	virtual BOOL OpenConn(const CString &dirName)=0;
	virtual void CloseConn() = 0;

	//用与执行create等无返回结果的sql语句
	virtual BOOL Execute(const CString &strSql) = 0;

	//用与执行查询等有结果的sql语句
	virtual BOOL ExecuteSql(const CString &strSql) = 0;
	//
	virtual BOOL IsOpen() = 0;

	const CAdoDatabase * GetDaoDataBase(){return &m_adoDatabase;}
	CAdoRecordset * GetRecordSet(){return &m_recordset;}

protected:
	//字符串
	CString  m_strCon;   //数据库连接字符串
	CAdoDatabase m_adoDatabase;
	CAdoRecordset m_recordset; //结果值
};

#endif