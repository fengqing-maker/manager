#ifndef  SYSTEM_CONFIG_DB_ADO_IW3E4Y8I
#define  SYSTEM_CONFIG_DB_ADO_IW3E4Y8I

#include "../inc/SystemCfgDbInterface.h"

class CSystemDbAdoImp: public CSystemCfgDbInc
{
public:
    CSystemDbAdoImp( CDaoSupportInterface* pDao ) ;

	virtual BOOL InsertProjectStep(  CProjectStep &step ); //插入
	virtual BOOL UpdateProjectStep( const CProjectStep &step );
	virtual BOOL GetProjectStepList( const CString &type, ListProjectStep &obj );
	virtual BOOL GetProjectStepType( ListString &lst ); //获取项目流程类型
	virtual BOOL GetProjectStep( const CString stepName, CProjectStep &step );
	virtual BOOL GetProjectStep( const CString& type, const CString stepName, CProjectStep &step );
	virtual BOOL GetProjectStep( int stepId, CProjectStep &step );
	//假期
	virtual BOOL IsHoliday( const COleDateTime &date ,BOOL &reslut );
	virtual BOOL AddHoliday( const COleDateTime &date );
	virtual BOOL DeleteHoliday( const COleDateTime &date );

	virtual BOOL ValidateProjectStepExist( const CProjectStep &data );
protected:
	BOOL ExecProjectStepSql( const CString &str, ListProjectStep &obj );

	void FillTimeDataFromFild( CAdoRecordset* pRecordSet, CProjectStep &obj  ) const;

protected:
	const CString m_tableHolidaysName; //假日表
	const CString m_prjProcTableName; //项目流程表
};

#endif