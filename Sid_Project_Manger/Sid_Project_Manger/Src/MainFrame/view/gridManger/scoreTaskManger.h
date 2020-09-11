#ifndef SCORE_TASK_MANGER_H_234
#define SCORE_TASK_MANGER_H_234
/**
*@file	CScoreTaskManger
*@brief	����ҳ��
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
	//��д
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //��������
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //������ͷ
	virtual void CreateRowItemMap();
	//��������
	virtual int     GetFlexRowCount(){return m_showTaskMap.size();};
protected:
	 void           SetShowTask( const CString &taskType =_T("") );
	 CTaskObj*      GetTaskByRow( int nRow );
	 CString*       GetEditSuggestion( int nRow );
protected:
	 ListString         m_showView; //�������
	 ListTask           m_taskList; //����Ҫ���������
	 IntToIntMap        m_showTaskMap;
};

#endif