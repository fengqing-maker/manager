#include "stdafx.h"
#include "baseBackGridManger.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CBackBaseGridManger::CBackBaseGridManger(  CActionServerFactory *pSeverFactory )
{
	ASSERT( pSeverFactory ); 
	m_pSeverFactory = pSeverFactory;
	m_pLoginSever = m_pSeverFactory->GetLoginServer();
	m_pBackSever = m_pSeverFactory->GetBackStageSver();
}
//////////////////////////////////////////////////////////////
//重写
void CBackBaseGridManger::PreCreateGridView()
{
	CreateColumnMap();
	CreateRowItemMap();
}
//
void CBackBaseGridManger::CreateColumnMap()
{
}
void CBackBaseGridManger::AftCreateGridView()
{
	if (m_gridCtrl != NULL)
	{	
		m_gridCtrl->EnableSelection(TRUE);				/* 允许选择表项 */
		m_gridCtrl->SetFixedRowSelection(TRUE);			/* 允许整行选择 */
		m_gridCtrl->SetFixedColumnSelection(TRUE);		/* 允许整列选择 */	
		/* 设置列宽 */
		GridColumnMapItor itor = m_gridColumnMap.begin();
		while(itor != m_gridColumnMap.end())
		{
			int columnIndex = itor->first+m_fixedColumnCount;
			int nWidth = itor->second->width;
			m_gridCtrl->SetColumnWidth(columnIndex,nWidth);
			itor++;
		}
		//m_gridCtrl->AutoSizeColumns();
		//m_gridCtrl->AutoSize(); //lhz add
	}
}
void     CBackBaseGridManger::AftUpdateGridView()
{
	AftCreateGridView();
}
CString CBackBaseGridManger::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	if ( nRow == GetFixedColumnCount() && nColumn >0 )
	{
		int index = nColumn-m_fixedColumnCount;	/* 可动表项的第一列索引 */
		GridColumnMapItor itor = m_gridColumnMap.find(index);
		if (itor != m_gridColumnMap.end())
		{
			text = itor->second->title;
		}
		return text;
	}
	else
	{
		text =  AbstractGridViewManager::GetFixedItemText(nRow,nColumn);
	}
	return text;
}
//////////////////////////////////////////////////////////////
//
CString CBackBaseGridManger::GetOwnerNameStr( int userId )
{
	CString text;
	CUser useObj;
	m_pSeverFactory->GetLoginServer()->GetUserById( userId, useObj );
	text = useObj.GetUserName_R();
	return text;
}

void CBackBaseGridManger::InsertUniqueColumn(GridColumnPtr columnInfo)
{
	GridColumnMapItor itor = m_gridColumnMap.begin();
	while (itor != m_gridColumnMap.end())
	{
		if (itor->second->title == columnInfo->title)
		{
			return;
		}
		itor++;
	}
	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}

void CBackBaseGridManger::InsertColumn( const CString &str, int width, int alignemnt, int type, int dataType )
{
	GridColumnPtr columnInfo(new GridColumnInfo());
	columnInfo->title = str;
	columnInfo->type = type;
	columnInfo->width = width;
	columnInfo->alignment = alignemnt;
	columnInfo->dataType = dataType;
	InsertUniqueColumn(columnInfo);
}
GridItemType CBackBaseGridManger::GetFlexItemType(int nRow,int nColumn)
{
	 GridColumnMapItor itor = m_gridColumnMap.find( nColumn );
	 if ( itor != m_gridColumnMap.end())
	 {
		 return (GridItemType)itor->second->type;
	 }else
	 {
		 return AbstractGridViewManager::GetFlexItemType(nRow, nColumn);
	 }
}

int CBackBaseGridManger::GetFlexColumnCount()
{
	if (m_gridColumnMap.size() != 0)
	{
		return m_gridColumnMap.size();
	}
	else
	{
		return GetFlexColumnCount();
	}
}

void CBackBaseGridManger::OnGridEndEidt( NMHDR* pNMHDR,LRESULT* pResult )
{
	NM_GRIDVIEW *gridView = reinterpret_cast<NM_GRIDVIEW*>(pNMHDR);
	EndEditGridView( gridView->iRow, gridView->iColumn );
}
