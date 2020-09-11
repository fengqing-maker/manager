#pragma once

#include "resource.h"
#include "../view/prjView/CProjectScheduleView.h"
#include "Src/DataBase/inc/daocomonDef.h"
#include "Src/Model/TaskChange.h"
// CDlgTaskPictrueView 对话框

class CDlgTaskPictrueView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTaskPictrueView)

public:
	CDlgTaskPictrueView(ListTask *pTaskList, const CTaskChange &chObj, const CTaskObj &chAfterObj, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgTaskPictrueView();

// 对话框数据
	enum { IDD = IDD_DLG_CHANGETASK_PICTRUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()
protected:
	CProjectScheduleView       m_ctrlView;
	CProjectScheduleView*       m_pctrlView;
    CFrameWnd*                  m_pFrame;
	ListTask*                  m_pListTask;
	CTaskChange                m_chObj;
	CTaskObj                   m_changeAfterTask;
};
