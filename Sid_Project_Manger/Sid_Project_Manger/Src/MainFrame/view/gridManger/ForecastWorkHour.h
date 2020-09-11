#ifndef  FORECAST_WORK_HOUR_HEAD
#define  FORECAST_WORK_HOUR_HEAD

#include "../baseGridManger.h"
#define SHOW_DAY_CNT   10 //һ����ʾ10��������Ԥ��
class CForecastWorkHour: public CBaseGridManger
{
public:
	CForecastWorkHour( CActionServerFactory *pSeverFactory );
	virtual ~CForecastWorkHour();
	
	virtual void SetBeginWorkDate( const COleDateTime &beginDate );
	virtual void SetSelDepartUser( const ListDepart &lstDepart, int index ); //index�������еڼ�������
    virtual void SetSelProject( int prjId );

protected:
	//��д
	virtual CString GetFixedItemText(int nRow, int nColumn);
	virtual CString GetFlexItemText(int nRow, int nColumn);				/* ���������ÿɶ����� */
	virtual BOOL    GetFlexItemEditable(int nRow,int nColumn) { return FALSE;} ;
	virtual BOOL    InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn);
	virtual int	    GetFixedRowCount(){ return 1; } 
	virtual int     GetFlexRowCount();
	virtual GridItemType GetFixedItemType( int nRow, int nColumn );
	virtual int     GetFixedColumnWidth(int columnIndex){ return 150;}
	virtual int     GetFixedRowHeight(int rowIndex){return 45;}
	virtual void    CreateColumnMap(); //������ͷ
	
protected:
	void            InitWorkDateArray(); //���ݿ�ʼ�Ĺ�����ɸѡ��ʾ�Ĺ�����
protected:
	COleDateTime        m_showWorkDay[SHOW_DAY_CNT]; //������
	const int           m_showDateCnt;
	COleDateTime        m_beginDate; //��ʼ�Ĺ�����
	CSystemCfgSeverInc  *m_pSystemCfgSever;
	ListUser            m_lstShowWorker;
    CDepartSeverInc     * m_pDepartSever;
    CProjectSeverInc    *m_prjSever;
};

#endif