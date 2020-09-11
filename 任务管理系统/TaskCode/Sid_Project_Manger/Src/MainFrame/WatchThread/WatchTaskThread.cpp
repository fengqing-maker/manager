m_attr
#include "stdafx.h"
#include "WatchTaskThread.h"
#include "Src/Tools/TRACE_D/trace_d.h"			/* 打印调试信息 */
#include "../Output/ClientOutputManager.h"
#include "../MainFrm.h"


IMPLEMENT_DYNCREATE(CWatchTaskThread,CBaseThread)

CWatchTaskThread::CWatchTaskThread()
{
	m_bCheckStoped = TRUE;

}
CWatchTaskThread::~CWatchTaskThread()
{
	StopCheckTask();
}

BEGIN_MESSAGE_MAP(CWatchTaskThread,CBaseThread)
	ON_THREAD_MESSAGE(WM_CHECK_TASK_START,		OnCheckStart)

	ON_THREAD_MESSAGE(WM_CHECK_TASK_EXCUTE,		OnCheckExcute)
	ON_THREAD_MESSAGE(WM_CHECK_TASK_FINISH,		OnCheckFinish)
END_MESSAGE_MAP()

void CWatchTaskThread::StartCheckTask()
{
	if (this->isRunning() == TRUE)
	{
		BOOL ret = this->PostThreadMessage(WM_CHECK_TASK_START,NULL,NULL);
		ASSERT(ret == TRUE);
		TRACE_D0("发送连接检查启动消息成功.\n");
	}
	else
	{
		TRACE_D0("发送连接检查启动消息失败.\n");
	}
}
void CWatchTaskThread::StopCheckTask()
{
	SmartCSLock lock(&m_stopCS);
	if (this->isRunning() == TRUE && m_bCheckStoped == FALSE)
	{
		m_bCheckStoped = TRUE;
	}
}
/****************************protected function***********************/


/****************************消息响应函数***********************************/
void CWatchTaskThread::OnCheckStart(WPARAM wParam,LPARAM lParam)
{
	m_bCheckStoped = FALSE;
	this->PostThreadMessage(WM_CHECK_TASK_EXCUTE,NULL,NULL);
	TRACE_D0("开始检查连接状态.\n");
}
BOOL CWatchTaskThread::FindTaskById( int taskId, const ListTask &lst, ListTaskConstItor &itor ) const
{
	itor = lst.begin();
	while( itor != lst.end() )
	{
		if ( taskId == itor->GetId() )
		{
			return TRUE;
		}
		itor++;
	}
	return FALSE;
}
BOOL CWatchTaskThread::FindScheduleById( int taskId, const ListSchedule &lst, ListScheduleConstItor &itor ) const
{
	itor = lst.begin();
	while( itor != lst.end() )
	{
		if ( taskId == itor->GetId() )
		{
			return TRUE;
		}
		itor++;
	}
	return FALSE;
}
BOOL CWatchTaskThread::FindChangeById( int taskId, const ListChange &lst, ListChangeConstItor &itor ) const
{
	itor = lst.begin();
	while( itor != lst.end() )
	{
		if ( taskId == itor->GetId() )
		{
			return TRUE;
		}
		itor++;
	}
	return FALSE;
}
void CWatchTaskThread::ChangeIncreaseHandle( int taskType, const ListTask &lst, const ListTask &orginLst ) const
{
	ListTaskConstItor findRet;
	for ( ListTaskConstItor itor = lst.begin(); itor != lst.end(); itor++ )
	{
		if ( FALSE == FindTaskById( itor->GetId(), orginLst, findRet) )
		{
			ClientOutputManager::GetObj()->OutputUpdateMsg(task_msg_add, taskType, itor->GetId() );
		}
	}
}
//返回是否有变化
bool CWatchTaskThread::OnCheckTask(  int taskType, const ListTask &lst, const ListTask &orginLst ) const
{
	bool isSend = false;
	const TaskIdStringMap statusMap =  m_orginSever->GetTaskSever()->GetTaskStausMap().GetTaskStringMap();
	if ( orginLst.size() > lst.size() )
	{//任务树监少
		ClientOutputManager::GetObj()->OutputUpdateMsg( task_msg_decrease, taskType, -1 );
		isSend = true;
	}else /*if ( orginLst.size() < lst.size() )*/
	//{//增加
	//	ChangeIncreaseHandle(  taskType, lst, orginLst );
	//}else
	{//变化
		for ( ListTaskConstItor itor = lst.begin(); itor != lst.end(); itor++ )
		{
			ListTaskConstItor findRet;
			if ( FindTaskById( itor->GetId(), orginLst, findRet) )
			{
				CString changeStr = findRet->GetDiffContent( *itor, statusMap );
				if ( !changeStr.IsEmpty() )
				{
					ClientOutputManager::GetObj()->OutputUpdateMsg(task_msg_update, taskType, itor->GetId(), changeStr );
				    isSend = true;
				}
			}else
			{
				CString str;
				str.Format(_T("新增任务【%d %s】"), itor->GetId(), itor->GetName() );
				ClientOutputManager::GetObj()->OutputUpdateMsg( task_msg_add, taskType, itor->GetId(), str );
			    isSend = true;
			}
		}
	}
	if( isSend )
	{
		postObserverMessage(  MSG_OUTPUT_SEL_SYS, 0, 0 );
	}
	return isSend;
}
//返回是否有变化
bool CWatchTaskThread::OnCheckScheule(  int taskType, const MapTaskSchedule &lst, const MapTaskSchedule &orginLst ) const
{//进度
	bool isSend = false;
	if ( orginLst.size() > lst.size() )
	{//任务树监少
		ClientOutputManager::GetObj()->OutputUpdateMsg( task_msg_decrease, taskType, -1 );
		isSend = true;
	}else 
	{//增加
		for ( MapTaskScheduleConstItor itor = lst.begin(); itor != lst.end(); itor++ )
		{
			MapTaskScheduleConstItor findRet = orginLst.find(itor->first);
			if ( findRet != orginLst.end() )
			{//变化
				if ( itor->second.size() >= findRet->second.size() )
				{
					ListScheduleConstItor schFindRet;
					for ( ListScheduleConstItor schItor = itor->second.begin(); schItor != itor->second.end(); schItor++ )
					{
						if ( FindScheduleById( schItor->GetId(), findRet->second, schFindRet) )
						{
							if ( !(*schItor == *schFindRet) )
							{
								ClientOutputManager::GetObj()->OutputUpdateMsg( task_msg_decrease, taskType, schItor->GetId() );
							    isSend = true;
							}
						}else
						{//增加
							 ClientOutputManager::GetObj()->OutputUpdateMsg(task_msg_add, taskType,schItor->GetId() );
						     isSend = true;
						}
					}
				}else
				{
					ClientOutputManager::GetObj()->OutputUpdateMsg( task_msg_decrease, taskType, -1 );
					isSend = true;
				}
			}else
			{//增加
			   for ( ListScheduleConstItor schItor = itor->second.begin(); schItor != itor->second.end(); schItor++ )
			   {
				   ClientOutputManager::GetObj()->OutputUpdateMsg(task_msg_add, taskType,schItor->GetId() );
				  isSend = true;
			   }
			}
		}
	
	}
	if( isSend  )
	{
		postObserverMessage(  MSG_OUTPUT_SEL_SYS, 0, 0 );
	}
	return isSend;
}
bool CWatchTaskThread::OnCheckTaskChange(  int taskType, const ListChange &lst, const ListChange &orginLst ) const
{
	bool isSend = false;
	if ( orginLst.size() > lst.size() )
	{//任务树减少
		ClientOutputManager::GetObj()->OutputUpdateMsg( task_msg_decrease, taskType, -1 );
		 isSend = true;
	}else if ( orginLst.size() < lst.size() )
	{//增加
		for ( ListChangeConstItor itor = lst.begin(); itor != lst.end(); itor++ )
		{
			ListChangeConstItor findRet;
			if ( FindChangeById( itor->GetId(), orginLst, findRet) )
			{
				if ( !(*findRet == *itor) )
				{
					ClientOutputManager::GetObj()->OutputUpdateMsg(task_msg_update, taskType, itor->GetId() );
					 isSend = true;
				}
			}else
			{//增加
				ClientOutputManager::GetObj()->OutputUpdateMsg(task_msg_add, taskType, itor->GetId() );
				 isSend = true;
			}
		}
	}
	if( isSend  )
	{
		postObserverMessage(  MSG_OUTPUT_SEL_SYS, 0, 0 );
	}
	return isSend;
}
void CWatchTaskThread::OnCheckExcute(WPARAM wParam,LPARAM lParam)
{
	SmartCSLock lock(&m_stopCS);		/* 同步外部置位停止 */
	if (m_bCheckStoped == FALSE && m_watchSever )
	{
		CTaskSeverInc* pTaskSever = m_watchSever->GetTaskSever();
		pTaskSever->UpdateLoginUser();
		const CUser loginUser = pTaskSever->GetLoginUser();
		if ( loginUser.GetId() > 0 && loginUser.GetOnLine() != onLine )
		{
			AfxMessageBox(_T("此用户已被强制下线") );
			postObserverMessage(  WM_CLOSE, 0, 0 );
			return;
		}
		CActionServerFactory *pOpreateSever = m_orginSever;
        if ( pOpreateSever )
        {//
			BOOL ret = pTaskSever->UpdateOwernTask();
			CTaskSeverInc* pTaskOpreate = pOpreateSever->GetTaskSever();
			int taskType = HandleMyTask;
			if ( ret == userExcOk )
			{
				if ( OnCheckTask( taskType, pTaskSever->GetOwernTask(), pTaskOpreate->GetOwernTask() ) )
				{
					postObserverMessage( MSG_UPDATE_TITLE_CNT, taskType, pTaskSever->GetOwernTask().size() );
					pTaskOpreate->SetOwernTask( pTaskSever->GetOwernTask() );
				}
			}
			//发布任务
			 taskType = PublishTask;
			ret = pTaskSever->UpdateReviewTask();
			if ( ret == userExcOk )
			{
				if ( OnCheckTask( taskType, pTaskSever->GetReviewTask(), pTaskOpreate->GetReviewTask() ) )
				{
					postObserverMessage( MSG_UPDATE_TITLE_CNT, taskType, pTaskSever->GetReviewTask().size() );
					pTaskOpreate->SetReviewTask( pTaskSever->GetReviewTask() );
				}
			}
	        //任务评个分
			taskType = ScoreTask;
			ret = pTaskSever->UpdateScoreTask();
			if ( ret == userExcOk )
			{
				if ( OnCheckTask( taskType, pTaskSever->GetScoreTask(), pTaskOpreate->GetScoreTask() ) )
				{
					postObserverMessage( MSG_UPDATE_TITLE_CNT, taskType, pTaskSever->GetScoreTask().size()  );
					pTaskOpreate->SetScoreTask( pTaskSever->GetScoreTask() );
				}
			}
			//进度点评
			//taskType = ReviewScheduleTask;
			//ret = pTaskSever->UpdateAllReviewSchedule();
			//if ( ret == userExcOk )
			//{
			//	if ( OnCheckScheule(   taskType, pTaskSever->GetReviewSchedule(), pTaskOpreate->GetReviewSchedule() ) )
			//	{
			//		pTaskOpreate->SetAllReviewSchedule( pTaskSever->GetReviewSchedule());
			//	}
			//}
			//变更
            taskType = ReviewChange;
			ret = pTaskSever->UpdateAllChange();
			if ( ret == userExcOk )
			{
				if (  OnCheckTaskChange( taskType, pTaskSever->GetReviewChange(), pTaskOpreate->GetReviewChange() ) )
				{
					size_t ownCount = pTaskSever->GetOwneChangeCount();
					size_t reviewChgCnt = pTaskSever->GetReviewChange().size();
					if ( reviewChgCnt > ownCount )
					{
						const ListChange &lstCh = pTaskSever->GetReviewChange();
						reviewChgCnt = 0;
						int loginUserId = pTaskSever->GetLoginUser().GetId();
						for ( ListChangeConstItor itor = lstCh.begin(); itor != lstCh.end(); itor++ )
						{
							CTaskObj obj;
							CProject prj;
							if ( itor->IsFinishChange() )
							{
								continue;
							}
							pTaskSever->GetTaskByTaskId( itor->GetTaskId(), obj );
							m_watchSever->GetProjectSever()->GetProjectById( obj.GetProjctId(), prj );
							if ( loginUserId == itor->GetAuditorId() )
							{
								if ( up_apper == itor->GetStatus() )
								{
									reviewChgCnt++;
								}
							}
							if ( loginUserId == itor->GetGeneralAuditorId() )
							{
								if ( departAgree == itor->GetStatus()  )
								{
									reviewChgCnt++;
								}
							}
							if ( obj.GetProjctId() > 0 && prj.GetMangerId() == loginUserId )
							{
								if ( prj_remark_on == itor->GetStatus() )
								{
									reviewChgCnt++;
								}
							}
						}	
					}else
					{
						reviewChgCnt = 0;
					}
					postObserverMessage( MSG_UPDATE_TITLE_CNT, taskType, reviewChgCnt  );
					pTaskOpreate->SetAllChange(  pTaskSever->GetReviewChange() );
				}
			}
        }
		TRACE_D0("执行任务检查.\n");
		m_bCheckStoped = FALSE;
		Sleep(2000);	/* 休眠500ms */
		this->PostThreadMessage(WM_CHECK_TASK_EXCUTE,NULL,NULL);
	
	}
}
void CWatchTaskThread::OnCheckFinish(WPARAM wParam,LPARAM lParam)
{
	TRACE_D0("检查已停止.\n");
	this->quit();	/* 退出 */
}

CString CWatchTaskThread::GetThreadName()
{
	return _T("任务监视");
}