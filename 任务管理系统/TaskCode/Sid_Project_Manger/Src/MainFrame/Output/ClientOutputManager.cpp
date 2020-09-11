#include "stdafx.h"
#include "ClientOutputManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

ClientOutputManager::ClientOutputManager(void)
{
}
ClientOutputManager::~ClientOutputManager(void)
{
}

void ClientOutputManager::OutputMsg(CString msg)
{
	if(SvrOutputManager::GetObj() != 0)
	{
		SvrOutputManager::GetObj()->ShowMessage(msg);
	}
}
void ClientOutputManager::OutputErr(CString err)
{
	if(SvrOutputManager::GetObj() != 0)
	{
		SvrOutputManager::GetObj()->ShowError(err);
	}
}
void  ClientOutputManager::OutputUpdateMsg( int taskMsgType,  int taskType,int id, CString extText )
{
	if(SvrOutputManager::GetObj() != 0)
	{
		SvrOutputManager::GetObj()->ShowUpdateTaskMsg(taskMsgType, taskType, id, extText );
	}
}
