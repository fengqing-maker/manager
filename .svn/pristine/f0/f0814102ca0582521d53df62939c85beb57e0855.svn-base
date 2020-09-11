#include "stdafx.h"
#include "EP61850Client.h"
#include "AbstractXMLAttrGridViewManager.h"

#include "XMLSOMElement.h"
#include "XMLSOMAttribute.h"
#include "XMLAttributeInsertDialog.h"

AbstractXMLAttrGridViewManager::AbstractXMLAttrGridViewManager(void)
{
}
AbstractXMLAttrGridViewManager::~AbstractXMLAttrGridViewManager(void)
{
}

/******************************protected function*************************/
void AbstractXMLAttrGridViewManager::OutputFileMsg(CString msg)
{

}
void AbstractXMLAttrGridViewManager::OutputFileErr(CString err)
{

}
void AbstractXMLAttrGridViewManager::OutputValidateMsg(CString msg)
{

}
void AbstractXMLAttrGridViewManager::OutputValidateErr(CString err)
{

}

void AbstractXMLAttrGridViewManager::PreCreateGridView()
{
	CreateGridColumnMap();

	CreateGridRowMap();
}
void AbstractXMLAttrGridViewManager::AftCreateGridView()
{
	if (m_gridCtrl != 0)
	{
		m_gridCtrl->AutoSizeColumns(GVS_DEFAULT);
		m_gridCtrl->ExpandLastColumn();
	}
}

void AbstractXMLAttrGridViewManager::PreUpdateGridView()
{
	CreateGridColumnMap();

	CreateGridRowMap();
}
void AbstractXMLAttrGridViewManager::AftUpdateGridView()
{
	if (m_gridCtrl != 0)
	{
		m_gridCtrl->AutoSizeColumns(GVS_DEFAULT);
		m_gridCtrl->ExpandLastColumn();
	}
}

int AbstractXMLAttrGridViewManager::GetFlexColumnCount()
{
	return m_gridColumnMap.size();
}
int AbstractXMLAttrGridViewManager::GetFlexRowCount()
{
	return m_gridRowMap.size();
}

int AbstractXMLAttrGridViewManager::GetFlexColumnWidth(int columnIndex)
{
	int width = 0;
	GridColumnMapItor itor = m_gridColumnMap.find(columnIndex);
	ASSERT(itor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = itor->second;
	ASSERT(columnInfo != 0);
	width  =  columnInfo->width;
	return width;
}

GridItemType AbstractXMLAttrGridViewManager::GetFlexItemType(int nRow,int nColumn)
{
	GridItemType itemType = GridItemType_Edit;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
		itemType = GridItemType_Edit;
		break;
	case ColumnType_AttrValue:
		{
			switch (rowInfo->type)
			{
			case RowType_ElemName:
				itemType = GridItemType_Edit;
				break;
			case RowType_ElemAttr:
				itemType = GetElemAttrItemType(rowInfo->weak_elem,rowInfo->attrName);
				break;
			case RowType_ElemText:
				itemType = GridItemType_Edit;
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return itemType;
}
int AbstractXMLAttrGridViewManager::GetFlexItemExtensionType(int nRow,int nColumn)
{
	AttrGridItemType itemType = AttrGridItemType_String;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
		ASSERT(0);
		break;
	case ColumnType_AttrValue:
		{
			switch (rowInfo->type)
			{
			case RowType_ElemName:
				ASSERT(0);
				break;
			case RowType_ElemAttr:
				itemType = GetElemAttrItemExtensionType(rowInfo->weak_elem,rowInfo->attrName);
				break;
			case RowType_ElemText:
				itemType = AttrGridItemType_String;
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	return itemType;
}
BOOL AbstractXMLAttrGridViewManager::InitFlexExtensionItem(int extensionItemType,int nRow,int nColumn)
{	
	/* 此处nRow与nColumn都是可变表项行、列号，
	需要加上固定表项数目才是在整张表中的表项行、列号 */
	BOOL result = TRUE;
	switch (extensionItemType)
	{
	case AttrGridItemType_Ref:
		{
			if(!m_gridCtrl->SetCellType(nRow+m_fixedRowCount,nColumn+m_fixedColumnCount,
				RUNTIME_CLASS(CXMLGridCellForRef)))
				return FALSE;
			CXMLGridCellForRef* pRefCell = dynamic_cast<CXMLGridCellForRef*>
				(m_gridCtrl->GetCell(nRow+m_fixedRowCount,nColumn+m_fixedColumnCount));
			result = InitFlexRefItem(pRefCell,nRow,nColumn);
		}
		break;
	case AttrGridItemType_Enum:
		{
			if (!m_gridCtrl->SetCellType(nRow+m_fixedRowCount,nColumn+m_fixedColumnCount,
				RUNTIME_CLASS(CXMLGridCellForEnum)))
				return FALSE;
			CXMLGridCellForEnum* pEnumCell = dynamic_cast<CXMLGridCellForEnum*>
				(m_gridCtrl->GetCell(nRow+m_fixedRowCount,nColumn+m_fixedColumnCount));
			result = InitFlexEnumItem(pEnumCell,nRow,nColumn);
		}
	default:
		result = TRUE;
		break;
	}
	return result;
}
BOOL AbstractXMLAttrGridViewManager::InitFlexRefItem(CXMLGridCellForRef* pCell,int nRow,int nColumn)
{
	AttrGridItemType itemType = AttrGridItemType_String;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	ASSERT(columnInfo->type == ColumnType_AttrValue);
	ASSERT(rowInfo->type == RowType_ElemAttr);
	RefOptionMap attrRefOptionMap;
	if (!GetElemAttrRefOptionMap(rowInfo->weak_elem,rowInfo->attrName,attrRefOptionMap))
	{
		attrRefOptionMap.clear();
	}
	pCell->SetOptions(attrRefOptionMap);
	return TRUE;
}
BOOL AbstractXMLAttrGridViewManager::InitFlexEnumItem(CXMLGridCellForEnum* pCell,int nRow,int nColumn)
{
	AttrGridItemType itemType = AttrGridItemType_String;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	ASSERT(columnInfo->type == ColumnType_AttrValue);
	ASSERT(rowInfo->type == RowType_ElemAttr);
	CStringArray attrEnumOptionArray;
	if (!GetElemAttrEnumArray(rowInfo->weak_elem,rowInfo->attrName,attrEnumOptionArray))
	{
		attrEnumOptionArray.RemoveAll();
	}
	pCell->SetOptions(attrEnumOptionArray);
	return TRUE;
}

BOOL AbstractXMLAttrGridViewManager::GetFlexItemEditable(int nRow,int nColumn)
{
	BOOL bEditable = FALSE;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
		bEditable = FALSE;
		break;
	case ColumnType_AttrValue:
		{
			switch (rowInfo->type)
			{
			case RowType_ElemName:
				bEditable = FALSE;
				break;
			case RowType_ElemAttr:
				bEditable = TRUE;
				break;
			case RowType_ElemText:
				bEditable = TRUE;
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return bEditable;
}
CString AbstractXMLAttrGridViewManager::GetFixedItemText(int nRow, int nColumn)
{
	CString text;
	int fixedColumnCount = GetFixedColumnCount();
	if (nColumn >= fixedColumnCount)
	{
		GridColumnMapItor colItor = m_gridColumnMap.find(nColumn-fixedColumnCount);
		ASSERT(colItor != m_gridColumnMap.end());
		GridColumnPtr columnInfo = colItor->second;
		text = columnInfo->title;
	} 
	else
	{
		text = AbstractXMLGridViewManager::GetFixedItemText(nRow,nColumn);
	}
	return text;
}
CString AbstractXMLAttrGridViewManager::GetFlexItemText(int nRow, int nColumn)
{
	CString text;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	Shared_XMLElem shared_elem = rowInfo->weak_elem.lock();
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
		{
			text = rowInfo->title;
		}
		break;
	case ColumnType_AttrValue:
		{
			switch (rowInfo->type)
			{
			case RowType_ElemName:
				text = shared_elem->GetNodeName();
				break;
			case RowType_ElemAttr:
				text = shared_elem->GetAttributeValue(rowInfo->attrName);
				break;
			case RowType_ElemText:
				text = shared_elem->GetNodeValue();
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return text;
}
BOOL AbstractXMLAttrGridViewManager::SetFlexItemText(int nRow, int nColumn,CString text)
{
	BOOL result = TRUE;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	Shared_XMLElem shared_elem = rowInfo->weak_elem.lock();
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
		result = TRUE;
		break;
	case ColumnType_AttrValue:
		{
			switch (rowInfo->type)
			{
			case RowType_ElemName:
				result = TRUE;
				break;
			case RowType_ElemAttr:
				{
					result = shared_elem->SetAttributeValue(rowInfo->attrName,text);
					if (result)
					{
						CString elemName = shared_elem->GetNodeName();
						CString msg;
						msg.Format(TRANS_T("编辑元素\"%s\"属性\"%s\"值\"%s\"成功."),
							elemName,rowInfo->attrName,text);
						OutputFileMsg(msg);
					}
					else
					{
						CString elemName = shared_elem->GetNodeName();
						CString err;
						err.Format(TRANS_T("编辑元素\"%s\"属性\"%s\"值\"%s\"失败."),
							elemName,rowInfo->attrName,text);
						OutputFileErr(err);
					}
				}
				break;
			case RowType_ElemText:
				{
					result = shared_elem->SetNodeValue(text);
					if (result)
					{
						CString elemName = shared_elem->GetNodeName();
						CString msg;
						msg.Format(TRANS_T("编辑元素\"%s\"文本值\"%s\"成功."),
							elemName,text);
						OutputFileMsg(msg);
					}
					else
					{
						CString elemName = shared_elem->GetNodeName();
						CString err;
						err.Format(TRANS_T("编辑元素\"%s\"文本值\"%s\"失败."),
							elemName,text);
						OutputFileErr(err);
					}
				}
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}
	VERIFY(UpdateElemGridView());
	return result;
}
BOOL AbstractXMLAttrGridViewManager::ValidateFlexItemText(int nRow,int nColumn,CString text)
{
	BOOL result = TRUE;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	Shared_XMLElem shared_elem = rowInfo->weak_elem.lock();
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
		result = TRUE;
		break;
	case ColumnType_AttrValue:
		{
			switch (rowInfo->type)
			{
			case RowType_ElemName:
				result = TRUE;
				break;
			case RowType_ElemAttr:
				result = ValidateElemAttrValue(rowInfo->weak_elem,rowInfo->attrName,text);
				break;
			case RowType_ElemText:
				result = ValidateElemTextValue(rowInfo->weak_elem,text);
				break;
			default:
				ASSERT(0);
				break;
			}
		}
		break;
	default:
		ASSERT(0);
		break;
	}

	return result;
}

BOOL AbstractXMLAttrGridViewManager::InitFlexItemPopupMenu(int nRow,int nColumn,CMenu* popupMenu)
{
	BOOL result = FALSE;
	GridColumnMapItor colItor = m_gridColumnMap.find(nColumn);
	ASSERT(colItor != m_gridColumnMap.end());
	GridColumnPtr columnInfo = colItor->second;
	AttrGridRowMapItor rowItor = m_gridRowMap.find(nRow);
	ASSERT(rowItor != m_gridRowMap.end());
	AttrGridRowPtr rowInfo = rowItor->second;
	switch (columnInfo->type)
	{
	case ColumnType_AttrName:
	case ColumnType_AttrValue:
		switch (rowInfo->type)
		{
		case RowType_ElemName:
		case RowType_ElemAttr:
		case RowType_ElemText:
			result = CreateGridPopupMenuMap(rowInfo->weak_elem,rowInfo->attrName,popupMenu);
			break;
		default:
			result = FALSE;
			break;
		}
		break;
	default:
		result = FALSE;
		break;
	}
	return result;
}
BOOL AbstractXMLAttrGridViewManager::ResponseFlexItemPopupMenu(UINT menuID)
{
	BOOL result = FALSE;
	AttrPopMenuMapItor menuItor = m_gridMenuMap.find(menuID);
	if (menuItor != m_gridMenuMap.end())
	{
		AttrPopMenuPtr menuInfo = menuItor->second;
		Shared_XMLElem shared_elem = menuInfo->weak_elem.lock();
		ASSERT(shared_elem != 0);
		switch (menuInfo->menuType)
		{
		case MenuType_Insert:
			result = ResponsePopupMenuInsertAttr(menuInfo->weak_elem,menuInfo->attrName);
			break;
		case MenuType_Delete:
			result = ResponsePopupMenuDeleteAttr(menuInfo->weak_elem,menuInfo->attrName);
			break;
		default:
			result = FALSE;	/* 未处理的菜单 */
			break;
		}
	}
	return result;
}

BOOL AbstractXMLAttrGridViewManager::UpdateElemGridView()
{
	return TRUE;
}
BOOL AbstractXMLAttrGridViewManager::UpdateAttrGridView()
{
	return TRUE;
}
/******************************private function***************************/
void AbstractXMLAttrGridViewManager::OutputValidateMsgList(XMLStringList msgList)
{
	XMLStringListItor itor = msgList.begin();
	while (itor != msgList.end())
	{
		CString msg = *itor;
		OutputValidateMsg(msg);
		itor++;
	}
}
void AbstractXMLAttrGridViewManager::OutputValidateErrList(XMLStringList errList)
{
	XMLStringListItor itor = errList.begin();
	while (itor != errList.end())
	{
		CString err = *itor;
		OutputValidateErr(err);
		itor++;
	}
}

void AbstractXMLAttrGridViewManager::CreateGridColumnMap()
{
	m_gridColumnMap.clear();
	InsertAttrNameColumn();
	InsertAttrValueColumn();
}
void AbstractXMLAttrGridViewManager::CreateGridRowMap()
{
	m_gridRowMap.clear();
	Shared_XMLElem shared_elem = m_pElem.lock();
	if (shared_elem != 0)
	{
		InsertElemNameRow(m_pElem);
		int attrCount = shared_elem->GetAttributeCount();
		for (int i = 0; i < attrCount; i++)
		{
			CString attrName = shared_elem->GetAttributeName(i);
			if (!attrName.IsEmpty())
			{
				InsertElemAttrRow(m_pElem,attrName);
			}
		}
		InsertElemTextRow(m_pElem);
	}
}

void AbstractXMLAttrGridViewManager::InsertAttrNameColumn()
{
	GridColumnPtr columnInfo( new GridColumnInfo());
	columnInfo->title = TRANS_T("属性名称");
	columnInfo->type = ColumnType_AttrName;
	columnInfo->width = 100;
	columnInfo->alignment = DT_CENTER;

	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}
void AbstractXMLAttrGridViewManager::InsertAttrValueColumn()
{
	GridColumnPtr columnInfo( new GridColumnInfo());
	columnInfo->title = TRANS_T("属性值");
	columnInfo->type = ColumnType_AttrValue;
	columnInfo->width = 100;
	columnInfo->alignment = DT_CENTER;

	m_gridColumnMap.insert(std::make_pair(m_gridColumnMap.size(),columnInfo));
}

void AbstractXMLAttrGridViewManager::InsertElemNameRow(Weak_XMLElem weak_elem)
{
	AttrGridRowPtr rowInfo(new AttrGridRowInfo());
	rowInfo->title = TRANS_T("元素名称");
	rowInfo->type = RowType_ElemName;
	rowInfo->weak_elem = weak_elem;
	rowInfo->attrName.Empty();

	m_gridRowMap.insert(std::make_pair(m_gridRowMap.size(),rowInfo));
}
void AbstractXMLAttrGridViewManager::InsertElemAttrRow(Weak_XMLElem weak_elem,CString attrName)
{
	AttrGridRowPtr rowInfo(new AttrGridRowInfo());
	rowInfo->title = GetElemAttrTitle(attrName);
	rowInfo->type = RowType_ElemAttr;
	rowInfo->weak_elem = weak_elem;
	rowInfo->attrName = attrName;

	m_gridRowMap.insert(std::make_pair(m_gridRowMap.size(),rowInfo));
}
void AbstractXMLAttrGridViewManager::InsertElemTextRow(Weak_XMLElem weak_elem)
{
	AttrGridRowPtr rowInfo(new AttrGridRowInfo());
	rowInfo->title = TRANS_T("元素文本");
	rowInfo->type = RowType_ElemText;
	rowInfo->weak_elem = weak_elem;
	rowInfo->attrName.Empty();

	m_gridRowMap.insert(std::make_pair(m_gridRowMap.size(),rowInfo));
}
CString AbstractXMLAttrGridViewManager::GetElemAttrTitle(CString attrName)
{
	CString attrTitle;
	if(attrName.IsEmpty())
	{
		attrName = TRANS_T("未知属性");
	}
	attrTitle.Format(TRANS_T("属性( %s )"),
		attrName);
	return attrTitle;
}

GridItemType	AbstractXMLAttrGridViewManager::GetElemAttrItemType
	(Weak_XMLElem weak_elem,CString attrName)
{
	GridItemType itemType = GridItemType_Edit;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return itemType;
	CXMLSOMAttributePtr pSOMAttribute = shared_elem->GetAttributeConstraint(attrName);

	XMLStringList attrValueEnumList;
	XMLStringList attrValueRefList;
	if(pSOMAttribute != NULL)
	{
		attrValueEnumList = pSOMAttribute->GetAttributeValueEnumList();
		attrValueRefList = pSOMAttribute->GetAttributeValueRefList();
	}
	if (attrValueEnumList.size() > 0 || attrValueRefList.size() > 0)
	{
		itemType = GridItemType_Extension;
	}
	else
	{
		itemType = GridItemType_Edit;
	}
	return itemType;
}
AttrGridItemType	AbstractXMLAttrGridViewManager::GetElemAttrItemExtensionType
	(Weak_XMLElem weak_elem,CString attrName)
{
	AttrGridItemType itemType = AttrGridItemType_String;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return itemType;
	XMLStringList attrValueEnumList = 
		shared_elem->GetAttributeValueEnumList(attrName);
	XMLStringList attrValueRefList = 
		shared_elem->GetAttributeValueRefList(attrName);
	if (attrValueRefList.size() > 0)
	{
		itemType = AttrGridItemType_Ref;
		return itemType;
	}
	if (attrValueEnumList.size() > 0)
	{
		itemType = AttrGridItemType_Enum;
		return itemType;
	}
	return itemType;
}
BOOL AbstractXMLAttrGridViewManager::GetElemAttrRefOptionMap
	(Weak_XMLElem weak_elem,CString attrName,RefOptionMap& refOptionMap)
{
	refOptionMap.clear();
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	XMLStringList refList = shared_elem->GetAttributeValueRefList(attrName);
	if (refList.size() == 0)
	{
		return FALSE;
	}
	XMLStringListItor itor = refList.begin();
	while (itor != refList.end())
	{
		CString refValue = *itor;
		RefOption option;
		option.optionKey = refValue;
		/* 从“attrName = attrValue , ”中截取attrValue值 */
		int attrNamePos = refValue.Find(attrName,0);
		ASSERT(attrNamePos != -1);
		CString tokenEqual = STR_TOKEN_EQUAL;
		int attrValueStartPos = refValue.Find(tokenEqual,attrNamePos) + tokenEqual.GetLength();
		CString tokenSeparator = STR_TOKEN_SEPARATOR;
		int attrValueEndPos = refValue.Find(tokenSeparator,attrNamePos);
		if (attrValueEndPos == -1)
		{
			attrValueEndPos = refValue.GetLength();
		}
		option.optionValue = refValue.Mid(attrValueStartPos,attrValueEndPos-attrValueStartPos);
		refOptionMap.insert(std::make_pair(option.optionKey,option));
		itor++;
	}
	return TRUE;
}
BOOL AbstractXMLAttrGridViewManager::GetElemAttrEnumArray
	(Weak_XMLElem weak_elem,CString attrName,CStringArray& enumArray)
{
	enumArray.RemoveAll();
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		return FALSE;
	}
	XMLStringList enumList = shared_elem->GetAttributeValueEnumList(attrName);
	if (enumList.size() == 0)
	{
		return FALSE;
	}
	XMLStringListItor itor = enumList.begin();
	while (itor != enumList.end())
	{
		CString enumValue = *itor;
		enumArray.Add(enumValue);
		itor++;
	}
	return TRUE;
}

BOOL AbstractXMLAttrGridViewManager::ValidateElemAttrValue(Weak_XMLElem weak_elem,CString attrName,CString attrValue)
{
	BOOL result = TRUE;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		/* 无法进行验证 */
		return TRUE;
	}
	CString elemName = shared_elem->GetNodeName();
	CString oldAttributeValue = shared_elem->GetAttributeValue(attrName);
	CString newAttributeValue = attrValue;
	/* 为属性设置新值，才能进行校验 */
	if (shared_elem->SetAttributeValue(attrName,newAttributeValue))
	{
		XMLStringList errorInfoList;
		if (!shared_elem->ValidateAttribute(attrName,errorInfoList))
		{
			OutputValidateErrList(errorInfoList);
			/* 新的属性值未通过校验，向用户征求意见是否允许错误的输入 */
			CString msg;
			msg.Format(TRANS_T("元素\"%s\"的属性\"%s\"值\"%s\"校验错误，\n是否强制通过校验并保持更改？"),
				elemName,attrName,newAttributeValue);
			int choice = YesNoDialog(msg);
			if (choice == IDYES)
			{
				result = TRUE;
			}
			else if (choice == IDNO)
			{
				result = FALSE;
			}
		}
		else
		{
			/* 新的属性值通过了校验 */
			result = TRUE;
		}
	}
	/* 还原属性值 */
	shared_elem->SetAttributeValue(attrName,oldAttributeValue);
	return result;
}
BOOL AbstractXMLAttrGridViewManager::ValidateElemTextValue(Weak_XMLElem weak_elem,CString textValue)
{
	BOOL result = TRUE;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
	{
		/* 无法进行验证 */
		return TRUE;
	}
	CString elemName = shared_elem->GetNodeName();
	CString oldTextValue = shared_elem->GetNodeValue();
	CString newTextValue = textValue;
	/* 为属性设置新值，才能进行校验 */
	if (shared_elem->SetNodeValue(newTextValue))
	{
		XMLStringList errorInfoList;
		/* 无法针对元素的文本值进行单独校验，因此只能对当前元素进行整体校验 */
		if (!shared_elem->ValidateElement(errorInfoList))
		{
			OutputValidateErrList(errorInfoList);
			/* 新的文本值未通过校验，向用户征求意见是否允许错误的输入 */
			CString msg;
			msg.Format(TRANS_T("元素\"%s\"校验结果存在错误，\n是否强制通过校验并保持更改？"),
				elemName);
			int choice = YesNoDialog(msg);
			if (choice == IDYES)
			{
				result = TRUE;
			}
			else if (choice == IDNO)
			{
				result = FALSE;
			}
		}
		else
		{
			/* 新的属性值通过了校验 */
			result = TRUE;
		}
	}
	/* 还原属性值 */
	shared_elem->SetNodeValue(oldTextValue);
	return result;
}
int AbstractXMLAttrGridViewManager::YesNoDialog(CString msg)
{
	HWND hWnd = AfxGetApp()->m_pMainWnd->m_hWnd;
	int result = ::MessageBox(hWnd,msg,_T("Warning!"),MB_YESNO|MB_ICONQUESTION);
	return result;
}

BOOL AbstractXMLAttrGridViewManager::CreateGridPopupMenuMap(Weak_XMLElem weak_elem,CString attrName,CMenu* popupMenu)
{
	m_gridMenuMap.clear();
	BOOL result = FALSE;
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	CXMLSOMElementPtr pSOMElement = shared_elem->GetElementConstraint();
	/* “插入属性”菜单项 */
	XMLStringList nonexistAttrList;
	if (pSOMElement != NULL)
	{
		nonexistAttrList = pSOMElement->GetNonExsitAttributeNameList();
	} 
	else
	{
		/* 对于未加载Schema约束的元素可以插入任意属性 */
		nonexistAttrList.push_back(_T("AnyAttribute"));
	}
	XMLStringListItor nonexistItor = nonexistAttrList.begin();
	if (nonexistItor != nonexistAttrList.end())
	{
		CMenu insertMenu;
		if(!insertMenu.CreatePopupMenu())
			return FALSE;
		while (nonexistItor != nonexistAttrList.end())
		{
			result = InsertPopupMenuInsertAttr(weak_elem,*nonexistItor,&insertMenu);
			ASSERT(result);
			nonexistItor++;
		}
		HMENU hMenu = insertMenu.Detach();
		popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hMenu,TRANS_T("插入属性"));
	}
	/* “删除属性”菜单项 */
	XMLStringList existAttrList;
	int existAttrCount = shared_elem->GetAttributeCount();
	for (int i = 0 ; i < existAttrCount ; i++)
	{
		CString attrName = shared_elem->GetAttributeName(i);
		existAttrList.push_back(attrName);
	}
	XMLStringListItor existItor = existAttrList.begin();
	if (existItor != existAttrList.end())
	{
		CMenu deleteMenu;
		if(!deleteMenu.CreatePopupMenu())
			return FALSE;
		while (existItor != existAttrList.end())
		{
			result = InsertPopupMenuDeleteAttr(weak_elem,*existItor,&deleteMenu);
			ASSERT(result);
			existItor++;
		}
		HMENU hMenu = deleteMenu.Detach();
		popupMenu->AppendMenu(MF_POPUP,(UINT_PTR)hMenu,TRANS_T("删除属性"));
	}
	return result;
}
BOOL AbstractXMLAttrGridViewManager::InsertPopupMenuInsertAttr(Weak_XMLElem weak_elem,CString attrName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	AttrPopMenuPtr menuInfo(new AttrPopMenuInfo());
	menuInfo->menuID = m_gridMenuMap.size()+WM_GRID_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Insert;
	menuInfo->menuTitle.Format(TRANS_T("%s"),
		attrName);
	menuInfo->weak_elem = weak_elem;
	menuInfo->attrName = attrName;

	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_gridMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractXMLAttrGridViewManager::ResponsePopupMenuInsertAttr(Weak_XMLElem weak_elem,CString attrName)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0 || attrName.IsEmpty())
		return FALSE;
	BOOL result = FALSE;
	CString elemName = shared_elem->GetNodeName();
	if (attrName == _T("AnyAttribute"))
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		CString defAttrName = _T("AnyAttribute");
		CString defAttrNamespace = _T("");
		CString defAttrValue = _T("");
		CXMLAttributeInsertDialog attrDialog(defAttrName,defAttrNamespace,defAttrValue,pMainFrm);
		attrDialog.DoModal();
		int choice = attrDialog.GetChoice();
		if (choice == IDCANCEL)
		{
			return TRUE;
		}
		else if (choice == IDOK)
		{
			attrName = attrDialog.GetAttributeName();
			CString attrNamespace = attrDialog.GetAttributeNamespaceURI();
			CString attrValue = attrDialog.GetAttributeValue();
			result = shared_elem->AddAttributeNode(attrName,attrNamespace,attrValue);
		}
		else
		{
			ASSERT(0);	/* should never happen */
			result = FALSE;
		}
	}
	else
	{
		result = shared_elem->AddAttributeNode(attrName);
	}
	/* 插入属性后同时更新元素与属性列表 */
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("向元素\"%s\"插入属性\"%s\"成功."),
			elemName,attrName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("向元素\"%s\"插入属性\"%s\"失败."),
			elemName,attrName);
		OutputFileErr(err);
	}
	return result;
}
BOOL AbstractXMLAttrGridViewManager::InsertPopupMenuDeleteAttr(Weak_XMLElem weak_elem,CString attrName,CMenu* popupMenu)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	AttrPopMenuPtr menuInfo(new AttrPopMenuInfo());
	menuInfo->menuID = m_gridMenuMap.size()+WM_GRID_POPUP_MENUID_MIN;
	menuInfo->menuType = MenuType_Delete;
	menuInfo->menuTitle.Format(TRANS_T("%s"),
		attrName);
	menuInfo->weak_elem = weak_elem;
	menuInfo->attrName = attrName;

	if(!popupMenu->AppendMenu(MF_STRING,menuInfo->menuID,menuInfo->menuTitle))
		return FALSE;
	m_gridMenuMap.insert(std::make_pair(menuInfo->menuID,menuInfo));
	return TRUE;
}
BOOL AbstractXMLAttrGridViewManager::ResponsePopupMenuDeleteAttr(Weak_XMLElem weak_elem,CString attrName)
{
	Shared_XMLElem shared_elem = weak_elem.lock();
	if (shared_elem == 0)
		return FALSE;
	CString elemName = shared_elem->GetNodeName();
	BOOL result = shared_elem->RemoveAttributeNode(attrName);
	/* 插入属性后同时更新元素与属性列表 */
	VERIFY(UpdateElemGridView());
	VERIFY(UpdateAttrGridView());
	if (result)
	{
		CString msg;
		msg.Format(TRANS_T("删除元素\"%s\"下属性\"%s\"成功."),
			elemName,attrName);
		OutputFileMsg(msg);
	}
	else
	{
		CString err;
		err.Format(TRANS_T("删除元素\"%s\"下属性\"%s\"失败."),
			elemName,attrName);
		OutputFileErr(err);
	}
	return result;
}