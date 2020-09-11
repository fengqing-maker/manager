#ifndef  SYTEM_CONFIG_SEVER_SEUR9I3_
#define  SYTEM_CONFIG_SEVER_SEUR9I3_

#include "../../DataBase/factory/CDaoFactory.h"

class CSystemCfgSeverInc
{
public:
	CSystemCfgSeverInc( CDaoFactory  *pObj ) { m_pDbDao = pObj; }
	virtual ~CSystemCfgSeverInc(){};

	virtual BOOL GetProjectStep( const CString &stepName, CProjectStep &step ) = 0;
    virtual BOOL GetProjectStepList( const CString &type, ListProjectStep &obj) = 0;
    virtual BOOL GetProjectStep( const CString&type, const CString &stepName, CProjectStep &step ) = 0;
	virtual BOOL GetProjectStep( int stepId, CProjectStep &step ) = 0;

	virtual BOOL IsHoliday( const COleDateTime &date ,BOOL &reslut ) = 0;
	virtual BOOL AddHoliday( const COleDateTime &date ) = 0;
	virtual BOOL DeleteHoliday( const COleDateTime &date ) =0;
	virtual double CaluateForecastWorkHour( int userId, const COleDateTime &beginDate ) = 0;
protected:
	CDaoFactory       *m_pDbDao;

};

#endif