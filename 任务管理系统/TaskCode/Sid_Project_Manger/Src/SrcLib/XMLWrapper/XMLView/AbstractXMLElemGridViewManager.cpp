#include "stdafx.h"
#include "AbstractXMLElemGridViewManager.h"


AbstractXMLElemGridViewManager::AbstractXMLElemGridViewManager(void)
{
}


AbstractXMLElemGridViewManager::~AbstractXMLElemGridViewManager(void)
{
}

Weak_XMLElem AbstractXMLElemGridViewManager::GetSelectedElem()
{
	Weak_XMLElem weak_elem;
	int rowMin = -1,colMin = -1,rowMax = -1, colMax = -1;
	BOOL ret = GetGridViewFlexSelection(rowMin,colMin,rowMax,colMax);
	if (ret == TRUE)
	{
		if (rowMin != -1 && colMin != -1 && rowMax != -1 && colMax != -1)
		{
			ASSERT(rowMin == rowMax);
			ElemGridRowMapItor itor = m_gridRowMap.find(rowMin);
			ASSERT(itor != m_gridRowMap.end());
			weak_elem = itor->second;
		}
	}
	return weak_elem;
}
/****************************protected function*****************************/
void AbstractXMLElemGridViewManager::PreCreateGridView()
{
	SetElemGridViewStyle();

	CreateGridColumnMap();

	CreateGridRowMap();
}
void AbstractXMLElemGridViewManager::AftCreateGridView()
{
	if (m_gridCtrl != 0)
	{
		m_gridCtrl->AutoSizeColumns(GVS_DEFAULT);
		m_gridCtrl->ExpandLastColumn();
	}
}

void AbstractXMLElemGridViewManager::PreUpdateGridView()
{
	SetElemGridViewStyle();

	CreateGridColumnMap();

	CreateGridRowMap();
}
void AbstractXMLElemGridViewManager::AftUpdateGridView()
{
	if (m_gridCtrl != 0)
	{
		m_gridCtrl->AutoSizeColumns(GVS_DEFAULT);
		m_gridCtrl->ExpandLastColumn();
	}
}

int AbstractXMLElemGridViewManager::GetFlexColumnCount()
{
	return m_gridColumnMap.size();
}
int AbstractXMLElemGridViewManager::GetFlexRowCount()
{
	return m_gridRowMap.size();
}

int AbstractXMLElemGridViewManager::GetFlexColumnWidth(int columnIndex)
{
	int width = 0;
	GridColumnMapItor itor = m_gridColumnMap.find(columnIndex);
	if (itor != m_gridColumnMap.end())
	{
		GridColumnPtr columnInfo = itor->second;
		width = columnInfo->width;
	} 
	else
	{
		width = AbstractGridViewManager::GetFlexColumnWidth(columnIndex);
	}
	return width;
}

BOOL AbstractXMLElemGridViewManager::GetFlexItemEditable(int nRow,int nColumn)
{
	return FALSE;
}
CString AbstractXMLElemGridViewManager::GetFixedItemText(int nRow, int nColumn)
{
	int nFixedColumnCount = GetFixedColumnCount();
	if (nColumn >= nFixedColumnCount)
	{
		CString text;
		GridColumnMapItor itor = m_gridColumnMap.find(nColumn - nFixedColumnCount);
		if (itor != m_gridColumnMap.end())
		{
			text = itor->second->title;
		}
		return text;
	}
	else 
	{
		return AbstractGridViewManager::GetFixedItemText(nRow,nColumn);
	}
}
CString AbstractXMLElemGridViewManager::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	ElemGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	GridColumnMapItor columnItor = m_gridColumnMap.find(nColumn);
	ASSERT(columnItor != m_gridColumnMap.end());
	Weak_XMLElem weak_elem = rowItor->second;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem != 0)
	{
		GridColumnPtr columnInfo = columnItor->second;
		switch (columnInfo->type)
		{
		case ColumnType_Name:
			{
				text = GetNodeNameValue(weak_elem);
			}
			break;
		case ColumnType_Attr:
			{
				CString attrName = GetAttrNameFromTitle(columnInfo->title);
				text = GetNodeAttrValue(weak_elem,attrName);
			}
			break;
		case ColumnType_Text:
			{
				text = GetNodeTextValue(weak_elem);
			}
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	return text;
}

/****************************private function********************************/
void AbstractXMLElemGridViewManager::SetElemGridViewStyle()
{
	if (m_gridCtrl != 0)
	{
		m_gridCtrl->SetListMode(TRUE);
		m_gridCtrl->EnableSelection(TRUE);
		m_gridCtrl->SetSingleRowSelection(TRUE);
		m_gridCtrl->SetSingleColSelection(FALSE);
		m_gridCtrl->SetFixedRowSelection(TRUE);		/* 单击固定项选中行 */
		m_gridCtrl->SetFixedColumnSelection(FALSE);	/* 单击固顶项不选中列 */
	}
}
void AbstractXMLElemGridViewManager::CreateGridColumnMap()
{
	m_gridColumnMap.clear();
	Shared_XMLElem shared_elem = m_pElem.lock();
	if (shared_elem != 0)
	{
		int childCount = shared_elem->GetChildCount();
		InsertNodeNameColumn();
		if (childCount != 0)	/* 其下有子节点则收集所有子节点的属性列 */
		{
			for (int i = 0; i < childCount; i++)
			{
				Weak_XMLElem weak_child = shared_elem->GetChildNode(i);
				Shared_XMLElem shared_child = weak_child.lock();
				int attrCount = shared_child->GetAttributeCount();
				for (int j = 0; j < attrCount ; j++)
				{
					CString attrName = shared_child->GetAttributeName(j);
					InsertNodeAttrColumn(attrName);
				}
			}
		}
		else					/* 其下没有子节点则收集自身属性列 */
		{
			int attrCount = shared_elem->GetAttributeCount();
			for (int i = 0; i < attrCount; i++)
			{
				CString attrName = shared_elem->GetAttributeName(i);
				InsertNodeAttrColumn(attrName);
			}
		}
		InsertNodeTextColumn();
	}
}
void AbstractXMLElemGridViewManager::CreateGridRowMap()
{
	m_gridRowMap.clear();
	Shared_XMLElem shared_elem = m_pElem.lock();
	if (shared_elem != 0)
	{
		int childCount = shared_elem->GetChildCount();
		if (childCount != 0)
		{
			for (int i = 0; i < childCount ; i++)
			{
				Weak_XMLElem weak_child = shared_elem->GetChildNode(i);
				Shared_XMLElem shared_child = weak_child.lock();
				if (shared_child != 0)
				{
					m_gridRowMap.insert(std::make_pair(m_gridRowMap.size(),shared_child));
				}
			}
		} 
		else
		{
			m_gridRowMap.insert(std::make_pair(m_gridRowMap.size(),shared_elem));
		}
	}
}

void AbstractXMLElemGridViewManager::InsertNodeNameColumn()
{
	GridColumnPtr columnInfo(new GridColumnInfo());
	columnInfo->type = ColumnType_Name;
	columnInfo->width = ColumnWidth_Name;
	columnInfo->title = TRANS_T("元素名称");
	columnInfo->alignment = DT_CENTER;
	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}
void AbstractXMLElemGridViewManager::InsertNodeAttrColumn(CString attrName)
{
	CString title = GetAttrColumnTitle(attrName);
	/* 相同title的列存在则不再插入 */
	GridColumnMapItor itor = m_gridColumnMap.begin();
	while (itor != m_gridColumnMap.end())
	{
		if (itor->second->title == title)
		{
			return ;
		}
		itor++;
	}
	GridColumnPtr columnInfo(new GridColumnInfo());
	columnInfo->type = ColumnType_Attr;
	columnInfo->width = ColumnWidth_Attr;
	columnInfo->title = title;
	columnInfo->alignment = DT_CENTER;
	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}
void AbstractXMLElemGridViewManager::InsertNodeTextColumn()
{
	GridColumnPtr columnInfo(new GridColumnInfo());
	columnInfo->type = ColumnType_Text;
	columnInfo->width = ColumnWidth_Text;
	columnInfo->title = TRANS_T("元素文本");
	columnInfo->alignment = DT_CENTER;
	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}
CString AbstractXMLElemGridViewManager::GetAttrColumnTitle(CString attrName)
{
	CString columnTitle;
	if(attrName.IsEmpty())
	{
		attrName = TRANS_T("未知属性");
	}
	columnTitle.Format(TRANS_T("属性( %s )"),
		attrName);
	return columnTitle;
}

CString AbstractXMLElemGridViewManager::GetNodeNameValue(Weak_XMLElem weak_elem)
{
	CString value;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem != 0)
	{
		value = shared_elem->GetNodeName();
	}
	return value;
}
CString AbstractXMLElemGridViewManager::GetNodeAttrValue(Weak_XMLElem weak_elem,CString attrName)
{
	CString value;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem != 0)
	{
		value = shared_elem->GetAttributeValue(attrName);
	}
	return value;
}
CString AbstractXMLElemGridViewManager::GetNodeTextValue(Weak_XMLElem weak_elem)
{
	CString value;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem != 0)
	{
		value = shared_elem->GetNodeValue();
	}
	return value;
}
CString AbstractXMLElemGridViewManager::GetAttrNameFromTitle(CString columnTitle)
{
	CString attrName;
	CString attrNameW;
	CStringW titleW;
	titleW = columnTitle;
	CStringW leftSegW = L"属性( ";
	CStringW rightSegW = L" )";
	int beginPos = leftSegW.GetLength();
	int endPos = titleW.GetLength() - rightSegW.GetLength();	/* 不包含endPos处字符 */
	attrNameW = titleW.Mid(beginPos,endPos-beginPos);
	attrName = attrNameW;
	return attrName;
}