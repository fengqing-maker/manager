#include "stdafx.h"
#include "ProductLineGridManger.h"
#include "resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CProductLineGridManger::CProductLineGridManger(  CActionServerFactory *pSeverFactory )
	:CBackBaseGridManger(pSeverFactory)
{
	m_projectSever = pSeverFactory->GetProjectSever();
}

void   CProductLineGridManger::InsertName()
{
	InsertColumn( _T("产品线"), 150, DT_LEFT, GridItemType_Edit,  ProductlineName );
}
void   CProductLineGridManger::InsertProjectList()
{
	InsertColumn( _T("项目列表"), 700, DT_LEFT, GridItemType_MutilEdit,  ProductProject );
}
void   CProductLineGridManger::InsertDc()
{
	InsertColumn( _T("描述"), 150, DT_LEFT, GridItemType_Edit,  COLUM_DC );
}
void CProductLineGridManger::InsertOwernColum()
{//责任人
	InsertColumn( _T("负责人"), 100, DT_LEFT, GridItemType_Edit,  COLUM_OWNER );
}
void CProductLineGridManger::CreateColumnMap()
{
	InsertName();
	InsertDc();
	InsertOwernColum();
	InsertProjectList();
}
void CProductLineGridManger::CreateRowItemMap()
{
	m_allProductLine.clear();
	m_projectSever->GetAllProjectLine( m_allProductLine );
}
void CProductLineGridManger::UpdateGrid(  )
{
	CreateRowItemMap();
	UpdateGridView();
}

CProductLine* CProductLineGridManger::GetProductLineByRow( int nRow )
{
	if ( m_allProductLine.size() <= 0 || nRow < 0 )
	{
		return NULL;
	}
    ListProjectLine::iterator itor = m_allProductLine.begin();
	advance( itor, nRow );
	if ( itor != m_allProductLine.end() )
	{
		return &(*itor);
	}
	return NULL;
}
CProductLine*  CProductLineGridManger::ValidateRow( int nRow, int nColumn,GridColumnMapItor &colItor  )
{
	colItor = m_gridColumnMap.find(nColumn);
	if (colItor == m_gridColumnMap.end())
		return NULL;
	return GetProductLineByRow( nRow );
}
void   CProductLineGridManger::GetProjectStrOfLine( int lineId, CString &text )
{
	ListProject lst;
	m_projectSever->GetProjectByLine( lineId, lst );
	int cnt = 0;
	for( ListProject::iterator itor = lst.begin(); itor != lst.end(); itor++ )
	{
		text += itor->GetShortName();
		text += _T("、");
		if ( cnt> 0 && cnt%5 == 0 )
		{
			text += _T("\r\n");
		}
		cnt++;
	}
}
CString CProductLineGridManger::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	GridColumnMapItor colItor;
	CProductLine* pObj = ValidateRow( nRow, nColumn, colItor );
	if ( !pObj )
	{
		return text;
	}
	switch( colItor->second->dataType )
	{
	case ProductlineName:
		text = pObj->GetName();
		break;
	case ProductProject:
		GetProjectStrOfLine( pObj->GetId(), text );
		break;
	case COLUM_DC:
		text = pObj->GetDec();
		break;
	case COLUM_OWNER:
	    text = GetOwnerNameStr( pObj->GetMangerId() );
		break;
	}
	return  text;
}
BOOL    CProductLineGridManger::SetFlexItemText(int nRow,int nColumn,CString text)
{
	GridColumnMapItor colItor;
	CProductLine* pObj = ValidateRow( nRow, nColumn, colItor );
	if ( !pObj )
	{
		return TRUE;
	}
	BOOL ret = paraError;
	switch( colItor->second->dataType )
	{
	case ProductlineName://
		ret = userExcOk;
		if ( text != pObj->GetName() )
		{
			ret = m_projectSever->UpdateProductName( pObj->GetId(), text );
		}
		break;
	case COLUM_DC:
		ret = userExcOk;
		if ( text != pObj->GetDec() )
		{
			ret = m_projectSever->UpdateProductDC( pObj->GetId(), text );
		}
		break;	
	}
	//
	if ( ret == userExcOk )
	{
		m_projectSever->GetProjectLineById(  pObj->GetId(), *pObj );
	}
	RefreshRow( nRow, nColumn );
	return TRUE;
}
BOOL   CProductLineGridManger::GetFlexItemEditable(int nRow,int nColumn)
{
	GridColumnMapItor colItor;
	CProductLine* pObj = ValidateRow( nRow, nColumn, colItor );
	if ( !pObj )
	{
		return TRUE;
	}
	if ( ProductProject == colItor->second->dataType || COLUM_OWNER == colItor->second->dataType )
	{
		return FALSE;
	}
	return TRUE;
}


void CProductLineGridManger::OnGridRclick( NMHDR* pNMHDR,LRESULT* pResult )
{
	CPoint pt;
	GetCursorPos( &pt );
	
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	int nRow = gridView->iRow - GetFixedRowCount();
	int nColumn = gridView->iColumn - GetFixedColumnCount();

	CMenu PopupMenu;
	PopupMenu.CreatePopupMenu();
	CString str;

	if ( nRow >= 0 && nColumn >= 0 )
	{
		GridColumnMapItor colItor;
		CProductLine* pObj = ValidateRow( nRow, nColumn, colItor );
		if ( !pObj )
		{
			return;
		}
		if ( COLUM_OWNER == colItor->second->dataType)
		{
			str.LoadString( IDS_STR_EDIT );
			PopupMenu.AppendMenu(MF_ENABLED, IDM_BACK_EDIT, str);
			PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
			return;
		}
	}
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{//-1去掉操作按钮不能选择， 删除
		str.LoadString( IDS_STR_DELETE );
		PopupMenu.AppendMenu( MF_ENABLED, IDM_DELTE_OBJ, str );
		PopupMenu.TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON , pt.x, pt.y, m_gridCtrl->GetParent());
	}
}

void CProductLineGridManger::OnDeleteProductLine()
{
	CPoint pt;
	GetCursorPos( &pt );
	CCellRange range = m_gridCtrl->GetSelectedCellRange();
	if ( range.GetMinRow() >= GetFixedRowCount() &&
		GetFixedColumnCount() == range.GetMinCol() && (range.GetMaxCol() == m_gridColumnMap.size())  )
	{
		CProductLine *pObj;
		for ( int i = range.GetMinRow(); i <= range.GetMaxRow(); i++ )
		{
			int nRow = i - GetFixedRowCount();
			pObj = GetProductLineByRow( nRow );
			if ( pObj )
			{
				BOOL ret = m_projectSever->DeleteProduct( pObj->GetId() );
				if ( ret != userExcOk )
				{
					CString str;
					str.Format( _T("删除产品线'%s'失败，可能有关联的项目，请先修改项目"), 
						pObj->GetName() );
					AfxMessageBox( str );
					return;
				}
			}
		}
		UpdateGrid();
	}
}
int  CProductLineGridManger::ValidateSelectColum( int colDataType )
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
void  CProductLineGridManger::OnChangeOwner(  )
{
	//
	GridColumnMapItor colItor;
	int nRow = ValidateSelectColum( COLUM_OWNER );
	CProductLine *pObj = NULL;
	if ( nRow >= 0 && NULL != (pObj = GetProductLineByRow( nRow ) ) )
	{
		BOOL ret = m_projectSever->UpdateProductOwner( *pObj ); 
		if ( userExcOk == ret )
		{
			m_projectSever->GetProjectLineById( pObj->GetId(), *pObj );
			UpdateGridRowView( nRow );
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSeverFactory->GetErrorString( ret ) );
		}
	}
}