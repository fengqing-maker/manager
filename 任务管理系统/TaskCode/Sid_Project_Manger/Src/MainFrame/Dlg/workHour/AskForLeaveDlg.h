#pragma once
#include "resource.h"
#include "../CComonDlg.h"
#include <map>
#include "afxwin.h"
using namespace std;
// CAskForLeaveDlg 对话框

class CAskForLeaveDlg : public CComonDlg
{
	DECLARE_DYNAMIC(CAskForLeaveDlg)

public:
	CAskForLeaveDlg(const COleDateTime &date,double taskWorkHour, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAskForLeaveDlg();

// 对话框数据
	enum { IDD = IDD_DLG_ASKFORLEAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboTimeType();
	DECLARE_MESSAGE_MAP()

protected:
	void               OnInitWorkTimeType();
	void               SetNorWorkHourTotal();
private:
	CComboBox           m_timeTypeBox;
	const COleDateTime &m_setAppForDate;
	CString             m_strContent;
	double               m_taskHour;
	double               m_holidayHour;
	map<int,double>      m_norTaskHours;

	CWorkHourSeverInc*   m_pWorkHourSever;
    int                  m_loginUserId;
public:
	CEdit m_editNorWorkHour;
	CEdit m_eidtHoliday;
	afx_msg void OnEnChangeEditWorhourTime();
	afx_msg void OnEnChangeEditWorkhourHoliday();

};
