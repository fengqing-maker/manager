#ifndef TASK_WORK_TYPE_1234598
#define TASK_WORK_TYPE_1234598

#include "TypeDefHead.h"


class CWorkTimeType
{
public:
	bool            GetWorkTimeTypeById( int id, CString &strStatus ) const;
	void            Clear(){ m_workTimeMap.clear();}
	TaskIdStringMap&  GetWorkTimeTypMap()  { return m_workTimeMap;}
	int                     FindWorkTimeIdByString( const CString &str ) const;
private:
	TaskIdStringMap m_workTimeMap; //״̬
};

#endif