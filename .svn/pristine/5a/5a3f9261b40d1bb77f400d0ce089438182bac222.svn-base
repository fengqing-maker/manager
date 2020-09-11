#include "stdafx.h"
#include "projectManger.h"
#include "resource.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CProjectViewManger::CProjectViewManger( CActionServerFactory *pSeverFactory )
	                 :CPrjBaseGridManger( pSeverFactory )
{
	m_pLoginUser = &m_pSeverFactory->GetLoginServer()->GetLoginUser();
	m_hasModiyPrj = FALSE;
	if (_T("管理员") == m_pSeverFactory->GetLoginServer()->GetLoginRole().GetRoleName() )
	{
		m_hasModiyPrj = TRUE;
	}
}
void CProjectViewManger::CreateColumnMap()
{
	InsertPrjNameColumn();
	InsertPrjLongNameColumn();
	InsertPrjLine();
	InsertPrjStepType();
	InsertPrjStep(); //所属阶段
	InsertBegongToStep(); //应属阶段
	InsertTaskOwernColum();
	InsertPrjMember();
	InsertUpDownColum();

}
void CProjectViewManger::SetShowPrject( int prjLineId /*= -1 */ )
{
	m_showTaskMap.clear();
	ListProject::const_iterator itor = m_allProject.begin();
	int cnt = 0;
	int index = 0;
	while( itor != m_allProject.end() )
	{
		BOOL isInsert = false;
		if ( prjLineId < 0 )
		{
			isInsert = true;
		}else if (  prjLineId == itor->GetLineId() )
		{
			isInsert = true;
		}
		if ( isInsert )
		{
			CString obj;
			m_showTaskMap.insert( std::make_pair( index++, cnt ) );
		}
		itor++;
		cnt++;
	}
}
void  CProjectViewManger::RefreshProject( )
{
	UpdateProject();
	SetShowPrject();
	UpdateGridView();
}
void  CProjectViewManger::RefreshProjectDataFromDb()
{
	UpdateProject();
	m_pProjectSever->GetAllProjectLine( m_allPrjLine );
}
void CProjectViewManger::CreateRowItemMap()
{
	RefreshProjectDataFromDb();
	SetShowPrject();
}
CProject* CProjectViewManger::GetProjectByRow( int nRow )
{
	return CPrjBaseGridManger::GetProjectByRow(nRow, m_allProject, m_showTaskMap );
}

CString CProjectViewManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return text;
	CProject *pObj = GetProjectByRow( nRow );
	if ( NULL == pObj )
	{
		return text;
	}
	const CProject &obj = *pObj;
    switch( colItor->second->dataType )
	{
	case ProjectType_Colom:
		text = obj.GetShortName();
		break;
	case  PrjLongName:
		text = obj.GetLongName();
		break;
	case  PrjLine:
		GetProjectLineNameById( obj.GetLineId(), text );
		break;
	case  taskOwern://项目负责人
		text = GetOwnerNameStr( obj.GetMangerId() );
		break;
	case  PrjMember://项目成员
		GetProjectMemberString( obj.GetId(), text );
		break;
	case  PrjStepType://项目类型
		text = obj.GetPrjStepType();
		break;
	case  PrjNowStep://所属阶段 
	    m_pProjectSever->GetProjectFactStep( obj, text );
		break;
	case  PrjBelongToStep:	//所属阶段
		m_pProjectSever->GetProjectBelongStep( obj, text );
		break;
	case  TaskOperate1://操作
		text = _T("查看任务");
		break;
	}
	return text;
}
BOOL     CProjectViewManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	if ( FALSE == m_hasModiyPrj || colItor == m_gridColumnMap.end())
		return TRUE;
	CProject *pObj = GetProjectByRow( nRow );
	if ( NULL == pObj )
	{
		return TRUE;
	}
	CProject &obj = *pObj;
	BOOL ret = -1;
	switch( colItor->second->dataType )
	{
	case ProjectType_Colom:
		if ( text != obj.GetShortName() )
		{
			ret = m_pProjectSever->UpdatePrjShortName( obj.GetId(), text );
		}
		break;
	case PrjLongName:
		if ( text != obj.GetLongName() )
		{
			ret = m_pProjectSever->UpdatePrjLongName( obj.GetId(), text );
		}
		break;
	case PrjLine:
		{
			int baseRow = GetFixedRowCount();
			int baseCol = GetFixedColumnCount();
			CGridCellCombo* pCell = 
				dynamic_cast<CGridCellCombo*>(m_gridCtrl->GetCell(nRow+baseRow,nColumn+baseCol));
			int selLine = pCell->GetCurSel();
			if ( selLine >= 0 && m_allPrjLine.size() > 0 )
			{
				ListProjectLine::iterator itor = m_allPrjLine.begin();
				advance( itor, selLine );
				if ( itor != m_allPrjLine.end() && itor->GetId() != obj.GetLineId() )
				{
					ret = m_pProjectSever->UpdatePrjProductLine( obj.GetId(), itor->GetId() );
				}
			}
			if ( ret != userExcOk )
			{
				selLine = 0;
				for ( ListProjectLine::iterator itor = m_allPrjLine.begin(); itor != m_allPrjLine.end(); itor++)
				{
					if ( itor->GetId() ==  obj.GetLineId())
					{
						break;
					}
					selLine++;
				}
				pCell->SetCurSel( selLine );
			}
		}
		break;

	}
    if ( userExcOk != ret )
    {
		RefreshRow( nRow, nColumn );
    }else
	{//更新项目
		m_pProjectSever->GetProjectById( obj.GetId(), obj );
	}
	return TRUE;
}
BOOL     CProjectViewManger::GetFlexItemEditable(int nRow,int nColumn)
{
	if ( m_hasModiyPrj )
	{
		GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
		if ( colItor != m_gridColumnMap.end() )
		{
			switch( colItor->second->dataType )
			{
			case  ProjectType_Colom:
			case  PrjLongName:
			case  PrjLine:
				return TRUE;
			}
		}
	}
	return FALSE;
}

void  CProjectViewManger::ChangeSelProjectLine( int selIndex )
{
	int lineId = -1;
	if ( selIndex > 0 && m_allPrjLine.size() > 0 )
	{
		ListProjectLine::iterator itor =  m_allPrjLine.begin();
		advance( itor, selIndex -1 );
		if ( itor == m_allPrjLine.end() )
		{
			return;
		}
		lineId = itor->GetId();
	}
	SetShowPrject( lineId );
	UpdateGridView();
}
BOOL CProjectViewManger::InitFlexComboItem(CGridCellCombo* pCell,int nRow,int nColumn)
{
	ListProjectLine::const_iterator itor = m_allPrjLine.begin();
	CStringArray lineString;
	while ( itor != m_allPrjLine.end() )
	{
		lineString.Add( itor->GetName() );
		itor++;
	}
	pCell->SetOptions( lineString );
	pCell->SetStyle( CBS_DROPDOWN );
	return TRUE;
}
void CProjectViewManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	CPoint pt;
	GetCursorPos( &pt );
	if (m_hasModiyPrj && nRow >= 0 && nColumn >= 0 )
	{
		
		GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
		BOOL ret = -1;
		CProject* pPrjObj = GetProjectByRow( nRow );
		if (NULL == pPrjObj )
		{
			return;
		}
		CMenu PopupMenu;
		PopupMenu.CreatePopupMenu();
		CString str;
		if ( PrjMember == itor->second->dataType )
		{
			str.LoadString( IDS_STR_EDIT );
			PopupMenu.AppendMenu(MF_ENABLED, IDM_PROJECT_MODIY_MEMBER, str);
			PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			return;
		}
		else if ( taskOwern == itor->second->dataType)
		{
			str.LoadString( IDS_STR_EDIT );
			PopupMenu.AppendMenu(MF_ENABLED, IDM_PROJECT_MODIY_OWENR, str);
			PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			return;
		}

	}
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if (m_hasModiyPrj && range.GetMinRow() >= GetFixedRowCount() &&
		 GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择
		CString str;
		CMenu PopupMenu;
		PopupMenu.CreatePopupMenu();
		str.LoadString( IDS_STR_DELETE );
		PopupMenu.AppendMenu(MF_ENABLED, IDM_PROJECT_DEL_PROJECT, str);
		PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	}
}
void CProjectViewManger::OnDeleteProject()
{
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if (m_hasModiyPrj && range.GetMinRow() >= GetFixedRowCount() )
	{
		int row;
		BOOL bUpdateGrid = false;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
			row = i - GetFixedRowCount();
			CProject* pPrjObj = GetProjectByRow( row );
			if ( pPrjObj )
			{
				BOOL ret = m_pProjectSever->DeleteProject( pPrjObj->GetId() );
				if ( ret == userExcOk )
				{
					bUpdateGrid = TRUE;
				}else
				{
					CString str;
					str.Format(  _T("删除项目'%s'失败,原因%s"), pPrjObj->GetShortName(), m_pSeverFactory->GetErrorString( ret ) );
					AfxMessageBox( str );
					break;
				}
			}
		}
		if ( bUpdateGrid )
		{
			RefreshProject( );
		}
	
	}
}
//点击查看
void CProjectViewManger::OnGridClick( NMHDR* pNMHDR,LRESULT* pResult  )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();
	if ( nRow >= 0 && nColumn >= 0 )
	{
		GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
		BOOL ret = -1;
		CProject* pPrjObj = GetProjectByRow( nRow );
		if (NULL == pPrjObj )
		{
			return;
		}
		if ( TaskOperate1 == itor->second->dataType )
		{//查看任务

			HWND hFatherFahterFahther = m_gridCtrl->GetParent()->GetParent()->GetParent()->GetSafeHwnd();
		    SendMessage( hFatherFahterFahther,PRJ_MANGER_MSG_CHANGE, pPrjObj->GetId(), NULL);
		}
	}
}
void  CProjectViewManger::OnChangeOwner(  )
{
	//
	int nRow = ValidateSelectColum( taskOwern );
	CProject *pProject = NULL;
	if ( nRow >= 0 && NULL != (pProject = GetProjectByRow(nRow) ) )
	{
		BOOL ret = m_pProjectSever->UpdatePrjOwner( *pProject );
		if ( userExcOk == ret )
		{
			m_pProjectSever->GetProjectById( pProject->GetId(), *pProject );
			UpdateGridRowView( nRow );
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		}
	}
}
int  CProjectViewManger::ValidateSelectColum( int colDataType )
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
void CProjectViewManger::OnChangePrjMember(  )
{
	int nRow = ValidateSelectColum( PrjMember );
	CProject *pProject = NULL;
	if ( nRow >= 0 && NULL != (pProject = GetProjectByRow(nRow) ) )
	{
		BOOL ret = m_pProjectSever->UpdatePrjMember( *pProject );
		if ( userExcOk == ret )
		{
			m_pProjectSever->GetProjectById( pProject->GetId(), *pProject );
			UpdateGridRowView( nRow );
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		}
	}

}