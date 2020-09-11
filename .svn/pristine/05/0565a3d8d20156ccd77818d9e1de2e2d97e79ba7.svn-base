#ifndef SCHEDULE_MANGER_TASK_MANGER
#define SCHEDULE_MANGER_TASK_MANGER
/**
*@file	myTaskGirdManager
*@brief	任务工时管理
*@note	
*		
*@author	lihuizhen
*/
#include "../baseGridManger.h"
#define HOIDAY_ROW_POS 2 //假日设置所在行
#define WEEK_DAY_CNT   7 //一周天数
#define MAX_WORK_HOURTYPE  9999
class CWorkHourManger: public CBaseGridManger
{
public:
	//
	CWorkHourManger( CActionServerFactory *pSeverFactory );
	virtual ~CWorkHourManger();
	//消息处理
	void OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult );
	void OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult );

	void OnUpdateShowBeginDate( COleDateTime &date );
	void SetSelUser( int userId );
	
	void OnCheckSelUser();
	void OnCheckAllUser();
	void OnExportAllWorkHour( int year, int month );
	void OnSetDefHoliday( int year, int month );
    void RefreshShowData();

	virtual void OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult );
protected:
	//重写
	virtual CString GetFixedItemText(int nRow, int nColumn);
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    SetFlexItemText(int nRow,int nColumn,CString text); //设置内容
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn);
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	virtual int	    GetFixedRowCount(){ return 3; } 
	virtual GridItemType GetFixedItemType( int nRow, int nColumn );
	virtual int          GetFixedColumnWidth(int columnIndex){ return 150;}
	

	virtual void    CreateColumnMap(); //创建列头
	virtual void    CreateRowItemMap();
	//行列设置
	virtual int     GetFlexRowCount();
protected:
	void              SetWeekDate( const COleDateTime &date );
	void              SetDefWeekDate();
    void              OnInitWorkHourData();
    CWorkHourData*    GetWorkHourData( int nRown, int nColum );
    BOOL              ValidateSetData( int nRow, int nColumn, double &value, double &sumData );
    void              CalWorkHourData( int selUserId, CWorkHourData  *pWorkHourList );
    BOOL              CheckWorkDataValidate( CWorkHourData *pData, BOOL *relust );
    void              SetValidateColor( BOOL *prelust );
protected:
	COleDateTime        m_weekDate[WEEK_DAY_CNT];
	TaskIdStringMap     m_workTypeList;
	CWorkHourSeverInc   *m_pWorkHourAction;
	CSystemCfgSeverInc  *m_pSystemCfgSever;
	int                  m_timeTypeCnt;
	const int            m_showDateCnt;
	CWorkHourData        *m_pWorkHourList;
	int                  m_selUerId;
	BOOL                 m_isHoliday[WEEK_DAY_CNT]; //是否为节假日
    COLORREF             m_validateOk;
    COLORREF             m_validateError;
	BOOL                 m_assginWorkHour;
	COleDateTime         m_showBeginDate;
};
#endif