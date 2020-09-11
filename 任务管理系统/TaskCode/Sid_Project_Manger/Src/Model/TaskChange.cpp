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

	m_isImpactSchdule = -1; //�Ƿ�Ӱ��
	m_taskType = 0;  //�Ƿ����ڽ׶α��
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
   {//��1
		m_changeResonOption = m_changeResonOption | (1 << index);
   }else
   {//��0
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
	m_type = obj.m_type; //�������
	m_changeContent = obj.m_changeContent;
	m_applyForTime = obj.m_applyForTime; // ����ʱ��
	m_changeReason = obj.m_changeReason;
	m_taskId = obj.m_taskId;
	m_bakTaskId = obj.m_bakTaskId; //���ǰ�ı�������id��

	m_reviewContent = obj.m_reviewContent; //�������
	m_auditorId = obj.m_auditorId;     //������
	m_reviewTime = obj.m_reviewTime;

	m_generalReviewContent = obj.m_generalReviewContent; //�������
	m_generalId = obj.m_generalId;     //����������
	m_generalReviewTime = obj.m_generalReviewTime; //����ʱ��

	m_status = obj.m_status; //�������״̬
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
			retStr = _T("�½������");
		    break;
		case  up_apper:
			retStr = _T("�ȴ����������");
			break;
		case  departAgree:
			if ( changeNormalDepartTask == m_taskType 
				||  (changeNormalPrjTask == m_taskType && 1 != m_isImpactSchdule )  )
			{
				retStr = _T("������ͬ����");
			}else
			{
				if ( changePrjPhase == m_taskType )
				{
					if ( changeStop== m_type  )
					{
						retStr = _T("������������,�ȴ��������");
					}else
					{
						retStr = _T("�ȴ��������");
					}
				}else
				{
					retStr = _T("������ͬ����,�ȴ��������");
				}
				
			}
		   break;
		case  departUnAgree:
			retStr = _T("�����˲�ͬ����");
			if ( isShowReson )
			{
				retStr += _T(",");
				retStr += m_reviewContent;
			}
			break;
		case  genernalUnAgree:
			retStr = _T("���ܾ���ͬ����");
			if ( isShowReson )
			{
				retStr += _T(",");
				retStr += m_generalReviewContent;
			}
			break;
		case  genernalAgree:
			retStr = _T("���ܾ���ͬ����");
			break;
		case  prj_remark_on:
			retStr = _T("�ȴ���Ŀ�������");
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
	{//�������
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
    } // ����ʱ��
	if ( m_changeReason != obj.m_changeReason )
	{
		return false;
	}
	if ( m_taskId != obj.m_taskId)
	{
		return false;
	}
	if ( m_bakTaskId != obj.m_bakTaskId )
	{ //���ǰ�ı�������id��
		return false;
	}
	if ( m_reviewContent != obj.m_reviewContent )
	{ //�������
		return false;
	}
	if ( m_auditorId != obj.m_auditorId )
	{//������
		return false;
	}
	if ( m_reviewTime != obj.m_reviewTime )
	{
		return false;
	}
    if ( m_generalReviewContent != obj.m_generalReviewContent )
    {//�������
		return false;
    }
    if ( m_generalId != obj.m_generalId )
    { //����������
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
		str = _T("���ʱ��");
		break;
	case changeSupsend:
		str = _T("�������");
		break;
	case changeReStart:
		str = _T("�ָ���������");
		break;
	case changeStop:
		str = _T("��ֹ����");
		break;

	}
   return str;
}
CString    CTaskChange::GetResonOptionString( int index ) const
{
	CString str = _T("��");
	switch( index )
	{
	case timeNotEnough:
		str = _T("����Ԥ����ƫ��");
		break;
	case changeOwner:
		str = _T("�л�������");
		break;
	case schemeChange:
		str = _T("�������");//�������
		break;
	case ralateTaskAffect:
		str = _T("��������Ӱ��");
		break;
	case technoloageNeck:
		str = _T("����ƿ��");
		break;
	case priorityMoidy:
		str = _T("���ȼ�����");
		break;
	case otherOption:
		str = _T("��������ԭ��");
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
				 _T("��") != (str = GetResonOptionString( i )) )
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
		if ( _T("��") != (str = GetResonOptionString( i )) )
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

