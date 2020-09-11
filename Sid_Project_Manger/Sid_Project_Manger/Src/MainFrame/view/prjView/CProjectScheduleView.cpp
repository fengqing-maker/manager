#include "stdafx.h"
#include "CProjectScheduleView.h"
#include "Src/Tools/TRACE_D/trace_d.h"
#include "../../MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNCREATE(CProjectScheduleView, CScrollView)

CProjectScheduleView::CProjectScheduleView(): m_defChananlInfoWide(CHANNEL_INFO_WIDTH),
      m_defHeightOfCoor(HEIGHTH_COONDIATE),m_nHeightOfRCScale(25)/*m_nHeightOfRCScale(20)*/,m_nHeightOfScale(5),m_nMinWidthOfDisScale(5),
	  m_nMinWidthOfLogScale(1)
{
	m_pTaskList = NULL;
	m_selShowDate = 0;
	m_useMinDayTask = 0;
	m_nHMulti = 1;						//水平缩放倍数
	m_dMulti = 1;						//放缩比率
	m_dNewMulti = 1;
	m_pUtil = CUtilityTool::GetObj();
	m_nHeightOfCoor = m_defHeightOfCoor;
	m_bPrint = false;
	m_curveHight = 10;
	m_xLogicValueOfCell = 1;			//记录X轴上一个刻度所表示的X的逻辑值
	m_colorGirdLine = RGB(210,210,210);
    m_factScheduleColor = RGB( 0, 128, 0);  //进度显示的颜色
	m_factScheduleColor2 = RGB( 255, 196, 255 ); //已进度画宽度
    m_timeScheduleColor = RGB( 255, 255, 0 );  //时间进度颜色
	m_changeDateColor = RGB(255, 128, 128);
	m_pActionFactory = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	m_bkColor = RGB(255,255,255);
	m_titleHight = 60;
	m_pChangObj = NULL;
}

CProjectScheduleView::~CProjectScheduleView()
{

}
BEGIN_MESSAGE_MAP(CProjectScheduleView, CMyView)
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND_RANGE(IDM_SHOW_ALL,IDM_AFTER_HALFYEAR,OnFilterTask)

END_MESSAGE_MAP()

void CProjectScheduleView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	GetClientRect(&m_curRect);
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 300;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void   CProjectScheduleView::SetTaskListPoint( ListTask *pTaskList )
{
	BOOL isNoRefres = FALSE;
	m_pTaskList = pTaskList;
	ResetShowTask( isNoRefres );
}
void   CProjectScheduleView::SetTaskChangePoint( CTaskChange *pChange, const CTaskObj &bakTask )
{
	m_pChangObj = pChange;
	m_changeAfterTask = bakTask;
	//m_dNewMulti = m_dMinMulti;	//111111111
}

void CProjectScheduleView::ResetShowTask( BOOL isNoRefres /*= false*/ )
{
	int cnt = m_ShowCurvePtrArray.GetCount();
	while( cnt > 0 )
	{
		m_ShowCurvePtrArray.RemoveAt( cnt - 1);
		cnt = m_ShowCurvePtrArray.GetCount();
	}
	m_minDate.SetDate( 3000, 1, 1 );
	m_maxDate.SetDate( 2000, 1, 1 );
	if ( m_pTaskList )
	{
		ListTaskItor itor = m_pTaskList->begin();
		for ( ; itor != m_pTaskList->end(); itor++ )
		{
			if ( itor->GetPlanStartTime() < m_minDate )
			{
				m_minDate = itor->GetPlanStartTime();
			}
			if ( itor->GetFactStartDateShowStr().GetLength() > 0 && itor->GetFactStartTime() < m_minDate )
			{
				m_minDate = itor->GetFactStartTime();
			}
			if ( itor->GetPlanEndTime() > m_maxDate )
			{
				m_maxDate = itor->GetPlanEndTime();
			}
			if ( itor->GetPanEndDateShowStr().GetLength() > 0 && itor->GetFactEndTime() > m_maxDate)
			{
				m_maxDate = itor->GetFactEndTime();
			}
		} 
		COleDateTimeSpan span;
		switch ( m_selShowDate )
		{
		case recent_month_task://最近一个月
			span.SetDateTimeSpan( 30, 0, 0, 0 );
			m_maxDate = m_minDate + span;
			break;
		case recent_quart_task:
			span.SetDateTimeSpan( 90, 0, 0, 0 );
			m_maxDate = m_minDate + span;
			break;
		case recent_halfyear_task:
			span.SetDateTimeSpan( 180, 0, 0, 0 );
			m_maxDate = m_minDate + span;
			break;
		case last_month_task:
			span.SetDateTimeSpan( 30, 0, 0, 0 );
			m_minDate = m_maxDate - span;
			break;
		case last_quart_task:
			span.SetDateTimeSpan( 90, 0, 0, 0 );
			m_minDate = m_maxDate - span;
			break;
		case last_halfyear_task:
			span.SetDateTimeSpan( 90, 0, 0, 0 );
			m_minDate = m_maxDate - span;
			break;
		}
		itor = m_pTaskList->begin();
		m_useMinDayTask = 1000;
		COleDateTime tmpBeginDate, tmpEndDate;
		tmpBeginDate = m_minDate;
		tmpEndDate = m_maxDate;
		for ( ; itor != m_pTaskList->end(); itor++ )
		{
			if ( itor->GetPlanStartTime() >= tmpBeginDate && itor->GetPlanStartTime() <= tmpEndDate )
			{
				m_ShowCurvePtrArray.Add( &(*itor) );
				if ( itor->GetPlanStartTime() < m_minDate )
				{
					m_minDate = itor->GetPlanStartTime();
				}
				if ( itor->GetFactStartDateShowStr().GetLength() > 0 && itor->GetFactStartTime() < m_minDate )
				{
					m_minDate = itor->GetFactStartTime();
				}
				if ( itor->GetPlanEndTime() > m_maxDate )
				{
					m_maxDate = itor->GetPlanEndTime();
				}
				if ( itor->GetPanEndDateShowStr().GetLength() > 0 && itor->GetFactEndTime() > m_maxDate)
				{
					m_maxDate = itor->GetFactEndTime();
				}
				if ( itor->GetTaskPanDay() < m_useMinDayTask )
				{
					m_useMinDayTask = itor->GetTaskPanDay();
				}
			}
		}
		if ( m_useMinDayTask >= 1000)
		{
			m_useMinDayTask = 1;
		}
	}
	//重新调整显示 
	SetCoorHeight( m_nHeightOfCoor, isNoRefres );
}

void CProjectScheduleView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here	
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
}
void CProjectScheduleView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CSize sizeTotal;
	CRect rc,rcView;
	GetWindowRect(&rcView);//View客户区实际大小
	GetDesktopWindow()->GetWindowRect(&rc);//屏幕整个大小
	rc.bottom -= rcView.top+HEIGHT_INFO+30;
	int height[2];
	height[0] = m_clientRc.Height();
	height[1] = rc.Height()-20;
	if ( m_pChangObj )
	{//2015-11-30 add
		m_sizeTotal.cy = max(height[0],rcView.Height());
	}else
	{
		m_sizeTotal.cy = max(height[0],height[1]);
	}
	//当不需要垂直滚动时需要把垂直滚动条的宽度加回来否则窗口会变窄
	if(m_sizeTotal.cy < m_clientRc.Height())
	{
		m_sizeTotal.cx=m_clientRc.Width()+20;
	}
	else
	{
		m_sizeTotal.cx = m_clientRc.Width();
	}
	if ( m_sizeTotal.cx < 0 )
	{
		return;
	}
	//计算最右点的横坐标
	SetScrollSizes(MM_TEXT,m_sizeTotal);
	//GetParentFrame()->RecalcLayout(); //1111
	ResizeParentToFit();   
}

BOOL CProjectScheduleView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll) 
{
	// TODO: Add your specialized code here and/or call the base class
	int xOrig, x;
	int yOrig, y;
	// don't scroll if there is no valid scroll range (ie. no scroll bar)
	CScrollBar* pBar;
	DWORD dwStyle = GetStyle();
	pBar = GetScrollBarCtrl(SB_VERT);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
	{
		// vertical scroll bar not enabled
		sizeScroll.cy = 0;
	}
	pBar = GetScrollBarCtrl(SB_HORZ);
	if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
	{
		// horizontal scroll bar not enabled
		sizeScroll.cx = 0;
	}

	// adjust current x position
	xOrig = x = GetScrollPos(SB_HORZ);
	int xMax = GetScrollLimit(SB_HORZ);
	x += sizeScroll.cx;
	if (x < 0)
		x = 0;
	else if (x > xMax)
		x = xMax;

	// adjust current y position
	yOrig = y = GetScrollPos(SB_VERT);
	int yMax = GetScrollLimit(SB_VERT);
	y += sizeScroll.cy;
	if (y < 0)
		y = 0;
	else if (y > yMax)
		y = yMax;

	// did anything change?
	if (x == xOrig && y == yOrig)
		return FALSE;

	if (bDoScroll)
	{
		// do scroll and update scroll positions
		//(与CScrollView::OnScrollBy所不同的就在这一个调用，
		//因为如何的滚动由的我们的CCurveView::DrawCurve已经做了，
		//如果这里再从重调于ScrollWindow，会让界面闪烁)
		//ScrollWindow(-(x-xOrig), -(y-yOrig));
		if (x != xOrig)
		{
			SetScrollPos(SB_HORZ, x);
		}
		if (y != yOrig)
		{

			SetScrollPos(SB_VERT, y);
		}
		//CScrollView::OnScrollBy中没有此调用，这里调用是让我们的CCurveView::DrawCurve实现滚动
		RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW);
	}
	return TRUE;
	//return CScrollView::OnScrollBy(sizeScroll, bDoScroll);
}
void CProjectScheduleView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	PAINTSTRUCT ps;	
	CDC* pDC = BeginPaint(&ps);	 
	if(/*!m_bPrint &&*/ m_pTaskList && m_pTaskList->size() > 0 )
	{
		DrawTaskSchedule(&dc);
	}
	EndPaint(&ps);
	//Do not call CScrollView::OnPaint() for painting messages
}
void CProjectScheduleView::DrawTaskSchedule( CDC* pDC )
{//
	if(m_pUtil->DoubleCmp(m_dNewMulti-m_dMulti)!=0)/*调整放大系数*/
	{
		m_dMulti = m_dNewMulti;
		SetCoorHeight( m_nHeightOfCoor );
	}
	CPoint pointScrollPos = GetScrollPosition();  //图形显示的坐标都基于滚动条为0点坐标
	GetClientRect(&m_curRect);
	m_curRect.left += pointScrollPos.x;
	m_curRect.right += pointScrollPos.x;
	m_curRect.top += pointScrollPos.y;
	m_curRect.bottom += pointScrollPos.y;
	m_mousePoint.x += pointScrollPos.x;
	m_mousePoint.y += pointScrollPos.y;
	ASSERT_VALID(pDC);
	UNUSED(pDC);

	long oldmode;
	CDC MemDC; //首先定义一个显示设备对象
	CBitmap MemBitmap;//定义一个位图对象
	CSize logicTotalSize;

	logicTotalSize = GetTotalSize();
	logicTotalSize.cx = 2*m_curRect.Width();
	logicTotalSize.cy = max(logicTotalSize.cy, m_curRect.Height()); 
	CRect rcDC(0,0,logicTotalSize.cx,logicTotalSize.cy);
	//随后建立与屏幕显示兼容的内存显示设备
	VERIFY(MemDC.CreateCompatibleDC(NULL));
	//这时还不能绘图，因为没有地方画 ^_^
	//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小
	VERIFY(MemBitmap.CreateCompatibleBitmap(pDC,rcDC.Width(),rcDC.Height()));
	//将位图选入到内存显示设备中
	//只有选入了位图的内存显示设备才有地方绘图，画到指定的位图上
	CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);

	//先用背景色将位图清除干净，这里我用的是白色作为背景
	//你也可以用自己应该用的颜色
	MemDC.FillSolidRect(rcDC,RGB(255,255,255));
	MemDC.FillSolidRect(m_rectCurve,m_bkColor);
   
	//绘图
	oldmode=MemDC.SetBkMode(TRANSPARENT);//设置文字背景透明，与窗口背景无关
	DrawGridLine(&MemDC);//画坐标轴和格线（合并波灰显区域），没有画坐标系的刻度
	DrawSchedule( &MemDC );//
	DrawTitleStr( &MemDC );
	MemDC.SetBkMode(oldmode);
	
	DrawScale(&MemDC);//把刻度线也画入位图中
	
	if(m_mousePoint.x>0)
	{//画提示内容
		DrawEventToolTips(&MemDC);
	}
	pDC->BitBlt(0,0,m_curRect.Width(),logicTotalSize.cy-m_curRect.top,
		&MemDC,0,m_curRect.top,SRCCOPY);
	// 绘图完成后的清理
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	m_mousePoint.x -= pointScrollPos.x;
	m_mousePoint.y -= pointScrollPos.y;

	 
}
void CProjectScheduleView::OnUpdate()
{
	OnUpdate( NULL, NULL, NULL );
}
void CProjectScheduleView::DrawGridLine(CDC *pDC)
{
	//CPen pen1(PS_SOLID,1, m_coordinateColor);
	//CPen pen2(PS_DASH,1,m_coordinateColor);
	//CPen pen3(PS_SOLID,1,RGB(233,233,233));
	CPen penGird(PS_DASH,1,m_colorGirdLine);
	CPen *pOldPen;
	CBrush brush(RGB(178,178,178));
	POINT origitalPoint;
	UINT nTopOfCoor;  //nWidthOfCell一个通道所占纵向网格的格数，nTopOfCoor当前坐标系的顶坐标
	//y轴
	CPoint pointScrollPos = GetScrollPosition();
	pDC->MoveTo(m_rectCurve.left, m_rectCurve.top );
	pDC->LineTo(m_rectCurve.left, m_rectCurve.bottom);
	//TRACE(_T("%d\r\n"), m_rectCurve.top );
	//横向线条
	int i = 0,nDisCoorHeight = m_nHeightOfCoor/2;/*可显示坐标系的宽度*/
	UINT nTopOfY = m_rectCurve.top; 

	pOldPen = pDC->SelectObject(&penGird); //换了虚线笔
	m_nFirstDisCurve = -1;
	m_nLastDisCurve = -1;
	UINT nIntervalOfCoorMid,nIntervalOfCoorTop=0; 
	origitalPoint.x=m_rectCurve.left;
	origitalPoint.y=0;
	nTopOfCoor = m_rectCurve.top;

	long LeftPixelOfGrey = 0,RightPixelOfGrey = 0;	
	int nCurveCount = m_ShowCurvePtrArray.GetCount();
	for(i = 0;i < nCurveCount; i++)
	{
		//求当前显示的曲线
		if( ( (nIntervalOfCoorTop+nTopOfY) > (UINT)m_curRect.bottom || (nIntervalOfCoorTop+m_nHeightOfCoor+nTopOfY) >= m_curRect.bottom) && m_nLastDisCurve == -1)
		{
			if(i>0)
			{
				m_nLastDisCurve = i-1;
				break;
			}
		}
	    origitalPoint.y = nTopOfCoor + m_nHeightOfCoor/2; 
		nTopOfCoor += m_nHeightOfCoor;
		
		nIntervalOfCoorMid = nIntervalOfCoorTop;/*新坐标系的中点基址为上一坐标系最下层坐标*/
		/*计算新坐标系中点与最下层坐标*/
		nIntervalOfCoorMid += (UINT)m_nHeightOfCoor/2;
		nIntervalOfCoorTop += (UINT)m_nHeightOfCoor;
		//绘制分隔虚线
		pDC->MoveTo(m_rectCurve.left,nTopOfY+nIntervalOfCoorTop);
		pDC->LineTo(m_rectCurve.left+m_curRect.Width(),nTopOfY+nIntervalOfCoorTop);/*向下画*/

		if(nIntervalOfCoorTop > (UINT)m_curRect.top && m_nFirstDisCurve == -1)
		{//计算m_nFirstDisCurve第一条要显示的通道
			m_nFirstDisCurve = i;
		}
	}

	if(m_nLastDisCurve == -1)
	{
		m_nLastDisCurve = nCurveCount-1;
	}
	pDC->SelectObject(pOldPen);
}
void  CProjectScheduleView::DrawSchedule(CDC *pDC)
{//绘制任务进度
	POINT origitalPoint;
	origitalPoint.x=m_defChananlInfoWide;
	UINT nTopOfCoor;  //nTopOfCoor当前坐标系的顶坐标
	long i = 0;
	nTopOfCoor = m_rectCurve.top;  //nTopOfCoor:整个显示画布的bottom值
	CString str;
	//在位图上画曲线时跳过不会显示的曲线
	for( i=0; i < m_nFirstDisCurve ;i++)
	{
		nTopOfCoor += m_nHeightOfCoor;
	}
	if( m_nFirstDisCurve <= -1 )
	{
		return;
	}
	CPen pen(PS_SOLID, 1, m_timeScheduleColor); //rect
	CPen changeTaskPen( PS_SOLID, 1, RGB(255, 128, 128) );
	CPen* pOldPen = pDC->SelectObject(&pen);
	CSize TextSize = pDC->GetTextExtent(_T("无意义"));
	long TextHeight = TextSize .cy;

	bool bFirstDigital = true;//第一条开出曲线
	CPoint beginPoint, endPoint/*, beginShowDatePixPos, endShowDatePixPos*/;
	CRect  timeRect, scheduleRect, valueRect, textRect;
	COleDateTime curTime;
	int          textWide = TextSize .cy*5;

	for(i = m_nFirstDisCurve;i<=m_nLastDisCurve;i++)
	{
		CTaskObj *pTask = (CTaskObj *)m_ShowCurvePtrArray.GetAt( i );
		if ( NULL == pTask )
		{
			continue;
		}
		origitalPoint.y = nTopOfCoor + m_nHeightOfCoor/2 - m_nHeightOfCoor/24; 
		nTopOfCoor += m_nHeightOfCoor;
		//画任务名
		CRect rc(m_clientRc.left+5,origitalPoint.y-5,m_clientRc.left+15,origitalPoint.y+5);
		rc.left=m_clientRc.left+5;
		rc.top = origitalPoint.y-TextSize.cy/2;
		rc.right=m_clientRc.left+180;
		if ( i != m_nFirstDisCurve )
		{
			rc.top -= TextHeight/2+5;
		}
		rc.bottom += TextHeight/2+5;
		CString ChannelDesc;
		ChannelDesc.Format(_T("%s"),pTask->GetName() );
		BOOL isHasChild = false;
		m_pActionFactory->GetProjectSever()->IsHasChildTask( pTask->GetId(), isHasChild );
		CTaskObj fatherTask;
		m_pActionFactory->GetProjectSever()->GetFatherTask( pTask->GetId(), fatherTask );
		if ( isHasChild )
		{//父级任务
			pDC->SetTextColor( RGB( 34, 41, 128 ) );
		}else if (  fatherTask.GetId() > 0 )
		{
			pDC->SetTextColor( RGB( 112, 146, 190) );
		}else
		{
			pDC->SetTextColor( RGB( 0, 0, 0 ) ); 
		}
		if ( m_pChangObj && m_pChangObj->GetTaskId() == pTask->GetId() )
		{//2015-11-2设置变更任务字体
			//pDC->SelectObject( &changeTaskPen );
			pDC->SetTextColor( RGB(255, 128, 128) );
		}
		pDC->DrawText(ChannelDesc,&rc, DT_LEFT);
		pOldPen = pDC->SelectObject(&pen);
		pDC->SetTextColor( RGB( 0, 0, 0 ) ); 

		//计算通道源坐标点 end;
		double xLoginValue = GetBeginXDays( pTask->GetPlanStartTime() );
		beginPoint.x = CalculateXPixelCoor( xLoginValue, origitalPoint.x );
		beginPoint.y = origitalPoint.y - m_nHeightOfCoor/4;
		xLoginValue = GetBeginXDays( pTask->GetPlanEndTime() );
		endPoint.x = CalculateXPixelCoor( xLoginValue, origitalPoint.x );
		endPoint.y = origitalPoint.y + m_nHeightOfCoor/4;

		int factBeginPos = beginPoint.x > m_defChananlInfoWide ?  beginPoint.x: m_defChananlInfoWide;
		endPoint.x = endPoint.x > m_defChananlInfoWide ?  endPoint.x: m_defChananlInfoWide;
		//
		pDC->Rectangle( factBeginPos, beginPoint.y, endPoint.x, endPoint.y );
		pDC->Rectangle( factBeginPos+1, beginPoint.y+1, endPoint.x-1, endPoint.y-1 );
		pDC->Rectangle( factBeginPos+2, beginPoint.y+2, endPoint.x-2, endPoint.y-2 );

		//beginShowDatePixPos.x = m_curRect.left + m_defChananlInfoWide;
		//beginShowDatePixPos.y = origitalPoint.y;
		//endShowDatePixPos.x = m_curRect.right;
		//endShowDatePixPos.y = origitalPoint.y;
  //      
		//double beginDateDays = CalculateXLogicCoor( beginShowDatePixPos.x, origitalPoint.x );
		//double endDateDays = CalculateXLogicCoor( endShowDatePixPos.x, origitalPoint.x );
		curTime = COleDateTime::GetCurrentTime();
		timeRect.top = origitalPoint.y - m_nHeightOfCoor/4;
		timeRect.bottom = origitalPoint.y + m_nHeightOfCoor/4;
		if ( curTime >=  pTask->GetPlanStartTime() )
		{
			if ( curTime > pTask->GetPlanEndTime() )
			{
				curTime = pTask->GetPlanEndTime();
			}
			xLoginValue = GetBeginXDays( curTime );
			
			timeRect.right = CalculateXPixelCoor( xLoginValue, origitalPoint.x );
			if ( timeRect.right < m_defChananlInfoWide )
			{
				timeRect.right = m_defChananlInfoWide;
			}
			timeRect.left = factBeginPos;
			
			pDC->FillSolidRect( timeRect, m_timeScheduleColor );
		}
		CSchedule schObj;
		if ( userExcOk == m_pActionFactory->GetScheduleSever()->GetMaxNewScheduleByTaskId( pTask->GetId(), schObj)
			  && schObj.GetPercent() > 0 )
		{
			xLoginValue = GetBeginXDays( pTask->GetFactStartTime() );
			scheduleRect.left = CalculateXPixelCoor( xLoginValue , origitalPoint.x );
			scheduleRect.left = scheduleRect.left > m_defChananlInfoWide ?  scheduleRect.left:m_defChananlInfoWide;
			scheduleRect.top = origitalPoint.y - m_nHeightOfCoor/12;
			scheduleRect.right = timeRect.right;
			scheduleRect.bottom = origitalPoint.y + m_nHeightOfCoor/12;
			if ( task_close == pTask->GetTaskStatus() || waitScore == pTask->GetTaskStatus() )
			{
				scheduleRect.right = CalculateXPixelCoor( GetBeginXDays( pTask->GetFactEndTime()) );
			}
			if ( scheduleRect.right < m_defChananlInfoWide )
			{
				scheduleRect.right = m_defChananlInfoWide;
			}
			pDC->FillSolidRect( scheduleRect, m_factScheduleColor );
			//以实际进度画宽度
			valueRect.left = factBeginPos;
			valueRect.top = scheduleRect.bottom;
			valueRect.bottom = valueRect.top + m_nHeightOfCoor/12;//12
			valueRect.right = valueRect.left + ( schObj.GetPercent()*(endPoint.x - factBeginPos) )/100;
			if ( valueRect.right < m_defChananlInfoWide )
			{
				valueRect.right = m_defChananlInfoWide;
			}
			pDC->FillSolidRect( valueRect, m_factScheduleColor2 );
		}
		if ( m_pChangObj && m_pChangObj->GetTaskId() == pTask->GetId() )
		{//2015-11-2 画修改后任务的计划时间
			CRect chRect;
			xLoginValue = GetBeginXDays( m_changeAfterTask.GetPlanEndTime() );
			chRect.left = factBeginPos;
			chRect.top = origitalPoint.y + m_nHeightOfCoor/6;
			chRect.right = CalculateXPixelCoor( xLoginValue , origitalPoint.x );
			chRect.bottom = chRect.top + m_nHeightOfCoor/12;
			if ( chRect.right < m_defChananlInfoWide )
			{
				chRect.right = m_defChananlInfoWide;
			}
            pDC->FillSolidRect( chRect, m_changeDateColor );
		}
		//画进度数值
		long maxRight = timeRect.right > scheduleRect.right ? timeRect.right:scheduleRect.right;
		maxRight = maxRight > valueRect.right ? maxRight:valueRect.right;
		maxRight = maxRight + textWide > endPoint.x  ? endPoint.x : maxRight;
		if ( schObj.GetPercent() <= 0 )
		{
			maxRight = endPoint.x;
		}

		textRect.left = maxRight;
		textRect.right = textRect.left + textWide;
		textRect.top = timeRect.top;
		textRect.bottom = timeRect.bottom;
		CString str;
		str.Format( _T("%d%%"), schObj.GetPercent() );
		if ( textRect.left > m_defChananlInfoWide )
		{
			pDC->DrawText(str, &textRect,DT_LEFT); 
		}
		//pCurve->DrawCurve(&Drawer);
	}
	pDC->SelectObject(pOldPen);

}
//绘制标题
void  CProjectScheduleView::DrawTitleStr( CDC *pDC )
{
	CString str = _T("部门任务");
	if ( m_titleStr.GetLength() > 0 )
	{
		str = m_titleStr;
	}else if ( m_ShowCurvePtrArray.GetCount() > 0 )
	{
		CTaskObj* pObj = ( CTaskObj* )m_ShowCurvePtrArray.GetAt( 0 );
		if ( pObj && pObj->GetProjctId() > 0 )
		{
			CProject prjObj;
			m_pActionFactory->GetProjectSever()->GetProjectById( pObj->GetProjctId(), prjObj );
			str = prjObj.GetShortName();
		}
	}
	CPoint pointScrollPos = GetScrollPosition();
	int left = m_curRect.left - pointScrollPos.x;
	if ( left < 0 )
	{
		left = m_curRect.left;
	}
	int right = m_curRect.right - pointScrollPos.x;
	if ( right < 0 )
	{
		right = m_curRect.right;
	}
	CRect rectTitle( left , m_curRect.top, m_curRect.right, m_curRect.top + m_titleHight );
	pDC->FillSolidRect( rectTitle, m_bkColor );
    pDC->FillSolidRect( rectTitle, m_bkColor );

	CPen *pOldPen,pen(PS_SOLID,1,RGB(0,0,0)),titlePen(PS_SOLID,3,RGB(0,0,0));
	CFont font, decFont,*pOldFont;
	font.CreatePointFont(160,_T("MS Sans Serif"));
	decFont.CreatePointFont( 100, _T("MS Sans Serif") );
	pOldFont=pDC->SelectObject(&font);
	pOldPen = pDC->SelectObject(&titlePen);

	CSize sizeTilte = pDC->GetTextExtent( str );
	CRect titleRc;
	titleRc.left = left/*m_curRect.left*/ + (m_curRect.Width() - sizeTilte.cx)/2;
	titleRc.right = titleRc.left + sizeTilte.cx;
	titleRc.top = m_curRect.top;
	titleRc.bottom = titleRc.top+ sizeTilte.cy;
	pDC->DrawText(str,&titleRc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//绘制颜色说明
	pDC->SelectObject(&pOldPen);
	pDC->SelectObject(&pOldFont);
	pOldPen = pDC->SelectObject(&pen);
	pOldFont = pDC->SelectObject( &decFont );
	COLORREF colTmp;
	CRect textRc, rectColor;
	sizeTilte = pDC->GetTextExtent( _T("任务实际进度") );
	int wide =  sizeTilte.cx + 15;
	textRc.top = titleRc.bottom;
	textRc.bottom = textRc.top + sizeTilte.cy;
	textRc.right = right;
	textRc.left = textRc.right - wide ;
	for ( int i = 0; i < 4; i++ )
	{
		switch(i)
		{
		case 0:
			str = _T("任务实际进度");
			colTmp = m_factScheduleColor2;
			break;
		case 1:
			str = _T("实际执行时间");
			colTmp = m_factScheduleColor;
			break;
		case 2:
			str = _T("任务计划时间");
			colTmp = m_timeScheduleColor;
			break;
		}
		if ( 3 == i )
		{
			if ( m_pChangObj )
			{
				str = _T("变更后时间");
				colTmp = m_changeDateColor;
			}else
			{
				break;
			}
		}
		pDC->DrawText(str,&textRc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
		rectColor.top = textRc.top + sizeTilte.cy/4;
		rectColor.bottom = textRc.bottom - sizeTilte.cy/4;
		rectColor.right = textRc.left - 2;
		rectColor.left = rectColor.right - 10;
		pDC->FillSolidRect( rectColor, colTmp );
		
		textRc.right = rectColor.left;
		textRc.left = textRc.right - wide;
	}
	pDC->SelectObject(&pOldFont);
	pDC->SelectObject(&pOldPen);
};
void  CProjectScheduleView::DrawScale(CDC *pDC)
{//画坐标系
	POINT origitalPoint,pTemp[6];
	double MinLogicX = 0.0,MaxLogicX = 0.0;
	origitalPoint.x=m_rectCurve.left;
	if (m_ShowCurvePtrArray.GetCount()<=0)
	{
		return;
	}
	double startTime = 0;
	pTemp[0].x=m_curRect.left+m_defChananlInfoWide;
	MinLogicX = CalculateXLogicCoor(pTemp[0].x);

	pTemp[0].x=m_curRect.right;
	MaxLogicX = CalculateXLogicCoor(pTemp[0].x);
	int nMinLogX= (int)(MinLogicX-startTime),nMaxLogX = (int)(MaxLogicX-startTime),nLogScale;
	double dLogScale;
	//保证每个逻辑刻度之间的显示横坐标差大于5个像素
	dLogScale = (MaxLogicX - MinLogicX)/((double)(m_curRect.Width()-m_defChananlInfoWide)/m_nMinWidthOfDisScale);
	nLogScale = (int)dLogScale;
	nLogScale = (m_pUtil->DoubleCmp(dLogScale - nLogScale) == 0) ?nLogScale:(nLogScale+1);//nLogScale为x方向一格刻度代表的逻辑时间
	if(nLogScale < m_nMinWidthOfLogScale)
	{
		nLogScale = m_nMinWidthOfLogScale;
	}
	CRect rcTopScale(0,m_curRect.top+m_titleHight,m_curRect.Width(),m_curRect.top+m_nHeightOfRCScale+m_titleHight)
		,rcBottonScale;
	if(m_curRect.bottom <= (m_rectCurve.bottom+m_nHeightOfRCScale))
	{
		rcBottonScale = CRect(0,m_curRect.bottom-m_nHeightOfRCScale,m_curRect.Width(),m_curRect.bottom);
	}
	else
	{
		rcBottonScale = CRect(0,m_rectCurve.bottom,m_curRect.Width(),m_rectCurve.bottom+m_nHeightOfRCScale);
	}
	pDC->FillSolidRect(rcTopScale,RGB(255,255,255));/*白色填充*/
	pDC->FillSolidRect(rcBottonScale,RGB(255,255,255));
	rcTopScale.left+=m_defChananlInfoWide;
	rcBottonScale.left += m_defChananlInfoWide;
	int i,nScaleBegin,nScaleEnd,nDisScalePos;
	nScaleBegin = nMinLogX%nLogScale ? nMinLogX/nLogScale+1 : nMinLogX/nLogScale;//启动前的刻度格数量
	nScaleEnd = nMaxLogX%nLogScale ? nMaxLogX/nLogScale+1 : nMaxLogX/nLogScale;//启动后刻度格数量
	double dLogRange = MaxLogicX-MinLogicX;
	double dDisRange = m_curRect.Width()-m_defChananlInfoWide;
	pTemp[0].y = rcTopScale.bottom;
	pTemp[1].y = rcTopScale.bottom-3;
	pTemp[2].y = rcTopScale.bottom-6;
	pTemp[3].y = rcBottonScale.top;
	pTemp[4].y = rcBottonScale.top+3;
	pTemp[5].y = rcBottonScale.top+6;
	CPen *pOldPen,pen(PS_SOLID,1,RGB(0,0,0)),penGird(PS_DOT,1,m_colorGirdLine);
	CFont font,*pOldFont;
	font.CreatePointFont(100,_T("MS Sans Serif"));
	pOldFont=pDC->SelectObject(&font);
	pOldPen = pDC->SelectObject(&pen);
	int nOldBKMode = pDC->SetBkMode(TRANSPARENT);
	BOOL bNotTimesOf5;
	bool bDrawGirdLine = ((5*nLogScale-MinLogicX)/dLogRange*dDisRange > 50);
	pDC->MoveTo(rcTopScale.left,rcTopScale.bottom);
	pDC->LineTo(rcTopScale.right,rcTopScale.bottom); //画坐标轴的上横线
	pDC->MoveTo(rcBottonScale.left,rcBottonScale.top); 
	pDC->LineTo(rcBottonScale.right,rcBottonScale.top); //画坐标轴的下横线
	CRect textRc, rectColor;
	CString str;
	textRc = CRect(20,rcTopScale.top,m_defChananlInfoWide,rcTopScale.bottom);
	str = _T("日期"); 
	pDC->DrawText(str,&textRc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	textRc.top = rcBottonScale.top;
	textRc.bottom = rcBottonScale.bottom;
	pDC->DrawText(str,&textRc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//COLORREF colTmp;
	//textRc.left = 15;
	//textRc.top = rcBottonScale.top;
	//textRc.bottom = rcBottonScale.bottom;
	//textRc.right = m_defChananlInfoWide/2;
	//for ( int i = 0; i < 2; i++ )
	//{
	//	switch(i)
	//	{
	//	case 0:
	//		str = _T("任务实际进度");
	//		colTmp = m_factScheduleColor2;
	//		break;
	//	case 1:
	//		str = _T("实际执行时间");
	//		colTmp = m_factScheduleColor;
	//		break;
	//	case 2:
	//		str = _T("任务计划时间");
	//		colTmp = m_timeScheduleColor;
	//		break;
	//	default:
	//		break;
	//	}
	//	pDC->DrawText(str,&textRc,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//	rectColor.top = textRc.top + rcBottonScale.Height()/4;
	//	rectColor.bottom = textRc.bottom - rcBottonScale.Height()/4;
	//	rectColor.right = textRc.left - 2;
	//	rectColor.left = rectColor.right - 10;
	//	pDC->FillSolidRect( rectColor, colTmp );
	//	if ( i == 1 )
	//	{
	//		str = _T("任务计划时间");
	//		colTmp = m_timeScheduleColor;
	//		CRect rectTmp;
	//		rectTmp = textRc;
	//		rectTmp.left = textRc.right + 15;
	//		rectTmp.right = m_defChananlInfoWide;
	//		pDC->DrawText(str,&rectTmp,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	//		rectColor.right = rectTmp.left;
	//		rectColor.left = rectColor.right - 10;
	//		pDC->FillSolidRect( rectColor, colTmp );
	//	}
	//	textRc.bottom = textRc.top;
	//	textRc.top = textRc.bottom - rcBottonScale.Height();
	//}

	int curDrawTime = 0;

	bool isFindFrist = false;
	int  lineSecondBegin = 0;
	COleDateTime tmpDate, lastShowDate;
	COleDateTimeSpan tmpSpan;
	for(i=nScaleBegin;i<=nScaleEnd;i++)
	{
		curDrawTime = i*nLogScale+startTime;

		bNotTimesOf5 = (i%5);
		nDisScalePos = (int)(( (i-lineSecondBegin)*nLogScale+startTime-MinLogicX)/dLogRange*dDisRange+m_defChananlInfoWide);
		if(nDisScalePos < m_defChananlInfoWide) continue;
		pTemp[0].x = pTemp[1].x = pTemp[2].x = pTemp[3].x = pTemp[4].x = pTemp[5].x = nDisScalePos; 
		pDC->MoveTo(pTemp[0]);
		bNotTimesOf5 ? pDC->LineTo(pTemp[1]) : pDC->LineTo(pTemp[2]);//每5个小刻度画一个中长刻度
		pDC->MoveTo(pTemp[3]);
		bNotTimesOf5 ? pDC->LineTo(pTemp[4]) : pDC->LineTo(pTemp[5]);
		if(i % 10 == 0)
		{//每10个小刻度加一个文字标注
			textRc = CRect(nDisScalePos-40,rcTopScale.top,nDisScalePos+40,pTemp[2].y);
			/*str.Format(_T("%d"),(int)(i*nLogScale));*/
			tmpSpan.SetDateTimeSpan( i*nLogScale, 0, 0, 0 );
			tmpDate = m_minDate + tmpSpan;
			if ( i == 0 || lastShowDate.GetYear() != tmpDate.GetYear() )
			{
				str = tmpDate.Format(_T("%Y/%m/%d"));
			}else
			{
				str = tmpDate.Format(_T("%m/%d"));
			}
			lastShowDate = tmpDate;
			pDC->DrawText(str,&textRc,DT_CENTER);
			textRc = CRect(nDisScalePos-40,pTemp[5].y,nDisScalePos+40,rcBottonScale.bottom);
			pDC->DrawText(str,&textRc,DT_CENTER);

			if(nDisScalePos == m_defChananlInfoWide || (!i)) continue;
			pDC->SelectObject(&penGird);
			pDC->MoveTo(pTemp[0]);
			pDC->LineTo(pTemp[3]);
			pDC->SelectObject(&pen);
		}
		else if((!bNotTimesOf5) && bDrawGirdLine && (nDisScalePos != m_defChananlInfoWide))
		{
			pDC->SelectObject(&penGird);
			pDC->MoveTo(pTemp[0]);
			pDC->LineTo(pTemp[3]);
			pDC->SelectObject(&pen);
		}
	}
	pDC->SelectObject(&pOldPen);
	pDC->SetBkMode(nOldBKMode);

}
extern int  GetStringLines( const CString &str, int &cnt );
void  CProjectScheduleView::DrawEventToolTips(CDC *pDC)
{//画任务提示信息
	UINT nTopOfCoor;  //nTopOfCoor当前坐标系的顶坐标
	long i = 0;
	nTopOfCoor = m_rectCurve.top;  //nTopOfCoor:整个显示画布的bottom值
	CString str;
	//在位图上画曲线时跳过不会显示的曲线
	for( i=0; i < m_nFirstDisCurve ;i++)
	{
		nTopOfCoor += m_nHeightOfCoor;
	}
	if( m_nFirstDisCurve <= -1)
	{
		return;
	}
	CString tempStr = _T("无意义");
	CSize TextSize = pDC->GetTextExtent(tempStr);
	int width = 250, hight = 10;
	hight += (TextSize.cy/2+10)*4;	
	CPoint pointScrollPos = GetScrollPosition();

	CRect clientRect;
	GetClientRect(clientRect);
	for ( i = m_nFirstDisCurve; i <= m_nLastDisCurve; i++ )
	{
		if ( m_mousePoint.y > nTopOfCoor && m_mousePoint.y < nTopOfCoor+m_nHeightOfCoor )
		{
			CTaskObj *pTask = (CTaskObj *)m_ShowCurvePtrArray.GetAt( i );
			long beginX = CalculateXPixelCoor( GetBeginXDays( pTask->GetPlanStartTime() ) );
			long endX = CalculateXPixelCoor( GetBeginXDays( pTask->GetPlanEndTime() ) + 3 );
			if ( (m_mousePoint.x >= beginX && m_mousePoint.x <= endX) || ( m_mousePoint.x  < m_defChananlInfoWide) )
			{
				CString tmpStr, endTimeStr;
				if ( m_mousePoint.x >= m_defChananlInfoWide )
				{
					if ( pTask->GetFactStartDateShowStr().GetLength() > 0 )
					{
						endTimeStr.Format(_T("实际%s"), pTask->GetFactStartDateShowStr() );
						if ( pTask->GetFactEndDateShowStr().GetLength() > 0 )
						{
							endTimeStr += _T("到") + pTask->GetFactEndDateShowStr();
						}
					}
					CUser obj;
					m_pActionFactory->GetLoginServer()->GetUserById( pTask->GetOwnerId(), obj );
					CString st;
					const CTaskStaus &map = m_pActionFactory->GetTaskSever()->GetTaskStausMap();
					 map.GetStrStausById( pTask->GetTaskStatus(), st );
					tmpStr.Format( _T("责任人:%s\r\n状态:%s\r\n计划%s到%s\r\n%s"), obj.GetUserName_R(), st, pTask->GetPanStartDateShowStr(),
						pTask->GetPanEndDateShowStr(), endTimeStr );
					int lineNum = 4;
					if ( m_pChangObj && m_pChangObj->GetTaskId() == pTask->GetId() )
					{//2015-11-2 显示变更时间
						int type = m_pChangObj->GetType();
						CString str;
						if ( changeTime == type )
						{
							int st = pTask->GetTaskStatus();
							bool isFinishChange = m_pChangObj->IsFinishChange()/*(changeWait1 != st) && (changeWait2 != st) && (changeWait0 != st)*/;
							if ( isFinishChange || m_changeAfterTask.GetPlanEndTime() != pTask->GetPlanEndTime() )
							{
								str.Format( _T("变更计划时间为:%s"), m_changeAfterTask.GetPanEndDateStr() ); 
							}else if ( m_changeAfterTask.GetPlanWorkHour() != pTask->GetPlanWorkHour() )
							{
								str.Format( _T("变更计划工时为:%0.1f"), m_changeAfterTask.GetPlanWorkHour() ); 
							}
						}else
						{
							str = m_pChangObj->GetChangeTypeStr();
						}
						tmpStr += _T("\r\n");
						tmpStr += str;
						lineNum++;
					}
					width = 230;
					hight = 10 + (TextSize.cy/2 + 10)*lineNum;	
				}else
				{//
					int len = 0, len2;
					if ( pTask->GetProjctId() > 0 )
					{
						CProject prjObj;
						m_pActionFactory->GetProjectSever()->GetProjectById( pTask->GetProjctId(), prjObj );
						tmpStr += prjObj.GetShortName();
						len = tmpStr.GetLength();
						tmpStr += _T("\r\n");
					}
					len2 = pTask->GetName().GetLength();
					if ( len2 > len )
					{
						len = len2;
					}
					if ( m_pChangObj && m_pChangObj->GetTaskId() == pTask->GetId() )
					{//2015-11-2 显示变更类型
						CString str;
						str = m_pChangObj->GetChangeTypeStr();
						len = str.GetLength();
						tmpStr += str;
						tmpStr += _T("\r\n");
					}
					if ( len2 > len )
					{
						len = len2;
					}
					tmpStr += pTask->GetName();
					TextSize =  pDC->GetTextExtent(tmpStr);
					width = TextSize.cx ;
					
					int cnt = 0;
					GetStringLines( tmpStr, cnt );
					cnt += 1;
					hight = 10 + (TextSize.cy/2 + 10)*cnt;
				}

				CRect rect;
				rect.left = m_mousePoint.x - pointScrollPos.x + 5;
				if (!((rect.left + width > clientRect.left + m_defChananlInfoWide) && (rect.left + width < clientRect.right)))
				{
					rect.left = rect.left - width;
				}
				rect.top = m_mousePoint.y /*- pointScrollPos.y */+ 5;
				if (!((rect.top + hight > clientRect.top + m_nHeightOfRCScale+m_titleHight) &&(rect.top + hight < clientRect.bottom - m_nHeightOfRCScale)))
				{
					rect.top = rect.top - hight;
				}
				rect.right = rect.left + width;
				rect.bottom = rect.top + hight;
				int oldmode = pDC->SetBkMode(TRANSPARENT);
				pDC->FillSolidRect(rect, RGB(207, 252, 242));
				pDC->DrawText(tmpStr, &rect, DT_LEFT);
				pDC->SetBkMode(oldmode);
			}
		}
		nTopOfCoor += m_nHeightOfCoor;
	}
}
void CProjectScheduleView::InitMulti()
{
	CRect rc;
	GetDesktopWindow()->GetWindowRect(rc);

	rc.right -=  m_defChananlInfoWide+20;
	double d1=m_clientRc.Width()- m_defChananlInfoWide;//整个虚拟位图的像素宽度

	m_dMinMulti = static_cast<double>(rc.Width())/(d1);
	//double allTime = CWareDataSubItem::GetAllTimePixel(); //1111111111111111111
	//if (allTime > 6000)
	//{
	//	m_nHMulti = static_cast<int>(allTime/6000);
	//	m_dNewMulti = m_dMinMulti *m_nHMulti;/*在GetCurve中将m_dNewMulti赋值给m_dMulti*/
	//} 
	//else
	//{
		m_dNewMulti = m_dMinMulti;	
	//}
}
void CProjectScheduleView::SetCoorHeight(int nHeightPara /*= DIGITAL_HEIGHTH_COONDIATE*/, BOOL isNoRefres /*=false*/)
{
	m_nHeightOfCoor = nHeightPara;
	CRect rc;
	long nWidth,nHeight= m_titleHight + m_nHeightOfRCScale*2;
	int nCurveCount = m_ShowCurvePtrArray.GetCount();
	double MaxOfX = 0.0;
	for (int i=0; i< nCurveCount; i++)
	{
		nHeight += nHeightPara;
	}
	m_curveHight = nHeight - m_nHeightOfRCScale*2 - m_titleHight;

	double days = GetShowDays();
	nWidth = CalculateXPixelCoor(days);
	nWidth += m_curRect.left; 
	GetClientRect(&m_clientRc);
	rc = m_clientRc;
	ClientToScreen(&rc);
	if(rc.left < m_defChananlInfoWide )
	{
		m_clientRc.right = m_clientRc.left + nWidth;
	}
	else if(m_clientRc.Width() < (nWidth+20))
	{
		m_clientRc.right = m_clientRc.left + nWidth;
	}
	//保证界面大小不被还原
	if ( m_clientRc.right < rc.right )
	{
		m_clientRc.right = rc.right;
	}
	if(rc.top <HEIGHT_INFO)
	{
		m_clientRc.bottom = m_clientRc.top + nHeight;
	}
	else if((nHeight+20) > m_clientRc.Height())
	{
		m_clientRc.bottom = m_clientRc.top + nHeight;
	}
	//m_clientRC区域为整个虚拟位图的大小，m_rectCurve区域为虚拟位图中整个虚拟坐标系区域
	m_rectCurve=CRect(m_clientRc.left+m_defChananlInfoWide, m_clientRc.top+m_nHeightOfRCScale+m_titleHight,
		m_clientRc.right, m_clientRc.top+nHeight-m_nHeightOfRCScale);
	if (!isNoRefres)
	{
		CDocument* pDoc = GetDocument();
		if ( pDoc )
		{
			pDoc->UpdateAllViews(NULL);
		}else
		{
			::RedrawWindow(m_hWnd, NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ALLCHILDREN);
		}
	}
}
double  CProjectScheduleView::GetShowDays()
{
   COleDateTimeSpan span = m_maxDate - m_minDate;
   double ret = span.GetDays() + 3;
   return ret;
}
long CProjectScheduleView::CalculateXPixelCoor(double XLogic, int originX/* = 0*/) const
{
	if ( !m_bPrint )
	{
		double xLogicalValueOfCell = GetXLogicValueOfCell();
		int xPixelValueOfCell = abs(m_defHeightOfCoor/CELL_COUNT);
		long XPixel = 0.0;
		XPixel = static_cast<long>(XLogic * xPixelValueOfCell*GetMulti()/xLogicalValueOfCell - m_curRect.left + m_defChananlInfoWide); 
		return XPixel;
	}else
	{
		return CalculatePrintXPixelCoor( XLogic, originX );
	}
}
double CProjectScheduleView::CalculateXLogicCoor(long XPixel, int originX/* = 0*/) const
{
	if( !m_bPrint )
	{
		double xLogicalValueOfCell = GetXLogicValueOfCell();
		int xPixelValueOfCell = abs(m_defHeightOfCoor/CELL_COUNT);
		double XLogic = 0.0f;
		XLogic = (XPixel- m_defChananlInfoWide)*xLogicalValueOfCell/(xPixelValueOfCell*GetMulti()); 
		return XLogic;
	}else
	{
		return CalculatePrintXLogicCoor( XPixel,  originX );
	}
}
long CProjectScheduleView::CalculatePrintXPixelCoor( double XLogic, int originX ) const
{
	double xLogicalValueOfCell = GetXLogicValueOfCell();
	int xPixelValueOfCell = abs(m_defHeightOfCoor/CELL_COUNT);
	long XPixel = 0.0;
	XPixel=(LONG)(XLogic*xPixelValueOfCell*m_dMulti/xLogicalValueOfCell+originX-m_curRect.left+m_rcPrintCurve.left);
	return XPixel;
}
double CProjectScheduleView::CalculatePrintXLogicCoor(long XPixel, int originX) const
{
	double xLogicalValueOfCell = GetXLogicValueOfCell();
	int xPixelValueOfCell = abs(m_defHeightOfCoor/CELL_COUNT);
	double XLogic = 0.0f;
	XLogic=(double)(XPixel- originX)/(xPixelValueOfCell*m_dMulti)*xLogicalValueOfCell;
	return XLogic;
}
double  CProjectScheduleView::GetBeginXDays( const COleDateTime &dateTime )
{
	COleDateTimeSpan span = dateTime - m_minDate;
	double ret = span.GetDays();
	return ret;
}
// 修改CCurveView的滚动条消息响应，在默认情况下滚动条最长可进行32768像素长度的移动，超过32768后
//消息响应函数取得的nPos值将不正确，因此改用重新获取nPos的方法来进行消息响应 [6/18/2013 gaofeng]
BOOL CProjectScheduleView::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
	// TODO: 在此添加专用代码和/或调用基类

	//return CMyView::OnScroll(nScrollCode, nPos, bDoScroll);
	// calc new x position
	SCROLLINFO info;
	info.cbSize = sizeof(SCROLLINFO);
	info.fMask = SIF_TRACKPOS;

	if (LOBYTE(nScrollCode) == SB_THUMBTRACK)
	{
		GetScrollInfo(SB_HORZ, &info);
		nPos = info.nTrackPos;
	}

	if (HIBYTE(nScrollCode) == SB_THUMBTRACK)
	{
		GetScrollInfo(SB_VERT, &info);
		nPos = info.nTrackPos;
	}

	int x = GetScrollPos(SB_HORZ);
	int xOrig = x;
	// 修改滚动一行和一页进行滚动的大小 [6/27/2013 gaofeng]
	m_lineDev.cx = m_curRect.Width()/5;
	m_pageDev.cx = m_curRect.Width();
	switch (LOBYTE(nScrollCode))
	{
	case SB_TOP:
		x = 0;
		break;
	case SB_BOTTOM:
		x = INT_MAX;
		break;
	case SB_LINEUP:
		x -= m_lineDev.cx;
		break;
	case SB_LINEDOWN:
		x += m_lineDev.cx;
		break;
	case SB_PAGEUP:
		x -= m_pageDev.cx;
		break;
	case SB_PAGEDOWN:
		x += m_pageDev.cx;
		break;
	case SB_THUMBTRACK:
		x = nPos;
		break;
	}

	// calc new y position
	int y = GetScrollPos(SB_VERT);
	int yOrig = y;

	switch (HIBYTE(nScrollCode))
	{
	case SB_TOP:
		y = 0;
		break;
	case SB_BOTTOM:
		y = INT_MAX;
		break;
	case SB_LINEUP:
		y -= m_lineDev.cy;
		break;
	case SB_LINEDOWN:
		y += m_lineDev.cy;
		break;
	case SB_PAGEUP:
		y -= m_pageDev.cy;
		break;
	case SB_PAGEDOWN:
		y += m_pageDev.cy;
		break;
	case SB_THUMBTRACK:
		y = nPos;
		break;
	}
	BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
	if (bResult && bDoScroll)
		UpdateWindow();

	return bResult;
}
void CProjectScheduleView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pTemp = point;
	CMenu PopupMenu;
	PopupMenu.CreatePopupMenu();

	PopupMenu.AppendMenu( all_task == m_selShowDate ? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_SHOW_ALL, _T("显示所有任务"));
	PopupMenu.AppendMenu( recent_month_task == m_selShowDate ? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_BEFOR_MONTH, _T("显示最近一个月任务"));
	PopupMenu.AppendMenu(  recent_quart_task == m_selShowDate ? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_BEFOR_QUART, _T("显示最近一个季度任务"));
	
	PopupMenu.AppendMenu(recent_halfyear_task == m_selShowDate? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_BEFOR_HALFYEAR, _T("显示最近半年任务"));
	PopupMenu.AppendMenu(last_month_task == m_selShowDate? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_AFTER_MONTH, _T("显示最后一个月任务"));
	PopupMenu.AppendMenu(last_quart_task == m_selShowDate? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_AFTER_QUART, _T("显示最后一个季度任务"));
	PopupMenu.AppendMenu(last_halfyear_task == m_selShowDate? MF_ENABLED|MF_CHECKED:MF_ENABLED, IDM_AFTER_HALFYEAR, _T("显示最后半年任务"));

	ClientToScreen(&pTemp);
	PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pTemp.x, pTemp.y, this);

	CScrollView::OnRButtonDown(nFlags, point);
}
void CProjectScheduleView::OnFilterTask(UINT menuID)
{
	int selPos = menuID - IDM_SHOW_ALL;
	if ( selPos >= 0 && selPos <= last_halfyear_task )
	{
	     m_selShowDate = selPos;
		 ResetShowTask( TRUE );
	}
}
void CProjectScheduleView::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rc;
	GetClientRect(&rc);
	m_mousePoint = point;
	if(!rc.PtInRect(point))
	{
		m_mousePoint.x=-1;
	}
	GetClientRect(&rc);
	InvalidateRect(&rc,FALSE);
}
BOOL CProjectScheduleView::CreateFormCtrl(UINT nID, CWnd *pParent)
{
	if (!pParent || !pParent->GetSafeHwnd())
	{
		return FALSE;
	}
	CWnd *pCtrl = pParent->GetDlgItem(nID);
	if (!pCtrl)
	{
		return FALSE;
	}
	CRect rcCtrl;
	pCtrl->GetWindowRect(rcCtrl);
	pParent->ScreenToClient(rcCtrl);
	UINT style = ::GetWindowLong(pCtrl->GetSafeHwnd(), GWL_STYLE);
	pCtrl->DestroyWindow();


	return Create(NULL, NULL, style | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
		rcCtrl, pParent, nID, NULL);
}