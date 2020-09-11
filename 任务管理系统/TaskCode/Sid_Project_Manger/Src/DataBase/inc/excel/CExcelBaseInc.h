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

	//����ִ��create���޷��ؽ����sql���
	virtual BOOL Execute(const CString &strSql) = 0;

	//����ִ�в�ѯ���н����sql���
	virtual BOOL ExecuteSql(const CString &strSql) = 0;
	//
	virtual BOOL IsOpen() = 0;

	const CAdoDatabase * GetDaoDataBase(){return &m_adoDatabase;}
	CAdoRecordset * GetRecordSet(){return &m_recordset;}

protected:
	//�ַ���
	CString  m_strCon;   //���ݿ������ַ���
	CAdoDatabase m_adoDatabase;
	CAdoRecordset m_recordset; //���ֵ
};

#endif