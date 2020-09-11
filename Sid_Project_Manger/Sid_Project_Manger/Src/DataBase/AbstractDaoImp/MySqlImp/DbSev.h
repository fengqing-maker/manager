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

	//����ִ��create���޷��ؽ����sql���
	virtual BOOL Execute(const CString &strSql);

	//����ִ�в�ѯ���н����sql���
	virtual BOOL ExecuteSql(const CString &strSql);
    
	virtual BOOL IsOpen(){ return m_adoDatabase.IsOpen();}

	//IP����, ��������
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