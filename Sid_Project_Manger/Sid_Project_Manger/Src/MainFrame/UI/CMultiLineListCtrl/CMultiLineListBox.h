#pragma once

class CMultiLineListBox : public CListBox
{
	// ���� 
public:
	CMultiLineListBox();
	void AddEntry(LPCTSTR lpszItem, COLORREF color, int nIndex = 0 );

	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	// ʵ��
public:
	virtual ~CMultiLineListBox();

};
