#ifndef SCORE_TASK_MANGER_H_234
#define SCORE_TASK_MANGER_H_234
/**
*@file	CScoreTaskManger
*@brief	评分页面
*@note	
*		
*@author	lihuizhen
*/
#include "../baseGridManger.h"
class CScoreTaskManger:public CBaseGridManger
{
public:
	CScoreTaskManger( CActionServerFactory *pSeverFactory );
	void UpdateShowAllTask();
	void UpdateShowDepartTask();
	void UpdateShowProjectTask();
	void RefreshTaskList();
	void UpdateDb();

	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void      UpdateTaskContent( int taskId );
	void     SelectTaskLine( int taskId );
	virtual BOOL EndEditGridView(int nRow,int nColumn);
protected:
	//重写
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //创建列头
	virtual void CreateRowItemMap();
	//行列设置
	virtual int     GetFlexRowCount(){return m_showTaskMap.size();};
protected:
	 void           SetShowTask( const CString &taskType =_T("") );
	 CTaskObj*      GetTaskByRow( int nRow );
	 CString*       GetEditSuggestion( int nRow );
protected:
	 ListString         m_showView; //评审意见
	 ListTask           m_taskList; //所有要处理的任务
	 IntToIntMap        m_showTaskMap;
};

#endif