#ifndef DAO_SUPPORTINTERFACE34j3_26526
#define DAO_SUPPORTINTERFACE34j3_26526

//所有数据库服务的基类
#include "daocomonDef.h"
#include "../AbstractDaoImp/base/AdoDatabase.h"
#include "../AbstractDaoImp/base/AdoRecordset.h"
#include <list>
typedef std::list<int> ListInt;
typedef ListInt::iterator          ListIntItor;

class CDaoSupportInterface
{
public:
	CDaoSupportInterface(){};
	virtual ~CDaoSupportInterface(){};

	virtual BOOL OpenConn(DWORD ip, const CString &strDbName)=0;
	virtual void CloseConn() = 0;

	//用与执行create等无返回结果的sql语句
	virtual BOOL Execute(const CString &strSql) = 0;

	//用与执行查询等有结果的sql语句
	virtual BOOL ExecuteSql(const CString &strSql) = 0;
	//
	virtual BOOL IsOpen() = 0;

	const CAdoDatabase * GetDaoDataBase(){return &m_adoDatabase;}
	CAdoRecordset * GetRecordSet()
	{
		return &m_recordset;
	}


protected:
	//字符串
	CString  m_strCon;   //数据库连接字符串
	CAdoDatabase m_adoDatabase;
	CAdoRecordset m_recordset; //结果值
};

#endif