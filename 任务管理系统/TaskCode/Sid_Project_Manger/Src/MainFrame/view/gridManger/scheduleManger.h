#ifndef SCHEDULE_MANGER_TASK_MANGER
#define SCHEDULE_MANGER_TASK_MANGER
/**
*@file	myTaskGirdManager
*@brief	������������б����
*@note	
*		
*@author	lihuizhen
*/
#include "../baseGridManger.h"
typedef struct SchedulePos
{
	int  dataPos; //�������б�ĵڼ���
	int  scheduleCount; //���ȵĸ���
	int  baseShowRow; //��ʼ��ʾ������
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
	//��Ϣ����
	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );
	void UpdateScheulContent( int schId );
	void SelectScheuleLine( int schId );
protected:
	//��д
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //��������
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);

	virtual void CreateColumnMap(); //������ͷ
	virtual void CreateRowItemMap();
	virtual void AftCreateGridView();
	//��������
	virtual int     GetFlexRowCount();
protected:
	const SchedulePos*  GetSchedulePos( int row );
	CSchedule*          GetScheduleByRow( int nRow );
    void                ShowAllSchedul(const CString &strType = _T(""), int prjId = -1); //0:ȫ�� 1���������� 2��Ŀ����
    int                 FindScheulById( int scheulId, ListSchedule::iterator &retItor );
protected:
	MapTaskSchedule   m_showSchedule;
	SchedulePosMap    m_showTaskMap;  //��ʾ˳�������˳���ӳ��
	int               m_showRowCount; 
	

	
};
#endif