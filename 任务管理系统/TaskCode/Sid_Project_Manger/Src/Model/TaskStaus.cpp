#include "stdafx.h"
#include "TaskStaus.h"

bool   CTaskStaus::InsertStaus( int id, CString &strStaus )
{
	switch(id)
	{
	case newTasking:
		if ( strStaus != _T("新建中") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case waitPublish:
		if ( strStaus != _T("审核中") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case waitStart:
		if ( strStaus != _T("未开始") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case ongoing:
		if ( strStaus != _T("进行中") ) //lhz 2015-4-8
		{//报数据库配置错误
			return false;  
		}
		break;
	case task_close:
		if ( strStaus != _T("关闭") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case task_suspend:
		if ( strStaus != _T("挂起") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case midStop:
		if ( strStaus != _T("中止") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case changeWait1:
		if ( strStaus != _T("待主管审核变更") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case changeWait2:
		if ( strStaus != _T("待副总审核变更") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case changeWait0:
		if ( strStaus != _T("待项目点评变更") )
		{//报数据库配置错误
			return false;  
		}
		break;
	case finishReviewWait:
		if ( strStaus != _T("待项目审核完成") )
		{//报数据库配置错误
			return false;  
		}
		break;
	}
	TaskIdStringMapItor itor;
	if ( (itor = m_idStausMap.find(id)) != m_idStausMap.end())
	{
	     (*itor).second = strStaus;
		return true;
	}
	else
	{
		m_idStausMap.insert( std::make_pair(id, strStaus) );
		return true;
	}
}

bool CTaskStaus::GetStrStausById( int id, CString &strStatus ) const
{
	TaskIdStringMapConstItor itor;
	if ( (itor = m_idStausMap.find(id)) != m_idStausMap.end())
	{
		strStatus = itor->second;
		return true ;
	}
	return false;
}
int      CTaskStaus::FindStatusIdByString( const CString &str ) const
{
	int ret = -1;
	for ( TaskIdStringMap::const_iterator itor = m_idStausMap.begin(); itor != m_idStausMap.end(); itor++ )
	{
		if ( itor->second == str )
		{
			ret = itor->first;
			break;
		}
	}
	return ret;
}