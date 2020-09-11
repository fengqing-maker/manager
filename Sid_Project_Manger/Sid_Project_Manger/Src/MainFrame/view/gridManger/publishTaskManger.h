#ifndef PUBLISH_TASK_MANGER
#define PUBLISH_TASK_MANGER

/**
*@file	myTaskGirdManager
*@brief	发布任务列表管理
*@note	
*		
*@author	lihuizhen
*/
#include "../baseGridManger.h"


class CPublishTaskManger: public CBaseGridManger
{
public:
	//
	CPublishTaskManger( CActionServerFactory *pSeverFactory );
	void      UpdateDbData();
	void      UpdateAllTask();
	void      UpdateDepartTask();
	void      UpdateProjectTask();

	//消息处理
	void      OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void      OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void      UpdateTaskContent( int taskId );
	 void     SelectTaskLine( int taskId );
protected:
	 //重写
	 virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	 virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	 virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	 virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);

	 virtual void CreateColumnMap(); //创建列头
	 virtual void CreateRowItemMap();
	 //行列设置
	 virtual int     GetFlexRowCount();

protected:
	CTaskObj*        GetTaskByRow( int nRow );
	CString*         GetEditSuggestion( int nRow );
	void             RefreshShowMap( bool isAll, const CString &str );
protected:
	 ListString         m_showView; //评审意见
	 ListTask           m_taskList; //所有要处理的任务
	 IntToIntMap       m_showTaskMap;
};

#endif