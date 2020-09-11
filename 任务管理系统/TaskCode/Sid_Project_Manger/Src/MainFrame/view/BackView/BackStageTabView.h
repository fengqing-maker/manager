#pragma once


// CDepartTabView ��ͼ
#include <map>
#include "../../../action/factory/actionServerFactory.h"


class CBackStageTabView : public CTabView
{
	DECLARE_DYNCREATE(CBackStageTabView)

protected:
	CBackStageTabView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CBackStageTabView();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

public:
		/** @brief ѡ���� */
	//���ž����ܾ�������
	enum Tab_Index{ userMangerTabInex, productLineTabIndex, rolePowerTabIndex, workTimeType };
	/**
	*@brief	����ָ��Tabҳ��
	*@param [in]	index	ָ�������ҳ���
	*@see 	Tab_Index
	*/
	void ActiveTab(int index);
	/**
	*@brief	����Tab��ǩ�ı�����
	*@param [in]	index	��Ҫ���µı�ǩ���
	*@note 	��ǩ���ʹ��Ĭ��ֵ-1��ȫ����ǩ�����£����������ָ����ǩ��
	*@see 	Tab_Index
	*/
	void UpdateTabLabel(Tab_Index index);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	//virtual int OnCreate(LPCREATESTRUCT lpCreateStruct)
	afx_msg void    OnSize(UINT nType, int cx, int cy);
	//afx_msg LRESULT OnRecordShowPrjIs( WPARAM wPara, LPARAM lPara );
	//afx_msg LRESULT OnRecordTaskStSelIndex( WPARAM wPara, LPARAM lPara );

	DECLARE_MESSAGE_MAP()
private:
		CString GetTabString( Tab_Index index ) const;
		int     OnCreateTab();
private:
	TabIndxeMap                      m_tabIndex;
	CActionServerFactory*            m_pSever;
	int             m_selPrjId;
	int             m_selTaskStatusIndex; //ѡ�����ѡ�˵ڼ���
	//std::unique_ptr<CMyTaskView>      m_myTaskView;

};


