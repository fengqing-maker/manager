#include "stdafx.h"
#include "AbstractGridViewManager.h"
#include "../../Sid_Project_Manger.h"
#include "NewCellTypes/gridCellButton.h"
//#include "NewCellTypes/GridCellComposite.h"

AbstractGridViewManager::AbstractGridViewManager(void)
{
}
AbstractGridViewManager::~AbstractGridViewManager(void)
{
}

BOOL AbstractGridViewManager::RegisterAsGridView(CGridCtrl* gridCtrl)
{
	if (gridCtrl != 0 && gridCtrl->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
	{
		m_gridCtrl = gridCtrl;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL AbstractGridViewManager::CreateDefaultGridView()
{
	if (m_gridCtrl == 0)
		return FALSE;
	PreCreateDefGridView();

	ResetDefGridView();

	AftCreateDefGridView();

	return TRUE;
}

BOOL AbstractGridViewManager::CreateGridView()
{
	if (m_gridCtrl == 0)
	{
		return FALSE;
	}

	m_gridCtrl->DeleteAllItems();

	PreCreateGridView();

	ResetGridView();

	AftCreateGridView();

	
	m_gridCtrl->RedrawWindow();

	return TRUE;
}

void AbstractGridViewManager::DestroyGridView()
{
	if (m_gridCtrl == 0)
		return ;
	m_gridCtrl->DeleteAllItems();
	m_gridCtrl->RedrawWindow();
}

BOOL AbstractGridViewManager::CreateGridViewContextMenu(CPoint clientPoint)
{
	if (m_gridCtrl == 0)
	{
		return TRUE;	/* 不创建任何菜单 */
	}
	CCellID clickCell = m_gridCtrl->GetCellFromPt(clientPoint);
	if (!clickCell.IsValid())
		return FALSE;
	int nRow = clickCell.row;
	int nColumn = clickCell.col;
	CMenu popupMenu;
	if(!popupMenu.CreatePopupMenu())
		return FALSE;
	if ((nRow >= 0 && nRow < m_fixedRowCount)
		||(nColumn >= 0 && nColumn < m_fixedColumnCount))
	{
		if(!InitFixedItemPopupMenu(nRow,nColumn,&popupMenu))
			return FALSE;
	}
	else if ((nRow >= m_fixedRowCount && nRow < m_fixedRowCount + m_flexRowCount)
		&& (nColumn >= m_fixedRowCount && nColumn < m_fixedColumnCount + m_flexColumnCount))
	{
		if(!InitFlexItemPopupMenu(nRow-m_fixedRowCount,nColumn-m_fixedColumnCount,&popupMenu))
			return FALSE;
	}
	CPoint screenPoint = clientPoint;
	m_gridCtrl->ClientToScreen(&screenPoint);

	m_popupMenuRowIndex = nRow;
	m_popupMenuColumnIndex = nColumn;
	CWnd* pParent = m_gridCtrl->GetParent();
	ASSERT(pParent != 0);
	theApp.GetContextMenuManager()->ShowPopupMenu(popupMenu.GetSafeHmenu(),
			screenPoint.x,screenPoint.y,pParent,TRUE,TRUE,FALSE);
	return TRUE;
}
BOOL AbstractGridViewManager::RespGridViewContextMenu(UINT menuID)
{
	if (menuID >= WM_GRID_POPUP_MENUID_MIN
		&& menuID <= WM_GRID_POPUP_MENUID_MAX)
	{
		if ((m_popupMenuRowIndex >= 0 && m_popupMenuRowIndex < m_fixedRowCount)
			||(m_popupMenuColumnIndex >= 0 && m_popupMenuColumnIndex < m_fixedColumnCount))
		{
			if(!ResponseFixedItemPopupMenu(menuID))
				return FALSE;
		}
		else if ((m_popupMenuRowIndex >= m_fixedRowCount && m_popupMenuRowIndex < m_fixedRowCount + m_flexRowCount)
			&& (m_popupMenuColumnIndex >= m_fixedRowCount && m_popupMenuColumnIndex < m_fixedColumnCount + m_flexColumnCount))
		{
			if(!ResponseFlexItemPopupMenu(menuID))
				return FALSE;
		}
		return TRUE;
	}
	else
	{
		return FALSE;	/* 菜单项ID错误 */
	}
}

BOOL AbstractGridViewManager::ValidateGridView(int nRow,int nColumn)
{
	int fixedRowCount = GetFixedRowCount();
	int fixedColumnCount = GetFixedColumnCount();
	int flexRowIndex = nRow - fixedRowCount;
	int flexColumnIndex = nColumn - fixedColumnCount;
	BOOL result = TRUE;
	ASSERT(flexRowIndex >= 0 && flexColumnIndex >= 0);
	if (flexRowIndex >= 0 && flexColumnIndex >= 0)
	{
		CString text = m_gridCtrl->GetItemText(nRow,nColumn);
		result = ValidateFlexItemText(flexRowIndex,flexColumnIndex,text);
	}
	return result;
}

BOOL AbstractGridViewManager::EndEditGridView(int nRow,int nColumn)
{
	int fixedRowCount = GetFixedRowCount();
	int fixedColumnCount = GetFixedColumnCount();
	int flexRowIndex = nRow - fixedRowCount;
	int flexColumnIndex = nColumn - fixedColumnCount;
	BOOL result = TRUE;
	ASSERT(flexRowIndex >= 0 && flexColumnIndex >= 0);
	CString text = m_gridCtrl->GetItemText(nRow,nColumn);
	if (flexRowIndex >= 0 && flexColumnIndex >= 0)
	{
		result = SetFlexItemText(flexRowIndex,flexColumnIndex,text);

		//lhz add by 2015-5-15
		m_gridCtrl->AutoSizeRow( nRow ); 
		m_gridCtrl->RedrawRow( nRow );
		int cnt =  m_gridCtrl->GetRowCount();
		if ( cnt > 1 )
		{
		   int curHight	= m_gridCtrl->GetRowHeight( nRow );
		   int oldHight = 0;
		   if ( nRow > 0 )
		   {
			   oldHight = m_gridCtrl->GetRowHeight( nRow - 1 );
		   }else
		   {
			   oldHight = m_gridCtrl->GetRowHeight( nRow + 1 );
		   }
		   if ( curHight != oldHight )
		   {
			   for ( int i = nRow+1; i < cnt ; i++ )
			   {
				   m_gridCtrl->RedrawRow( i );
			   }
		   }
		}
		//
	}
	return result;
}

BOOL AbstractGridViewManager::UpdateGridView()
{
	if (m_gridCtrl == 0)
	{
		return TRUE;	/* 没有可以刷新的表格控件 */
	}

	m_gridCtrl->DeleteAllItems();

	PreUpdateGridView();

	ResetGridView();

	AftUpdateGridView();

	m_gridCtrl->ResetScrollBars();//1111111111
	m_gridCtrl->RedrawWindow();

	return TRUE;
}
//lhz add  2015-4-7 
BOOL AbstractGridViewManager::UpdateGridRowView( int row )
{
	if (m_bVirtualMode == FALSE)
	{
		int nFixedColumn = m_gridCtrl->GetFixedColumnCount();
		int nFixedRow = m_gridCtrl->GetFixedRowCount();
		int nFlexColumn = m_gridCtrl->GetColumnCount() - nFixedColumn;
		//int nFlexRow = m_gridCtrl->GetRowCount()- nFixedRow;
		/* 刷新整张表的内容 */
		row += nFixedRow;
		for (int j = nFixedColumn; j < m_gridCtrl->GetColumnCount(); j++)
		{
			SetGridItemType(row, j);
			SetGridItemState(row, j);
		}
		m_gridCtrl->RedrawRow(row);
		return TRUE;
	}
	return FALSE;
}
BOOL AbstractGridViewManager::UpdateGridFixedRowView( int row )
{
	if (m_bVirtualMode == FALSE)
	{
		int nFixedColumn = m_gridCtrl->GetFixedColumnCount();
		/* 刷新整张表的内容 */
		for (int j = nFixedColumn; j < m_gridCtrl->GetColumnCount(); j++)
		{
			SetGridItemState(row, j);
		}
		m_gridCtrl->RedrawRow(row);
		return TRUE;
	}
	return FALSE;
}
BOOL AbstractGridViewManager::GetGridViewFlexSelection(int& rowMin,int& colMin,int& rowMax,int& colMax)
{
	rowMin = -1;colMin = -1; rowMax = -1; colMax = -1;
	if (m_gridCtrl != 0 && m_gridCtrl->IsSelectable())
	{
		CCellRange selRange = m_gridCtrl->GetSelectedCellRange();
		if (selRange.IsValid() == TRUE)
		{
			rowMin = selRange.GetMinRow()-m_fixedRowCount;
			colMin = selRange.GetMinCol()-m_fixedColumnCount;
			rowMax = selRange.GetMaxRow()-m_fixedRowCount;
			colMax = selRange.GetMaxCol()-m_fixedColumnCount;
		}else
		{
			return FALSE;
		}
		return TRUE;
	}
	return FALSE;
}

BOOL AbstractGridViewManager::GetCheckItemStatus(int nRow,int nColumn)
{
	GridItemType itemType = GetFlexItemType(nRow,nColumn);
	if (itemType != GridItemType_Check)
		return FALSE;
	CGridCellCheck* pCheckCell = static_cast<CGridCellCheck*>
		(m_gridCtrl->GetCell(nRow+m_fixedRowCount,nColumn+m_fixedColumnCount));
	if (pCheckCell == 0)
		return FALSE;
	BOOL bCheck = pCheckCell->GetCheck();
	return bCheck;
}
void AbstractGridViewManager::SelectAllCheckItem()
{
	for (int i = 0; i < m_flexRowCount; i++)
	{
		for (int j = 0; j < m_flexColumnCount;j++)
		{
			GridItemType itemType = GetFlexItemType(i,j);
			if (itemType == GridItemType_Check)
			{
				CGridCellCheck* pCheckCell = static_cast<CGridCellCheck*>
					(m_gridCtrl->GetCell(i+m_fixedRowCount,j+m_fixedColumnCount));
				if (pCheckCell != 0)
				{
					pCheckCell->SetCheck(TRUE);
				}
			}
		}
	}
}
void AbstractGridViewManager::DeselectAllCheckItem()
{
	for (int i = 0; i < m_flexRowCount; i++)
	{
		for (int j = 0; j < m_flexColumnCount;j++)
		{
			GridItemType itemType = GetFlexItemType(i,j);
			if (itemType == GridItemType_Check)
			{
				CGridCellCheck* pCheckCell = static_cast<CGridCellCheck*>
					(m_gridCtrl->GetCell(i+m_fixedRowCount,j+m_fixedColumnCount));
				if (pCheckCell != 0)
				{
					pCheckCell->SetCheck(FALSE);
				}
			}
		}
	}
}
/****************************protected function*****************************/


CString AbstractGridViewManager::GetDefFixedItemText(int nRow, int nColumn)
{
	CString text;
	if (!(nRow < m_gridCtrl->GetRowCount()) || !(nColumn < m_gridCtrl->GetColumnCount())
		||nRow < 0 || nColumn < 0)
	{
		return text;	/* 不符合条件的表项 */
	}
	else if (nRow == nColumn)
	{
		return text;	/* 对角线上的表项 */
	}
	if (nRow < nColumn)	/* 右上方表项 */
	{
		char baseChar = 'A';
		char endChar = 'Z';
		if ( nColumn -1 <= ((int)endChar-(int)baseChar))
		{
			char curChar = (char)(baseChar+nColumn-1);
			text.Format(_T("%c"),curChar);
		}
		else
		{ 
			int num = (int)(nColumn-1)/((int)endChar - (int)baseChar);
			int index = (int)(nColumn-1)%((int)endChar - (int)baseChar);
			char curChar = (char)(baseChar+index-1);
			text.Format(_T("%c%d"),curChar,num);
		}
	}
	else if (nRow > nColumn&&nRow > m_fixedRowCount-1)	/* 左下方表项 */
	{
		text.Format(_T("%d"),nRow-m_fixedRowCount+1);	/* nRow将从1开始 */
	}
	return text;
}


CString AbstractGridViewManager::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	if (!(nRow < m_gridCtrl->GetRowCount()) || !(nColumn < m_gridCtrl->GetColumnCount()) 
		||nRow < 0 || nColumn < 0)
	{
		return text;	/* 不符合条件的表项 */
	}
	else if (nRow == nColumn)
	{
		return text;	/* 对角线上的表项 */
	}
	if (nRow < nColumn)	/* 右上方表项 */
	{
		char baseChar = 'A';
		char endChar = 'Z';
		if ( nColumn -1 <= ((int)endChar-(int)baseChar))
		{
			char curChar = (char)(baseChar+nColumn-1);
			text.Format(_T("%c"),curChar);
		}
		else
		{ 
			int num = (int)(nColumn-1)/((int)endChar - (int)baseChar);
			int index = (int)(nColumn-1)%((int)endChar - (int)baseChar);
			char curChar = (char)(baseChar+index-1);
			text.Format(_T("%c%d"),curChar,num);
		}
	}
	else if (nRow > nColumn&&nRow > m_fixedRowCount-1)	/* 左下方表项 */
	{
		text.Format(_T("%d"),nRow-m_fixedRowCount+1);	/* nRow将从1开始 */
	}
	return text;
}




/****************************private function********************************/
void AbstractGridViewManager::ResetDefGridView()
{
	/* 设置外观 */
	SetDefGridLook();
	/* 设置尺寸 */
	SetDefGridSize();
	/* 设置内容 */
	SetDefGridContent();
}

void AbstractGridViewManager::SetDefGridLook()
{
	if(m_gridCtrl == 0)
		return;
	m_gridCtrl->EnableDragAndDrop(TRUE);		/* 允许拖拽 */

	CImageList* imageList = GetDefGridImageList();	/* 得到图标列表 */
	if(imageList != 0)
	{
		m_gridCtrl->SetImageList(imageList);
		m_bImageValid = TRUE;
	}
	else
	{
		m_bImageValid = FALSE;
	}

	m_bVirtualMode = GetDefGridVirtualMode();
	m_gridCtrl->SetVirtualMode(m_bVirtualMode);
	m_gridCtrl->SetEditable(TRUE);

	m_gridCtrl->SetGridBkColor(RGB(255,255,255));
	m_gridCtrl->EnableColumnHide(FALSE);
	m_gridCtrl->EnableRowHide(FALSE);
	m_gridCtrl->EnableDragAndDrop(TRUE);
	m_gridCtrl->EnableTitleTips(TRUE);
}
void AbstractGridViewManager::SetDefGridSize()
{
	/* 数目设置 */
	m_fixedColumnCount = GetDefFixedColumnCount();
	m_gridCtrl->SetFixedColumnCount(m_fixedColumnCount);
	m_fixedRowCount = GetDefFixedRowCount();
	m_gridCtrl->SetFixedRowCount(m_fixedRowCount);

	m_flexColumnCount = GetDefFlexColumnCount();
	m_gridCtrl->SetColumnCount(m_flexColumnCount+m_fixedColumnCount);
	m_flexRowCount = GetDefFlexRowCount();
	m_gridCtrl->SetRowCount(m_flexRowCount+m_fixedRowCount);

	/* 尺寸设置 */
	int nFixedWidth = 0,nFixedHeight = 0, nFlexWidth = 0, nFlexHeight = 0;
	
	CRect       rect;
	m_gridCtrl->GetClientRect( rect );//2015-6-29 add

	for (int i = 0; i < m_fixedColumnCount ; ++i)
	{
		nFixedWidth = GetDefFixedColumnWidth(i);
		
		m_gridCtrl->SetColumnWidth(i,nFixedWidth);
	}
	
	for (int i = 0; i < m_fixedRowCount; ++i)
	{
		nFixedHeight = GetDefFixedRowHeight(i);
		m_gridCtrl->SetRowHeight(i,nFixedHeight);
	}
	
	for (int i=m_fixedColumnCount; i < m_fixedColumnCount+m_flexColumnCount; ++i)
	{
		nFlexWidth = GetDefFlexColumnWidth(i-m_fixedColumnCount);
		m_gridCtrl->SetColumnWidth(i,nFlexWidth);
	}
	
	for (int i = m_fixedRowCount; i < m_fixedRowCount+m_flexRowCount; ++i)
	{
		nFlexHeight = GetDefFlexRowHeight(i-m_fixedRowCount);
		m_gridCtrl->SetRowHeight(i,nFlexHeight);
	}

}
void AbstractGridViewManager::SetDefGridContent()
{
	if (m_gridCtrl == 0)
		return;
	if (m_bVirtualMode == FALSE)
	{
		int nFixedColumn = m_gridCtrl->GetFixedColumnCount();
		int nFixedRow = m_gridCtrl->GetFixedRowCount();
		int nFlexColumn = m_gridCtrl->GetColumnCount() - nFixedColumn;
		int nFlexRow = m_gridCtrl->GetRowCount()- nFixedRow;

		/* 刷新整张表的内容 */
		for (int i  = 0; i < nFlexRow+nFixedRow; i++)
		{
			for (int j = 0; j < nFlexColumn+nFixedColumn; j++)
			{
				SetDefGridItemState(i,j);
			}
		}
	}
	else
	{
		/* 使用回调函数设置内容 */
		GRIDCALLBACK pFun = DefCallBackFunction;
		m_gridCtrl->SetCallbackFunc(pFun,reinterpret_cast<LPARAM>(this));
	}
}

void AbstractGridViewManager::SetDefGridItemState(int nRow,int nColumn)
{
	GV_ITEM gridItem;
	gridItem.row = nRow;
	gridItem.col = nColumn;
	UINT mask = 0;
	mask |= GVIF_TEXT;

	if (nRow < m_fixedRowCount || nColumn < m_fixedColumnCount)
	{
		gridItem.mask = mask;
		gridItem.strText = GetDefFixedItemText(nRow,nColumn);
	}
	else if (nRow >= m_fixedRowCount && nColumn >= m_fixedColumnCount)
	{
		/* 只改变可动表项的状态值 */
		mask |= GVIF_STATE;
		gridItem.mask = mask;
		UINT oldState = 0;
		if (GetDefFlexItemEditable(nRow-m_fixedRowCount,nColumn-m_fixedColumnCount) == TRUE)
			gridItem.nState = oldState&(~GVIS_READONLY);
		else
			gridItem.nState = oldState|(GVIS_READONLY);
		gridItem.strText = GetDefFlexItemText(nRow-m_fixedRowCount,nColumn-m_fixedColumnCount);
	}
	m_gridCtrl->SetItem(&gridItem);
}
BOOL CALLBACK AbstractGridViewManager::DefCallBackFunction(GV_DISPINFO* pDispInfo,LPARAM lParam)
{
	int nRow = pDispInfo->item.row;
	int nColumn =  pDispInfo->item.col;
	/* 通过参数传递进来的对象指针仍然能够实现动态绑定 */
	AbstractGridViewManager* pManager = reinterpret_cast<AbstractGridViewManager*>(lParam);
	ASSERT(pManager != 0);
	pDispInfo->item.row = nRow;
	pDispInfo->item.col = nColumn;
	pDispInfo->item.mask |= GVIF_TEXT;

	if (nRow < pManager->m_fixedRowCount || nColumn < pManager->m_fixedColumnCount)
	{
		pDispInfo->item.strText = pManager->GetDefFixedItemText(nRow,nColumn);
	}
	else if (nRow >= pManager->m_fixedRowCount && nColumn >= pManager->m_fixedColumnCount)
	{
		/* 只改变可动表项的状态值 */
		pDispInfo->item.mask |= GVIF_STATE;
		if (pManager->GetDefFlexItemEditable(nRow-pManager->m_fixedRowCount,nColumn-pManager->m_fixedColumnCount) == TRUE)
			pDispInfo->item.nState &= (~GVIS_READONLY);
		else
			pDispInfo->item.nState |= (GVIS_READONLY);
		pDispInfo->item.strText = pManager->GetDefFlexItemText(nRow-pManager->m_fixedRowCount,nColumn-pManager->m_fixedColumnCount);
	}

	return TRUE;
}
void AbstractGridViewManager::ResetGridView()
{
	/* 设置外观 */
	SetGridLook();
	/* 设置尺寸 */
	SetGridSize();
	/* 设置内容 */
	SetGridContent();

	m_gridCtrl->ResetScrollBars();
}
void AbstractGridViewManager::SetGridLook()
{
	if(m_gridCtrl == 0)
		return;
	m_gridCtrl->EnableDragAndDrop(TRUE);		/* 允许拖拽 */
	m_gridCtrl->SetAutoSizeStyle(GVS_BOTH);		/* 自动调整 */
	

	CImageList* imageList = GetGridImageList();	/* 得到图标列表 */
	if(imageList != 0)
	{
		m_gridCtrl->SetImageList(imageList);
		m_bImageValid = TRUE;
	}
	else
	{
		m_bImageValid = FALSE;
	}

	m_bVirtualMode = GetGridVirtualMode();
	m_gridCtrl->SetVirtualMode(m_bVirtualMode);
	m_gridCtrl->SetEditable(TRUE);

	m_gridCtrl->SetGridBkColor(RGB(255,255,255));
	m_gridCtrl->EnableColumnHide(FALSE);
	m_gridCtrl->EnableRowHide(FALSE);
	m_gridCtrl->EnableDragAndDrop(TRUE);
	m_gridCtrl->EnableTitleTips(TRUE);

	m_gridCtrl->SetFixedBkColor( RGB(41, 57, 85) );
	m_gridCtrl->SetFixedTextColor( RGB( 255, 255, 255 ));
	
}
void AbstractGridViewManager::SetGridSize()
{
	/* 数目设置 */
	m_fixedColumnCount = GetFixedColumnCount();
	m_gridCtrl->SetFixedColumnCount(m_fixedColumnCount);
	m_fixedRowCount = GetFixedRowCount();
	m_gridCtrl->SetFixedRowCount(m_fixedRowCount);

	m_flexColumnCount = GetFlexColumnCount();
	m_gridCtrl->SetColumnCount(m_flexColumnCount+m_fixedColumnCount);
	m_flexRowCount = GetFlexRowCount();
	m_gridCtrl->SetRowCount(m_flexRowCount+m_fixedRowCount);

	/* 尺寸设置 */
	int nFixedWidth = 0,nFixedHeight = 0, nFlexWidth = 0, nFlexHeight = 0;
	CRect       rect;
	m_gridCtrl->GetClientRect( rect );//2015-6-29 add

	for (int i = 0; i < m_fixedColumnCount ; ++i)
	{
		nFixedWidth = GetFixedColumnWidth(i);
		m_gridCtrl->SetColumnWidth(i,nFixedWidth);
	}
	for (int i = 0; i < m_fixedRowCount; ++i)
	{
		nFixedHeight = GetFixedRowHeight(i);
		m_gridCtrl->SetRowHeight(i,nFixedHeight);
	}
	
	for (int i=m_fixedColumnCount; i < m_fixedColumnCount+m_flexColumnCount; ++i)
	{
		nFlexWidth = GetFlexColumnWidth(i-m_fixedColumnCount);
		m_gridCtrl->SetColumnWidth(i,nFlexWidth);
	}

	for (int i = m_fixedRowCount; i < m_fixedRowCount+m_flexRowCount; ++i)
	{
		nFlexHeight = GetFlexRowHeight(i-m_fixedRowCount);
		m_gridCtrl->SetRowHeight(i,nFlexHeight);
	}
}

void AbstractGridViewManager::SetGridContent()
{
	if (m_gridCtrl == 0)
		return;
	if (m_bVirtualMode == FALSE)
	{
		int nFixedColumn = m_gridCtrl->GetFixedColumnCount();
		int nFixedRow = m_gridCtrl->GetFixedRowCount();
		int nFlexColumn = m_gridCtrl->GetColumnCount() - nFixedColumn;
		int nRowNum = GetFlexRowCount() + nFixedRow/*m_gridCtrl->GetRowCount()- nFixedRow*/;
		int nGridRow = m_gridCtrl->GetRowCount();
		//if ( nRowNum > nGridRow )
		//{
		//	for ( int i = nGridRow; i < nRowNum; i++)
		//	{
  //              m_gridCtrl->InsertRow( _T("") );
		//		for (int i=m_fixedColumnCount; i < m_fixedColumnCount+m_flexColumnCount; ++i)
		//		{
		//			m_gridCtrl->SetColumnWidth(i, GetFlexColumnWidth(i-m_fixedColumnCount));
		//		}

		//		for (int i = m_fixedRowCount; i < m_fixedRowCount+m_flexRowCount; ++i)
		//		{
		//			m_gridCtrl->SetRowHeight(i,GetFlexRowHeight(i-m_fixedRowCount));
		//		}
		//	}
		//	
		//}
		/* 刷新整张表的内容 */
		for (int i  = 0; i < nRowNum; i++)
		{
			for (int j = 0; j < nFlexColumn+nFixedColumn; j++)
			{
				SetGridItemType(i,j);

				SetGridItemState(i,j);
			}
		}
	}
	else
	{
		/* 使用回调函数设置内容 */
		GRIDCALLBACK pFun = CallBackFunction;
		m_gridCtrl->SetCallbackFunc(pFun,reinterpret_cast<LPARAM>(this));
	}
}
void AbstractGridViewManager::SetGridItemType(int nRow,int nColumn)
{
	int flexRowIndex = nRow;
	int flexColumnIndex = nColumn;
	GridItemType itemType = GridItemType_Edit;
	if (nRow >= m_fixedRowCount && nColumn >= m_fixedColumnCount)
	{
		 flexRowIndex = nRow-m_fixedRowCount;
		 flexColumnIndex = nColumn-m_fixedColumnCount;
		 itemType = GetFlexItemType(flexRowIndex,flexColumnIndex);
	}else 
	{// 2015-6-4 添加可修改固定行列类型
		itemType = GetFixedItemType(flexRowIndex, flexColumnIndex);
	}
	switch (itemType)
	{
	case GridItemType_Edit:
		break;
	case GridItemType_MutilEdit:
		{
			/*CGridCellBase *pcell = m_gridCtrl->GetCell( nRow, nColumn );
			if ( pcell )
			{
				pcell->SetFormat( pcell->GetFormat() &0xffffffdf);
			}*/
			//m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellRich)); //11111111111
			CGridCellBase *pcell = m_gridCtrl->GetCell( nRow, nColumn );
			if ( pcell )
			{
				pcell->SetFormat( pcell->GetFormat() & DT_CENTER|DT_VCENTER/*|DT_SINGLELINE*/|DT_END_ELLIPSIS|DT_NOPREFIX &0xffffffdf );
			}
				
		}
		break;;
	case GridItemType_Combo:
		if(m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellCombo)))
		{
			CGridCellCombo* pCell = 
				dynamic_cast<CGridCellCombo*>(m_gridCtrl->GetCell(nRow,nColumn));
			if(!InitFlexComboItem(pCell,flexRowIndex,flexColumnIndex))
			{
				TRACE("表项(%d,%d)组合框初始化失败.",nRow,nColumn);
			}
		}
		break;
	case GridItemType_Check:
		if (m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellCheck)))
		{
			CGridCellCheck* pCell = 
				dynamic_cast<CGridCellCheck*>(m_gridCtrl->GetCell(nRow,nColumn));
			if(!InitFlexCheckItem(pCell,flexRowIndex,flexColumnIndex))
			{
				TRACE("表项(%d,%d)复选框框初始化失败.",nRow,nColumn);
			}
		}
		break;
	case GridItemType_DateTime:
		if (m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellDateTime)))
		{
			CGridCellDateTime* pCell = 
				dynamic_cast<CGridCellDateTime*>(m_gridCtrl->GetCell(nRow,nColumn));
			if(!InitFlexDateTimeItem(pCell,flexRowIndex,flexColumnIndex))
			{
				TRACE("表项(%d,%d)时间控件初始化失败.",nRow,nColumn);
			}
		}
		break;
	case GridItemType_Button:
		if (!m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellButton)))
		{
			TRACE("表项(%d,%d)按键控件初始化失败.",nRow,nColumn);	
		}
		break;
	case GridItemType_NumEidt:
		if ( m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellNumeric)) )
		{
			CGridCellNumeric* pCell = 
				dynamic_cast<CGridCellNumeric*>(m_gridCtrl->GetCell(nRow,nColumn));
			if(!InitFlexNumEditItem(pCell,flexRowIndex,flexColumnIndex))
			{
				TRACE("表项(%d,%d)数字编辑控件初始化失败.",nRow,nColumn);
			}	
		}
		break;
	case GridItemType_RichEidt:
		if ( m_gridCtrl->SetCellType(nRow,nColumn,RUNTIME_CLASS(CGridCellRich)) )
		{
			CGridCellRich* pCell = 
				dynamic_cast<CGridCellRich*>(m_gridCtrl->GetCell(nRow,nColumn));
			if(!InitFlexRichEditItem(pCell,flexRowIndex,flexColumnIndex))
			{
				TRACE("表项(%d,%d)数字编辑控件初始化失败.",nRow,nColumn);
			}	
		}
		break;
	case GridItemType_Extension:
		{
			int extensitonItemType = GetFlexItemExtensionType(flexRowIndex,flexColumnIndex);
			if (!InitFlexExtensionItem(extensitonItemType,flexRowIndex,flexColumnIndex))
			{
				TRACE("表项(%d,%d)扩展控件初始化失败.",nRow,nColumn);
			}
		}
		break;
	default:
		break;
	}
}
void AbstractGridViewManager::RefreshRow( int nRow, int nColumn )
{
	CString text = GetFlexItemText( nRow, nColumn );
	int  row = nRow + GetFixedRowCount();
	int  column = nColumn + GetFixedColumnCount();
	m_gridCtrl->SetItemText( row, column, text );
}
void AbstractGridViewManager::SetGridItemState(int nRow,int nColumn)
{
	GV_ITEM gridItem;
	gridItem.row = nRow;
	gridItem.col = nColumn;
	UINT mask = 0;
	mask |= GVIF_TEXT;

	BOOL isEdit = FALSE;
	if (nRow < m_fixedRowCount || nColumn < m_fixedColumnCount)
	{//lhz moidy
		isEdit = GetFixedItmeEditable ( nRow, nColumn);
		gridItem.strText = GetFixedItemText(nRow,nColumn);
	}
	else if (nRow >= m_fixedRowCount && nColumn >= m_fixedColumnCount)
	{

		isEdit = GetFlexItemEditable( nRow-m_fixedRowCount,nColumn-m_fixedColumnCount );
		gridItem.strText = GetFlexItemText(nRow-m_fixedRowCount,nColumn-m_fixedColumnCount);
	}
	mask |= GVIF_STATE;
	gridItem.mask = mask;
	UINT oldState = m_gridCtrl->GetItemState( nRow, nColumn );
	if ( isEdit )
		gridItem.nState = oldState&(~GVIS_READONLY);
	else
		gridItem.nState = oldState|(GVIS_READONLY);
	gridItem.mask = mask;

	m_gridCtrl->SetItem(&gridItem);
}
BOOL CALLBACK AbstractGridViewManager::CallBackFunction(GV_DISPINFO* pDispInfo,LPARAM lParam)
{
	int nRow = pDispInfo->item.row;
	int nColumn =  pDispInfo->item.col;
	/* 通过参数传递进来的对象指针仍然能够实现动态绑定 */
	AbstractGridViewManager* pManager = reinterpret_cast<AbstractGridViewManager*>(lParam);
	ASSERT(pManager != 0);
	pDispInfo->item.row = nRow;
	pDispInfo->item.col = nColumn;
	pDispInfo->item.mask |= GVIF_TEXT;

	if (nRow < pManager->m_fixedRowCount || nColumn < pManager->m_fixedColumnCount)
	{
		pDispInfo->item.strText = pManager->GetFixedItemText(nRow,nColumn);
	}
	else if (nRow >= pManager->m_fixedRowCount && nColumn >= pManager->m_fixedColumnCount)
	{
		/* 只改变可动表项的状态值 */
		pDispInfo->item.mask |= GVIF_STATE;
		if (pManager->GetFlexItemEditable(nRow-pManager->m_fixedRowCount,nColumn-pManager->m_fixedColumnCount) == TRUE)
			pDispInfo->item.nState &= (~GVIS_READONLY);
		else
			pDispInfo->item.nState |= (GVIS_READONLY);
		pDispInfo->item.strText = pManager->GetFlexItemText(nRow-pManager->m_fixedRowCount,nColumn-pManager->m_fixedColumnCount);
	}

	return TRUE;
}
void AbstractGridViewManager::SelectRow(int nRow )
{
	CCellID selId;
	selId.row = nRow;
	m_gridCtrl->SelectRows( selId );
}
void AbstractGridViewManager::SetColumnBkColor( int nColNum, const COLORREF &cofor )
{
	int beginRow = GetFixedRowCount();
	int endRow = m_gridCtrl->GetRowCount();
	for ( int i = beginRow; i < endRow; i++ )
	{
		m_gridCtrl->SetItemBkColour( i, nColNum, cofor );
	}
}