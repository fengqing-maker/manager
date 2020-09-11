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
	
	/** @brief ���أ�ɾ���б���ʱͬʱ����ItemData */
	virtual BOOL DeleteItem(_In_ int nItem);
	/** @brief ���أ�ɾ���б���ͬʱ����ItemData */
	virtual BOOL DeleteAllItems();

	virtual BOOL SetItemText(int nItem,int nSubItem,LPCTSTR lpszText);

	/** @brief ��ȡָ���е�ͼ�����(������) */
	int	GetItemImageIndex(int itemIndex);
	/** @brief ����ָ���е�ͼ�����(������) */
	void SetItemImageIndex(int itemIndex,int imageIndex);

	/** @brief չ�������� */
	void ExpandAll();
	/** @brief �۵������� */
	void CollapseAll();

	/** @brief ��ȡָ����ȫ���ı�����С��� */
	int GetMinColumnStringWidth(int nColumn);
	/** @brief ��ȡָ����ȫ���ı�������� */
	int GetMaxColumnStringWidth(int nColumn);
protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActivate(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblClick(NMHDR *pNMHDR, LRESULT *pResult);

	/** @brief ���ݱ���������Ϊ�����趨����չ����Ŀ */
	void CalcNumberOfLines(int nItem, LPCTSTR lpszText, bool insert = true);
	/** @brief ��õ�ǰ��������ʾ���������� */
	CString GetItemDisplayText(int nItem,int nSubItem);

	/** @brief �л�����չ��\�۵�״̬ */
	void OpenCloseRow(int row);


	CPen m_dotPen;				/**< @brief �Ի�ʱʹ�õĻ��� */
	
	bool m_repeatEvent;
	bool m_skipTextCheck;

	bool m_bShowGrid;			/**< @brief �Ƿ���ʾ���� */
	COLORREF m_gridColor;		/**< @brief ������ɫ */
	COLORREF m_symbolColor;		/**< @brief "+"���ŵ���ɫ */

	bool m_bShowMinimized;		/**< @brief �Ƿ���ı���С����ʾ */
	

	int m_selRow;				/**< @brief ��ǰѡ����к� */
	int m_lastSelRow;			/**< @brief ��һ��ѡ����к� */
	int m_topSelRow;			/**< @brief ��ǰѡ��Ķ����к� */
	int m_lastTopSelRow;		/**< @brief ��һ��ѡ��Ķ����к� */

	


};
