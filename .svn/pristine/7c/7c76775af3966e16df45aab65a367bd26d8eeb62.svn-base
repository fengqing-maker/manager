#include "stdafx.h"
#include "UserGridManger.h"
#include "resource.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUserGridManger::CUserGridManger(  CActionServerFactory *pSeverFactory )
	               :CBackBaseGridManger(pSeverFactory)
{
	m_selDepart = 0;
}
void   CUserGridManger::InsertDepart()
{
	InsertColumn( _T("所在部门"), 150, DT_LEFT, GridItemType_Combo,  UserDepartName );
}
void   CUserGridManger::InsertUserName()
{
	InsertColumn( _T("姓名"), 150, DT_LEFT, GridItemType_Edit,  UserName );
}
void   CUserGridManger::InsertUserLoginName()
{
	InsertColumn( _T("登陆名"), 150, DT_LEFT, GridItemType_Edit,  UserLoginName );
}
void   CUserGridManger::InsertUserRoleName()
{
	InsertColumn( _T("角色"), 150, DT_LEFT, GridItemType_Combo,  UserRoleName );
}
void   CUserGridManger::InsertUserStatus()
{
	InsertColumn( _T("在职状态"), 150, DT_LEFT, GridItemType_Combo,  UserStats );
}
void   CUserGridManger::InsertUserOnline()
{
	InsertColumn( _T("在线状态"), 150, DT_LEFT, GridItemType_Combo,  UserOnline );
}
void    CUserGridManger::InsertUserExportSeq()
{
	InsertColumn( _T("导出顺序"), 150, DT_LEFT, GridItemType_NumEidt,  UserExportSeq );
}

void CUserGridManger::CreateColumnMap()
{
	InsertUserName();
	InsertUserLoginName();
	InsertDepart();
	InsertUserRoleName();
	InsertUserStatus();
	InsertUserOnline();
	InsertUserExportSeq();
}
void CUserGridManger::UpdateDb()
{
	m_allUser.clear();
	m_pLoginSever->GetAllUser( m_allUser );
	RefreshShowMap( m_selDepart );
	UpdateGridView();
}
void CUserGridManger::CreateRowItemMap()
{
	m_allUser.clear();
	m_pLoginSever->GetAllUser( m_allUser );
	RefreshShowMap();
	m_workStatusMap.clear();
    CUser::GetOnWorkStrings( m_workStatusMap );
	m_onLineStatusMap.clear();
	CUser::GetOnLineStrings( m_onLineStatusMap );
	m_allDepart.clear();
	m_pSeverFactory->GetDepartSever()->GetAllDepart( m_allDepart );
	m_allRole.clear();
	m_pBackSever->GetAllRole( m_allRole );
}
void CUserGridManger::UpdateGridByDepart( int departId )
{
	RefreshShowMap( departId );
	UpdateGridView();
}
void CUserGridManger::RefreshShowMap( int showDepart/* = -1*/)
{
	m_showTaskMap.clear();
	m_selDepart = showDepart;
	int userPos = 0; 
	int showPos = 0;
	for ( ListUserItor itor = m_allUser.begin(); itor != m_allUser.end(); itor++ )
	{
		if ( showDepart<= 0 || showDepart == itor->GetDepartId() )
		{
			m_showTaskMap.insert( std::make_pair( showPos++, userPos ) );
		}
		userPos++;
	}
}
CUser* CUserGridManger::GetShowUserByRow( int nRow )
{
	if ( m_allUser.size() <= 0)
	{
		return NULL;
	}
	IntToIntMap::const_iterator dataItor = m_showTaskMap.find( nRow );
	if ( dataItor != m_showTaskMap.end() || dataItor->second < 0 )
	{
		ListUserItor itor = m_allUser.begin();
		advance( itor, dataItor->second );
		if ( itor == m_allUser.end() )
		{
			return NULL;
		}
		return &(*itor);
	}
	return NULL;
}
CUser*  CUserGridManger::ValidateRow( int nRow,int nColumn, GridColumnMapItor &colItor  )
{
	colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return NULL;
	return GetShowUserByRow( nRow );
}
CString CUserGridManger::GetFlexItemText(int nRow, int nColumn)
{
  CString text;
  GridColumnMapItor colItor;
  CUser *pShowUser = ValidateRow( nRow, nColumn, colItor );
  if ( !pShowUser )
  {
	  return text;
  }
  switch( colItor->second->dataType )
  {
  case UserDepartName:
	  {
		  CDepartment depart;
		  m_pSeverFactory->GetDepartSever()->GetDepartById( pShowUser->GetDepartId(), depart );
		  text = depart.GetName();
	  }
	  break;
  case UserName:
	  text = pShowUser->GetUserName_R();
	  break;
  case UserLoginName:
	  text = pShowUser->GetLoginName();
	  break;
  case UserRoleName:
	  {
		  CRole role;
		  m_pLoginSever->GetRoleById( pShowUser->GetRolId(), role );
		  text = role.GetRoleName();
	  }
	  break;
  case UserStats:
	  pShowUser->GetOnWorkString( pShowUser->GetWorkStatus(), text );
	  break;
  case UserOnline:
	  pShowUser->GetOnLineString( pShowUser->GetOnLine(), text );
	  break;
  case  UserExportSeq:
	  text.Format( _T("%d"), pShowUser->GetExportSeq() );
	  break;
  }
  return  text;
}
BOOL    CUserGridManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor;
	CUser *pShowUser = ValidateRow( nRow, nColumn, colItor );
	if ( !pShowUser )
	{
		return TRUE;
	}
	BOOL ret = paraError;
	int baseRow = GetFixedRowCount();
	int baseCol = GetFixedColumnCount();
	CGridCellCombo* pCell = 
		dynamic_cast<CGridCellCombo*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
	switch( colItor->second->dataType )
	{
	case UserDepartName://更改部门
		{
			int selCul = pCell->GetCurSel();
			if ( m_allDepart.size() > 0 && selCul >= 0 )
			{
				ListDepartConstItor itor = m_allDepart.begin();
				advance( itor, selCul );
				if ( itor != m_allDepart.end() )
				{
					ret = m_pBackSever->UpdateUserDepart( pShowUser->GetId(), itor->GetId() );
				}
			}
			if ( ret != userExcOk )
			{
				int selCul = 0;
				for ( ListDepartConstItor itor = m_allDepart.end(); itor != m_allDepart.end(); itor++ )
				{
					if ( itor->GetId() == pShowUser->GetDepartId() )
					{
						break; 
					}
					selCul++;
				}
				pCell->SetCurSel( selCul );
			}
		}
		break;
	case UserName:
		ret = m_pBackSever->UpdateUserName(pShowUser->GetId(), text );
		break;	
	case UserLoginName:
		ret = m_pBackSever->UpdateUserLoginName( pShowUser->GetId(), text );
		break;	
	case UserRoleName:
		{
			int selCul = pCell->GetCurSel();
			if ( m_allRole.size() > 0 && selCul >= 0)
			{
				ListRoleItor itor = m_allRole.begin();
				advance( itor, selCul );
				if ( itor != m_allRole.end() )
				{
					ret = m_pBackSever->UpdateUserRole( pShowUser->GetId(), itor->GetId() );
				}
			}
		}
		break;	
	case UserStats:
		{
			ret = m_pBackSever->UpdateUserStatus( pShowUser->GetId(), text );
			if ( ret != userExcOk )
			{
				pCell->SetCurSel( pShowUser->GetWorkStatus() );
			}
		}
		break;	
	case UserOnline:
		{
			int selCul = pCell->GetCurSel();
			if ( m_workStatusMap.size() > 0 && selCul >= 0 )
			{
				IntStringMapConstItor itor = m_workStatusMap.begin();
				advance( itor, selCul );
				if ( itor != m_workStatusMap.end() && itor->first == leftLine )
				{
					if ( IDYES == AfxMessageBox(_T("是否确认执行强制让此用户下线"), MB_YESNO ) )
					{
						ret = m_pBackSever->UpdateUserOnlineStatus( pShowUser->GetId(), itor->first );
					}
				}	
			}
			if ( ret != userExcOk )
			{
				pCell->SetCurSel( pShowUser->GetOnLine() );
			}
		}
		break;	
	case  UserExportSeq:
		{
			//	int value = (int)( ((CGridCellNumeric *)pCell)->GetNumber() );
			int value;
			if ( CUtilityTool::GetObj()->ConvertStringToInt(text,  value ) )
			{
				ret = m_pBackSever->UpdateExportSeq(pShowUser->GetId(), value ); 
			}
		}
		break;
	}
	//
	if ( userExcOk == ret )
	{
		m_pLoginSever->GetUserById( pShowUser->GetId(), *pShowUser );
	}
	RefreshRow( nRow, nColumn );
	return TRUE;
}
BOOL    CUserGridManger::InitFlexNumEditItem(CGridCellNumeric* pCell,int nRow,int nColumn)
{
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor != m_gridColumnMap.end())
	{
		if ( UserExportSeq == itor->second->dataType )
		{//
			pCell->SetFlags( CGridCellNumeric::Integer );

		}
	}
	return TRUE;
}
BOOL   CUserGridManger::GetFlexItemEditable(int nRow,int nColumn)
{
	GridColumnMapItor colItor;
	CUser *pShowUser = ValidateRow( nRow, nColumn, colItor );
	if ( !pShowUser )
	{
		return TRUE;
	}
	if ( UserOnline == colItor->second->dataType  && leftLine == pShowUser->GetUserStatus() )
	{
		return FALSE;
	}
	return TRUE;
}
BOOL  CUserGridManger::InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn)
{
	GridColumnMapItor colItor;
	CUser *pShowUser = ValidateRow( nRow, nColumn, colItor );
	if ( !pShowUser )
	{
		return TRUE;
	}
	CStringArray strs;
    switch ( colItor->second->dataType )
	{
	case UserDepartName:
		for ( ListDepartConstItor itor = m_allDepart.begin(); itor != m_allDepart.end(); itor++ )
		{
			strs.Add( itor->GetName() );
		}
		break;
	case  UserRoleName:
		for ( ListRoleItor itor = m_allRole.begin(); itor != m_allRole.end(); itor++ )
		{
			strs.Add( itor->GetRoleName() );
		}
		break;
	case  UserStats:
		for ( IntStringMapConstItor itor = m_workStatusMap.begin(); itor != m_workStatusMap.end(); itor++ )
		{
			strs.Add( itor->second );
		}
		break;
	case  UserOnline:
		for ( IntStringMapConstItor itor = m_onLineStatusMap.begin(); itor != m_onLineStatusMap.end(); itor++ )
		{
			strs.Add( itor->second );
		}
		break;
	}
	pCell->SetOptions( strs );
	return TRUE;
}
void CUserGridManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择， 删除
		CString str;
		CMenu PopupMenu;
		PopupMenu.CreatePopupMenu();
		str.LoadString( IDS_STR_CLEARN_PWD );
		PopupMenu.AppendMenu(MF_ENABLED, IDM_USER_CLEARN_PWD, str);
		str.LoadString( IDS_STR_DELETE );
		PopupMenu.AppendMenu( MF_ENABLED, IDM_DELTE_OBJ, str );
		PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	}
}
void  CUserGridManger::OnClearnPwd()
{
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		CUser *pUser;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
			int nRow = i - GetFixedRowCount();
			pUser = GetShowUserByRow( nRow );
			if ( pUser )
			{
				BOOL ret = m_pBackSever->ClearnUserLoginPwd( pUser->GetId() );
				if ( ret != userExcOk )
				{
					CString str;
					str.Format( _T("清空用户'%s'密码出错，原因%s "), pUser->GetUserName_R() );
					AfxMessageBox( str );
					return;
				}
			}
		}
		AfxMessageBox(_T("清空密码成功"));
	}
}
void CUserGridManger::OnDeleteUser()
{
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		CUser *pUser;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
			int nRow = i - GetFixedRowCount();
			pUser = GetShowUserByRow( nRow );
			if ( pUser )
			{
				BOOL ret = m_pBackSever->DeleteUser( pUser->GetId() );
				if ( ret != userExcOk )
				{
					CString str;
					str.Format( _T("删除用户'%s'失败，有可能有关联任务或为部门产品线项目负责人，请先删除相信息再做此操作"), 
						        pUser->GetUserName_R() );
					AfxMessageBox( str );
					return;
				}
			}
		}
		UpdateDb();
	
	}
}