#include "stdafx.h"
#include "WorkHourManger.h"
#include "Src/MainFrame/messageDef.h"
#include "Src/MainFrame/MainFrm.h"
#include "Src/MainFrame/Output/ClientOutputManager.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"
#include "Src/action/ExcelAction/CTaskExcelAction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CWorkHourManger::CWorkHourManger( CActionServerFactory *pSeverFactory )
	:CBaseGridManger( pSeverFactory ),m_showDateCnt(WEEK_DAY_CNT)
{
	m_pWorkHourAction = pSeverFactory->GetWorkHourSever();
    m_timeTypeCnt = 0;
	m_selUerId = pSeverFactory->GetLoginServer()->GetLoginUser().GetId();
	m_pSystemCfgSever = pSeverFactory->GetSystemCfgSever();
	SetDefWeekDate();
	m_pWorkHourList = NULL;
	memset( m_isHoliday, 0, sizeof(m_isHoliday) );
	m_validateOk = RGB( 255, 255, 255 );
	m_validateError = RGB( 255, 232, 255); //粉红
	m_assginWorkHour = pSeverFactory->GetLoginServer()->FindLoginPower( allocateWorkHour );
}
CWorkHourManger::~CWorkHourManger()
{
	if ( m_pWorkHourList )
	{
		delete [] m_pWorkHourList;
		m_pWorkHourList = NULL;
	}
}
void CWorkHourManger::CreateColumnMap()
{
	CString name;
	for ( int i = 0; i < 14; i++ )
	{
	    name.Format(_T("col%d"), i );
		InsertNumEditColumn( name , 80, DT_CENTER, i/2 );
	}
}
GridItemType CWorkHourManger::GetFixedItemType( int nRow, int nColumn )
{
	if( nRow == HOIDAY_ROW_POS && nColumn >= GetFixedColumnCount() )
	{
		return GridItemType_Check;
	}
	return CBaseGridManger::GetFixedItemType( nRow, nColumn );
}
void  CWorkHourManger::CreateRowItemMap()
{
	m_workTypeList = m_pTaskSever->GetWorkHourString();
	m_timeTypeCnt = m_workTypeList.size();
    //添加请假条目
	m_workTypeList.insert( std::make_pair( 0, _T("请假工时")) );
	m_timeTypeCnt += 1;
	m_workTypeList.insert( std::make_pair( MAX_WORK_HOURTYPE, _T("工时合计")) ); //不包含请假
	if ( m_pWorkHourList )
	{
		delete [] m_pWorkHourList;
		m_pWorkHourList = NULL;
	}
	m_pWorkHourList = new CWorkHourData[m_workTypeList.size()*m_showDateCnt];
	OnInitWorkHourData();
}
void CWorkHourManger::SetSelUser( int userId )
{
	m_selUerId = userId;
	OnInitWorkHourData();
	UpdateGridView();
}
void CWorkHourManger::CalWorkHourData( int selUserId, CWorkHourData  *pWorkHourList)
{
	int pos = 0;
	double workHourSum[WEEK_DAY_CNT];
	double norWorkHourSum[WEEK_DAY_CNT];
	memset( workHourSum, 0, sizeof(workHourSum) );
	memset( norWorkHourSum, 0, sizeof(norWorkHourSum) );
	for ( TaskIdStringMapItor itor = m_workTypeList.begin();  itor != m_workTypeList.end(); itor++ )
	{

		for (int i = 0; i < m_showDateCnt; i++ )
		{
			CWorkHourData data;
			data.SetDate( m_weekDate[i] );
			data.SetTimeType( itor->first );
			data.SetUserId( m_selUerId );
			if ( itor->first >= 0 && itor->first < MAX_WORK_HOURTYPE )
			{//获取任务工时
				m_pWorkHourAction->GetWorkTimeData( itor->first, selUserId, m_weekDate[i], data );
				double taskWorkHour = 0.0;
				if ( itor->first > 0 )
				{
					m_pWorkHourAction->GetTaskHourSum( selUserId, itor->first, m_weekDate[i], taskWorkHour );
				}
				data.SetTaskWorkHour( taskWorkHour );
				workHourSum[i] += taskWorkHour;
				norWorkHourSum[i] += data.GetNorTaskWorkHour();
			}else
			{//合计
				data.SetTaskWorkHour( workHourSum[i] );
				data.SetNorTaskWorkHour( norWorkHourSum[i] );
			}
			//
			pWorkHourList[pos] = data ;
			pos++;
		}
	}
}
void CWorkHourManger::OnInitWorkHourData()
{
	CalWorkHourData( m_selUerId, m_pWorkHourList);
}
int CWorkHourManger::GetFlexRowCount()
{
    return m_workTypeList.size();
}
void CWorkHourManger::OnUpdateShowBeginDate( COleDateTime &date )
{
	CString temp = date.Format( _T("%Y-%m-%d") );
	SetWeekDate( date );
	UpdateGridView();
}
void CWorkHourManger::RefreshShowData()
{
	SetWeekDate( m_showBeginDate );
	UpdateGridView();
}
void CWorkHourManger::SetWeekDate( const COleDateTime &date )
{
	m_showBeginDate = date;
	int weekDay  = CUtilityTool::GetObj()->GetDayOfWeek( date );
	COleDateTimeSpan span( weekDay - 1, 0, 0, 0 );
	COleDateTime beginDate = date - span;
	span.SetDateTimeSpan( 1, 0, 0, 0 );
	for ( int i = 0; i < WEEK_DAY_CNT; i++ )
	{
		m_weekDate[i] = beginDate;
		ASSERT( userExcOk == m_pSystemCfgSever->IsHoliday( beginDate, m_isHoliday[i] ) );
		beginDate += span;
		
	}
	OnInitWorkHourData();
}
void CWorkHourManger::SetDefWeekDate()
{
	SetWeekDate( COleDateTime::GetCurrentTime() );
}

//固定行列内容
CString CWorkHourManger::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	if ( nRow == 0 )
	{//日期
		const LPWSTR weekName[7] = { _T("星期一"),_T("星期二"),_T("星期三"),_T("星期四"),
		                  _T("星期五"), _T("星期六"), _T("星期日") } ;
		if ( nColumn > 0 )
		{
			if ( nColumn%2 == 1  )
			{
				text = weekName[nColumn/2];
			}else
			{//日期
				int selDate = nColumn/2 - 1;
				if ( selDate >= 0 )
				{
					text = m_weekDate[selDate].Format(_T("%m/%d"));
				}
			}
		}else
		{
			text = _T("日期");
		}
	}else if ( nRow == 1 )
	{
		if ( nColumn > 0  )
		{
		    text = nColumn % 2 == 0 ? _T("任务工时"):_T("非任务工时");  
		}else
		{
			text = _T("类型");
		}
	}else if ( nRow == HOIDAY_ROW_POS  )
	{
		if ( nColumn == 0 )
		{
			text = _T("假日");
		}else if( nColumn >= GetFixedColumnCount() )
		{
			int selDate = nColumn/2;
			if ( nColumn%2 == 0  )
			{
				selDate -= 1;
			}
			CGridCellCheck *pCell = (CGridCellCheck *)m_gridCtrl->GetCell( nRow, nColumn );
			if ( pCell )
			{
				BOOL isHoliday = FALSE;
				m_pSystemCfgSever->IsHoliday( m_weekDate[selDate], m_isHoliday[selDate] );
				pCell->SetCheck( m_isHoliday[selDate] );
			}
		}
	}else
	{
		if ( 0 == nColumn )
		{
			nRow -= GetFixedRowCount();
			if ( nRow >= 0 && nRow < m_workTypeList.size() )
			{
				TaskIdStringMapConstItor itor = m_workTypeList.begin();
				advance( itor, nRow);
				text = itor->second;
			}
		}
		
	}
	return text;
}
CWorkHourData* CWorkHourManger::GetWorkHourData( int nRown, int nColum )
{
	int pos = nRown*m_showDateCnt + nColum/2;
	if ( pos >= 0  )
	{
		return m_pWorkHourList + pos;
	}
	return NULL;
}
CString CWorkHourManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	if ( m_workTypeList.size() < 0 )
	{
		return text;
	}
	CWorkHourData *pData = GetWorkHourData(  nRow,  nColumn );
	if ( pData )
	{
		
		int selDate = nColumn/2;

		if ( nColumn%2 == 0  )
		{//非任务工时
			text.Format(_T("%0.1f"), pData->GetNorTaskWorkHour() );
			if ( m_isHoliday[selDate] )
			{
				SetColumnBkColor( nColumn+GetFixedColumnCount(), RGB(224, 224, 224 ) );
			}/*else
			 {
			 SetColumnBkColor( nColumn+GetFixedColumnCount(), RGB(255, 255, 255 ) );
			 }*/
		}else 
		{
			text.Format( _T("%0.1f"), pData->GetTaskWorkHour() );
			//设置背景色
			SetColumnBkColor( nColumn+GetFixedColumnCount(), RGB(224, 224, 224 ) );
		}
	}
	return text;

}
//判断输入数字是否合法
BOOL   CWorkHourManger::ValidateSetData( int nRow, int nColumn, double &value, double &sumData )
{
	if ( nColumn%2 == 1 )
	{
		return FALSE;
	}
	int colCnt = m_showDateCnt;
	int col = nColumn/2;
	int rowCnt = m_timeTypeCnt;
	int setDataPos = nRow*colCnt + col;
	sumData = 0.0;
	double taskWorkHourSum = 0.0;
	double orginValue = 0.0;
	for ( int i = 0; i < rowCnt; i++ )
	{
		int pos = i*colCnt + col;
		//非工作
		if ( pos != setDataPos )
		{
			sumData += m_pWorkHourList[pos].GetNorTaskWorkHour();
		}else
		{
			sumData += value;
	        orginValue = m_pWorkHourList[pos].GetNorTaskWorkHour();
		}
		taskWorkHourSum += m_pWorkHourList[pos].GetTaskWorkHour();
	}
	long bRet = CUtilityTool::GetObj()->DoubleCmp( (taskWorkHourSum + sumData) - 7.5 );
	if (  bRet <= 0 )
	{
		return TRUE;
	}
	double diff = (double)7.5-(taskWorkHourSum + sumData-value);
	CString info;
	info.Format( _T("天总工时超出7.5小时，应填范围[0~%0.1f]"), diff );
	AfxMessageBox( info );
	return FALSE;
}

BOOL   CWorkHourManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	CWorkHourData *pData = GetWorkHourData(  nRow,  nColumn );
	if ( pData )
	{
		int row = nRow + GetFixedRowCount();
		int colum = nColumn + GetFixedColumnCount();
		CGridCellNumeric *pcell = (CGridCellNumeric *)m_gridCtrl->GetCell( row, colum );
		if ( pcell )
		{
			double value = pcell->GetNumber();
			int pos = nRow*m_showDateCnt + nColumn/2;
			double orginValue = m_pWorkHourList[ pos ].GetNorTaskWorkHour();
			if ( value == orginValue )
			{
				return TRUE;
			}
			BOOL isOk = FALSE;
			double sumData = 0.0; 
			if ( value >= 0 )
			{
				double tempValue = value + Precision_2;
	            int    value1 = (int)(tempValue*10);
				int    value2 = (int)(value*10);
				if ( value1 != value2 )
				{
					value = value1*1.0/10;
				}
				isOk = ValidateSetData(  nRow, nColumn, value, sumData );
			}
			if ( isOk )
			{
				m_pWorkHourList[ pos ].SetNorTaskWorkHour( value );
				//更新数据库
				BOOL ret = m_pWorkHourAction->AddOrUpdateWorkHour( m_pWorkHourList[ pos ] );
				if ( ret != userExcOk )
				{
					AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
					m_pWorkHourList[ pos ].SetNorTaskWorkHour( orginValue );
					UpdateGridRowView( nRow );
				}
				m_pWorkHourAction->DeleteUnUpdateDateByUserId( m_selUerId, m_pWorkHourList[ pos ].GetDate() );
				pos = m_timeTypeCnt*m_showDateCnt + nColumn/2;
				m_pWorkHourList[ pos ].SetNorTaskWorkHour( sumData );
				UpdateGridRowView( m_timeTypeCnt );
			}else
			{
				UpdateGridRowView( nRow );
			}
			
		}
	}
	return TRUE;
}
BOOL    CWorkHourManger::GetFlexItemEditable(int nRow,int nColumn)
{
  if ( nRow < m_timeTypeCnt && nColumn%2 == 0 )
  {
	  int selDate = nColumn/2;
	 if ( !m_isHoliday[selDate] )
	 {
		 return TRUE;
	 }
 }
  return FALSE;
}
BOOL    CWorkHourManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
{
	pCell->SetFlags( CGridCellNumeric::Real  );
	return TRUE;
}
void  CWorkHourManger::OnClickGrid( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	if ( gridView->iRow == HOIDAY_ROW_POS && gridView->iColumn >= GetFixedColumnCount() )
	{//设置固定行
		CGridCellCheck *pCell = ( CGridCellCheck *)m_gridCtrl->GetCell( gridView->iRow, gridView->iColumn );
	
		if ( m_assginWorkHour && pCell )
		{
			int nSelDate = gridView->iColumn/2;
			if ( gridView->iColumn %2 == 0 )
			{
				nSelDate -= 1;
			}
		    BOOL isHoilday = !m_isHoliday[nSelDate];
			BOOL ret;
			if ( isHoilday )
			{
				ret = m_pSystemCfgSever->AddHoliday( m_weekDate[ nSelDate ] );
			}else
			{
				ret = m_pSystemCfgSever->DeleteHoliday( m_weekDate[ nSelDate ]  );
			}
			if ( ret != userExcOk )
			{
				AfxMessageBox( m_pSeverFactory->GetErrorString(ret) );
				UpdateGridFixedRowView( gridView->iRow );
				return;
			}
			if ( m_isHoliday)
			{
				m_isHoliday[nSelDate] = isHoilday;
				pCell->SetCheck( m_isHoliday[nSelDate] );
				if ( gridView->iColumn %2 == 0 )
				{
					pCell = ( CGridCellCheck *)m_gridCtrl->GetCell( gridView->iRow, gridView->iColumn - 1 );
				}else
				{
                    pCell = ( CGridCellCheck *)m_gridCtrl->GetCell( gridView->iRow, gridView->iColumn + 1 );
				}
				if ( pCell )
				{
					pCell->SetCheck( m_isHoliday[nSelDate] );
				}
			}
		}
	}

}
void CWorkHourManger::OnGridSelChange( NMHDR* pNMHDR,LRESULT* pResult )
{
}
void CWorkHourManger::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	if ( gridView->iRow >= GetFixedRowCount() && gridView->iColumn >= GetFixedColumnCount() )
	{
		EndEditGridView( gridView->iRow, gridView->iColumn );
	}
}
BOOL CWorkHourManger::CheckWorkDataValidate( CWorkHourData *pData, BOOL *relust )
{
	BOOL isHasError = FALSE;
	for ( int i = 0; i < m_showDateCnt; i++ )
	{
		relust[i] = TRUE;
		if ( FALSE == m_isHoliday[i] )
		{
			int pos = m_timeTypeCnt*m_showDateCnt + i; 
			double sum = pData[pos].GetNorTaskWorkHour() + pData[pos].GetTaskWorkHour();
			
			if ( CUtilityTool::GetObj()->DoubleCmp( sum - 7.5 ) != 0 )
			{
				relust[i] = FALSE;
				isHasError = TRUE;
			}
		}
	}
	return isHasError;
}

void CWorkHourManger::SetValidateColor( BOOL *prelust )
{
	int fixColumNum = GetFixedColumnCount();
	for( int i = 0; i < WEEK_DAY_CNT; i++ )
	{
		int colNum = i*2+fixColumNum;
		if ( m_isHoliday[i] )
		{
			continue;
		}
		if ( prelust[i] )
		{
			SetColumnBkColor( i*2+fixColumNum, m_validateOk );
		}else
		{
			SetColumnBkColor( i*2+fixColumNum, m_validateError );
		}
		m_gridCtrl->RedrawColumn( colNum );
	}
	//UpdateGridView();
}
void CWorkHourManger::OnCheckSelUser()
{//
	BOOL relust[WEEK_DAY_CNT];
	CheckWorkDataValidate( m_pWorkHourList, relust );
	SetValidateColor( relust );
	
}
void CWorkHourManger::OnCheckAllUser()
{
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	ListUser lst;
	m_pSeverFactory->GetLoginServer()->GetAllUser( lst );
	CWorkHourData *pData = new CWorkHourData[m_showDateCnt*m_workTypeList.size()];
	CWorkHourData *pValidateData = NULL;
	BOOL relust[WEEK_DAY_CNT];
	BOOL hasError;
	for ( ListUserItor itor = lst.begin(); itor != lst.end(); itor++ )
	{
	/*	if ( onTheJob == itor->GetWorkStatus() )
		{*/
			if ( m_selUerId != itor->GetId())
			{
				pValidateData = m_pWorkHourList;
				CalWorkHourData( itor->GetId(), pValidateData );
				hasError = CheckWorkDataValidate( pValidateData, relust );

			}else
			{
				pValidateData = pData;
				hasError = CheckWorkDataValidate( pValidateData, relust );
				SetValidateColor( relust );
			}
			if ( hasError )
			{//send msg out put 
				CString errorInfo;
				errorInfo.Format(_T("用户'%s'工时有误"), itor->GetUserName_R() );
				ClientOutputManager::GetObj()->OutputUpdateMsg( check_msg_workHour, WorkHourView, itor->GetId(), errorInfo );
			}
		/*}*/
	}
	delete [] pData;
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg( );
}
void CWorkHourManger::OnExportAllWorkHour( int year, int month )
{//
	((CMainFrame *)AfxGetMainWnd())->ShowProgressMsg( MSG_READ_TASK_DATA );
	ListUser lst;
	m_pSeverFactory->GetLoginServer()->GetAllUserOrdyByExport( lst );
	int  dataRowCnt = m_timeTypeCnt + 1; //+1为合计
	int  dataColCnt = lst.size() + 1; //+1为合计
	int  userCnt = lst.size();
	CWorkHourData *pData = new CWorkHourData[dataRowCnt*dataColCnt];
	memset( pData, 0, sizeof(CWorkHourData)*dataRowCnt*dataColCnt );
	int  row = 0, col = 0; 
	TaskIdStringMapItor typeItor = m_workTypeList.begin();
	ListUserItor itor = lst.begin();
	COleDateTime dateBegin( year, month, 1, 0, 0, 0 );
	COleDateTimeSpan span( 1, 0, 0, 0 ); 
	int pos = 0;
	for ( ; row < m_timeTypeCnt; row++)
	{
		if ( typeItor != m_workTypeList.end() )
		{
			itor = lst.begin();
			for ( col = 0; col < dataColCnt - 1 ;col++, itor++ )
			{
				dateBegin.SetDate( year, month, 1 );
				if ( itor != lst.end() )
				{
					pData[pos].SetTaskWorkHour( 0.0 );
					while( dateBegin.GetMonth() == month )
					{
						pData[pos].SetDate( dateBegin );
						pData[pos].SetTimeType( typeItor->first );
						pData[pos].SetUserId( m_selUerId );
						pData[pos].SetNorTaskWorkHour( 0.0 );
						if ( typeItor->first >= 0 && typeItor->first < MAX_WORK_HOURTYPE )
						{//获取任务工时
							m_pWorkHourAction->GetWorkTimeData( typeItor->first, itor->GetId(), dateBegin, pData[pos] );
							double taskWorkHour = 0.0;
							if ( typeItor->first > 0 )
							{
								m_pWorkHourAction->GetTaskHourSum( itor->GetId(), typeItor->first, dateBegin, taskWorkHour );
							}
							//pData[pos].SetTaskWorkHour( taskWorkHour );
							double valueSum = taskWorkHour + pData[pos].GetNorTaskWorkHour();
							pData[pos].SetTaskWorkHour( valueSum + pData[pos].GetTaskWorkHour() );//将合计值填到任务工时
						}
						dateBegin += span;
					}
					double tempValue = 0.0;
					tempValue = pData[pos].GetTaskWorkHour() + pData[row*dataColCnt + dataColCnt - 1].GetTaskWorkHour();
					pData[row*dataColCnt + dataColCnt-1].SetTaskWorkHour( tempValue );
					tempValue = pData[pos].GetTaskWorkHour() + pData[m_timeTypeCnt*dataColCnt+col].GetTaskWorkHour();
					pData[m_timeTypeCnt*dataColCnt+col].SetTaskWorkHour( tempValue ) ;

					pos++;
				}else
				{
					break;
				}
				
			}
		    pos++; //跳过行总合
			typeItor++;
		}else
		{
			break;
		}
	}
	//
	CString fileName;
	fileName.Format( _T("研发体系%d年%d月工时表"), year, month );
	CFileDialog dlg(FALSE, _T(".xls"), fileName, OFN_CREATEPROMPT | OFN_PATHMUSTEXIST ,_T( "(*.xls)|*xls" ));  
	if( dlg.DoModal() == IDOK )
	{//
		CString path=dlg.GetPathName();
		CTaskExcelAction action( m_pSeverFactory );
		BOOL ret = action.ExportMonthWorkHour( path, pData, lst, m_workTypeList );
		if ( ret == userExcOk )
		{
			AfxMessageBox(_T("导出成功"));
		}else if ( ret != paraError )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) ); 
		}
	}
	//
	delete [] pData;
	((CMainFrame *)AfxGetMainWnd())->CloseMsgDlg();
}
void CWorkHourManger::OnSetDefHoliday( int year, int month )
{
	COleDateTime dateBegin;
	dateBegin.SetDate( year, month, 1 );
	if ( dateBegin.GetStatus() != COleDateTime::valid )
	{
		AfxMessageBox(_T("年月设置错误,请检查再重试"));
		return;
	}
	COleDateTimeSpan span( 1, 0, 0, 0 ); 
	while( dateBegin.GetMonth() == month )
	{
		if ( dateBegin.GetDayOfWeek() == 1 || dateBegin.GetDayOfWeek() == 7 )
		{
			m_pSystemCfgSever->AddHoliday( dateBegin );
		}
		dateBegin += span;
	}
	UpdateGridView();
	AfxMessageBox(_T("设置成功"));
}
