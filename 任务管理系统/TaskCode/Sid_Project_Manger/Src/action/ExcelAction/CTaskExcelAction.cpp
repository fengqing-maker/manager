#include "stdafx.h"
#include "CTaskExcelAction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CTaskExcelAction::CTaskExcelAction( CActionServerFactory *pFactory )
	:CTaskExcelActionInc( pFactory )
{
	m_pTaskSever = m_pFactory->GetTaskSever();
	m_pLoginActionInc = m_pFactory->GetLoginServer();
}
BOOL CTaskExcelAction::ValidateExcelTask( const CProject &prj,const CExcelTaskModel &excelObj, CTaskObj &obj ) 
{
	if ( excelObj.GetTaskName().IsEmpty() )
	{
		AfxMessageBox( _T("任务名不能为空") );
		return paraError;
	}
	obj.SetName( excelObj.GetTaskName() );
	//翻译工时
	CString infoStr;
	CString str = excelObj.GetWorkHourTypeStr();
	m_pTaskSever->GetWorkHourString();
	const CWorkTimeType& timeType = m_pTaskSever->GetWorkTypeObj();
	int workTimeType = timeType.FindWorkTimeIdByString( str ) ;
	if ( workTimeType < 0)
	{
		infoStr.Format( _T("任务'%s'字段[工时类型]有误,'%s'不存在此种类型工时"), excelObj.GetTaskName(), str );
		AfxMessageBox(infoStr);
		return paraError;
	}
	obj.SetWorkTypeId( workTimeType );
	//翻译任务状态
	int statusId = m_pTaskSever->GetTaskStausMap().FindStatusIdByString( excelObj.GetTaskStatus() );
	if ( statusId < 0 )
	{
		infoStr.Format(_T("任务'%s'字段[任务状态]有误,导入失败"), excelObj.GetTaskName() );
		AfxMessageBox( infoStr );
		return paraError;
	}
	obj.SetTaskStatus( statusId );
	//查找责任人
	CUser owner;
	m_pLoginActionInc->GetUserByName( excelObj.GetOwnerName(), owner );
	if ( owner.GetId() < 0 )
	{
		infoStr.Format(_T("任务'%s'字段[责任人]有误,系统无%s用户"), excelObj.GetTaskName(), excelObj.GetOwnerName() );
		AfxMessageBox( infoStr );
		return paraError; 
	}
	obj.SetOwnerId( owner.GetId() );
	//获取评审人
	CUser reviewObj;
	m_pLoginActionInc->GetUserLeader( owner, reviewObj );
	if ( m_pLoginActionInc->FindUserPower( reviewObj.GetId(), taskAgree))
	{
		obj.SetMakeScoreUserId( reviewObj.GetId() );
	}else
	{
		obj.SetMakeScoreUserId( m_pLoginActionInc->GetGernalMangerUser().GetId() );
	}
	if ( reviewObj.GetId() < 0 )
	{//
		infoStr.Format(_T("初始化%s的评审人失败"), excelObj.GetOwnerName() );
		AfxMessageBox(infoStr);
		return paraError;
	}
	//2015-6-3 获取项目阶段
	if ( prj.GetId() > 0 && excelObj.GetTaskAtrrbuteString().GetLength() > 0 )
	{
		CProjectStep stepObj;
		m_pFactory->GetSystemCfgSever()->GetProjectStep( prj.GetPrjStepType() ,excelObj.GetTaskAtrrbuteString(), stepObj );
		if ( stepObj.GetId() < 0 )
		{
			infoStr.Format(_T("任务'%s'字段[任务所属阶段]未找到为此名‘%s’的阶段"),
				obj.GetName(), excelObj.GetTaskAtrrbuteString() );
			AfxMessageBox( infoStr );
			return FALSE;
		}
		obj.SetAttribute( stepObj.GetId() );
	}
	//

	//难度系数
	double difPara = excelObj.GetDifficultyParam();
	if ( ( newTasking != statusId )  )
	{
		obj.SetPublishTime( COleDateTime::GetCurrentTime() );
		
		if ( difPara < 1.0 || difPara > 3.0 )
		{
			infoStr.Format(_T("任务'%s'字段[难度系数]有误,%0.1f不在范围【1.0~3.0】"), excelObj.GetTaskName(), difPara );
		}else if( excelObj.GetPlanStartTime().m_status == COleDateTime::invalid )
		{
			infoStr.Format( _T("任务'%s'字段[计划开始]是无效日期"), excelObj.GetTaskName() );
		}else if ( excelObj.GetPlanEndTime().m_status == COleDateTime::invalid )
		{
			infoStr.Format( _T("任务'%s'字段[计划结束]是无效日期"), excelObj.GetTaskName() );
		}else if ( excelObj.GetPlanEndTime() < excelObj.GetPlanStartTime() )
		{
			infoStr.Format( _T("任务'%s'字段[计划结束]%s比[计划开始]%s早"), excelObj.GetTaskName(),
				excelObj.GetPlanEndTime(), excelObj.GetPlanStartTime() );
		}
		double workHour = excelObj.GetPlanWorkHour();
		if ( workHour <= 0.0 )
		{
			infoStr.Format( _T("任务'%s'字段[计划工时]有误,%0.1f小于等于0"), excelObj.GetTaskName(), workHour );
		}
		if ( infoStr.GetLength() > 0 ) 
		{
			AfxMessageBox( infoStr );
			return paraError; 
		}
	}
	obj.SetDifficultyParam( difPara );
	obj.SetPlanStartTime( excelObj.GetPlanStartTime() );
	obj.SetPlanEndTime( excelObj.GetPlanEndTime() );
	obj.SetPlanWorkHour( excelObj.GetPlanWorkHour() );
 
	int score = excelObj.GetScore();
	if ( (task_close == statusId) && score <= 0 )
	{
		infoStr.Format( _T("任务'%s'字段[难度系数]有误,分数%d必须大于0"), excelObj.GetTaskName(), score );
		AfxMessageBox( infoStr );
		return paraError; 
	}
	obj.SetScore( score );
	if ( task_close == statusId || waitScore == statusId )
	{//判断计划完成日期
		if ( excelObj.GetFactStartTime().m_status == COleDateTime::invalid )
		{
			infoStr.Format( _T("任务'%s'字段[实际开始]是无效日期"), excelObj.GetTaskName() );
		}
		if ( excelObj.GetFactEndTime().m_status == COleDateTime::invalid )
		{
			infoStr.Format( _T("任务'%s'字段[实际结束]是无效日期"), excelObj.GetTaskName() );
		}
		if ( excelObj.GetFactEndTime() < excelObj.GetFactStartTime() )
		{
			infoStr.Format( _T("任务'%s'字段[实际结束]%s比[实际开始]早"), excelObj.GetTaskName(),
				excelObj.GetFactEndTime(), excelObj.GetFactStartTime() );
		}
		double workHour = excelObj.GetFactWorkHour();
		if ( workHour <= 0.0 )
		{
			infoStr.Format( _T("任务'%s'字段[实际工时]有误,%0.1f小于等于0"), excelObj.GetTaskName(), workHour );
		}
		if ( infoStr.GetLength() > 0)
		{
			AfxMessageBox( infoStr );
			return paraError; 
		}
	}
	if ( excelObj.GetNum() > 0 &&  excelObj.GetFatherNum() > 0 &&  excelObj.GetFatherNum() == excelObj.GetNum() )
	{
		infoStr.Format(_T("任务'%s',父任务不能为任务本身"), excelObj.GetTaskName() ); 
		AfxMessageBox( infoStr );
		return paraError;
	}
	obj.SetFactStartTime( excelObj.GetFactStartTime() );
	obj.SetFactEndTime( excelObj.GetFactEndTime() );
	obj.SetFactWorkHour( excelObj.GetWorkHours() );
	obj.SetDec( excelObj.GetDec());

	return userExcOk;
}
BOOL CTaskExcelAction::UpdateTask( const CTaskObj &obj )
{
	TaskIdStringMap filds;
	CString value;
	value.Format( _T("%d"), obj.GetTaskStatus() );
	filds.insert( std::make_pair( taskStatus,  value ) );
	value.Format( _T("%d"), obj.GetOwnerId() );
	filds.insert( std::make_pair( fild_task_ownerid,  value ) );
	value.Format( _T("%d"), obj.GetScore() );
	filds.insert( std::make_pair( fild_task_score,  value ) );
	value.Format( _T("'%s'"), obj.GetScoreTimeStr() );
	filds.insert( std::make_pair( fild_task_score_date,  value ) );
	value.Format( _T("%0.1f"), obj.GetDifficultyParam() );
	filds.insert( std::make_pair( taskDiffParam,  value ) );

	//
	if ( obj.GetPlanStartTime().m_status == COleDateTime::valid )
	{
		value.Format( _T("'%s'"), obj.GetPanStartDateStr() );
		filds.insert( std::make_pair( fild_task_plan_starttime,  value ) );
	}
	if ( obj.GetPlanEndTime().m_status == COleDateTime::valid )
	{
		value.Format( _T("'%s'"), obj.GetPanEndDateStr() );
		filds.insert( std::make_pair( fild_task_plan_closetime,  value ) );
	}
	value.Format( _T("%0.1f"), obj.GetPlanWorkHour() );
	filds.insert( std::make_pair( fild_task_plan_period,  value ) );
	//
	if ( obj.GetFactStartTime().m_status == COleDateTime::valid )
	{
		value.Format( _T("'%s'"), obj.GetFactStartDateStr() );
		filds.insert( std::make_pair( fild_task_ac_starttime,  value ) );
		//value.Format( _T("'%s'"), obj.GetCheckBeginDateStr() );
		//filds.insert( std::make_pair( fild_task_check_begin,  value ) );

	}
	if ( obj.GetFactEndTime().m_status == COleDateTime::valid )
	{
		value.Format( _T("'%s'"), obj.GetFactEndDateStr() );
		filds.insert( std::make_pair( fild_task_ac_closetime,  value ) );
	}
	value.Format( _T("%0.1f"), obj.GetFactWorkHour() );
	filds.insert( std::make_pair( fild_task_ac_period,  value ) );
	//
	value.Format( _T("'%s'"), obj.GetDec() );
	filds.insert( std::make_pair( taskDec,  value ) );
	//
	value.Format( _T("%d"), obj.GetWorkTypeId() );
	filds.insert( std::make_pair( fild_task_timeType_id,  value ) );
	//评分人
	value.Format( _T("%d"), obj.GetMakeScoreUserId() );
	filds.insert( std::make_pair( fild_task_auditorid, value) );
	//属性
	value.Format( _T("%d"), obj.GetAttribute() );
	filds.insert( std::make_pair( file_task_attri, value) );

	return m_pFactory->GetDaoFactory().GetTaskDbDao()->UpdateTaskField( obj.GetId(), filds );

}
BOOL CTaskExcelAction::ImportTask(const CString &dirStr, const CProject &prj )
{
	if ( FALSE == m_pExcelTaskDao->OpenConn( dirStr ) )
	{
	   return connect_error;
	} 
	//
	BOOL ret;
	ListExcelTaskMode modeList; 
	ret = m_pExcelTaskDao->GetExcelTask( modeList );
	m_pExcelTaskDao->CloseConn();
	if ( ret != userExcOk )
	{
		return ret;
	}
	for (ListExcelTaskMode::iterator itor = modeList.begin(); itor != modeList.end(); itor++ )
	{
		CTaskObj obj;
		obj.SetProjctId( prj.GetId() );
        if ( prj.GetId() > 0 )
        {
			obj.SetType( TASKTYPE_PRJSTR );
        }else
		{
			obj.SetType( TASKTYPE_DEPARTSTR );
		}
		ret = ValidateExcelTask( prj, *itor, obj );
		if ( ret != userExcOk )
		{
			return ret;
		}
		//插入任务
		obj.SetCreateTime( COleDateTime::GetCurrentTime() );
		obj.SetScoreTime( COleDateTime::GetCurrentTime() );
		obj.SetCheckBeginDate( COleDateTime::GetCurrentTime()  );
		obj.SetCreateId( m_pLoginActionInc->GetLoginUser().GetId() ); 
		//
		ret = m_pTaskSever->InsertTask( obj );
		BOOL isUpdate = FALSE;
		if ( ret == insertNameRepeat && obj.GetId() > 0 )
		{//修改记录
              ret = UpdateTask( obj );
			  isUpdate = TRUE;
		}	
		if ( ret < 0  )
		{
			return ret;
		}
		itor->SetRelateTaskId( obj.GetId() );
		//更新进度
		ret = UpdateSchude( *itor, isUpdate );
        if ( ret != userExcOk )
        {
			return ret;
        }
	}
	UpdateFatherRelate( modeList );
	return userExcOk;
}
BOOL CTaskExcelAction::UpdateFatherRelate( ListExcelTaskMode &lst )
{
	//更新父级关系表
	CTaskDaoInc *pDbTask = m_pFactory->GetDaoFactory().GetTaskDbDao();
	CString temp;
	for (ListExcelTaskMode::iterator itor = lst.begin(); itor != lst.end(); itor++ )
	{
		int fatherNum = itor->GetFatherNum();
		if ( fatherNum <= 0 )
		{
			continue;
		}
		for (ListExcelTaskMode::iterator itor2 = lst.begin(); itor2 != lst.end(); itor2++ )
		{
			if ( itor2->GetNum() == fatherNum && itor2->GetRelateTaskId() > 0 )
			{
				//出入父级表
			    pDbTask->UpdateTaskFather( itor->GetRelateTaskId(), itor2->GetRelateTaskId() );
				break;
			}
		}
	}
	return TRUE;
}

//更新进度,isUpdateTask:任务的添加是否为更新的方式
BOOL CTaskExcelAction::UpdateSchude( const CExcelTaskModel &excelObj, BOOL isUpdateTask )
{
	int tempValue = excelObj.GetPercent();
	if ( tempValue == 0 )
	{
		return userExcOk;
	}
	CString str;
	if ( tempValue > 100 || tempValue < 0 )
	{
		str.Format( _T("任务'%s'字段[最新进度]%d有误"),excelObj.GetTaskName(), tempValue ); 
		AfxMessageBox(str);
	    return paraError;
	}
	double tempFloat = excelObj.GetWorkHours();
	if ( tempFloat < 0 || tempFloat > excelObj.GetPlanWorkHour() )
	{
		str.Format( _T("任务'%s'字段[最新进度工时],应在[0 ~ %0.1f]范围内"), excelObj.GetTaskName(), excelObj.GetPlanWorkHour() );
		AfxMessageBox(str);
		return paraError;
	}
	CTaskDaoInc *pDbTask = m_pFactory->GetDaoFactory().GetTaskDbDao();
	CSchedule schObj;
	schObj.SetTaskId( excelObj.GetRelateTaskId() );
	schObj.SetPercent( excelObj.GetPercent() );
	if ( isUpdateTask )
	{
		pDbTask->GetScheduleByPer( excelObj.GetRelateTaskId(), excelObj.GetPercent(), schObj );
	}
	schObj.SetWorkHours( excelObj.GetWorkHours() );
	schObj.SetUpdateContent( excelObj.GetUpdateContent() );
	schObj.SetUpdateTime(  COleDateTime::GetCurrentTime() );
	BOOL ret;
	if ( schObj.GetId() > 0 )
	{//更新
		TaskIdStringMap filds;
		CString value;
		value.Format( _T("%d"), schObj.GetTaskId() );
		filds.insert( std::make_pair( fild_task_id,  value ) );
		value.Format( _T("'%s'"), schObj.GetUpdateContent() );
		filds.insert( std::make_pair( fild_schedule_des,  value ) );
		value.Format( _T("'%s'"), schObj.GetUpdateTimeStr() );
		filds.insert( std::make_pair( fild_update_time,  value ) );
		value.Format( _T("%d"), schObj.GetPercent() );
		filds.insert( std::make_pair( fild_schedule_percent,  value ) );
		value.Format( _T("%0.1f"), schObj.GetWorkHours() );
		filds.insert( std::make_pair( fild_schedule_tim,  value ) );
		ret = pDbTask->UpdateSchedule(schObj.GetId(), filds );
	}else
	{
		ret = pDbTask->AddTaskSchedule( schObj );
	}
	return ret;
}
BOOL CTaskExcelAction::OpenExportDir( const CString &dirStr )
{
	CFileFind fileFinder;
	if(fileFinder.FindFile(dirStr))
	{
		if(!DeleteFile(dirStr))
		{
			CString strDeleteFailed;
			strDeleteFailed.Format(_T("%s文件正在被别的程序所使用，无法替换它！"), dirStr);
			AfxMessageBox(strDeleteFailed);
			return paraError;
		}
	}
	if ( !m_pExcelTaskDao->OpenConn( dirStr ) )
	{
		return paraError;
	}
	return userExcOk;
}
BOOL  CTaskExcelAction::ExportTask( const CString &dirStr, const CString &tabName ,const ListTask &lst )
{
	BOOL ret = OpenExportDir( dirStr );
	if ( userExcOk != ret )
	{
		return ret;
	}
   ListExcelTaskMode exportLst;
   CTaskStaus stausObj = m_pTaskSever->GetTaskStausMap();
   m_pTaskSever->GetWorkHourString();
   const CWorkTimeType& timeType = m_pTaskSever->GetWorkTypeObj();
   CScheduleActionInc* pSchAction = m_pFactory->GetScheduleSever();
   int cnt = 0;
   for ( ListTask::const_iterator itor=lst.begin(); itor != lst.end(); itor++ )
   {
	   CExcelTaskModel model;
	   model.SetNum( cnt++ );
	   model.FillContentFromTask( *itor );
	   //状态
	   CString temp;
	   stausObj.GetStrStausById( itor->GetTaskStatus(), temp );
	   model.SetTaskStatus( temp );
	   //责任人
	   CUser owner;
	   m_pLoginActionInc->GetUserById( itor->GetOwnerId(), owner );
	   model.SetOwnerName( owner.GetUserName_R() );
	   //工期
	   CString workTypeStr;
	   timeType.GetWorkTimeTypeById( itor->GetWorkTypeId(), workTypeStr );
	   model.SetWorkHourTypeStr( workTypeStr );
	   //类型
	   model.SetTaskTypeString( itor->GetType() );
	   if ( itor->GetType() == TASKTYPE_PRJSTR && itor->GetProjctId() > 0 )
	   {
		   CProject tempPj;
		   m_pFactory->GetProjectSever()->GetProjectById( itor->GetProjctId(), tempPj );
		   model.SetTaskTypeString( tempPj.GetShortName() );
	   }
	   //获取最新进度
	   CSchedule schObj;
	   if( userExcOk == pSchAction->GetMaxNewScheduleByTaskId( itor->GetId(), schObj ) )
	   {
		   model.FillContentFromSchedul( schObj ); 
	   }
	   exportLst.push_back( model );
   }
   //导出
   ret = m_pExcelTaskDao->ExportTask ( tabName, exportLst );
   m_pExcelTaskDao->CloseConn();
   return ret;
}
BOOL   CTaskExcelAction::ExportWorkHour( const CString &dirStr, COleDateTime &beginTime, COleDateTime &endTime )
{
	BOOL ret = OpenExportDir( dirStr );
	if ( userExcOk != ret )
	{
		return ret;
	}
	//获取进度根据更新时间
	ListSchedule schList;
	ret = m_pTaskSever->GetScheduleContentByDate( m_pLoginActionInc->GetLoginUser().GetId(), beginTime, endTime, schList );
	if ( ret != userExcOk )
	{
		return ret;
	}
	WorkHourList datas;
	const TaskIdStringMap workHourMap = m_pTaskSever->GetWorkHourString();
	for ( TaskIdStringMapConstItor itor = workHourMap.begin(); itor != workHourMap.end(); itor++ )
	{
		WorkHourInfo info;
		info.workId = itor->first;
		info.workHourType = itor->second;
		GetWorkHourSchduleInfo( itor->first, schList, info );
		datas.push_back( info );
	}
	//导出
	CString tabName;
	tabName.Format(_T("%s_%s"), m_pLoginActionInc->GetLoginUser().GetUserName_R(), 
		                  m_pLoginActionInc->GetLoginDepart().GetName() );
	ret = m_pExcelTaskDao->ExportWorkHour ( tabName, beginTime, endTime, datas );
	m_pExcelTaskDao->CloseConn();
	return ret;
}

void  CTaskExcelAction::GetWorkHourSchduleInfo( int workId, const ListSchedule &lst, WorkHourInfo &info )
{
	for (ListScheduleConstItor itor = lst.begin(); itor != lst.end(); itor++ )
	{
		CTaskObj obj;
		m_pTaskSever->GetTaskByTaskId( itor->GetTaskId(), obj );
		if ( obj.GetId() > 0 && obj.GetWorkTypeId() == workId )
		{
			info.totalWorkHour += itor->GetWorkHours();
			CString str;
			str.Format(_T("任务[%d], 工时%0.1f 更新内容：%s 日期%s\r\n"), obj.GetId(), itor->GetWorkHours(),itor->GetUpdateContent(), itor->GetUpdateTimeStr() );
			info.schudulInfo += str;
		}
	}
	
}
BOOL CTaskExcelAction::ExportMonthWorkHour( const CString &dirStr, CWorkHourData *pData, const ListUser& lstUser, const TaskIdStringMap &typeMap )
{
	BOOL ret = OpenExportDir( dirStr );
	if ( userExcOk != ret )
	{
		return ret;
	}
	//导出
	CString tabName(_T("工时"));
	ret = m_pExcelTaskDao->ExportWorkHour ( tabName, pData, lstUser, typeMap );
	m_pExcelTaskDao->CloseConn();
	return ret;
}