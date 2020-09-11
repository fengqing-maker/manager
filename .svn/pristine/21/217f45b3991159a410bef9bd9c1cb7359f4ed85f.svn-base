#include "stdafx.h"
#include "departSeverImp.h"
#include <Src/MainFrame/Dlg/DlgUserChose.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDepartSeverImp::CDepartSeverImp( CDaoFactory *pFactory )
	              :CDepartSeverInc( pFactory )
{
	m_pDepartDao = pFactory->GetDepartDbDao();
}

BOOL CDepartSeverImp::GetDepartById( int id, CDepartment &obj )
{
	return m_pDepartDao->GetDepartmentById( id, obj );
}
BOOL CDepartSeverImp::GetDepartByName( const CString str, CDepartment &obj)
{
	return m_pDepartDao->GetDepartmentByName( str, obj);
}
BOOL CDepartSeverImp::GetTopDepartByMangerId( int userId, ListDepart &list )
{
	return m_pDepartDao->GetDepartmentByMangerId( userId, list, 0 );
}
BOOL CDepartSeverImp::GetAllDepart( ListDepart &list )
{
	return m_pDepartDao->GetAllDepart( list );
}

BOOL  CDepartSeverImp::GetDepartUser( int departId, ListUser &usr, BOOL isIncludeChildDepart )
{
	BOOL ret = m_pDbDao->GetUserDao()->GetDepartMember( departId, usr );
	if ( FALSE == isIncludeChildDepart  || ret != userExcOk )
	{
		return ret;
	}
	ListDepart list; 
	ret = m_pDepartDao->GetChildDepart( departId, list );
	ListDepartConstItor itor = list.begin();
	while( (ret == userExcOk || ret == notfind ) && itor != list.end() )
	{
		ret = m_pDbDao->GetUserDao()->GetDepartMember( itor->GetId(), usr );
		itor++;
	}
	if ( ret == notfind )
	{
		ret = userExcOk;
	}
	return ret;
}
BOOL CDepartSeverImp::GetDepartByMangerId( int userId,  const CString &roleName, ListDepart &list )
{
	if( roleName == _T("管理员") || roleName == _T("系统管理员"))
	{
		return GetAllDepart( list );
	}
	//
	BOOL ret = m_pDepartDao->GetDepartmentByMangerId( userId, list, -1 );
	if ( ret != userExcOk )
	{
		return ret;
	}
	int count = list.size();
	ListDepartItor itor = list.begin();
	for ( int i = 0; i < count; i++ )
	{
		m_pDepartDao->GetChildDepart( itor->GetId(), list );
		itor++;
	}
	itor = list.begin();
	for ( ;itor != list.end(); )
	{
		ListDepartItor itor2;
		for ( itor2= list.begin(); itor2 != list.end(); itor2++ )
		{
			if ( itor != itor2 && itor->GetId() == itor2->GetId() )
			{
				itor = list.erase( itor );
				break;
			}
		}
		if ( itor2 == list.end() )
		{
			itor++;
		}
	}
	return userExcOk;
}
BOOL CDepartSeverImp::UpdateDepartName( int id, const CString &name )
{
	if ( name.IsEmpty()  )
	{
		AfxMessageBox(_T("名称不能为空"));
		return paraError;
	}
	CDepartment departTmp;
	BOOL ret = m_pDepartDao->GetDepartmentByName( name, departTmp );
	if ( ret == userExcOk )
	{
		AfxMessageBox( _T("有同名的部门存在") );
		return paraError;
	}
	if( ret != notfind )
	{
		return ret;
	}
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("'%s'"), name );
	filds.insert( std::make_pair( fild_dep_name, strValue ));
	return m_pDepartDao->UpdateDepartField( id ,filds );
}
BOOL CDepartSeverImp::UpdateDepartDec( int id, const CString &str )
{
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("'%s'"), str );
	filds.insert( std::make_pair( fild_dep_des, strValue ));
	return m_pDepartDao->UpdateDepartField( id ,filds );
}
BOOL CDepartSeverImp::UpdateBakValidate( int id, BOOL validate )
{
	CString strValue;
	TaskIdStringMap filds;
	strValue.Format( _T("%d"), validate );
	filds.insert( std::make_pair( fild_dep_bak_validate, strValue ));
	return m_pDepartDao->UpdateDepartField( id ,filds );
}
BOOL CDepartSeverImp::UpdateDepartManger( const CDepartment &depart )
{
	CDlgUserChose dlg( FALSE );
	dlg.SetDelSelUser( depart.GetMangerId() );
	dlg.DoModal();
	const CPtrArray* pUser = dlg.GetSelArrary();
	if ( 1 == pUser->GetCount()  )
	{
		int selUserId = ( (const CUser*)pUser->GetAt( 0 ))->GetId();
		if ( selUserId != depart.GetMangerId() )
		{
			CString strValue;
			TaskIdStringMap filds;
			strValue.Format( _T("%d"), selUserId );
			filds.insert( std::make_pair( fild_dep_manager, strValue ));
			return m_pDepartDao->UpdateDepartField(depart.GetId() ,filds );
		}
	}
	return userExcOk;
}
BOOL CDepartSeverImp::UpdateDepartBakManger( const CDepartment &depart )
{
	CDlgUserChose dlg( FALSE );
	if ( depart.GetBakManger() > 0  )
	{
		dlg.SetDelSelUser( depart.GetBakManger() );
	}
	dlg.DoModal();
	const CPtrArray* pUser = dlg.GetSelArrary();
	if ( 1 == pUser->GetCount()  )
	{
		int selUserId = ( (const CUser*)pUser->GetAt( 0 ))->GetId();
		if ( selUserId != depart.GetMangerId() )
		{
			CString strValue;
			TaskIdStringMap filds;
			strValue.Format( _T("%d"), selUserId );
			filds.insert( std::make_pair( fild_dep_bak_manger, strValue ));
			return m_pDepartDao->UpdateDepartField(depart.GetId() ,filds );
		}
	}
	return userExcOk;
}
BOOL CDepartSeverImp::UpdateFatherDepart( int departId, const CString &fatherNameStr  )
{
	CString strValue;
	TaskIdStringMap filds;
	if ( fatherNameStr.IsEmpty() || fatherNameStr == _T("无")  )
	{
		strValue.Format( _T("%d"), 0 );//
		filds.insert( std::make_pair( fild_dep_top, strValue ));
		return m_pDepartDao->UpdateDepartField( departId ,filds );
	}

;   CDepartment fatherDepart;
    BOOL ret = m_pDepartDao->GetDepartmentByName( fatherNameStr, fatherDepart );
	if ( ret == notfind )
	{
		AfxMessageBox( _T("所选上级部门不存在") );
		return paraError;
	}
	if ( ret != userExcOk )
	{
		return ret;
	}

	strValue.Format( _T("%d"), fatherDepart.GetId() );
	filds.insert( std::make_pair( fild_dep_top, strValue ));
	return m_pDepartDao->UpdateDepartField(departId ,filds );

	return userExcOk;
}
BOOL  CDepartSeverImp::DeleteDepart( int departId )
{
	ListUser tempMember;
	BOOL ret = m_pDbDao->GetUserDao()->GetDepartMember( departId, tempMember );
	if ( tempMember.size() > 0 )
	{
		AfxMessageBox( _T("部门有成员存在,请先修改成员部门，再删除部门") );
		return paraError;
	}
	if ( ret != userExcOk )
	{
		return ret;
	}
	return m_pDepartDao->DeleteDepart( departId );
}
BOOL CDepartSeverImp::InsertDepart( CDepartment &obj )
{
	return m_pDepartDao->AddDepartement( obj );
}