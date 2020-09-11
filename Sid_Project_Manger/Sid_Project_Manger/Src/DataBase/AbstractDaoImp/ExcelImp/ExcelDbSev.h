#ifndef BASE_DB_SEV_
#define BASE_DB_SEV_

#include "../../inc/excel/CExcelBaseInc.h"

class CExcelBaseImp:public CExcelBaseInc
{
public:
	CExcelBaseImp();
	virtual ~CExcelBaseImp();

	virtual BOOL OpenConn( const CString &strFileDir );
	virtual void CloseConn();

	//用与执行create等无返回结果的sql语句
	virtual BOOL Execute(const CString &strSql);

	//用与执行查询等有结果的sql语句
	virtual BOOL ExecuteSql(const CString &strSql);
    
	virtual BOOL IsOpen(){ return m_adoDatabase.IsOpen();}


protected:

private:
};
#endif