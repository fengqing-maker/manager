#pragma once

// CMultiLineListCtrl

class CMultiLineListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMultiLineListCtrl)

public:
	CMultiLineListCtrl();
	virtual ~CMultiLineListCtrl();
	void ShowGrid(bool show);
	void ShowMinimized(bool show);
	BOOL SetGridColor(COLORREF color); 
	BOOL SetBkColor(COLORREF cr);

	virtual int InsertItem(const LVITEM* pItem);
	virtual int InsertItem(int nItem,LPCTSTR lpszItem);
	virtual int InsertItem(int nItem,LPCTSTR lpszItem,int nImage);
	
	/** @brief 重载，删除列表项时同时重置ItemData */
	virtual BOOL DeleteItem(_In_ int nItem);
	/** @brief 重载，删除列表项同时重置ItemData */
	virtual BOOL DeleteAllItems();

	virtual BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);

	/** @brief 获取指定行的图标序号(非重载) */
	int	GetItemImageIndex(int itemIndex);
	/** @brief 设置指定行的图标序号(非重载) */
	void SetItemImageIndex(int itemIndex,int imageIndex);

	/** @brief 展开所有行 */
	void ExpandAll();
	/** @brief 折叠所有行 */
	void CollapseAll();

	/** @brief 获取指定列全部文本的最小宽度 */
	int GetMinColumnStringWidth(int nColumn);
	/** @brief 获取指定列全部文本的最大宽度 */
	int GetMaxColumnStringWidth(int nColumn);
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblClick(NMHDR *pNMHDR, LRESULT *pResult);

	/** @brief 根据表项中文字为表项设定其扩展行数目 */
	void CalcNumberOfLines(int nItem, LPCTSTR lpszText, bool insert = true);
	/** @brief 获得当前表项中显示的文字内容 */
	CString GetItemDisplayText(int nItem,int nSubItem);

	/** @brief 切换表项展开\折叠状态 */
	void OpenCloseRow(int row);


	CPen m_dotPen;				/**< @brief 自绘时使用的画笔 */
	
	bool m_repeatEvent;
	bool m_skipTextCheck;

	bool m_bShowGrid;			/**< @brief 是否显示网格 */
	COLORREF m_gridColor;		/**< @brief 网格颜色 */
	COLORREF m_symbolColor;		/**< @brief "+"符号的颜色 */

	bool m_bShowMinimized;		/**< @brief 是否对文本最小化显示 */
	

	int m_selRow;				/**< @brief 当前选择的行号 */
	int m_lastSelRow;			/**< @brief 上一次选择的行号 */
	int m_topSelRow;			/**< @brief 当前选择的顶端行号 */
	int m_lastTopSelRow;		/**< @brief 上一次选择的顶端行号 */

	


};
