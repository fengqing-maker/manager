#ifndef  WORK_HOUR_INC_3847
#define WORK_HOUR_INC_3847
/*
 任务变更的操作数据接口
*/
#include "DaoSupportInterface.h"
#include "../../Model/WorkHourData.h"
#include < list >
typedef std::list<CWorkHourData> HourDataList;
typedef HourDataList::iterator   HourDataListItor;
typedef HourDataList::const_iterator   HourDataListConstItor;

class CWorkHourDbInc
{
public:
	CWorkHourDbInc( CDaoSupportInterface* pDao ){ m_pDaoSupportInc = pDao;}
	virtual ~CWorkHourDbInc(){};

	virtual BOOL UpdateTimeData( CWorkHourData &data ) = 0;
	virtual BOOL GetTimeData( int timeType, int userId, const COleDateTime &time, CWorkHourData &obj ) = 0;
    virtual BOOL GetNorTaskHourSum( int userId, const COleDateTime &time, double &relsut ) = 0;

protected:
	CDaoSupportInterface* m_pDaoSupportInc;
};
#endif