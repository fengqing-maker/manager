#ifndef  CEXCELTASKDAO_INC_HEARD_3947
#define  CEXCELTASKDAO_INC_HEARD_3947

#include <boost/scoped_ptr.hpp>
#include "CExcelBaseInc.h"
#include "../../AbstractDaoImp/ExcelImp/ExcelDbSev.h"
#include "Src/Model/excelObj/CExcelTaskModel.h"
#include "../taskdaointerface.h"
#include "Src/Model/WorkHourData.h"
#include <list>
typedef std::list<CExcelTaskModel> ListExcelTaskMode;
typedef ListExcelTaskMode::iterator ListExcelTaskModeItor;
struct WorkHourInfo
{
	int     workId;
	CString workHourType;
	CString schudulInfo;
	double   totalWorkHour;
	WorkHourInfo()
	{
		totalWorkHour = 0.0;
	}
	void CopyObj( const WorkHourInfo &obj )
	{
		workId = obj.workId;
		workHourType = obj.workHourType;
		schudulInfo = obj.schudulInfo;
		totalWorkHour = obj.totalWorkHour;
	}
	WorkHourInfo& operator=( const WorkHourInfo &obj)
	{
		CopyObj( obj );
		return *this;
	}
	WorkHourInfo( const WorkHourInfo &obj )
	{
		CopyObj( obj );
	}
};
typedef std::list<WorkHourInfo> WorkHourList; //工时类型id，以及其描述

class CExcelTaskDaoInc
{
public:
	CExcelTaskDaoInc() { m_pDaoSupport.reset( new CExcelBaseImp() );};
	virtual ~CExcelTaskDaoInc(){};

	virtual BOOL GetExcelTask( ListExcelTaskMode &modeList ) = 0;
	virtual BOOL ExportTask( const CString &prjName,const ListExcelTaskMode &lst ) = 0;
	virtual BOOL ExportWorkHour( const CString &tableName, const COleDateTime &beginDate, const COleDateTime &endDate,WorkHourList &datas  ) = 0;
	virtual BOOL ExportWorkHour( const CString &tableName, CWorkHourData *pData, const ListUser& lstUser, const TaskIdStringMap &typeMap ) = 0;
	virtual BOOL OpenConn(const CString &dirName) { ASSERT(m_pDaoSupport.get()); return m_pDaoSupport->OpenConn(dirName); };
	virtual void CloseConn(){ASSERT(m_pDaoSupport.get());  m_pDaoSupport->CloseConn();} 

	
protected:
   boost::scoped_ptr<CExcelBaseInc> m_pDaoSupport;

};

#endif