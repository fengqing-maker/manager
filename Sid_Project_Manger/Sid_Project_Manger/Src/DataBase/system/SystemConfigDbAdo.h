#ifndef  SYSTEM_CONFIG_DB_ADO_IW3E4Y8I
#define  SYSTEM_CONFIG_DB_ADO_IW3E4Y8I

#include "../inc/SystemCfgDbInterface.h"

class CSystemDbAdoImp: public CSystemCfgDbInc
{
public:
    CSystemDbAdoImp( CDaoSupportInterface* pDao ) ;

	virtual BOOL InsertProjectStep(  CProjectStep &step ); //����
	virtual BOOL UpdateProjectStep( const CProjectStep &step );
	virtual BOOL GetProjectStepList( const CString &type, ListProjectStep &obj );
	virtual BOOL GetProjectStepType( ListString &lst ); //��ȡ��Ŀ��������
	virtual BOOL GetProjectStep( const CString stepName, CProjectStep &step );
	virtual BOOL GetProjectStep( const CString& type, const CString stepName, CProjectStep &step );
	virtual BOOL GetProjectStep( int stepId, CProjectStep &step );
	//����
	virtual BOOL IsHoliday( const COleDateTime &date ,BOOL &reslut );
	virtual BOOL AddHoliday( const COleDateTime &date );
	virtual BOOL DeleteHoliday( const COleDateTime &date );

	virtual BOOL ValidateProjectStepExist( const CProjectStep &data );
protected:
	BOOL ExecProjectStepSql( const CString &str, ListProjectStep &obj );

	void FillTimeDataFromFild( CAdoRecordset* pRecordSet, CProjectStep &obj  ) const;

protected:
	const CString m_tableHolidaysName; //���ձ�
	const CString m_prjProcTableName; //��Ŀ���̱�
};

#endif