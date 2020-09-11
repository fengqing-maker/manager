#ifndef BASE_DB_SEV_
#define BASE_DB_SEV_

#include "../../inc/DaoSupportInterface.h"

class CMySqlBaseImp:public CDaoSupportInterface
{
public:
	CMySqlBaseImp();
	virtual ~CMySqlBaseImp();

	virtual BOOL OpenConn(DWORD ip, const CString &strDbName);
	virtual void CloseConn();

	//用与执行create等无返回结果的sql语句
	virtual BOOL Execute(const CString &strSql);

	//用与执行查询等有结果的sql语句
	virtual BOOL ExecuteSql(const CString &strSql);
    
	virtual BOOL IsOpen(){ return m_adoDatabase.IsOpen();}

	//IP操作, 都有需求
	void SetStrIP(const CString & strIP);
	void SetdwIP(const DWORD dwIP);
	const CString & GetStrIP()const {return m_strIP;}
	const DWORD GetdwIP()const {return m_dwIP;}

	//
protected:
	CString m_strIP;
	DWORD   m_dwIP;
	CCriticalSection	       m_dataCS;

private:
};
#endif