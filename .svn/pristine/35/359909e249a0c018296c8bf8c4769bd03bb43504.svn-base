#include "stdafx.h"
#include "ForecastWorkHour.h"
#include "Src/MainFrame/MainFrm.h"

CForecastWorkHour::CForecastWorkHour( CActionServerFactory *pSeverFactory ):
CBaseGridManger( pSeverFactory ),m_showDateCnt(SHOW_DAY_CNT),m_pSystemCfgSever(NULL)
{
	m_beginDate = COleDateTime::GetCurrentTime();
	m_pSystemCfgSever = m_pSeverFactory->GetSystemCfgSever();
	m_pDepartSever = m_pSeverFactory->GetDepartSever();
	m_prjSever = m_pSeverFactory->GetProjectSever();
	InitWorkDateArray();
}
CForecastWorkHour::~CForecastWorkHour()             
{
}
//
void CForecastWorkHour::InitWorkDateArray( )
{//
	COleDateTime beginDate = m_beginDate;
	COleDateTimeSpan span( 1, 0, 0, 0 );
	BOOL isHoliday = FALSE;
	int cnt = 0; 
	while( cnt < m_showDateCnt )
	{
		isHoliday = FALSE;
		m_pSystemCfgSever->IsHoliday( beginDate, isHoliday );
		if ( FALSE == isHoliday )
		{
			m_showWorkDay[cnt++] = beginDate;
		}
		beginDate += span;
	}
}
CString CForecastWorkHour::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	if ( 0 == nRow )
	{
		if ( nColumn > 0 )
		{//具体工作日
			if ( 1 == nColumn || m_showWorkDay[ nColumn - 1].GetYear() != m_showWorkDay[0].GetYear() )
			{
				text = m_showWorkDay[ nColumn - 1].Format( _T("%Y/%m/%d") );
			}else
			{
				text = m_showWorkDay[ nColumn - 1].Format( _T("%m/%d") );
			}
			
		}else
		{
			text = _T("工作日");
		}
	}else
	{
		if ( nRow > 0 && nRow <= m_lstShowWorker.size() )
		{
			ListUserItor itor = m_lstShowWorker.begin();
		    advance( itor, nRow - 1 );
			text = itor->GetUserName_R();
		}
	}
	return text;
}
CString CForecastWorkHour::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	if ( nRow < m_lstShowWorker.size() )
	{
		ListUserItor itorWorker = m_lstShowWorker.begin();
		advance( itorWorker, nRow );
		double workHour = m_pSystemCfgSever->CaluateForecastWorkHour( itorWorker->GetId(), m_showWorkDay[nColumn] );
		text.Format( _T("%0.1f"), workHour );
		int factRow =  nRow + GetFixedRowCount();
		int factCol = nColumn + GetFixedColumnCount();
		if ( workHour > 10 )
		{//修改颜色
			m_gridCtrl->SetItemFgColour( factRow, factCol, RGB(255, 102, 152) );
		}
		if ( workHour < 7.0 )
		{
			m_gridCtrl->SetItemFgColour( factRow, factCol, RGB( 0, 0, 160) );
		}
	}
	return text;
}
void CForecastWorkHour::CreateColumnMap()
{
	CString name;
	for ( int i = 0; i < m_showDateCnt; i++ )
	{
		name.Format(_T("col%d"), i );
		InsertNumEditColumn( name , 80, DT_CENTER, i/2 );
	}
}
BOOL  CForecastWorkHour::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
{
	pCell->SetFlags( CGridCellNumeric::Real  );
	return TRUE;
}
GridItemType CForecastWorkHour::GetFixedItemType( int nRow, int nColumn )
{
	return CBaseGridManger::GetFixedItemType( nRow, nColumn );
}
void CForecastWorkHour::SetBeginWorkDate( const COleDateTime &beginDate )
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_DATA );
	m_beginDate = beginDate;
	InitWorkDateArray();
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg();
}
void  CForecastWorkHour::SetSelDepartUser( const ListDepart &lstDepart, int index  )
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_DATA );
	m_lstShowWorker.clear();
	if ( lstDepart.size() > 0 )
	{
		if ( index >= lstDepart.size() || index < -1 )
		{
			index = 0;
		}
		ListDepartConstItor itor = lstDepart.begin();
		advance( itor, index );
		m_pDepartSever->GetDepartUser( itor->GetId(), m_lstShowWorker, FALSE );
		//for ( ListDepartConstItor itor = lstDepart.begin(); itor != lstDepart.end(); itor++ )
		//{
		//	if ( !(_T("研发管理组") == itor->GetName() || _T("研发体系") == itor->GetName()) )
		//	{
		//		m_pDepartSever->GetDepartUser( itor->GetId(), m_lstShowWorker, FALSE );
		/*	}
		}*/
	}else
	{
		m_lstShowWorker.push_back( m_pSeverFactory->GetLoginServer()->GetLoginUser() );
	}
	UpdateGridView();
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void  CForecastWorkHour::SetSelProject( int prjId )
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_DATA );
	m_lstShowWorker.clear();
	m_prjSever->GetProjectMember( prjId, m_lstShowWorker );
	UpdateGridView();
 	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
int  CForecastWorkHour::GetFlexRowCount()
{
	return m_lstShowWorker.size();
}