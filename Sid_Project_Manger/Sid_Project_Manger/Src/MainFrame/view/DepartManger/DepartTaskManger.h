#ifndef PUBLISH_TASK_MANGER
#define PUBLISH_TASK_MANGER

/**
*@file	myTaskGirdManager
*@brief	部门管理的任务页面
*@note	
*		
*@author	lihuizhen
*/
#include "./DepartMangerBase.h"


class CDepartTaskManger: public CDepartBaseGridManger
{
public:
	//
	CDepartTaskManger( CActionServerFactory *pSeverFactory );
	void      SetDefDepart(  int id, int taskST ); 
	void      SetSelDepart( int  id, int taskST, int selUserId, int diffPer, int year, int quarter );
	void      SetFirstShowTaskStatus( int st ){ m_showStatus = st;};
	void      ImportDepartTask();
	void      ExportDepartTask();
	void      ExportPerformanTask( int year, int quart, int depart, int selUserId  );
    void      SetTaskStatus( int st );
	//消息处理
	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridClick( NMHDR* pNMHDR,LRESULT* pResult );
    void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridDepartReview();
	void OnGridDepartMoidyScore();
	ListTask* GetTaskListPoint(){ return &m_taskList; };
protected:
	 //重写
	 virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	 virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	 virtual BOOL    GetFlexItemEditable(int nRow,int nColumn){ return FALSE;};

	 virtual void CreateColumnMap(); //创建列头
	 virtual void CreateRowItemMap();
	 //行列设置
	 virtual int     GetFlexRowCount();

protected:
	CTaskObj*        GetTaskByRow( int nRow );
	void             RefreshShowMap(   );
	void             UpdateDbData();
protected:
	 ListTask           m_taskList; //所有要处理的任务
	 IntToIntMap        m_showTaskMap;
	 int                m_showDepartId;
	 int                m_showStatus;
	 int                m_diffPer;
	 int                m_selUserId; //0,表示所有员工
	 BOOL               m_hasReviewPower;
	 int                m_year;
	 int                m_quart;
};

#endif