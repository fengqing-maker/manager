#pragma once

#include "../print/PreView.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#define CELL_COUNT         8   //1倍格线宽度
#define CHANNEL_INFO_WIDTH 210  //左边通道信息的显示宽度
#define HEIGHTH_COONDIATE  60   //模拟量的纵坐标高度
#define HEIGHT_INFO    100      //第一条横向坐标轴顶端

#define IDM_SHOW_ALL                20
#define IDM_BEFOR_MONTH             21         //前一个月
#define IDM_BEFOR_QUART             22         //前一个季度 
#define IDM_BEFOR_HALFYEAR          23         //前半年
#define IDM_AFTER_MONTH             24         //最后一个月
#define IDM_AFTER_QUART             25         //最后一个季度 
#define IDM_AFTER_HALFYEAR          26         //最后半年

class CProjectScheduleView : public CMyView
{
public:
	CProjectScheduleView();
	DECLARE_DYNCREATE(CProjectScheduleView)

	BOOL CreateFormCtrl(UINT nID, CWnd *pParent);
    void OnUpdate();
public:
	virtual ~CProjectScheduleView();

	void      SetTaskListPoint( ListTask *pTaskList );
	void      ResetShowTask( BOOL isNoRefres = false );
	void      SetTiltleString( const CString titleStr ) { m_titleStr = titleStr;};
	void      SetTaskChangePoint( CTaskChange *pChange, const CTaskObj &bakTask ); //2015-10-29 修改变更添加
	virtual void OnInitialUpdate();     // first time after construct
protected:
	
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE); 
	virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
	//virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	void    SetCoorHeight(int nHeight = HEIGHTH_COONDIATE, BOOL isNoRefres = false );

	void    DrawGridLine(CDC *pDC);
	void    DrawTaskSchedule( CDC* pDc);
	void    DrawSchedule(CDC *pDC);					//绘制任务进度
	void	DrawScale(CDC *pDC);				//画坐标系与事件标识
	void	DrawEventToolTips(CDC *pDC);        //画任务提示信息
	void    DrawTitleStr( CDC *pDC );

	void    InitMulti();
	long	CalculateXPixelCoor(double XLogic, int orginX = 0) const;	//由X方向逻辑值计算映射后的X方向像素值
	double	CalculateXLogicCoor(long   XPixel, int orginX = 0) const ;	//由X方向像素值计算映射前的X方向逻辑值
	long    CalculatePrintXPixelCoor( double XLogic, int originX ) const;
	double  CalculatePrintXLogicCoor(long XPixel, int originX) const;

	double  GetShowDays();
	double  GetXLogicValueOfCell() const {return m_xLogicValueOfCell;}
	double  GetMulti() const{return m_dMulti;}
	double  GetBeginXDays( const COleDateTime &dateTime );

	enum SEL_SHOWTAS_KWAY{ all_task, recent_month_task, recent_quart_task, recent_halfyear_task, 
		                             last_month_task, last_quart_task, last_halfyear_task  };
protected:
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnFilterTask(UINT menuID);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
protected://成员
	ListTask     *m_pTaskList;           //显示的任务列表
	CTaskChange  *m_pChangObj;          //变更申请单
	CTaskObj      m_changeAfterTask;    //变更后任务

	CPtrArray    m_ShowCurvePtrArray;
	COleDateTime m_minDate;         //最小时间
	COleDateTime m_maxDate;         //最大时间
	int          m_selShowDate;     //0:全部 1:最近一个月 2：最近一个季度 3：最近半年 
	int          m_useMinDayTask;   //耗时最短的任务
	int          m_nFirstDisCurve;	//显示区域中第一个需要显示的坐标系序号，从0开始
	int          m_nLastDisCurve;	//显示区域中最后一个需要显示的坐标系序号，从0开始

	double	  m_dMulti;				//记录当前的显示比率/r	
	double    m_dMinMulti;			//水平方向最小化放缩系数
	double	  m_dNewMulti;			//记录即将显示的倍率
	int		  m_nHMulti;			//水平缩放倍数

	CSize	  m_sizeTotal;					//滚动条的最大值
	CRect	  m_curRect;				//当前的显示区域,参照点为当前CCurveView的滚动条/r
	CRect	  m_clientRc;				//记录所有曲线全部绘完后的画布大小	
	CRect	  m_rectCurve;        //记录曲线绘制的区域，包括水平滚动条和曲线绘制区域
	CPoint	  m_mousePoint;			//记录WM_MOUSEMOVE事件时的位置，用于记录鼠标的位置

	COLORREF	m_colorGirdLine; //网格颜色
	COLORREF    m_factScheduleColor;  //进度显示的颜色(以实际时间画宽度)
	COLORREF    m_factScheduleColor2; //进度显示的颜色(以进度值画宽度)
	COLORREF    m_timeScheduleColor;  //时间进度颜色
	COLORREF    m_changeDateColor; //变更后进度颜色
	COLORREF	m_bkColor;						//记录整个显示区域的背景色
	
	//刻度相关
	const int m_nHeightOfScale;           //一般刻度线的高度
	int       m_nHeightOfRCScale;              //整个刻度尺区域的高度
	int       m_nMinWidthOfDisScale;      //最小刻度在显示器上的显示间距
	const int m_nMinWidthOfLogScale;      //最小刻度在逻辑坐标系中所表示的最大逻辑值
	int       m_titleHight;               //标题宽度
	CString   m_titleStr;
	//
	int   m_nHeightOfCoor;		//开入开出量通道坐标系的高度/r
	int   m_defHeightOfCoor;
	int   m_defChananlInfoWide; //默认的通道描述显示宽度
	int   m_curveHight;         //所有显示任务的高度
	double	m_xLogicValueOfCell;     //记录X轴上一个刻度所表示的X的逻辑值/r
	//打印相关
	bool  m_bPrint;
	CRect m_rcPrintCurve;

	const CUtilityTool *m_pUtil;
	CActionServerFactory                 *m_pActionFactory;
};