#ifndef  SYSTEM_CONFIG_IMP_SEVER_H
#define  SYSTEM_CONFIG_IMP_SEVER_H

#include "../inc/SystemConfigInc.h"

class  CSystemCfgSeverImp:public CSystemCfgSeverInc
{
public:
	CSystemCfgSeverImp(  CDaoFactory *pFactory );
    virtual BOOL GetProjectStep( const CString &stepName, CProjectStep &step );
	virtual BOOL GetProjectStep( const CString&type, const CString &stepName, CProjectStep &step );
    virtual BOOL GetProjectStepList( const CString &type, ListProjectStep &obj) ;
    virtual BOOL GetProjectStep( int stepId, CProjectStep &step );

	virtual BOOL   IsHoliday( const COleDateTime &date ,BOOL &reslut );
	virtual BOOL   AddHoliday( const COleDateTime &date );
	virtual BOOL   DeleteHoliday( const COleDateTime &date );
	virtual double CaluateForecastWorkHour( int userId, const COleDateTime &beginDate );

	double CaluateTaskWorkHourEveday( const CTaskObj &obj );
protected:
	CSystemCfgDbInc *m_pSystemCfgDb;
};


#endif