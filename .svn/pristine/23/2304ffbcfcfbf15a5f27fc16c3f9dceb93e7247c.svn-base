#include "stdafx.h"
#include "WorkTimeTypeGridManger.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CWorkTimeTypeGridManger::CWorkTimeTypeGridManger(  CActionServerFactory *pSeverFactory )
	:CBackBaseGridManger(pSeverFactory)
{
	m_projectSever = pSeverFactory->GetProjectSever();
}

void   CWorkTimeTypeGridManger::InsertId()
{
	InsertColumn( _T("编号"), 150, DT_LEFT, GridItemType_Edit,  workTimeTypeId );
}
void   CWorkTimeTypeGridManger::InsertName()
{
	InsertColumn( _T("工时名称"), 300, DT_LEFT, GridItemType_Edit,  workTimeTypeName );
}

void CWorkTimeTypeGridManger::CreateColumnMap()
{
	InsertId();
	InsertName();
}
void CWorkTimeTypeGridManger::CreateRowItemMap()
{
	m_workTypeMap.clear();
	m_pBackSever->GetAllWorkType( m_workTypeMap );
}
void CWorkTimeTypeGridManger::UpdateGrid()
{
	CreateRowItemMap();
	UpdateGridView();
}

BOOL CWorkTimeTypeGridManger::GetWorkTypeByRow( int nRow, TaskIdStringMapItor &itor  )
{
	if ( m_workTypeMap.size() <= 0 || nRow < 0 )
	{
		return FALSE;
	}
    itor = m_workTypeMap.begin();
	advance( itor, nRow );
	if ( itor != m_workTypeMap.end() )
	{
		return TRUE;
	}
	return FALSE;
}
BOOL  CWorkTimeTypeGridManger::ValidateRow( int nRow, int nColumn, GridColumnMapItor &colItor,TaskIdStringMapItor &dataItor )
{
	colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return NULL;
	return GetWorkTypeByRow( nRow, dataItor );
}

CString CWorkTimeTypeGridManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor;
	TaskIdStringMapItor dataItor;
	if ( !ValidateRow( nRow, nColumn, colItor, dataItor ) )
	{
		return text;
	}
	switch( colItor->second->dataType )
	{
	case workTimeTypeId:
		text.Format( _T("%d"), dataItor->first );
		break;
	case workTimeTypeName:
		text = dataItor->second;
		break;
	}
	return  text;
}
BOOL    CWorkTimeTypeGridManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor;
	TaskIdStringMapItor dataItor;
	if ( !ValidateRow( nRow, nColumn, colItor, dataItor ) )
	{
		return TRUE;
	}
	BOOL ret = paraError;
	if (  workTimeTypeName == colItor->second->dataType)
	{
		ret = userExcOk;
		if ( text != dataItor->second )
		{
			ret = m_pBackSever->UpdateWorkTypeStringById( dataItor->first, text);
		}
	}
	//
	if ( ret != userExcOk )
	{
		AfxMessageBox( m_pSeverFactory->GetErrorString( ret) );
		CString text;
	}else
	{
		if ( userExcOk == m_pBackSever->GetWorkTypeString( dataItor->first, text ) )
		{
			dataItor->second = text;
		}
	}
	RefreshRow( nRow, nColumn);
	return TRUE;
}
BOOL   CWorkTimeTypeGridManger::GetFlexItemEditable(int nRow,int nColumn)
{
	GridColumnMapItor colItor;
	TaskIdStringMapItor dataItor;
	if ( !ValidateRow( nRow, nColumn, colItor, dataItor ) )
	{
		return TRUE;
	}
	if ( workTimeTypeId == colItor->second->dataType  )
	{
		return FALSE;
	}
	return TRUE;
}


void CWorkTimeTypeGridManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	CPoint pt;
	GetCursorPos( &pt );

	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();

	CMenu PopupMenu;
	PopupMenu.CreatePopupMenu();
	CString str;

	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择， 删除
		str.LoadString( IDS_STR_DELETE );
		PopupMenu.AppendMenu( MF_ENABLED, IDM_DELTE_OBJ, str );
		PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	}
}

void CWorkTimeTypeGridManger::OnDelete()
{
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		TaskIdStringMapItor dataItor;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
				int nRow = i - GetFixedRowCount();
			if ( GetWorkTypeByRow( nRow, dataItor ))
			{
				BOOL ret =  m_pBackSever->DeleteWorkTypeString( dataItor->first );
				if ( ret != userExcOk )
				{
					CString str;
					str.Format( _T("删除工时类型'%s'失败，可能还有任务为此工时"), 
						dataItor->second );
					AfxMessageBox( str );
					return;
				}
			}
		}
		UpdateGrid();
	}
}
