#include "stdafx.h"
#include "TaskChange.h"

CTaskChange::CTaskChange()
{
	m_id = -1;	
	m_type = changeTime;
	m_taskId = -1;
	m_bakTaskId = -1;
	m_auditorId = -1;
	m_generalId = -1;
	m_status = 0;
	m_applyForTime.SetDate( 2000, 1, 1 );
	m_reviewTime.SetDate( 2000, 1, 1 );
	m_generalReviewTime.SetDate( 2000, 1, 1 );

	m_isImpactSchdule = -1; //是否影响
	m_taskType = 0;  //是否属于阶段变更
	m_changeResonOption = 0;

}

void CTaskChange::SetChangeDateTimeStr( const CString &time )
{
	m_applyForTime.ParseDateTime( time );
}
CString CTaskChange::GetChangeDateTimeStr() const
{
	CString temp;
	temp.Format(_T("%d-%d-%d %d:%d:%d"),
		m_applyForTime.GetYear(),m_applyForTime.GetMonth(), m_applyForTime.GetDay(), 
		m_applyForTime.GetHour(), m_applyForTime.GetMinute(), m_applyForTime.GetSecond());
	return temp;
	//return m_applyForTime.Format( _T("%Y-%m-%d %H:%M:%S") );
}
void CTaskChange::SetReviewTimeStr( const CString &time )
{
	m_reviewTime.ParseDateTime( time );
}
CString CTaskChange::GetReviewTimeStr() const
{
	CString temp;
	temp.Format(_T("%d-%d-%d %d:%d:%d"),
		m_reviewTime.GetYear(),m_reviewTime.GetMonth(), m_reviewTime.GetDay(), 
		m_reviewTime.GetHour(), m_reviewTime.GetMinute(), m_reviewTime.GetSecond());
	return temp;
	//return m_reviewTime.Format( _T("%Y-%m-%d %H:%M:%S") );
}
void CTaskChange::SetGeneralReviewTimeStr( const CString &time )
{
	m_generalReviewTime.ParseDateTime( time );
}
 CString CTaskChange::GetGeneralReviewTimeStr() const
{
	CString temp;
	temp.Format(_T("%d-%d-%d %d:%d:%d"),
		m_generalReviewTime.GetYear(),m_generalReviewTime.GetMonth(), m_generalReviewTime.GetDay(), 
		m_generalReviewTime.GetHour(), m_generalReviewTime.GetMinute(), m_generalReviewTime.GetSecond());
	return temp;
//	return m_generalReviewTime.Format( _T("%Y-%m-%d %H:%M:%S") );
}
void    CTaskChange::SetResonOptionSelectSt(int index, bool val)
{
   if ( val )
   {//赋1
		m_changeResonOption = m_changeResonOption | (1 << index);
   }else
   {//清0
	   m_changeResonOption = m_changeResonOption & ( ~(1<<index) );
   }
}
 CTaskChange&   CTaskChange::operator=( const CTaskChange &obj )
 {
	 CopyObj( obj );
	 return *this;
 }
CTaskChange::CTaskChange( const CTaskChange &obj  )
{
	CopyObj( obj );
}
void   CTaskChange::CopyObj( const CTaskChange &obj )
{
	m_id = obj.m_id;
	m_type = obj.m_type; //变更类型
	m_changeContent = obj.m_changeContent;
	m_applyForTime = obj.m_applyForTime; // 申请时间
	m_changeReason = obj.m_changeReason;
	m_taskId = obj.m_taskId;
	m_bakTaskId = obj.m_bakTaskId; //变更前的备份任务id号

	m_reviewContent = obj.m_reviewContent; //评审意见
	m_auditorId = obj.m_auditorId;     //评审人
	m_reviewTime = obj.m_reviewTime;

	m_generalReviewContent = obj.m_generalReviewContent; //评审意见
	m_generalId = obj.m_generalId;     //副总评审人
	m_generalReviewTime = obj.m_generalReviewTime; //评审时间

	m_status = obj.m_status; //变更流程状态
	m_isImpactSchdule = obj.m_isImpactSchdule;
	m_taskType = obj.m_taskType;
	m_projectReview = obj.m_projectReview;
	m_changeResonOption = obj.m_changeResonOption;
}

CString CTaskChange::GetChangeStatusString( int st, bool isShowReson /*= false*/ ) const
{
	CString retStr;
	switch( st )
	{
        case  changenewing:
			retStr = _T("新建变更中");
		    break;
		case  up_apper:
			retStr = _T("等待评审人审核");
			break;
		case  departAgree:
			if ( changeNormalDepartTask == m_taskType 
				||  (changeNormalPrjTask == m_taskType && 1 != m_isImpactSchdule )  )
			{
				retStr = _T("评审人同意变更");
			}else
			{
				if ( changePrjPhase == m_taskType )
				{
					if ( changeStop== m_type  )
					{
						retStr = _T("评审人已评分,等待二级审核");
					}else
					{
						retStr = _T("等待二级审核");
					}
				}else
				{
					retStr = _T("评审人同意变更,等待二级审核");
				}
				
			}
		   break;
		case  departUnAgree:
			retStr = _T("评审人不同意变更");
			if ( isShowReson )
			{
				retStr += _T(",");
				retStr += m_reviewContent;
			}
			break;
		case  genernalUnAgree:
			retStr = _T("副总经理不同意变更");
			if ( isShowReson )
			{
				retStr += _T(",");
				retStr += m_generalReviewContent;
			}
			break;
		case  genernalAgree:
			retStr = _T("副总经理同意变更");
			break;
		case  prj_remark_on:
			retStr = _T("等待项目经理点评");
			break;
	}
	return retStr;
}
bool   CTaskChange::operator==( const CTaskChange &obj ) const
{
	if ( m_id != obj.m_id )
	{
		return false;
	}
	if ( m_type != obj.m_type )
	{//变更类型
		return false;
	}
	if ( m_status != obj.m_status )
	{
		return false;
	}
	if ( m_changeContent != obj.m_changeContent )
	{
		return false;
	}
    if ( m_applyForTime != obj.m_applyForTime )
    {
		return false;
    } // 申请时间
	if ( m_changeReason != obj.m_changeReason )
	{
		return false;
	}
	if ( m_taskId != obj.m_taskId)
	{
		return false;
	}
	if ( m_bakTaskId != obj.m_bakTaskId )
	{ //变更前的备份任务id号
		return false;
	}
	if ( m_reviewContent != obj.m_reviewContent )
	{ //评审意见
		return false;
	}
	if ( m_auditorId != obj.m_auditorId )
	{//评审人
		return false;
	}
	if ( m_reviewTime != obj.m_reviewTime )
	{
		return false;
	}
    if ( m_generalReviewContent != obj.m_generalReviewContent )
    {//评审意见
		return false;
    }
    if ( m_generalId != obj.m_generalId )
    { //副总评审人
		return false;
    }
    if ( m_generalReviewTime != obj.m_generalReviewTime )
    {
		return false;
    }
	return true;
}
CString CTaskChange::GetChangeTypeStr() const
{
	CString str;
	switch( m_type )
	{
	case changeTime:
		str = _T("变更时间");
		break;
	case changeSupsend:
		str = _T("任务挂起");
		break;
	case changeReStart:
		str = _T("恢复挂起任务");
		break;
	case changeStop:
		str = _T("中止任务");
		break;

	}
   return str;
}
CString    CTaskChange::GetResonOptionString( int index ) const
{
	CString str = _T("无");
	switch( index )
	{
	case timeNotEnough:
		str = _T("任务预估量偏少");
		break;
	case changeOwner:
		str = _T("切换负责人");
		break;
	case schemeChange:
		str = _T("方案变更");//方案变更
		break;
	case ralateTaskAffect:
		str = _T("关联任务影响");
		break;
	case technoloageNeck:
		str = _T("技术瓶颈");
		break;
	case priorityMoidy:
		str = _T("优先级调整");
		break;
	case otherOption:
		str = _T("其他个人原因");
		break;
	}
	return str;
}
CString    CTaskChange::GetResonSelsOptionString() const
{
	CString retStr;
	if ( m_changeResonOption > 0)
	{
		for ( int i = 0; i < 32; i++ )
		{
			CString str;
			if ( GetResonOptionSelectSt( i ) && 
				 _T("无") != (str = GetResonOptionString( i )) )
			{
				retStr += str;
				str += _T(",");
			}
		}
	}
	return retStr;
}
void        CTaskChange::GetAllResonOptionMap( IntStringMap &mapStr ) const
{
	mapStr.clear();
	for ( int i = 0; i < 32; i++ )
	{
		CString str;
		if ( _T("无") != (str = GetResonOptionString( i )) )
		{
			mapStr.insert( std::make_pair( i, str ) );
		}
	}
}
bool       CTaskChange::IsFinishChange() const
{
	bool ret = false;
	if( departAgree == m_status )
	{
		if ( changeNormalDepartTask == m_taskType 
			||  (changeNormalPrjTask == m_taskType && 1 != m_isImpactSchdule )  )
		{
			ret = true;
		}
	}else if ( departUnAgree == m_status || genernalUnAgree == m_status || genernalAgree == m_status )
	{
		ret = true;
	}
	return ret;
}

