#include "stdafx.h"
#include "changeActionImp.h"
#include "Src/MainFrame/Dlg/StopTaskDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

ChangeActionImp::ChangeActionImp(  CDaoFactory *pFactory )
	:CChangeActionInc( pFactory )
{
	m_pChangeDbDao = pFactory->GetChangeDbDao();
	m_pTaskDbDao = pFactory->GetTaskDbDao();
}

//
BOOL ChangeActionImp::InsertChange( const CTaskChange &obj)
{
	return m_pChangeDbDao->InsertChangeItem( obj );
}
BOOL  ChangeActionImp::GetChangeById( int id, CTaskChange &obj )
{
	return m_pChangeDbDao->GetChangeItem( id, obj );
}
//变更不同意，传递
BOOL  ChangeActionImp::ChangeUnAgree( const CTaskChange &chObj )
{
	CTaskObj taskBakObj;
	BOOL ret = m_pTaskDbDao->GetTaskContentById( chObj.GetBakTaskId(),taskBakObj);
	if ( userExcOk != ret )
	{
		return ret;
	}
	TaskIdStringMap fields;
	CString valueStr;
	valueStr.Format(_T("%d"), taskBakObj.GetTaskStatus() );
	fields.insert( std::make_pair( taskStatus, valueStr) );
	ret = m_pTaskDbDao->UpdateTaskField( chObj.GetTaskId() , fields );
	//if ( userExcOk == ret )
	//{
	//	m_pTaskDbDao->DeleteTaskById( taskBakObj.GetId() );
	//}
	return ret;
}
BOOL  ChangeActionImp::ChangeAgree( const CTaskChange &chObj, int *pScore /*= NULL*/ )
{
	TaskIdStringMap fields;
	CString valueStr;
	BOOL ret;
	int type = chObj.GetType() ;
	CTaskObj taskBakObj;
	ret = m_pTaskDbDao->GetTaskContentById( chObj.GetBakTaskId(),taskBakObj);
	if ( userExcOk != ret )
	{
		return ret;
	}
	if ( departAgree == chObj.GetStatus() )
	{
		if ( changeStop == type )
		{
			CStopTaskDlg dlg( taskBakObj);
			if (IDOK != dlg.DoModal() )
			{
				return paraError;
			}
			//设置任务评分
			TaskIdStringMap fieldBak;
			valueStr.Format(_T("%d"), taskBakObj.GetScore() );
			fieldBak.insert( std::make_pair(fild_task_score,valueStr));
			valueStr.Format(_T("'%s'"), taskBakObj.GetScoreTimeStr() );
			fieldBak.insert( std::make_pair(fild_task_score_date, valueStr) );
			valueStr.Format( _T("'%s'"), taskBakObj.GetFactEndDateStr() );
			fieldBak.insert( std::make_pair(taskFactEndTime, valueStr) );
			ret =  m_pTaskDbDao->UpdateTaskField( taskBakObj.GetId(),fieldBak );
			if ( pScore )
			{
				*pScore = taskBakObj.GetScore();
			}
		}
		fields.clear();
		valueStr.Format(_T("%d"), changeWait2 );
		fields.insert( std::make_pair(taskStatus, valueStr) );
		ret =  m_pTaskDbDao->UpdateTaskField( chObj.GetTaskId(),fields );  
	}else if ( genernalAgree == chObj.GetStatus() )
	{
		CTaskObj taskOrginObj;
		ret = m_pTaskDbDao->GetTaskContentById( chObj.GetTaskId(),taskOrginObj);
		if ( userExcOk != ret )
		{
			return ret;
		}

		//追加描述
		valueStr.Format(_T("'%s\r\n任务追加描述:%s'"),taskOrginObj.GetDec(), taskBakObj.GetDec() );
		fields.insert( std::make_pair(taskDec, valueStr) );

		if ( changeTime == type ||  changeReStart == type)
		{//修改时间/恢复挂起
			valueStr.Format(_T("%0.1f"), taskBakObj.GetPlanWorkHour() );
			fields.insert( std::make_pair(fild_task_plan_period, valueStr) );
			valueStr.Format(_T("'%s'"), taskBakObj.GetPanEndDateStr() );
			fields.insert( std::make_pair(fild_task_plan_closetime, valueStr)); 
			valueStr.Format(_T("%d"), taskBakObj.GetTaskStatus() );
 			fields.insert( std::make_pair(taskStatus, valueStr) );
			COleDateTime date = COleDateTime::GetCurrentTime();
			COleDateTimeSpan span( 1, 0, 0, 0 );
			date -= span; //
			valueStr.Format( _T("'%s'"), date.Format(_T("%Y-%m-%d")) );
			fields.insert( std::make_pair( fild_task_check_begin, valueStr) );
			
			if (  taskBakObj.GetTaskStatus() == ongoing)
			{//2015-7-13 add 
				double relust = 0.0f;
				m_pTaskDbDao->GetTaskWorkHourSum( taskBakObj.GetOwnerId(), -1, date, relust );
				if ( relust > 0 )
				{
					m_pTaskDbDao->AddEmptySchedule( chObj.GetTaskId(), date );
				}
			}
			
		}else if ( changeSupsend == type )
		{//挂起
			valueStr.Format(_T("%d"), task_suspend );
			fields.insert( std::make_pair(taskStatus, valueStr) );
			
		}else if ( changeStop == type )
		{//任务中止
			valueStr.Format(_T("%d"), midStop );
			fields.insert( std::make_pair(taskStatus, valueStr) );
			//评分
			valueStr.Format(_T("%d"), taskBakObj.GetScore() );
			fields.insert( std::make_pair(fild_task_score,valueStr));
			valueStr.Format(_T("'%s'"), taskBakObj.GetScoreTimeStr() );
			fields.insert( std::make_pair(fild_task_score_date, valueStr) );
			valueStr.Format( _T("'%s'"), taskBakObj.GetFactEndDateStr() );
			fields.insert( std::make_pair(taskFactEndTime, valueStr) );
		}
		ret = m_pTaskDbDao->UpdateTaskField( chObj.GetTaskId(), fields );
		if ( changeStop == type  && userExcOk == ret )
		{//删除为空的进度
			m_pTaskDbDao->DeleteNoneShcdule( taskBakObj.GetId() );
		}
		
	}
	return ret;
}
//CTaskObj:
BOOL ChangeActionImp::ChangeDepartReview(BOOL isAgree,const CString &reviewName,/*const CTaskObj &taskObj ,*/const CTaskChange &chObj )
{//部门评审
	ReviewContent obj;
	obj.m_id = chObj.GetId();
	obj.m_reviewTime = COleDateTime::GetCurrentTime();
	BOOL ret ;
	if ( isAgree )
	{
		obj.m_reviewContent.Format(_T("%s\r\n审核人意见：%s %s同意任务变更"), chObj.GetReviewContent(), reviewName, obj.m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S")) );
		//2015-10-27 add 修改变更流程
		int st = departAgree;
		//
		obj.m_result = ( st );
		CTaskChange tempObj = chObj;
		tempObj.SetStatus( st );
		int  Score = 0;
		ret = ChangeAgree( tempObj, &Score  );
		if ( changeStop == tempObj.GetType() )
		{
			CString str;
			str.Format( _T(",终止任务评分为%d"), Score );
			obj.m_reviewContent += str;
		}
		if ( changeNormalDepartTask == chObj.GetChangeTaskType() 
			|| (changeNormalPrjTask == chObj.GetChangeTaskType() && 1 != chObj.GetImpactSchudle() ) 
			|| ( chObj.GetAuditorId() == chObj.GetGeneralAuditorId() && changePrjPhase != chObj.GetChangeTaskType() ) )
		{//2015-10-27 add 直接走副总同意流程
			tempObj.SetStatus( genernalAgree );
			ret = ChangeAgree( tempObj, NULL  );
		}
	}else
	{
		obj.m_reviewContent.Format(_T("审核人意见：%s\r\n%s %s不同意任务变更"), chObj.GetReviewContent(), reviewName, obj.m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S")) );
	   	obj.m_result = (departUnAgree );
		ret = ChangeUnAgree( chObj  );
	}
	if ( userExcOk == ret )
	{
		ret = m_pChangeDbDao->UpdateFirstReview( obj );
	}
	return ret;
}
BOOL ChangeActionImp::ChangeGernalReview(BOOL isAgree,const CString &reviewName,/*const CTaskObj &taskObj, */const CTaskChange &chObj )
{//总经理点评
	ReviewContent obj;
	obj.m_id = chObj.GetId();
	obj.m_reviewTime = COleDateTime::GetCurrentTime();
	BOOL ret;
	if ( isAgree )
	{
		obj.m_reviewContent.Format(_T("%s\r\n副总意见：%s %s同意任务变更"), chObj.GetGeneralReviewContent(), reviewName, obj.m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S")) );
		obj.m_result = ( genernalAgree );
		CTaskChange tempObj = chObj;
		tempObj.SetStatus( obj.m_result );
		ret = ChangeAgree( tempObj  );
	}else
	{
		obj.m_reviewContent.Format(_T("%s\r\n副总意见：%s %s不同意任务变更"), chObj.GetGeneralReviewContent(), reviewName, obj.m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S")) );
		obj.m_result = (genernalUnAgree );
		ret = ChangeUnAgree( chObj  );
	}
	if ( userExcOk == ret )
	{
		ret = m_pChangeDbDao->UpdateSecondReview( obj );
	}
	return ret;
}
BOOL ChangeActionImp::ChangePrjectReview(const CString &reviewName,/*const CTaskObj &taskObj, */const CTaskChange &chObj )
{//总经理点评
	if ( chObj.GetProjectReview().GetLength() <= 0 )
	{
		AfxMessageBox( _T("点评意见不能为空"));
		return paraError;
	}
	CString str;
	COleDateTime reviewTime = COleDateTime::GetCurrentTime();
	str.Format(_T("%s\r\n项目经理:%s %s点评"), chObj.GetProjectReview(), reviewName, reviewTime.Format( _T("%Y-%m-%d %H:%M:%S")) );
	int nextSt;
	int nextTaskSt;
	BOOL ret;
	if ( changeNormalPrjTask == chObj.GetChangeTaskType() 
		   || ( changePrjPhase == chObj.GetChangeTaskType() && changeStop == chObj.GetType()) )
	{
		nextSt = up_apper;
		nextTaskSt = changeWait1;
	}else if ( changePrjPhase == chObj.GetChangeTaskType() && changeStop != chObj.GetType() )
	{
		nextSt = departAgree;
		nextTaskSt = changeWait2;
		CTaskChange chObjTmp = chObj;
		chObjTmp.SetStatus( nextSt );
		if ( userExcOk != (ret = ChangeAgree( chObjTmp )) )
		{
			return ret;
		}
	}else
	{
		AfxMessageBox( _T("变更状态不正确") );
		return paraError;
	}

	TaskIdStringMap fileds;
	CString valueStr;
	valueStr.Format(_T("%d"), chObj.GetImpactSchudle() );
	fileds.insert( std::make_pair(fild_is_impact_schdule, valueStr));
	valueStr.Format(_T("'%s'"), str );
	fileds.insert( std::make_pair(fild_project_review, valueStr));
	valueStr.Format(_T("%d"), nextSt );
	fileds.insert( std::make_pair(fild_status, valueStr));
	ret = m_pChangeDbDao->UpdateChange( chObj.GetId(), fileds );
	if ( userExcOk != ret )
	{
		return ret;
	}
	//修改任务状态
	TaskIdStringMap filedTask;
	CString tmpStr;
	tmpStr.Format(_T("%d"), nextTaskSt );
	filedTask.insert( std::make_pair(taskStatus, tmpStr) );
	if ( userExcOk != (ret = m_pTaskDbDao->UpdateTaskField( chObj.GetTaskId(),filedTask )) )
	{
		return ret;
	}
	return ret;
}
//
BOOL ChangeActionImp::SaveChange(  CTaskChange &chObj,  CTaskObj &bakObj, BOOL isApply )
{
	BOOL ret;
	TaskIdStringMap fields;
	CString valueStr;
	int type = chObj.GetChangeTaskType();
	if ( changeNormalDepartTask == type )
	{
		chObj.SetStatus(  up_apper );
	}else
	{
		chObj.SetStatus(  prj_remark_on );
	}
	if ( bakObj.GetId() <= 0 )
	{//
		ret = m_pTaskDbDao->InsertTask( bakObj );
		if ( userExcOk != ret || userExcOk != m_pTaskDbDao->GetLastInsertId(ret)  )
		{
			return ret;
		}
		bakObj.SetId( ret );
		chObj.SetBakTaskId( ret );
	}else
	{//任务更新
		fields.clear();
		valueStr.Format(_T("%0.1f"), bakObj.GetPlanWorkHour() );
		fields.insert( std::make_pair(fild_task_plan_period, valueStr) );
		valueStr.Format(_T("'%s'"), bakObj.GetPanEndDateStr() );
		fields.insert( std::make_pair(fild_task_plan_closetime, valueStr)); 
		valueStr.Format( _T("'%s'"), bakObj.GetDec() );
		fields.insert( std::make_pair(taskDec, valueStr));
		valueStr.Format( _T("%d"), bakObj.GetTaskStatus() );
		fields.insert( std::make_pair(taskStatus, valueStr));
		ret = m_pTaskDbDao->UpdateTaskField( chObj.GetBakTaskId(), fields );

		if ( userExcOk != ret )
		{
			return ret;
		}
	}
	if ( chObj.GetId() > 0)
	{//更新
		fields.clear();
		valueStr.Format( _T("'%s'"),  chObj.GetChangeContent() );
		fields.insert( std::make_pair(fild_change_content, valueStr)); 
		valueStr.Format( _T("'%s'"),  chObj.GetChangeDateTimeStr() );
		fields.insert( std::make_pair(fild_change_time, valueStr));
		valueStr.Format( _T("'%s'"),  chObj.GetChangeReason() );
		fields.insert( std::make_pair(fild_change_reason, valueStr));
		valueStr.Format(_T("%d"), chObj.GetType());
        fields.insert( std::make_pair(fild_change_item, valueStr));
		valueStr.Format(_T("%d"), chObj.GetAuditorId());
		fields.insert( std::make_pair(fild_auditor_id, valueStr));
		valueStr.Format(_T("%d"), chObj.GetGeneralAuditorId());
		fields.insert( std::make_pair(fild_general_manager_id, valueStr));
	    valueStr.Format(_T("%d"), chObj.GetChangeTaskType() );
		fields.insert( std::make_pair(fild_task_type, valueStr));
		valueStr.Format(_T("%d"), chObj.GetChangeResonOption() );
		fields.insert( std::make_pair(fild_change_reason_option, valueStr));

		if ( chObj.GetBakTaskId() != bakObj.GetId() )
		{
			valueStr.Format(_T("%d"), bakObj.GetId());
			fields.insert( std::make_pair(fild_change_bakId, valueStr));
		}
		if ( isApply )
		{
			valueStr.Format(_T("%d"), chObj.GetStatus());
			fields.insert( std::make_pair(fild_status, valueStr));
		}
		ret = m_pChangeDbDao->UpdateChange( chObj.GetId(), fields );
	}else
	{//插入
		ret = m_pChangeDbDao->InsertChangeItem( chObj );
		if ( userExcOk == ret && ( m_pTaskDbDao->GetLastInsertId(ret) > 0 ) )
		{
			return ret;
		}
		chObj.SetId( ret );
		ret = userExcOk;
	}
	if ( userExcOk ==  ret && isApply )
	{//提交申请
		fields.clear();
		if ( changeNormalDepartTask != type )
		{//2015-10-26 add 
			valueStr.Format( _T("%d"), changeWait0 );
		}else
		{
			valueStr.Format(_T("%d"), changeWait1);
		}
		fields.insert( std::make_pair(taskStatus, valueStr) );
		ret = m_pTaskDbDao->UpdateTaskField( chObj.GetTaskId(), fields );
	}
	return ret;
}
BOOL ChangeActionImp::DeleteChange( const CTaskChange &chObj )
{
	BOOL ret = userExcOk;
	if ( chObj.GetBakTaskId() > 0 )
	{
		 m_pTaskDbDao->DeleteTaskById( chObj.GetBakTaskId() );
	}
	ret = m_pChangeDbDao->DeleteChange( chObj.GetId() );
	return ret;
}
BOOL ChangeActionImp::GetChangeByTaskId( int taskId, ListChange &list )
{
	return m_pChangeDbDao->GetChangeItemsByTaskId( taskId, list);
}
BOOL ChangeActionImp::GetNewingChangeByTaskId( int taskId, ListChange &list )
{
	return m_pChangeDbDao->GetChangeByTaskId( taskId, changenewing, list);
}
BOOL ChangeActionImp::GetMaxNewChangeByTaskId( int taskId, CTaskChange &obj )
{
	return m_pChangeDbDao->GetMaxNewChangeItem( taskId, obj );
}
BOOL ChangeActionImp::GetMaxNewSupentChangeByTaskId( int taskId, CTaskChange &obj )
{
	return m_pChangeDbDao->GetMaxNewChangeItem( taskId, changeSupsend, obj );
}
BOOL  ChangeActionImp::GetAllChange( ListChange &list )
{
	return m_pChangeDbDao->GetAllChange( list );
}