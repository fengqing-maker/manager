#ifndef SCHEDULE_MANGER_TASK_MANGER
#define SCHEDULE_MANGER_TASK_MANGER
/**
*@file	myTaskGirdManager
*@brief	发布任务进度列表管理
*@note	
*		
*@author	lihuizhen
*/
#include "../baseGridManger.h"
typedef struct SchedulePos
{
	int  dataPos; //在数据列表的第几个
	int  scheduleCount; //进度的个数
	int  baseShowRow; //开始显示的行数
	SchedulePos()
	{
		dataPos = 0;
		scheduleCount = 0;
		baseShowRow = 0;
	}
}SCHEDULEPOS;
typedef std::map<int, SchedulePos>  SchedulePosMap;
class CTaskScheduleManger: public CBaseGridManger
{
public:
	//
	CTaskScheduleManger( CActionServerFactory *pSeverFactory );
	void UpdateShowAllSchedul();
	void UpdateShowDepartSchedul();
	void UpdateShowAllPrjSchedul();
	void UpdateShowPrjSchedul( const CProject &prjObj );
	void UpdateDbDataMapAndShowAll();
	//消息处理
	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateScheulContent( int schId );
	void SelectScheuleLine( int schId );
protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
	virtual void AftCreateGridView();
	//行列设置
	virtual int     GetFlexRowCount();
protected:
	const SchedulePos*  GetSchedulePos( int row );
	CSchedule*          GetScheduleByRow( int nRow );
    void                ShowAllSchedul(const CString &strType = _T(""), int prjId = -1); //0:全部 1：部门任务 2项目任务
    int                 FindScheulById( int scheulId, ListSchedule::iterator &retItor );
protected:
	MapTaskSchedule   m_showSchedule;
	SchedulePosMap    m_showTaskMap;  //显示顺序和数据顺序的映射
	int               m_showRowCount; 
	

	
};
#endif