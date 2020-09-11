#ifndef  FORECAST_WORK_HOUR_VIEW_HEAD
#define FORECAST_WORK_HOUR_VIEW_HEAD

#include "../BaseFormView.h"
#include "../../ui/CGridCtrl/GridCtrl.h"
#include "../gridManger/ForecastWorkHour.h"
#include <boost/scoped_ptr.hpp>
#include "Src/action/factory/actionServerFactory.h"
#include "afxwin.h"
#include "atlcomtime.h"

class CForecastWorkHourView: public CBaseFormView
{
	DECLARE_DYNCREATE(CForecastWorkHourView)
public:
	CForecastWorkHourView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CForecastWorkHourView();

public:
	enum { IDD = IDD_FROMVIEW_FORECASTWORKHOUR };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange( CDataExchange* pDX);    // DDX/DDV ֧��
	// Generated message map functions
	//{{AFX_MSG(CEventView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	int      CreateCtrl();
	void     InitCtrl();
private:
	CGridCtrl		                         m_taskGrid;    //����������Ŀ�Ͳ������
	boost::scoped_ptr<CForecastWorkHour>       m_gridManger;
	CActionServerFactory*                    m_pActionFactory; 
	COleDateTime                             m_showBeginDate; //��������
	CComboBox                                m_ctlFindObj;
	CUser                                    m_loginUser;

	ListDepart                               m_lstDepart;
	ListProject                              m_lstProject;
public:
	afx_msg void OnDtnDatetimechangeDateBegine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeComboFindObj();
};

#endif