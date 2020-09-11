#include "StdAfx.h"
#include "AbstractOutputManager.h"
#include "Src/MainFrame/Sid_Project_Manger.h"
#include <time.h>
#include <assert.h>
//#define DEBUG_TRACE		/* 该宏定义用于打开调输出，必须在包含trace_d.h前定义 */
#include "Src/Tools/TRACE_D/trace_d.h"	/* 调试输出 */
#include "Src/SrcLib/XMLWrapper/XMLParser/XMLDOMFile.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"
#include "Src/Tools/BaseThread/SmartLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

Output_ListColumn AbstractOutputManager::m_columnList[4] = 
{
	{indexIndex,	TRANS_T("序号"),	LVCFMT_LEFT,	indexWidth},/* 注意第一列只能靠左对齐 */
	{typeIndex,		TRANS_T("类型"),	LVCFMT_LEFT,	typeWidth},
	{contentIndex,	TRANS_T("内容"),	LVCFMT_LEFT,	contentWidth},
	{timeIndex,		TRANS_T("时间"),	LVCFMT_LEFT,	timeWidth}
};

AbstractOutputManager::AbstractOutputManager(void)
{
	m_outputList = 0;
	m_messageIndex = 0;
}
AbstractOutputManager::~AbstractOutputManager(void)
{
	ClearnMessageList();
}

BOOL AbstractOutputManager::RegisterAsOutputList(COutputListCtrl* outputList)
{
	if (outputList != 0
		&&outputList->IsKindOf(RUNTIME_CLASS(COutputListCtrl)))
	{
		m_outputList = outputList;
		TRACE_D0("注册输出列表成功.\n");
		return TRUE;
	}
	else
	{
		TRACE_D0("注册输出列表失败.\n");
		return FALSE;
	}
}

void AbstractOutputManager::InitOutputList()
{
	if (m_outputList == 0)
		return;
	int nColumn = sizeof(m_columnList)/sizeof(Output_ListColumn);
	m_outputList->DeleteAllItems();
	while(m_outputList->DeleteColumn(0) == TRUE);

	for (int i = 0; i< nColumn; ++i)
	{
		int ret = m_outputList->InsertColumn(m_columnList[i].index,
			m_columnList[i].title,m_columnList[i].alignment);
		if(ret == -1)
		{
			TRACE_D0("插入“列”失败.\n");
		}
	}
	SetListIcon();			/* 设置图标 */
	ResetColumnWidth();		/* 重置列宽 */
	m_messageIndex = 0;
	ClearnMessageList();
}
void  AbstractOutputManager::ClearnMessageList()
{
	for( MessageListConstItor itor = m_messageList.begin(); itor != m_messageList.end(); itor++ )
	{
		if ( itor->pPara )
		{
			delete itor->pPara;
		}
	}
	m_messageList.clear();
}
//void AbstractOutputManager::ShowOutputPane()
//{
	//CDockablePane* dockPane = static_cast<CDockablePane*>(m_outputList->GetParent());
	//if (dockPane != 0 && dockPane->IsKindOf(RUNTIME_CLASS(CDockablePane)))
	//{
	//	dockPane->ShowPane(TRUE,FALSE,TRUE);
	//}
//}

void AbstractOutputManager::ShowOutputContextMenu(CPoint point)
{
	if (m_outputList == 0)
	{
		return ;
	}
	CMenu popupMenu;
	if(!popupMenu.CreatePopupMenu())
		return ;
	
	//VERIFY(popupMenu.AppendMenu(MF_STRING,popupMenuID_CollapseContentH,TRANS_T("水平折叠")));
	//VERIFY(popupMenu.AppendMenu(MF_STRING,popupMenuID_ExpandContentH,TRANS_T("水平展开")));
	//VERIFY(popupMenu.AppendMenu(MF_STRING,popupMenuID_CollapseContentV,TRANS_T("垂直折叠")));
	//VERIFY(popupMenu.AppendMenu(MF_STRING,popupMenuID_ExpandContentV,TRANS_T("垂直展开")));
	/*VERIFY(popupMenu.AppendMenu(MF_SEPARATOR,(UINT_PTR)0,(LPCTSTR)NULL));*/
	VERIFY(popupMenu.AppendMenu(MF_STRING,popupMenuID_ClearContent,TRANS_T("清除内容")));
	VERIFY(popupMenu.AppendMenu(MF_STRING,popupMenuID_ExportContent,TRANS_T("导出内容")));

	HMENU hPopupMenu = popupMenu.Detach();
	CWnd* pParent = m_outputList/*->GetParent()*/;
	theApp.GetContextMenuManager()->
		ShowPopupMenu(hPopupMenu,point.x,point.y,pParent,TRUE,TRUE,FALSE);
}
void AbstractOutputManager::RespOutputContextMenu(UINT menuID)
{
	if(menuID >= WM_OUTPUT_POPUP_MENUID_MIN 
		&&menuID <= WM_OUTPUT_POPUP_MENUID_MAX)
	{
		ResponseContextMenu(menuID);
	}
}

void AbstractOutputManager::ResetColumnWidth()
{
	CRect rectList;
	m_outputList->GetClientRect(&rectList);
	int nColumn = sizeof(m_columnList)/sizeof(Output_ListColumn);
	int columnWidth/*,orginColumnWidth*/;
	for (int i = 0; i < nColumn; ++i)
	{
		/* 计算列宽 */
		//orginColumnWidth = m_outputList->GetColumnWidth(i);
		columnWidth = 
			static_cast<int>((m_columnList[i].widthRatio*rectList.Width())/totalWidth);
		//if ( columnWidth > orginColumnWidth  )
		//{
			m_outputList->SetColumnWidth(i/*m_columnList[i].index*/,columnWidth);
		//}

	}
}
void AbstractOutputManager::CollapseContentColumnH()
{
	int minColumnWidth = 0;
	m_outputList->SetRedraw(FALSE);
	/* 获取当前列宽 */
	minColumnWidth = m_outputList->GetColumnWidth(contentIndex);
	/* 与默认列宽比较 */
	CRect rectList;
	m_outputList->GetClientRect(&rectList);
	int defColumnWith = static_cast<int>((m_columnList[contentIndex].widthRatio*rectList.Width())/totalWidth);
	minColumnWidth = min(minColumnWidth,defColumnWith);
	
	VERIFY(m_outputList->SetColumnWidth(contentIndex,minColumnWidth));
	m_outputList->SetRedraw(TRUE);
}
void AbstractOutputManager::ExpandContentColumnH()
{
	int maxColumnWidth = 0;
	m_outputList->SetRedraw(FALSE);
	/* 获取当前列宽 */
	maxColumnWidth = m_outputList->GetColumnWidth(contentIndex);
	/* 与当前内容宽度比较 */

	int textWidth = m_outputList->GetMaxColumnStringWidth(contentIndex);
	maxColumnWidth = max(maxColumnWidth,textWidth);

	VERIFY(m_outputList->SetColumnWidth(contentIndex,maxColumnWidth));
	m_outputList->SetRedraw(TRUE);
}
void AbstractOutputManager::CollapseContentColumnV()
{
	if (m_outputList != 0)
	{
		m_outputList->SetRedraw(FALSE);
		m_outputList->CollapseAll(); 
		m_outputList->SetRedraw(TRUE);
	}
}
void AbstractOutputManager::ExpandContentColumnV()
{
	if (m_outputList != 0)
	{
		m_outputList->SetRedraw(FALSE);
		m_outputList->ExpandAll(); 
		m_outputList->SetRedraw(TRUE);
	}
}

void AbstractOutputManager::UpdateContent()
{
	int messageIndex = m_messageIndex;
	int messageCount = m_messageList.size();
	while (messageIndex < messageCount)
	{
		Output_Message_Private* pPrivateMsg = GetMessage(messageIndex);
		if ( pPrivateMsg && ::IsWindow( m_outputList->GetSafeHwnd()) )
		{
			DisplayMessage( *pPrivateMsg );
		}		
		messageIndex++;
	}
	m_messageIndex = messageIndex;
}
void AbstractOutputManager::ClearContent()
{
	if (m_outputList==0)
		return;
	InitOutputList();
}
void AbstractOutputManager::ExportContent()
{
	/* 是否满足导出条件 */
	if (m_outputList == 0)
	{
		return;
	}
	if (m_messageList.size() == 0)
	{
		CString msg;
		msg.Format(TRANS_T("没有内容可以导出!"));
		CUtilityTool::GetObj()->MessageDialog(msg);
		return;
	}
	/* 默认保存文件名 */
	CString defFileFullName = GetExportDefaultFileFullName();
	/* 默认保存路径 */
	CString defFileDirectory = defFileFullName;
	LPCTSTR lpDefFileDirectory = defFileDirectory.GetBuffer();
	VERIFY(::PathRemoveFileSpec(defFileDirectory.GetBuffer()));
	defFileDirectory = lpDefFileDirectory;
	/* 创建文件夹 */
	BOOL result = CUtilityTool::GetObj()->CreateDir(defFileDirectory);
	ASSERT(result == TRUE);
	/* 文件保存对话框 */
	CFileDialog dlg(FALSE,_T("xml"),defFileFullName,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,/* 隐藏只读|覆写提示|不改变当前文件夹路径 */
		_T("(*.xml)|*.xml||"),0,0,
		TRUE);	/* 不能使用vista style，如果使用vista style则默认路径不能正确设置 */
	dlg.GetOFN().lpstrInitialDir = defFileDirectory;
	int choice = dlg.DoModal();
	if(choice != IDOK)  return ;

	CString sFilePath = dlg.GetPathName();
	if (ExportContentToXML(sFilePath))
	{
		CString msg;
		msg.Format(TRANS_T("输出信息导出成功."));
		CUtilityTool::GetObj()->MessageDialog(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("输出信息导出失败."));
		CUtilityTool::GetObj()->MessageDialog(err);
	}
}

void AbstractOutputManager::ShowMessage(CString msg)
{
	Output_Message outputMsg;
	outputMsg.content = msg;
	outputMsg.type = messageType_message;
	int index = RecordMessage(outputMsg);
	if (index != -1 && m_outputList != 0)
	{
		UpdateContent();
		//m_outputList->PostMessage(WM_OUTPUT_UPDATE,NULL,NULL);
	}
}
void AbstractOutputManager::ShowParaMsg( CString msg, void *pPara )
{
	Output_Message outputMsg;
	outputMsg.content = msg;
	outputMsg.type = messageType_message;

	int index = -1;
	if ( msg.GetLength() > 0 )
	{
		index = RecordMessage(outputMsg, pPara);
	}
	if (index != -1 && m_outputList != 0)
	{
		UpdateContent();
		//m_outputList->PostMessage(WM_OUTPUT_UPDATE,NULL,NULL);
	}
}
void AbstractOutputManager::ShowError(CString err)
{
	Output_Message outputErr;
	outputErr.content = err;
	outputErr.type = messageType_error;
	int index = RecordMessage(outputErr);
	if (index != -1 && m_outputList != 0)
	{
		UpdateContent();
		//m_outputList->PostMessage(WM_OUTPUT_UPDATE,NULL,NULL);
	}
}

/******************************protected function*************************/
void AbstractOutputManager::ResponseContextMenu(UINT popmenuID)
{
	switch (popmenuID)
	{
	case popupMenuID_CollapseContentH:
		CollapseContentColumnH();
		break;
	case popupMenuID_ExpandContentH:
		ExpandContentColumnH();
		break;
	case popupMenuID_CollapseContentV:
		CollapseContentColumnV();
		break;
	case popupMenuID_ExpandContentV:
		ExpandContentColumnV();
		break;
	case popupMenuID_ClearContent:
		ClearContent();
		break;
	case popupMenuID_ExportContent:
		ExportContent();
		break;
	default:
		break;
	}
}

CString AbstractOutputManager::GetExportDirectoryName()
{
	return _T("Output\\Default");
}
CString AbstractOutputManager::GetExportDefaultName()
{
	return _T("Untitle.xml");
}

/******************************private function***************************/
void AbstractOutputManager::SetListIcon()
{
	if (m_outputList == 0)
		return;
	CImageList imageList;CBitmap bitMap;
	if(!imageList.Create(16,16,ILC_COLOR32|ILC_MASK,3,0)
		||!bitMap.LoadBitmap(IDB_BITMAP_ICONLIST_OUTPUT))
	{
		TRACE("图标设置失败.\n");
	}
	else
	{
		imageList.Add(&bitMap,RGB(0,0,0));
		m_outputList->SetImageList(&imageList,LVSIL_SMALL);
	}
	imageList.Detach();
	bitMap.Detach();
}

int AbstractOutputManager::RecordMessage(const Output_Message& msg,void *pPara /*= NULL*/)
{
	SmartCSLock lock( &m_FillOutputCS );
	int index = m_messageList.size();
	Output_Message_Private privateMsg;
	privateMsg.index.Format(_T("%d"),index);
	privateMsg.pPara = pPara;
	switch (msg.type)
	{
	case messageType_message:
		privateMsg.type = TRANS_T("消息");
		privateMsg.imageIndex = iconIndex_message;
		break;
	case messageType_para:
		privateMsg.type = TRANS_T("更新");
		privateMsg.imageIndex = iconIndex_message;
		break;
	case messageType_error:
		privateMsg.type = TRANS_T("错误");
		privateMsg.imageIndex = iconIndex_error;
		break;
	default:
		privateMsg.type = TRANS_T("未知");
		privateMsg.imageIndex = iconIndex_unknow;
		break;
	}
	privateMsg.content = msg.content;
	privateMsg.time = CurrentTime();

	m_messageList.push_back(privateMsg);
	return index;
}
Output_Message_Private* AbstractOutputManager::GetMessage(int index)
{
	int cnt = m_messageList.size();
	if ( cnt <= 0 || index < 0 || index >= cnt )
	{
		return NULL;
	}
	MessageListItor itor = m_messageList.begin();
	advance( itor, index );
	if ( itor != m_messageList.end() )
	{
		return &(*itor);
	}
	//	int curIndex = 0;
	//while (itor != m_messageList.end())
	//{
	//	if (index == curIndex)
	//	{
	//		message = *itor;
	//		break;
	//	}
	//	curIndex++;
	//	itor++;
	//}
	return NULL;
}
void AbstractOutputManager::DisplayMessage(Output_Message_Private privateMsg)
{
	if ( FALSE == IsWindow( m_outputList->GetSafeHwnd() ) )
	{
          return;
	}
	m_outputList->SetRedraw( FALSE );
	if (m_outputList == 0)
		return ;
	if ( NULL == m_outputList->m_hWnd )
	{
		return;
	}
	int itemIndex = m_outputList->GetItemCount();
	VERIFY(-1 != m_outputList->InsertItem(itemIndex,_T(""),privateMsg.imageIndex));

	int nColumn = sizeof(m_columnList)/sizeof(Output_ListColumn);
	for (int i= 0; i < nColumn ; ++i)
	{	
		int subItemIndex = i;
		CString subItemText;
		switch (i)
		{
		case indexIndex:
			subItemText = privateMsg.index;
			break;
		case typeIndex:
			subItemText = privateMsg.type;
			break;
		case contentIndex:
			subItemText = privateMsg.content;
			break;
		case timeIndex:
			subItemText = privateMsg.time;
			break;
		default:
			TRACE_D0("服务器输出窗口的未知内容.\n");
			break;
		}
		VERIFY(m_outputList->SetItemText(itemIndex,subItemIndex,subItemText));
	}
	VERIFY(m_outputList->EnsureVisible(itemIndex,FALSE));
	m_outputList->SetRedraw( TRUE ); 
}

CString AbstractOutputManager::CurrentTime()
{
	time_t tempTime;
	time(&tempTime);
	tm tempTm;
	ZeroMemory(&tempTm,sizeof(tempTm));
	tm * pTime = &tempTm;
	localtime_s(pTime,&tempTime);
	char strTime[256];
	memset(strTime,0,256);
	strftime(strTime,256,"%Y-%m-%d   %H:%M:%S",pTime);
	CStringA timeA = strTime;
	CString timeT;
	timeT = timeA;
	return timeT;
}

BOOL AbstractOutputManager::ExportContentToXML(CString filePath)
{
	CXMLDOMFilePtr pXMLDOMFile(new CXMLDOMFile());
	DesBool desRet = pXMLDOMFile->New(filePath,_T("OutputList"),_T(""));
	if (desRet.result == FALSE)
	{
		return FALSE;	/* 文件新建失败 */
	}
	desRet = pXMLDOMFile->Open(filePath);
	if (desRet.result == FALSE)
	{
		return FALSE;	/* 文件打开失败 */
	}
	CXMLDOMNodePtr pOutputList = pXMLDOMFile->GetRoot();
	if (pOutputList == NULL)
	{
		return FALSE;
	}
	MessageListItor msgItor = m_messageList.begin();
	while (msgItor != m_messageList.end())
	{
		Output_Message_Private privateMsg = *msgItor;
		CXMLDOMNodePtr pOutputMsg = pOutputList->AppendChildNode(_T("OutputMessage"));
		/* 导出序号 */
		CXMLDOMNodePtr pMsgIndex = pOutputMsg->AppendChildNodeWithValue(_T("Index"),privateMsg.index);
		ASSERT(pMsgIndex != NULL);
		/* 导出类型 */
		CXMLDOMNodePtr pMsgType = pOutputMsg->AppendChildNodeWithValue(_T("Type"),privateMsg.type);
		ASSERT(pMsgType != NULL);
		/* 导出内容 */
		CXMLDOMNodePtr pMsgContent = pOutputMsg->AppendChildNodeWithValue(_T("Content"),privateMsg.content);
		ASSERT(pMsgContent != NULL);
		/* 导出时间 */
		CXMLDOMNodePtr pMsgTime = pOutputMsg->AppendChildNodeWithValue(_T("Time"),privateMsg.time);
		ASSERT(pMsgTime != NULL);
		msgItor++;
	}
	if(!pXMLDOMFile->Close(TRUE).result)
	{
		return FALSE;
	}
	return TRUE;
}
CString AbstractOutputManager::GetExportDefaultFileFullName()
{
	CString defFileName = GetExportDefaultName();
	CString dirName = GetExportDirectoryName();
	/* 默认保存路径 */
	CString defFileDirectory = CUtilityTool::GetObj()->GetModuleDirPath();
	defFileDirectory = CUtilityTool::GetObj()->JoinDirPath(defFileDirectory,dirName);
	defFileName = CUtilityTool::GetObj()->JoinDirPath(defFileDirectory,defFileName);
	return defFileName;
}