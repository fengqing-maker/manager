#ifndef  FORECAST_WORK_HOUR_HEAD
#define  FORECAST_WORK_HOUR_HEAD

#include "../baseGridManger.h"
#define SHOW_DAY_CNT   10 //一此显示10个工作日预测
class CForecastWorkHour: public CBaseGridManger
{
public:
	CForecastWorkHour( CActionServerFactory *pSeverFactory );
	virtual ~CForecastWorkHour();
	
	virtual void SetBeginWorkDate( const COleDateTime &beginDate );
	virtual void SetSelDepartUser( const ListDepart &lstDepart, int index ); //index，数组中第几个部门
    virtual void SetSelProject( int prjId );

protected:
	//重写
	virtual CString GetFixedItemText(int nRow, int nColumn);
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* 重载以设置可动表项 */
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn) { return FALSE;} ;
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	virtual int	    GetFixedRowCount(){ return 1; } 
	virtual int     GetFlexRowCount();
	virtual GridItemType GetFixedItemType( int nRow, int nColumn );
	virtual int     GetFixedColumnWidth(int columnIndex){ return 150;}
	virtual int     GetFixedRowHeight(int rowIndex){return 45;}
	virtual void    CreateColumnMap(); //创建列头
	
protected:
	void            InitWorkDateArray(); //根据开始的工作日筛选显示的工作日
protected:
	COleDateTime        m_showWorkDay[SHOW_DAY_CNT]; //工作日
	const int           m_showDateCnt;
	COleDateTime        m_beginDate; //开始的工作日
	CSystemCfgSeverInc  *m_pSystemCfgSever;
	ListUser            m_lstShowWorker;
    CDepartSeverInc     * m_pDepartSever;
    CProjectSeverInc    *m_prjSever;
};

#endif