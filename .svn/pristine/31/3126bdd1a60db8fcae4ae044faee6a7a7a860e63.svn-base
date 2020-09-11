#include "stdafx.h"
#include "CExportPerformanceTable.h"
#include "Src/MainFrame/MainFrm.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"


CExportPerformanceTable::CExportPerformanceTable( CActionServerFactory *pFactory ):CTaskExcelActionInc( pFactory )
{
}
CExportPerformanceTable::~CExportPerformanceTable()
{
	m_books.ReleaseDispatch();
	m_book.ReleaseDispatch();
	m_sheets.ReleaseDispatch();
	m_sheet.ReleaseDispatch();
	m_range.ReleaseDispatch();
	m_font.ReleaseDispatch();
	m_appExcel.Quit();
	m_appExcel.ReleaseDispatch();
	::CoUninitialize();
}
/*****************************************************
函数名：ExportPerformanceFile
函数描述：按季度导出考核表
参数：year[输入] 导出绩效年
      quarter[输入] 导出第几个季度
	  departId[输入] 导出的部门Id，<= 0 全部
	  userId[输入]  导出的用户Id，<= 0 全部
返回：无
修改记录：
*****************************************************/
void CExportPerformanceTable::ExportPerformanFile(const CString &modeDirStr,  int year, int quarter, int departId, int userId )
{
	//参数有效性判断
	if ( year <= 2000 || quarter <= 0 )
	{
		AfxMessageBox( _T("选择导出的季度有误,请重新配置") );
		return;
	}
	//实时化组件
	if ( FALSE == InitExcelCo() )
	{
		return;
	}
	m_templetDirString = modeDirStr;
	//
	ListInt departIdArrary; 
	ListInt userIdArrary;
	GetExportArray(  departId,  userId,  departIdArrary, userIdArrary );
	//
	CString exportDir;
	CString exportDepartDir;
	CString fileDir;
	exportDir.Format(_T("D:\\%04d年第%d季度(%s)"), year, quarter, COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H%M%S")) );//"%Y-%m-%d %H%M%S"
	CreateDirectory( exportDir, NULL );
	UINT exportCnt = 0;
	CMainFrame *pMainFrame =((CMainFrame *)AfxGetMainWnd());
	pMainFrame->ShowProgressMsg( MSG_READ_TASK_DATA );
	for (ListIntItor itor = departIdArrary.begin(); itor != departIdArrary.end(); itor++ )
	{
		//创建部门文件夹
		CDepartment depart;
		if ( userExcOk == m_pFactory->GetDepartSever()->GetDepartById( *itor, depart ) &&  exportCnt < userIdArrary.size())
		{
			exportDepartDir.Format(_T("%s\\%s"), exportDir, depart.GetName() );
			CreateDirectory( exportDepartDir, NULL );
			//导出用户
			ListIntItor itorUser = userIdArrary.begin();
			advance( itorUser, exportCnt );
			CUser userObj;
			for (  ; itorUser != userIdArrary.end(); itorUser++ )
			{ 
				//绑定模板
				if ( FALSE == BindExportTemplet() )
				{
					pMainFrame->CloseMsgDlg();
					return;
				}
				if ( userExcOk == m_pFactory->GetLoginServer()->GetUserById( *itorUser, userObj ) )
				{
					fileDir.Format(_T("%s\\%s_%04d年第%d季度考核表.xls"),exportDepartDir, userObj.GetUserName_R(), year, quarter );
				}else
				{
					pMainFrame->CloseMsgDlg();
					AfxMessageBox(_T("获取用户信息失败,请重试"));
					return;
				}
				if( userObj.GetDepartId() != depart.GetId() )
				{
					break;
				}
				//
				if ( FALSE == ExportUserTask( year, quarter, *itorUser ) )
				{
					pMainFrame->CloseMsgDlg();
					return;
				}
				//保存文件
				m_book.SaveAs( _variant_t(fileDir), vtMissing, vtMissing, vtMissing, vtMissing,
					vtMissing, Excel::xlNoChange, vtMissing, vtMissing, vtMissing, vtMissing, vtMissing );
				m_book.Close( _variant_t(FALSE), vtMissing, vtMissing );
				m_books.Close();
				m_books.ReleaseDispatch();
				m_book.ReleaseDispatch();
				m_sheets.ReleaseDispatch();
				m_sheet.ReleaseDispatch();
				m_range.ReleaseDispatch();
				m_font.ReleaseDispatch(); 
				exportCnt++;
			}
		}else
		{
			AfxMessageBox(_T("获取部门信息失败,请重试"));
			pMainFrame->CloseMsgDlg();
			return;
		}
	}
	pMainFrame->CloseMsgDlg();
	AfxMessageBox(_T("导出完成"));
}
//初始化excel组件
BOOL CExportPerformanceTable::InitExcelCo()
{
	//初始化COM
	if(::CoInitialize(NULL) == E_INVALIDARG)
	{
		AfxMessageBox(_T("初始化COM失败！"));
		return FALSE;
	}

	//启动Excel
	if(!m_appExcel.CreateDispatch(_T("Excel.Application"), NULL))
	{
		AfxMessageBox(_T("创建Excel失败!"));
		::CoUninitialize();
		return FALSE;
	}

	//COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	m_appExcel.put_Visible(FALSE);   //使EXCEL表格不可见，以后台的形式完成报表的生成
	//m_appExcel.put_Visible(TRUE);  //EXCEL可见
	m_appExcel.put_DisplayAlerts( 0); //,  VARIANT_FALSE
	m_appExcel.put_AlertBeforeOverwriting( 0 );//, VARIANT_FALSE

	m_books.AttachDispatch(m_appExcel.get_Workbooks());
	//m_book.AttachDispatch(m_books.Add(covOptional));
	
	return TRUE;
}
BOOL CExportPerformanceTable::BindExportTemplet()
{
	//打开模板
	try
	{
		m_books.AttachDispatch(m_appExcel.get_Workbooks());
		m_book = m_books.Add( _variant_t(m_templetDirString) );
	}catch(CException *)
	{
		AfxMessageBox( _T("打开模板失败") );
		return FALSE;
	}
	//
	m_sheets.AttachDispatch(m_book.get_Worksheets());   //得到worksheets
	m_sheet = m_book.get_ActiveSheet();

	//m_interior.AttachDispatch(m_sheet.get_Interior()); //111111111
	//m_column.AttachDispatch(m_sheet.get_Range(_variant_t("A1"), _variant_t("A1")));

	return TRUE;
}
void CExportPerformanceTable::GetExportArray( int departId, int userId, ListInt &departIdArrary, ListInt &userIdArrary )
{
	if ( departId > 0 )
	{
		departIdArrary.push_back( departId );
	}else
	{
		ListDepart departList;
		m_pFactory->GetDepartSever()->GetAllDepart( departList );
		for ( ListDepartConstItor itor = departList.begin(); itor != departList.end(); itor++ )
		{
			departIdArrary.push_back( itor->GetId() );
		}
	}
	//
	if ( userId > 0 )
	{
		userIdArrary.push_back( userId );
	}else 
	{
		ListUser lstUserTmp;
		for ( ListIntItor itor = departIdArrary.begin(); itor != departIdArrary.end(); itor++ )
		{
			m_pFactory->GetDepartSever()->GetDepartUser( *itor, lstUserTmp, false );
			for (ListUserItor itorJ = lstUserTmp.begin(); itorJ != lstUserTmp.end(); itorJ++ )
			{
				userIdArrary.push_back( itorJ->GetId() );
			}
			lstUserTmp.clear();
		}
	}
}
BOOL CExportPerformanceTable::ExportUserTask( int year, int quarter,int userId )
{
	//查询项目任务
    try
    {
		ListTask lstTmp;
		CTaskSeverInc *pTaskSever = m_pFactory->GetTaskSever();
		if ( userExcOk !=  pTaskSever->GetPerformProjectTask(  year, quarter, userId, lstTmp ) )
		{
			AfxMessageBox(_T("查询项目任务失败") );
			return FALSE;
		}
		//
		CPosExcle curPosExcel;
		int lstCnt = lstTmp.size();
		if ( lstCnt > DEF_PRJ_NUM )
		{//插入空白行
			lstCnt -= DEF_PRJ_NUM;
			CString strCurStartPos = curPosExcel.GetPos(BEGIN_DATA_ROW+1, BEGIN_DATA_COL);
			// VARIANT copyOrgini;
			m_range = m_sheet.get_Range( COleVariant( strCurStartPos), COleVariant(strCurStartPos) );
			CRange rangeInsert = m_range.get_EntireRow();
			for ( int i = 0; i < lstCnt; i++ )
			{
				rangeInsert.Insert( _variant_t((long)-4121), vtMissing );
			}
			rangeInsert.ReleaseDispatch();
		}
		//
		if ( FALSE == WriteUserTask( year, quarter, 0, lstTmp ) )
		{
			AfxMessageBox(_T("写入项目任务失败"));
			return FALSE;
		}
		int lstPrjCnt = lstTmp.size();
		lstTmp.clear();
		if ( userExcOk !=  pTaskSever->GetPerformDepartTask(  year, quarter, userId, lstTmp ) )
		{
			AfxMessageBox(_T("查询项目任务失败") );
			return FALSE;
		}
		if ( lstPrjCnt < DEF_PRJ_NUM  )
		{
			lstPrjCnt = DEF_PRJ_NUM;
		}
		lstCnt = lstTmp.size();
		if ( lstCnt > DEF_DEPART_NUM )
		{
			lstCnt -= DEF_DEPART_NUM;
			CString strCurStartPos = curPosExcel.GetPos(BEGIN_DATA_ROW+1+lstPrjCnt+1, BEGIN_DATA_COL);
			m_range = m_sheet.get_Range( COleVariant( strCurStartPos), COleVariant(strCurStartPos) );
			CRange rangeInsert = m_range.get_EntireRow();
			for ( int i = 0; i < lstCnt; i++ )
			{
				rangeInsert.Insert( _variant_t((long)-4121), vtMissing );
			}
			rangeInsert.ReleaseDispatch();
		}
		if ( FALSE == WriteUserTask( year, quarter, lstPrjCnt+1, lstTmp ) )
		{
			AfxMessageBox(_T("写入部门任务失败"));
			return FALSE;
		}
		return TRUE;
    }
    catch (CException* )
    {
		AfxMessageBox(_T("写导出数据异常"));
		return FALSE;
    }
}
BOOL CExportPerformanceTable::WriteUserTask(int year, int quarter, int beginRow, ListTask &lstTask )
{
	int index = 1;
	int row = beginRow;
	int col = 0;
	CString tmpValueStr;
	CTaskSeverInc *pTaskSever = m_pFactory->GetTaskSever();
	const CTaskStaus stMap = pTaskSever->GetTaskStausMap();
	CProject tmpPrj;
	COleDateTime beginDate, endDate, tmpDate, tmpEndDate;
	COleDateTimeSpan span( 7, 0, 0, 0);
	COleDateTimeSpan span2( 7, 0, 0, 0);

	if ( FALSE == CUtilityTool::GetObj()->GetQuarterDateRange( year, quarter, beginDate, endDate ) )
	{
		AfxMessageBox(_T("获取季度时间范围失败"));
		return FALSE;
	}

	for ( ListTaskConstItor itor = lstTask.begin(); itor != lstTask.end(); itor++ )
	{
		//导出一个任务的考核信息
		col = 0;
		//第一列
		tmpValueStr.Format(_T("%d"), index );
		SetCellValue(  row, col++, tmpValueStr );
		//所属项目
		if ( itor->GetProjctId() > 0 )
		{
			if ( userExcOk == m_pFactory->GetProjectSever()->GetProjectById( itor->GetProjctId(), tmpPrj ) )
			{
				tmpValueStr = tmpPrj.GetShortName();
			}
		}else
		{
			tmpValueStr = itor->GetType();
		}
		SetCellValue( row, col++, tmpValueStr );
		//任务名称
		SetCellValue( row, col++, itor->GetName() );
		//任务状态
		stMap.GetStrStausById( itor->GetTaskStatus(), tmpValueStr);
		SetCellValue( row, col++, tmpValueStr);
		//计划开始时间
		SetCellValue( row, col++, itor->GetPanStartDateStr() );
		//计划结束时间
		SetCellValue( row, col++, itor->GetPanEndDateStr() );
		//计划工时
		tmpValueStr.Format( _T("%0.1f"), itor->GetPlanWorkHour() );
		SetCellValue( row, col++, tmpValueStr);
		//
		tmpDate = beginDate;
		double workHourSum = 0.0;
		while( tmpDate <= endDate )
		{
			int dayOfWeeck = tmpDate.GetDayOfWeek();
			tmpEndDate = tmpDate;
			if ( dayOfWeeck > 1 )
			{
				span.SetDateTimeSpan( 7 - dayOfWeeck + 1, 23, 59, 59 );
				span2.SetDateTimeSpan( 7 - dayOfWeeck + 2, 0, 0, 0 );
			}else
			{
				span.SetDateTimeSpan( 0, 23, 59, 59 );
				span2.SetDateTimeSpan( 1, 0, 0, 0 );
			}
			tmpEndDate += span; 
			double workHour = 0.0;
			CString temp = tmpEndDate.Format(_T("%Y-%m-%d"));
			CString temp2 = tmpDate.Format(_T("%Y-%m-%d"));
			if ( userExcOk != m_pFactory->GetScheduleSever()->GetTaskWorkHourSum( itor->GetId(), tmpDate, tmpEndDate, workHour) )
			{
				AfxMessageBox(_T("计算周工时出错"));
				return FALSE;
			}
			workHourSum += workHour;
			if ( workHour > 0.0 )
			{
				tmpValueStr.Format( _T("%0.1f"), workHour );
				SetCellValue( row, col, tmpValueStr);
			}
			col++;
			tmpDate += span2;
		}
		//时间开始日期
		SetCellValue( row, col++, itor->GetFactStartDateShowStr() );
		//实际结束日期
		SetCellValue( row, col++, itor->GetFactEndDateShowStr() );
		//本季度实际使用工时
		tmpValueStr.Format( _T("%0.1f"), workHourSum );
		SetCellValue( row, col++, tmpValueStr );
		//进度评分
		if ( itor->GetScore() > 0 )
		{
			tmpValueStr.Format(_T("%d"), itor->GetScore() );
			SetCellValue( row, col, tmpValueStr );
		}
		col++;
		//
		SetCellValue( row, col++, itor->GetScoreDec() );
		//变更
		ListChange lstCh;
		m_pFactory->GetChangeSever()->GetChangeByTaskId( itor->GetId(), lstCh );
		CString orgin, changeContent, changeDate, relsutStr;
		//CString tempStr;
		int cnt = 0; 
		for ( ListChangeConstItor chItor = lstCh.begin(); chItor != lstCh.end(); chItor++ )
		{
			orgin += chItor->GetChangeReason();
			changeContent += chItor->GetChangeContent();
			tmpValueStr = chItor->GetChangeDateTimeStr();
			changeDate += tmpValueStr;
			relsutStr += chItor->GetChangeStatusString( chItor->GetStatus(), true );
			if ( cnt < lstCh.size() - 1)
			{
				orgin += _T("\r\n...................");
				orgin += _T("\r\n");
				changeContent += _T("\r\n.....................................");
				changeContent += _T("\r\n");
				changeDate += _T("\r\n............");
				changeDate += _T("\r\n");
				relsutStr += _T("\r\n.............");
				relsutStr += _T("\r\n");

			}
			cnt++;
		}
		//变更原因
		SetCellValue( row, col++, orgin );
		//变更内容
		SetCellValue( row, col++, changeContent );
		//变更内容
		SetCellValue( row, col++, changeDate );
		//变更结果
		SetCellValue( row, col++, relsutStr );
		index++;
		row++;
	}
	return TRUE;
}
//设置行列数据表格内容
void  CExportPerformanceTable::SetCellValue( int row, int col, const CString &strValue )
{
	CPosExcle curPosExcel;
	row += BEGIN_DATA_ROW; //相对数据表格的位置
	col += BEGIN_DATA_COL;
	CString strCurStartPos = curPosExcel.GetPos(row, col);
	m_range = m_sheet.get_Range( COleVariant( strCurStartPos), COleVariant(strCurStartPos) );
	m_range.put_Item(_variant_t(1), _variant_t(1), _variant_t(strValue));
}