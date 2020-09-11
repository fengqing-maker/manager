#ifndef CTASK_EXCEL_ACTION_HEARD_347
#define CTASK_EXCEL_ACTION_HEARD_347

#include "../inc/TaskExcelActionInc.h"

class CTaskExcelAction: public CTaskExcelActionInc
{

public:
	
	CTaskExcelAction( CActionServerFactory *pFactory );

	BOOL  ImportTask(const CString &dirStr, const CProject &prj );
	BOOL  ExportTask( const CString &dirStr, const CString &tabName ,const ListTask &lst );
	BOOL  ExportWorkHour(const CString &dirStr, COleDateTime &beginTime, COleDateTime &endTime );
	BOOL  ExportMonthWorkHour( const CString &dirStr, CWorkHourData *pData, const ListUser&,  const TaskIdStringMap &);
protected:
	BOOL  ValidateExcelTask( const CProject &prj, const CExcelTaskModel &excelObj, CTaskObj &obj );
	BOOL  UpdateTask( const CTaskObj &obj );
	BOOL  UpdateSchude( const CExcelTaskModel &excelObj, BOOL isUpdateTask );
	BOOL  UpdateFatherRelate( ListExcelTaskMode &lst );
	BOOL  OpenExportDir( const CString &dirStr );
	void  GetWorkHourSchduleInfo( int workId, const ListSchedule &lst, WorkHourInfo &info );
protected:
    CTaskSeverInc *m_pTaskSever;
	CLoginActionInc *m_pLoginActionInc;

};

#endif