#ifndef MY_TASK_MANGER_KU394U
#define MY_TASK_MANGER_KU394U
/**
*@file	myTaskGirdManager
*@brief	我的任务列表管理(单例)
*@note	
*		
*@author	lihuizhen
*/
#include "../baseGridManger.h"
#include "Src/Tools/SingletonTemplate/SingletonTemplate.h"

typedef std::map< int , ListDate >   TaskUpdateDateMap;
typedef TaskUpdateDateMap::iterator TaskUpdateDateMapItor; 

class CTaskGridManager: public CBaseGridManger/*,public SingletonS<CTaskGridManager>*/
{
public:
	//
	CTaskGridManager( CActionServerFactory *pSeverFactory );

	void UpdateShowAllTask( int year, int quart ); //更新显示所有任务
	void UpdateOwnerTask(); //要处理的任务
	void UpdateNewingTask();
	void UpdateOngoingTask();
	void UpdateFinishTask();
	void UpdateWaitStartTask();
	void UpdateWaitReveiwTask();
	void SetUpdateDate( const CString &date );
	const ListDate&  GetSelListDate() { return m_setUpdateDateList;};
	//消息处理
	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnDeleteTask();
	void OnApplyForChangeTask();
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateTaskContent( int taskId );
	void SelectTaskLine( int taskId );
	void OnUpdateSchdule();
protected:
	//重写
	virtual CString      GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL         SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL         GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL         InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	virtual BOOL         InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn);
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
	//CBaseGridManger的虚函数
	virtual void CreateColumnMap();
	virtual void CreateRowItemMap();
	//行列设置
	virtual int     GetFlexRowCount();
	//私有操作
	void RefreshSchduleMap( BOOL reSetValue = FALSE );

    void OnOpreateBtnClick( CTaskObj &editTask, int row, int ncol);
protected:
	void          GetTaskBySelect(const CCellRange &selRange, ListTask &itor );
	CTaskObj*     GetTaskByRow( int nRow );
	CTaskObj*     ValidateRow( int nRow, int nColumn,  GridColumnMapItor &colItor  );
	void          ReSetUpdateDateList();
	BOOL          IsFinishTask( int taskId );
	BOOL          IsEnableUpdateSchdule( int taskId );
	double        GetTotalWorkHour();
	int           GetOpertorColunmNum();
	int           GetWorkTypeColunmNum();

private:
	 ListTask                  m_showTaskList;
	 TaskScheduleMap           m_currentUpdate;  
	// boost::scoped_ptr<BOOL>   m_operateFlag; //操作表示
	 bool                      m_isSelAllCol; //是否为整行选中
	 ListDate                  m_setUpdateDateList; //可设置进度的日期
	 TaskUpdateDateMap         m_taskUpdateDateMap;
	 COleDateTime              m_selUpdateDate; //选择更新的日期
	 CScheduleActionInc        *m_pSchduleAction;
	 CWorkHourSeverInc         *m_pWorkHourAction;
	 int                        m_loginUserId;
};

#endif