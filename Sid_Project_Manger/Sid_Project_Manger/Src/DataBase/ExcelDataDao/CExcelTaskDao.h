#ifndef  CEXCELL_DATA_DAO_HEARD_234
#define  CEXCELL_DATA_DAO_HEARD_234

#include "../inc/excel/CExcelTaskDaoInc.h"
#include "../inc/daocomonDef.h"



//导入导出任务数据的excel
class CExcelTaskDao: public CExcelTaskDaoInc
{
public:
	CExcelTaskDao();
	virtual ~CExcelTaskDao();

	virtual BOOL GetExcelTask( ListExcelTaskMode &modeList );
	virtual BOOL ExportTask(const CString &prjName,const ListExcelTaskMode &lst );
	virtual BOOL ExportWorkHour( const CString &tableName, const COleDateTime &beginDate, const COleDateTime &endDate,WorkHourList &datas);
	virtual BOOL ExportWorkHour( const CString &tableName, CWorkHourData *pData, const ListUser& lstUser, const TaskIdStringMap &typeMap );
protected:
	BOOL          FillExcelTaskFild( CAdoRecordset* pRecordSet, CExcelTaskModel &obj ) const;
	void          FillFildData(  CAdoRecordset* pRecordSet, const CExcelTaskModel &obj ) const;
protected:
	//ListString  m_tableColomName;
	CString     m_strSheetName;
};


#endif