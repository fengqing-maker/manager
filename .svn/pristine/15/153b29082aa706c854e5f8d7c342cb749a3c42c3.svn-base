
#include "stdafx.h"
#include "SvrOutputManager.h"
#include "Src/MainFrame/messageDef.h"
//#include "ClientInterface.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"


/** @brief 服务器信息输出窗口信息保存默认目录 */
#define STR_OUTPUT_SERVER_DIR_PATH				_T("User\\Output\\SystemOutput")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

SvrOutputManager::SvrOutputManager(void)
{
}
SvrOutputManager::~SvrOutputManager(void)
{
}

void SvrOutputManager::UpdateOutputPaneCaption()
{
	CDockablePane* dockPane = static_cast<CDockablePane*>(m_outputList->GetParent());
	if (dockPane != 0 && dockPane->IsKindOf(RUNTIME_CLASS(CDockablePane)))
	{
		CString serverName;
		serverName = TRANS_T("系统输出");
		CString strLabel;
		strLabel.Format(TRANS_T("%s系统信息视图"),
			serverName);
		dockPane->SetWindowText(strLabel);
	}
}

/******************************protected function*************************/
CString SvrOutputManager::GetExportDirectoryName()
{
	COleDateTime dateTime = COleDateTime::GetCurrentTime();
	CString strDate = dateTime.Format(TRANS_T("%Y年%m月%d日"));
	CString dirName = CUtilityTool::GetObj()->JoinDirPath(STR_OUTPUT_SERVER_DIR_PATH,strDate);
	return dirName;
}
CString SvrOutputManager::GetExportDefaultName()
{
	CString fileName;
	//CDockablePane* dockPane = static_cast<CDockablePane*>(m_outputList->GetParent());
	//if (dockPane != 0 && dockPane->IsKindOf(RUNTIME_CLASS(CDockablePane)))
	//{
		CString paneName;
		//dockPane->GetWindowText(paneName);
		m_outputList->GetWindowText( paneName );
		COleDateTime dateTime = COleDateTime::GetCurrentTime();
		CString strDateTime = dateTime.Format(TRANS_T("%Y年%m月%d日 %H-%M-%S"));
		fileName.Format(_T("%s(%s)"),paneName,strDateTime);
	/*}
	else
	{
		fileName = AbstractOutputManager::GetExportDefaultName();
	}*/
	return fileName;
}
CString SvrOutputManager::MakeOutputString( int taskMsgType,  int taskType, int id, const CString &extenStr )
{
	CString content;
	switch ( taskMsgType )
	{
	case task_msg_add:
		content = _T("新增");
		break;
	case task_msg_update:
		content = _T("更新");
		break;
	case task_msg_decrease:
		content = _T("处理了 ");
		break;
	case  check_msg_workHour:
		content = _T("工时检查");
		break;
	default:
		content = _T("未知更新类型");
		return content;
	}
	switch( taskType )
	{
	case HandleMyTask://新增我要处理任务
		content += _T("待处理任务");
		break;
	case PublishTask://新增待发布任务
		content += _T("待发布任务");
		break;
	case ReviewScheduleTask://新增进度点评任务
		content += _T("待进度点评任务");
		break;
	case ScoreTask:    //新增待评分任务
		content += _T("待评分任务");
		break;
	case ReviewChange: //新增待评审变更任务
		content += _T("待评审变更申请");
		break;
	case WorkHourView:
		content += _T("[工时]");
		break;
	default:
		content += _T("未知更新内容");
	}
	if ( id > 0 &&  taskType != ReviewChange && taskType != ReviewScheduleTask )
	{//111111	
		//CString temp;
	 //   temp.Format(_T("\r\n任务名'%s'"), obj.GetName() );
		//content += temp;
	}
	//content += extenStr;
	content += _T(";双击更新数据表格");
	if ( !extenStr.IsEmpty() )
	{
		content += _T("\r\n");
		content += extenStr;
	}
	return content;
}
void SvrOutputManager::ShowUpdateTaskMsg( int taskMsgType,  int taskType,int id, const CString &extContent )
{
	//
	TaskMsgParam *pPara = new TaskMsgParam();
	pPara->msgType = taskMsgType;
	pPara->taskType = taskType;
	pPara->taskId = id;
	pPara->isSendUpdateMsg = FALSE;
    CString content = MakeOutputString( taskMsgType, taskType, id, extContent );
	ShowParaMsg( content, pPara );
}
void SvrOutputManager::OnDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
    int row = lpnmitem->iItem;
	//int cloum = lpnmitem->iSubItem;
	Output_Message_Private *pData = GetMessage( row );
	if ( pData && pData->pPara )
	{
		TaskMsgParam *pTaskInfo = (TaskMsgParam *)pData->pPara;
		int msgId = -1;
		if ( FALSE == pTaskInfo->isSendUpdateMsg )
		{
			switch( pTaskInfo->msgType )
			{
			case  task_msg_add:
				msgId = MSG_ADD_CNT_CHANGE_UPDATE;
				break;
			case  task_msg_decrease:
				msgId = MSG_DECREASE_CNT_CHANGE_UPDATE;
				break;
			case  task_msg_update:
				msgId = MSG_CONTENT_CHANGE_UPDAE;
				break;
			case  check_msg_workHour:
			    msgId = MSG_WORKHOUR_CHECK;
				break;
			}
		}else
		{
			msgId = MSG_SELECT_TASK_ITEM;
		}
		if ( msgId > 0 )
		{
			PostMessage( AfxGetMainWnd()->GetSafeHwnd(), msgId, pTaskInfo->taskType,pTaskInfo->taskId );
		}
		
	}
}
void SvrOutputManager::OnClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	int row = lpnmitem->iItem;
	//int cloum = lpnmitem->iSubItem;
	Output_Message_Private *pData = GetMessage( row );
	if ( pData && pData->pPara )
	{
		TaskMsgParam *pTaskInfo = (TaskMsgParam *)pData->pPara;
		if ( pTaskInfo->msgType != check_msg_workHour )
		{
				PostMessage( AfxGetMainWnd()->GetSafeHwnd(), MSG_SELECT_TASK_ITEM,pTaskInfo->taskId, pTaskInfo->taskType );
		}
	}
}
