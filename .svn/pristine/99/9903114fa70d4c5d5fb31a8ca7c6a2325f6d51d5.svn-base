#pragma once

#include "SvrOutputList.h"
#include "SvrOutputManager.h"
#include "Src/MainFrame/ui/COutputList/OutputList.h"

// CSvrOutputList

class CSvrOutputList : public COutputListCtrl
{
	DECLARE_DYNAMIC(CSvrOutputList)

public:
	CSvrOutputList();
	virtual ~CSvrOutputList();

	//void AdjustLayout();

protected:
	/* 标准Windows消息响应 */
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//afx_msg void OnPaint();
	afx_msg LRESULT OnUpdateOutputList(WPARAM wParam,LPARAM lParam);
	afx_msg BOOL OnDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL OnClick(NMHDR *pNMHDR, LRESULT *pResult);
	/* 右键弹出菜单消息响应 */
	afx_msg void OnRespPopupMenu(UINT menuID);
	afx_msg void OnUpdatePopupMenu(CCmdUI *pCmdUI);


	DECLARE_MESSAGE_MAP()

private:
	void installComponent();
	void uninstallComponent();

	
};


