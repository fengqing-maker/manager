// TaskTabView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProjectTabView.h"
#include "../../MainFrm.h"
#include "CProjectView.h"
#include "CProjectTaskView.h"
#include "CProjectScheduleView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CProjectTabView

IMPLEMENT_DYNCREATE(CProjectTabView, CTabView)

CProjectTabView::CProjectTabView()
{
	m_selPrjId = 0;
	m_selTaskStatusIndex = -1;
}

//����ͼ�˳���ʱ��ɾ�����е�Tab��
CProjectTabView::~CProjectTabView()
{
	if ( ::IsWindow( m_wndTabs.GetSafeHwnd() )  )
	{
		m_wndTabs.RemoveAllTabs();
	}
}

BEGIN_MESSAGE_MAP(CProjectTabView, CTabView)
	ON_WM_CREATE()
	ON_MESSAGE( PRJ_MANGER_MSG_CHANGE,  OnChangePrjTaskView)
	ON_MESSAGE( PRJ_VIEW_MSG_RECODE_PRJID,  OnRecordShowPrjIs)
	ON_MESSAGE( PRJ_VIEW_MSG_RECODE_TASK_INDEX,  OnRecordTaskStSelIndex)
	ON_MESSAGE( PRJ_VIEW_MSG_CHANGE_TASKLIST, OnActivteTaskPictrue)
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CProjectTabView ���

#ifdef _DEBUG
void CProjectTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CProjectTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CProjectTabView ��Ϣ�������
void CProjectTabView::ActiveTab(int index)
{
	SetActiveView( index );
	//SetActiveTab(index);
}

//��ѡ��е������������ѡ��ؼ����һ�����ı�ǩ���������ѡ��
void CProjectTabView::UpdateTabLabel(Tab_Index index)
{
	CString tabLabel;
	//map<int,int>�ĵ�����
	TabIndxeMap_Itor itor = m_tabIndex.find( index );			//�����в���ѡ���
	if ( itor == m_tabIndex.end() )
	{
		return;
	}
	int tabIndex = (*itor).second;	
	CString str;
	//Ϊѡ���ӱ���
	str = GetTabString(  index );
	m_wndTabs.SetTabLabel(tabIndex,str);		//
}
CString  CProjectTabView::GetTabString( Tab_Index index ) const
{
	CString str;
	switch( index)
	{
		case projectTabIndex:
			str = _T("��Ŀ�б�");
			break;
		case  taskTabIndex:
			str = _T("��Ŀ����");
			break;
		case  schedulePictrue:
			str = _T("�������ͼ");
			break;
	}
	return str;
}

//��ѡ������һ������ѡ�
int  CProjectTabView::OnCreateTab()				
{
	//
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	/* ͨ���ı�VS�Զ�Ĭ������ı��ǩҳ�����壬�������ڱ�ǩ�ؼ�����֮ǰ */
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	//ɾ��ϵͳ���壬��������ϵͳ����
	lf.lfHeight = 15;
	lstrcpy(lf.lfFaceName,_T("����"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);
	CRect rect;
	rect.SetRectEmpty();
	/* ������Ŀ�б� */
	int count = 0;
	int setDefIndex = 0;
	//��ʼ����Ŀ����ҳ�棬�����б���ͼ
	if ( AddView( RUNTIME_CLASS(CProjectView), GetTabString(projectTabIndex),count ) < 0 )
	{
		TRACE("��Ŀ����ҳ��ʧ��\n");
		return -1;
	}
	m_tabIndex.clear();
	//��̫����
	m_tabIndex[projectTabIndex] = count++;
	/* �����������ͼ */
	if ( AddView( RUNTIME_CLASS(CProjectScheduleView), GetTabString(schedulePictrue), schedulePictrue ) < 0 )
	{
		TRACE("��Ŀ����ҳ��ʧ��\n");
		return -1;
	}
	m_tabIndex[schedulePictrue] = count++;

	/* ������Ŀ���� */
	if ( AddView( RUNTIME_CLASS(CProjectTaskView), GetTabString(taskTabIndex), taskTabIndex ) < 0 )
	{
		TRACE("��Ŀ����ҳ��ʧ��\n");
		return -1;
	}
	m_tabIndex[taskTabIndex] = count++;


	///* �ı䱻����ѡ�����ɫ��������ɫ */
	m_wndTabs.SetActiveTabColor(RGB(137,213,228));
	m_wndTabs.SetActiveTabTextColor(RGB(146,78,235));

	m_wndTabs.SetActiveTab( setDefIndex );
	GetActiveView()->SendMessage(WM_INITIALUPDATE,0,0);
	/* ����ѡ����� */
	m_wndTabs.EnableTabSwap(FALSE);		//������ѡ�����
	
	return 0;
}

void CProjectTabView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

int CProjectTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	OnCreateTab();
	
	return 0;
}

//�����  ��Ŀ������ͼ
LRESULT CProjectTabView::OnChangePrjTaskView( WPARAM wPara, LPARAM lPara )
{
	//����ѡ��Ƿ��Ѿ��������Ŀ�������ͼ
	TabIndxeMap::const_iterator itor = m_tabIndex.find( taskTabIndex );
	if ( itor != m_tabIndex.end() )
	{
		SetActiveView( taskTabIndex );
	    CView *pView = GetActiveView();
		m_selPrjId = wPara;
		if ( pView->IsKindOf( RUNTIME_CLASS(CProjectTaskView) ) )			//��ʾ��Ŀ������ͼ��������ѡ���ͼ���ڵ������
		{
			( (CProjectTaskView *)pView )->SetSelProjectByPrjId( m_selPrjId );
			OnActivteTaskPictrue( (WPARAM)( ((CProjectTaskView *)pView )->GetTaskListPoint() ), NULL );
		}
	}
	return TRUE;
}
LRESULT CProjectTabView::OnRecordShowPrjIs( WPARAM wPara, LPARAM lPara )
{
	m_selPrjId = wPara;
	return TRUE;
}
LRESULT CProjectTabView::OnRecordTaskStSelIndex( WPARAM wPara, LPARAM lPara )
{
	m_selTaskStatusIndex = wPara;
	return TRUE;
}
void CProjectTabView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CTabView::OnPaint()
	CTabView::OnPaint();
}

void CProjectTabView::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CTabView::OnActivate(nState, pWndOther, bMinimized);

	// TODO: �ڴ˴������Ϣ����������
}

//ת������ͼѡ�
LRESULT CProjectTabView::OnActivteTaskPictrue( WPARAM wPara, LPARAM lPara )
{
	TabIndxeMap::const_iterator itor = m_tabIndex.find( schedulePictrue );
	if ( itor != m_tabIndex.end()  )
	{
		CProjectScheduleView *pWnd = NULL;
		if ( m_tabIndex.size() <= schedulePictrue )
		{
			pWnd = (CProjectScheduleView *)GetTabControl().GetTabWnd( itor->second );
		}
		else
		{
			pWnd = (CProjectScheduleView *)GetTabControl().GetTabWnd( schedulePictrue ); //111111111111
		}
		if ( wPara )
		{
			pWnd->SetTaskListPoint( (ListTask*)wPara );
		}	
		
	}
	return 0;
}