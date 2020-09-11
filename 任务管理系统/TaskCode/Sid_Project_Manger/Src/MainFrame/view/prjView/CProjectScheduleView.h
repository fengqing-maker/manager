#pragma once

#include "../print/PreView.h"
#include "Src/action/factory/actionServerFactory.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#define CELL_COUNT         8   //1�����߿��
#define CHANNEL_INFO_WIDTH 210  //���ͨ����Ϣ����ʾ���
#define HEIGHTH_COONDIATE  60   //ģ������������߶�
#define HEIGHT_INFO    100      //��һ�����������ᶥ��

#define IDM_SHOW_ALL                20
#define IDM_BEFOR_MONTH             21         //ǰһ����
#define IDM_BEFOR_QUART             22         //ǰһ������ 
#define IDM_BEFOR_HALFYEAR          23         //ǰ����
#define IDM_AFTER_MONTH             24         //���һ����
#define IDM_AFTER_QUART             25         //���һ������ 
#define IDM_AFTER_HALFYEAR          26         //������

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
	void      SetTaskChangePoint( CTaskChange *pChange, const CTaskObj &bakTask ); //2015-10-29 �޸ı�����
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
	void    DrawSchedule(CDC *pDC);					//�����������
	void	DrawScale(CDC *pDC);				//������ϵ���¼���ʶ
	void	DrawEventToolTips(CDC *pDC);        //��������ʾ��Ϣ
	void    DrawTitleStr( CDC *pDC );

	void    InitMulti();
	long	CalculateXPixelCoor(double XLogic, int orginX = 0) const;	//��X�����߼�ֵ����ӳ����X��������ֵ
	double	CalculateXLogicCoor(long   XPixel, int orginX = 0) const ;	//��X��������ֵ����ӳ��ǰ��X�����߼�ֵ
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
protected://��Ա
	ListTask     *m_pTaskList;           //��ʾ�������б�
	CTaskChange  *m_pChangObj;          //������뵥
	CTaskObj      m_changeAfterTask;    //���������

	CPtrArray    m_ShowCurvePtrArray;
	COleDateTime m_minDate;         //��Сʱ��
	COleDateTime m_maxDate;         //���ʱ��
	int          m_selShowDate;     //0:ȫ�� 1:���һ���� 2�����һ������ 3��������� 
	int          m_useMinDayTask;   //��ʱ��̵�����
	int          m_nFirstDisCurve;	//��ʾ�����е�һ����Ҫ��ʾ������ϵ��ţ���0��ʼ
	int          m_nLastDisCurve;	//��ʾ���������һ����Ҫ��ʾ������ϵ��ţ���0��ʼ

	double	  m_dMulti;				//��¼��ǰ����ʾ����/r	
	double    m_dMinMulti;			//ˮƽ������С������ϵ��
	double	  m_dNewMulti;			//��¼������ʾ�ı���
	int		  m_nHMulti;			//ˮƽ���ű���

	CSize	  m_sizeTotal;					//�����������ֵ
	CRect	  m_curRect;				//��ǰ����ʾ����,���յ�Ϊ��ǰCCurveView�Ĺ�����/r
	CRect	  m_clientRc;				//��¼��������ȫ�������Ļ�����С	
	CRect	  m_rectCurve;        //��¼���߻��Ƶ����򣬰���ˮƽ�����������߻�������
	CPoint	  m_mousePoint;			//��¼WM_MOUSEMOVE�¼�ʱ��λ�ã����ڼ�¼����λ��

	COLORREF	m_colorGirdLine; //������ɫ
	COLORREF    m_factScheduleColor;  //������ʾ����ɫ(��ʵ��ʱ�仭���)
	COLORREF    m_factScheduleColor2; //������ʾ����ɫ(�Խ���ֵ�����)
	COLORREF    m_timeScheduleColor;  //ʱ�������ɫ
	COLORREF    m_changeDateColor; //����������ɫ
	COLORREF	m_bkColor;						//��¼������ʾ����ı���ɫ
	
	//�̶����
	const int m_nHeightOfScale;           //һ��̶��ߵĸ߶�
	int       m_nHeightOfRCScale;              //�����̶ȳ�����ĸ߶�
	int       m_nMinWidthOfDisScale;      //��С�̶�����ʾ���ϵ���ʾ���
	const int m_nMinWidthOfLogScale;      //��С�̶����߼�����ϵ������ʾ������߼�ֵ
	int       m_titleHight;               //������
	CString   m_titleStr;
	//
	int   m_nHeightOfCoor;		//���뿪����ͨ������ϵ�ĸ߶�/r
	int   m_defHeightOfCoor;
	int   m_defChananlInfoWide; //Ĭ�ϵ�ͨ��������ʾ���
	int   m_curveHight;         //������ʾ����ĸ߶�
	double	m_xLogicValueOfCell;     //��¼X����һ���̶�����ʾ��X���߼�ֵ/r
	//��ӡ���
	bool  m_bPrint;
	CRect m_rcPrintCurve;

	const CUtilityTool *m_pUtil;
	CActionServerFactory                 *m_pActionFactory;
};