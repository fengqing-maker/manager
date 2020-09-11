#ifndef MY_TASK_MANGER_KU394U
#define MY_TASK_MANGER_KU394U
/**
*@file	myTaskGirdManager
*@brief	�ҵ������б����(����)
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

	void UpdateShowAllTask( int year, int quart ); //������ʾ��������
	void UpdateOwnerTask(); //Ҫ���������
	void UpdateNewingTask();
	void UpdateOngoingTask();
	void UpdateFinishTask();
	void UpdateWaitStartTask();
	void UpdateWaitReveiwTask();
	void SetUpdateDate( const CString &date );
	const ListDate&  GetSelListDate() { return m_setUpdateDateList;};
	//��Ϣ����
	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult );
	void OnDeleteTask();
	void OnApplyForChangeTask();
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateTaskContent( int taskId );
	void SelectTaskLine( int taskId );
	void OnUpdateSchdule();
protected:
	//��д
	virtual CString      GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL         SetFlexItemText(int nRow,int nColumn,CString text); //��������
	virtual BOOL         GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL         InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	virtual BOOL         InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn);
	virtual GridItemType GetFlexItemType(int nRow,int nColumn);
	//CBaseGridManger���麯��
	virtual void CreateColumnMap();
	virtual void CreateRowItemMap();
	//��������
	virtual int     GetFlexRowCount();
	//˽�в���
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
	// boost::scoped_ptr<BOOL>   m_operateFlag; //������ʾ
	 bool                      m_isSelAllCol; //�Ƿ�Ϊ����ѡ��
	 ListDate                  m_setUpdateDateList; //�����ý��ȵ�����
	 TaskUpdateDateMap         m_taskUpdateDateMap;
	 COleDateTime              m_selUpdateDate; //ѡ����µ�����
	 CScheduleActionInc        *m_pSchduleAction;
	 CWorkHourSeverInc         *m_pWorkHourAction;
	 int                        m_loginUserId;
};

#endif