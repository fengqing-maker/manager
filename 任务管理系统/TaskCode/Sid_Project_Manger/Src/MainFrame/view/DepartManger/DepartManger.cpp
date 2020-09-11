#include "stdafx.h"
#include "DepartManger.h"
#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDepartViewManger::CDepartViewManger( CActionServerFactory *pSeverFactory )
	                 :CDepartBaseGridManger( pSeverFactory )
{
	m_pLoginUser = &m_pSeverFactory->GetLoginServer()->GetLoginUser();
	m_hasModiyPrj = FALSE;
	if (_T("管理员") == m_pSeverFactory->GetLoginServer()->GetLoginRole().GetRoleName() )
	{
		m_hasModiyPrj = TRUE;
	}
}
void CDepartViewManger::CreateColumnMap()
{
	InsertDepartNameColumn();
	InsertFahterDepart();
	InsertDepartDecColumn();
	InsertTaskOwernColum();
	InsertAgentUser();
	InsertAgentValidate();
	InsertDepartMember();
	InsertUpDownColum();

}

void  CDepartViewManger::RefreshDepartment( )
{
	UpdateDepart();
	UpdateGridView();
	m_gridCtrl->ResetScrollBars();
}

void CDepartViewManger::CreateRowItemMap()
{
	RefreshDepartment();
}
CDepartment* CDepartViewManger::GetDepartByRow( int nRow )
{
	return CDepartBaseGridManger::GetDepartByRow( nRow, m_allDepart );
}

CString CDepartViewManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	CDepartment *pObj = GetDepartByRow( nRow );
	if ( NULL == pObj )
	{
		return text;
	}
	const CDepartment &obj = *pObj;
    switch( colItor->second->dataType )
	{
	case DepartName:
		text = obj.GetName();
		break;
	case  DepartDec:
		text = obj.GetDec();
		break;
	case  DepartMember:
		GetDepartMemberString( obj.GetId(), text );
		break;
	case  taskOwern://部门负责人
		text = GetOwnerNameStr( obj.GetMangerId() );
		break;
	case  DepartAgentUser://代理人
		if ( obj.GetBakManger() > 0 )
		{
			text = GetOwnerNameStr( obj.GetBakManger() );			
		}else
		{
			text = _T("无");
		}
		break;
	case DepartAgentValidate:
		{
            text = _T("启动代理");
		   	int baseRow = GetFixedRowCount();
		   	int baseCol = GetFixedColumnCount();
		   	CGridCellCheck* pCell = 
		   		dynamic_cast<CGridCellCheck*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
			if ( pCell )
			{
				pCell->SetCheck( pObj->GetBakValidate() );
			}
		}
		break;
	case DepartFather: //上级部门
		if ( obj.GetTopDepartId() > 0 )
		{
			GetDepartNameById( obj.GetTopDepartId(), text );
		}else
		{
			text = _T("无");
		}
		break;
	case  TaskOperate1://操作
		text = _T("查看任务");
		break;
	}
	return text;
}
BOOL     CDepartViewManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if ( colItor == m_gridColumnMap.end() )
	{
		return TRUE;
	}
	CDepartment *pObj = GetDepartByRow( nRow );
	if ( NULL == pObj )
	{
		return TRUE;
	}
	CDepartment &obj = *pObj;

	int baseRow = GetFixedRowCount();
	int baseCol = GetFixedColumnCount();
	CGridCellCombo* pCell = 
		dynamic_cast<CGridCellCombo*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
	BOOL ret = paraError;
	switch( colItor->second->dataType )
	{
	case DepartName:
		if ( text != obj.GetName() )
		{
			ret = m_pDepartSever->UpdateDepartName( obj.GetId(), text ); 
		}
		break;
	case DepartDec:
		if ( text != obj.GetDec() )
		{
			ret = m_pDepartSever->UpdateDepartDec( obj.GetId(), text ); 
		}
		break;
	case DepartFather:
		{//
			ret = m_pDepartSever->UpdateFatherDepart( obj.GetId(), text );
			int selCul = 0;
			for ( ListDepartConstItor itor = m_allDepart.end(); itor != m_allDepart.end(); itor++ )
			{
				if ( itor->GetId() == obj.GetTopDepartId() )
				{
					break; 
				}
				selCul++;
			}
			if ( selCul == m_allDepart.size() )
			{
				selCul = 0;
			}
			pCell->SetCurSel( selCul );
		}
		break;

	}
    if ( userExcOk != ret )
    {
		RefreshRow( nRow, nColumn );
    }else
	{//更新项目
		m_pDepartSever->GetDepartById( obj.GetId(), obj );
	}
	return TRUE;
}
BOOL     CDepartViewManger::GetFlexItemEditable(int nRow,int nColumn)
{
	
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if ( colItor != m_gridColumnMap.end() )
	{
		if ( DepartAgentValidate == colItor->second->dataType )
		{
			return TRUE;
		}
		if ( m_hasModiyPrj )
		{
			switch( colItor->second->dataType )
			{
			case  DepartName:
			case  DepartDec:
			case  DepartFather:
				return TRUE;
			}
		}

	}
	return FALSE;
}


BOOL CDepartViewManger::InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn)
{
	CDepartment *pObj = GetDepartByRow( nRow );
	if ( NULL == pObj )
	{
		return TRUE;
	}
	ListDepart::const_iterator itor = m_allDepart.begin();
	CStringArray lineString;
	lineString.Add( _T("无") );
	int count = 1, selIndex = 0;
	while ( itor != m_allDepart.end() )
	{
		if ( pObj->GetName() != itor->GetName() )
		{
			lineString.Add( itor->GetName() );
			if ( pObj->GetTopDepartId() == itor->GetId() )
			{ 
				selIndex = count;
			}
		}
		count++;
		itor++;
	}
	pCell->SetOptions( lineString );
	pCell->SetStyle( CBS_DROPDOWN );
	pCell->SetCurSel( selIndex );
	return TRUE;
}
BOOL    CDepartViewManger::InitFlexCheckItem(CGridCellCheck* pCell,int nRow,int nColumn)
{
	return TRUE;
}
void CDepartViewManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	CPoint pt;
	GetCursorPos( &pt );
	if ( nRow >= 0 && nColumn >= 0 )
	{
		
		GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
		BOOL ret = -1;
		CDepartment* pPrjObj = GetDepartByRow( nRow );
		if (NULL == pPrjObj )
		{
			return;
		}
		CMenu PopupMenu;
		PopupMenu.CreatePopupMenu();
		CString str;
	    if ( m_hasModiyPrj && taskOwern == itor->second->dataType)
		{//负责人
			str.LoadString( IDS_STR_EDIT );
			PopupMenu.AppendMenu(MF_ENABLED, IDM_DEPART_MODIY_MANGER, str);
			PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			return;
		}else if ( DepartAgentUser == itor->second->dataType )
		{//代理人
			str.LoadString( IDS_STR_EDIT );
			PopupMenu.AppendMenu(MF_ENABLED, IDM_DEPART_MODIY_BAKMNAGER, str);
			PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			return;
		}

	}
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if (m_hasModiyPrj && range.GetMinRow() >= GetFixedRowCount() &&
		 GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择， 删除
		CString str;
		CMenu PopupMenu;
		PopupMenu.CreatePopupMenu();
		str.LoadString( IDS_STR_DELETE );
		PopupMenu.AppendMenu(MF_ENABLED, IDM_DEL_DEPART, str);
		PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	}
}
void CDepartViewManger::OnDeleteDepart()
{
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if (m_hasModiyPrj && range.GetMinRow() >= GetFixedRowCount() )
	{
		int row;
		BOOL bUpdateGrid = false;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
			row = i - GetFixedRowCount();
			CDepartment* pObj = GetDepartByRow( row );
			if ( pObj )
			{
				BOOL ret = m_pDepartSever->DeleteDepart( pObj->GetId() ); 
				if ( ret == userExcOk )
				{
					bUpdateGrid = TRUE;
				}else
				{
					if ( ret != paraError )
					{
						CString str;
						str.Format(  _T("删除部门'%s'失败,原因%s"), pObj->GetName(), m_pSeverFactory->GetErrorString( ret ) );
						AfxMessageBox( str );
					}
					break;
				}
			}
		}
		if ( bUpdateGrid )
		{
			RefreshDepartment( );
		}
	
	}
}
//点击查看
void CDepartViewManger::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult  )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	if ( nRow >= 0 && nColumn >= 0 )
	{
		GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
		BOOL ret = -1;
		CDepartment* pObj = GetDepartByRow( nRow );
		if (NULL == pObj )
		{
			return;
		}
		if ( TaskOperate1 == itor->second->dataType )
		{//查看任务
			HWND hFatherFahterFahther = m_gridCtrl->GetParent()->GetParent()->GetParent()->GetSafeHwnd();
		    SendMessage( hFatherFahterFahther,DEPART_MANGER_MSG_CHANGE, pObj->GetId(), NULL);
		}
		if ( DepartAgentValidate == itor->second->dataType )
		{
			CGridCellCheck* pCell = 
				dynamic_cast<CGridCellCheck*>(m_gridCtrl->GetCell(gridView->iRow, gridView->iColumn));
			if ( pCell && pObj->GetBakValidate() != pCell->GetCheck() )
			{
				if ( pObj->GetBakManger() > 0 )
				{
					BOOL ret = m_pDepartSever->UpdateBakValidate( pObj->GetId(), pCell->GetCheck() );
					if ( ret == userExcOk )
					{
						m_pDepartSever->GetDepartById( pObj->GetId(), *pObj );
					}
				}else
				{
					AfxMessageBox(_T("请选择代理人后再启动代理"));
				}
				UpdateGridRowView( nRow );

			}
			
		}
	}
}
void  CDepartViewManger::OnChangeOwner(  )
{
	//
	int nRow = ValidateSelectColum( taskOwern );
	CDepartment *pObj = NULL;
	if ( nRow >= 0 && NULL != (pObj = GetDepartByRow( nRow ) ) )
	{
		BOOL ret = m_pDepartSever->UpdateDepartManger( *pObj );
		if ( userExcOk == ret )
		{
			m_pDepartSever->GetDepartById( pObj->GetId(), *pObj );
			UpdateGridRowView( nRow );
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		}
	}
}
int  CDepartViewManger::ValidateSelectColum( int colDataType )
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
void CDepartViewManger::OnChangeDepartBakManger()
{
	//
	int nRow = ValidateSelectColum( DepartAgentUser );
	CDepartment *pObj = NULL;
	if ( nRow >= 0 && NULL != (pObj = GetDepartByRow( nRow ) ) )
	{
		BOOL ret = m_pDepartSever->UpdateDepartBakManger( *pObj );
		if ( userExcOk == ret )
		{
			m_pDepartSever->GetDepartById( pObj->GetId(), *pObj );
			UpdateGridRowView( nRow );
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		}
	}

}
