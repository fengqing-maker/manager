#include "stdafx.h"
#include "RolePowerManger.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRolePowerGridManger::CRolePowerGridManger(  CActionServerFactory *pSeverFactory )
	:CBackBaseGridManger(pSeverFactory)
{
	m_projectSever = pSeverFactory->GetProjectSever();
}

void   CRolePowerGridManger::InsertName()
{
	InsertColumn( _T("角色"), 150, DT_LEFT, GridItemType_Edit,  RoleName );
}
void   CRolePowerGridManger::InsertPowerList()
{
	InsertColumn( _T("权限列表"), 700, DT_LEFT, GridItemType_MutilEdit,  RolePowers );
}
void   CRolePowerGridManger::InsertDc()
{
	InsertColumn( _T("角色描述"), 150, DT_LEFT, GridItemType_Edit,  RoleDec );
}

void CRolePowerGridManger::CreateColumnMap()
{
	InsertName();
	InsertPowerList();
	InsertDc();
}
void CRolePowerGridManger::CreateRowItemMap()
{
	m_allRole.clear();
	m_pBackSever->GetAllRole( m_allRole );
}
void CRolePowerGridManger::UpdateGrid()
{
	CreateRowItemMap();
	UpdateGridView();
}

CRole* CRolePowerGridManger::GetRoleByRow( int nRow )
{
	if ( m_allRole.size() <= 0 || nRow < 0)
	{
		return NULL;
	}
	ListRoleItor itor = m_allRole.begin();
	advance( itor, nRow );
	if ( itor != m_allRole.end() )
	{
		return &(*itor);
	}
	return NULL;
}
CRole*  CRolePowerGridManger::ValidateRow( int nRow, int nColumn, GridColumnMapItor &colItor  )
{
	colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return NULL;
	return GetRoleByRow( nRow );
}

void   CRolePowerGridManger::GetPowerListStr( int roleId, CString &text )
{
    IntStringMap lst;
	m_pBackSever->GetPowerByRoleId( roleId, lst );
	int cnt = 0;
	for( IntStringMapItor itor = lst.begin(); itor != lst.end(); itor++ )
	{
		text += itor->second;
		text += _T("、");
		if ( cnt> 0 && cnt%7 == 0 )
		{
			text += _T("\r\n");
		}
		cnt++;
	}
}
CString CRolePowerGridManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor;
	CRole* pObj = ValidateRow( nRow, nColumn, colItor );
	if ( !pObj )
	{
		return text;
	}
	switch( colItor->second->dataType )
	{
	case RoleName:
        text = pObj->GetRoleName();
		break;
	case RolePowers:
		GetPowerListStr( pObj->GetId(), text );
		break;
	case RoleDec:
		text = pObj->GetRolDec();
		break;
	
	}
	return  text;
}
BOOL    CRolePowerGridManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor;
	CRole* pObj = ValidateRow( nRow, nColumn, colItor );
	if ( !pObj )
	{
		return TRUE;
	}
	BOOL ret = paraError;
	switch( colItor->second->dataType )
	{
	case RoleName://
		ret = userExcOk;
		if ( text != pObj->GetRoleName() )
		{
			CRole temp = *pObj;
			temp.SetRoleName( text );
			ret = m_pBackSever->UpdateRoleName( temp ); 
		}
		break;
	case RoleDec:
		ret = userExcOk;
		if ( text != pObj->GetRolDec() )
		{
			CRole temp = *pObj;
			temp.SetRoleDec( text );
			ret = m_pBackSever->UpdateRoleDec( temp ); 
		}
		break;	
	}
	//
	if ( ret == userExcOk )
	{
		m_pLoginSever->GetRoleById(  pObj->GetId(), *pObj );
	}
	RefreshRow( nRow, nColumn );
	return TRUE;
}
BOOL   CRolePowerGridManger::GetFlexItemEditable(int nRow,int nColumn)
{
	GridColumnMapItor colItor;
	CRole* pObj = ValidateRow( nRow, nColumn, colItor );
	if ( !pObj )
	{
		return TRUE;
	}
	if ( RolePowers == colItor->second->dataType  )
	{
		return FALSE;
	}
	return TRUE;
}


void CRolePowerGridManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	CPoint pt;
	GetCursorPos( &pt );

	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();

	CMenu PopupMenu;
	PopupMenu.CreatePopupMenu();
	CString str;

	//if ( nRow >= 0  )
	//{
	//	GridColumnMapItor colItor;
	//	CRole* pObj = ValidateRow( nRow, nColumn, colItor );
	//	if ( !pObj )
	//	{
	//	return;
	//	}
	//	if ( RolePowers == colItor->second->dataType)
	//	{
	//	   str.LoadString( IDS_STR_EDIT_POWER );
	//	   PopupMenu.AppendMenu(MF_ENABLED, IDM_BACK_EDIT, str);
	//	   PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	//	}
	//}
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择， 删除
		if( range.GetMinRow() == range.GetMaxRow() )
		{
			str.LoadString( IDS_STR_EDIT_POWER );
			PopupMenu.AppendMenu(MF_ENABLED, IDM_BACK_EDIT, str);
			//PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
		}
		str.LoadString( IDS_STR_DELETE );
		PopupMenu.AppendMenu( MF_ENABLED, IDM_DELTE_OBJ, str );
		PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	}
}

void CRolePowerGridManger::OnDeleteRole()
{
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		CRole *pObj;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
			int nRow = i - GetFixedRowCount();
			pObj = GetRoleByRow( nRow );
			if ( pObj )
			{
				BOOL ret =  m_pBackSever->DeleteRole( pObj->GetId()) ;
				if ( ret != userExcOk )
				{
					CString str;
					str.Format( _T("删除角色'%s'失败，可能还有用户为此角色，请先修改用户角色"), 
						pObj->GetRoleName() );
					AfxMessageBox( str );
					return;
				}
			}
		}
		UpdateGrid();
	}
}
int  CRolePowerGridManger::ValidateSelectColum( int colDataType )
{
	CCellRange selRange = m_gridCtrl->GetSelectedCellRange();
	if (TRUE != selRange.IsValid() || selRange.GetMinRow() < GetFixedRowCount()
		|| selRange.GetMinRow() != selRange.GetMaxRow() || 
		selRange.GetMinCol() != selRange.GetMaxCol() )
	{
		return -1;
	}
	int nRow = selRange.GetMaxRow() - GetFixedRowCount();
	int nColumn = selRange.GetMaxCol() - GetFixedColumnCount();
	//获取列信息
	GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	if ( itor != m_gridColumnMap.end() && colDataType == itor->second->dataType )
	{
		return nRow; 
	}
	return -1;
}
void  CRolePowerGridManger::OnChangePowers(  )
{
	//

	CCellRange selRange = m_gridCtrl->GetSelectedCellRange();
	if (TRUE != selRange.IsValid() || selRange.GetMinRow() < GetFixedRowCount()
		|| selRange.GetMinRow() != selRange.GetMaxRow()/* || 
		selRange.GetMinCol() != selRange.GetMaxCol()*/ )
	{
		return;
	}
	int nRow = selRange.GetMaxRow() - GetFixedRowCount();
	CRole *pObj = NULL;
	if ( nRow >= 0 && NULL != (pObj = GetRoleByRow( nRow ) ) )
	{
		BOOL ret = m_pBackSever->UpdateRolePower( *pObj ); 
		if ( userExcOk != ret && paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		}
		UpdateGridRowView( nRow );
	}
}