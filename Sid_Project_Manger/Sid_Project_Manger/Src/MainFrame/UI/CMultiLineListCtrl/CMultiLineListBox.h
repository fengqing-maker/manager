#pragma once

class CMultiLineListBox : public CListBox
{
	// 构造 
public:
	CMultiLineListBox();
	void AddEntry(LPCTSTR lpszItem, COLORREF color, int nIndex = 0 );

	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	// 实现
public:
	virtual ~CMultiLineListBox();

};
