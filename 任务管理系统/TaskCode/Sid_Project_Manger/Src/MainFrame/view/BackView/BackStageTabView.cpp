// TaskTabView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BackStageTabView.h"
#include "../../MainFrm.h"
#include "CUserMangerView.h"
#include "CProductLineView.h"
#include "RolePowerView.h"
#include "WorkTypeView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CDepartTabView

IMPLEMENT_DYNCREATE(CBackStageTabView, CTabView)

CBackStageTabView::CBackStageTabView()
{
	m_selPrjId = 0;
	m_selTaskStatusIndex = -1;
}

CBackStageTabView::~CBackStageTabView()
{
	if ( ::IsWindow( m_wndTabs.GetSafeHwnd() )  )
	{
		m_wndTabs.RemoveAllTabs();
	}
}

BEGIN_MESSAGE_MAP(CBackStageTabView, CTabView)
	ON_WM_CREATE()

	ON_WM_PAINT()
END_MESSAGE_MAP()


// CBackStageTabView ���

#ifdef _DEBUG
void CBackStageTabView::AssertValid() const
{
	CTabView::AssertValid();
}

#ifndef _WIN32_WCE
void CBackStageTabView::Dump(CDumpContext& dc) const
{
	CTabView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBackStageTabView ��Ϣ�������
void CBackStageTabView::ActiveTab(int index)
{
	SetActiveView( index );
	//SetActiveTab(index);
}
void CBackStageTabView::UpdateTabLabel(Tab_Index index)
{
	CString tabLabel;
	TabIndxeMap_Itor itor = m_tabIndex.find( index );
	if ( itor == m_tabIndex.end() )
	{
		return;
	}
	int tabIndex = (*itor).second;
	CString str;
	str = GetTabString(  index );
	
	m_wndTabs.SetTabLabel(tabIndex,str);
}
CString  CBackStageTabView::GetTabString( Tab_Index index ) const
{
	CString str;
	switch( index)
	{
	case userMangerTabInex:
		str = _T("�û�����");
		break;
	case  productLineTabIndex:
		str = _T("��Ʒ�߹���");
		break;
	case  rolePowerTabIndex:
		str = _T("Ȩ�޹���");
		break;	
	case  workTimeType:
		str = _T("��ʱ����");
		break;
	}
	return str;
}
int  CBackStageTabView::OnCreateTab()
{
	//
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	/* ͨ���ı�VS�Զ�Ĭ������ı��ǩҳ�����壬�������ڱ�ǩ�ؼ�����֮ǰ */
	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);
	afxGlobalData.fontRegular.DeleteObject();
	lf.lfHeight = 15;
	lstrcpy(lf.lfFaceName,_T("����"));
	afxGlobalData.fontRegular.CreateFontIndirect(&lf);
	CRect rect;
	rect.SetRectEmpty();
	/* ������Ŀ�б� */
	int count = 0;
	int setDefIndex = 0;
	if ( AddView( RUNTIME_CLASS(CUserMangerView), GetTabString(userMangerTabInex),count ) < 0 )
	{
		TRACE("�û�����ҳ��ʧ��\n");
		return -1;
	}

	m_tabIndex.clear();
	m_tabIndex[userMangerTabInex] = count++;
	/* ������Ʒ�� */
	if ( AddView( RUNTIME_CLASS(CProductLineView), GetTabString(productLineTabIndex),count ) < 0 )
	{
		TRACE("��Ʒ��ҳ��ʧ��\n");
		return -1;
	}
	m_tabIndex[productLineTabIndex] = count++;

	/* ������ɫȨ�� */
	if ( AddView( RUNTIME_CLASS(CRolePowerView), GetTabString(rolePowerTabIndex),count ) < 0 )
	{
		TRACE("���Ź���ҳ��ʧ��\n");
		return -1;
	}
	m_tabIndex[rolePowerTabIndex] = count++;
	/* ���� */
	if ( AddView( RUNTIME_CLASS(CWorkTimeTypeView), GetTabString(workTimeType),count ) < 0 )
	{
		TRACE("��ʱ����ʧ��\n");
		return -1;
	}
	m_tabIndex[workTimeType] = count++;

	///* �ı䱻����ѡ�����ɫ��������ɫ */
	m_wndTabs.SetActiveTabColor(RGB(137,213,228));
	m_wndTabs.SetActiveTabTextColor(RGB(146,78,235));

	m_wndTabs.SetActiveTab( setDefIndex );
	GetActiveView()->SendMessage(WM_INITIALUPDATE,0,0);
	/* ����ѡ����� */
	m_wndTabs.EnableTabSwap(FALSE);
	
	return 0;
}

void CBackStageTabView::OnSize(UINT nType, int cx, int cy)
{
	CTabView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
}

int CBackStageTabView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTabView::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  �ڴ������ר�õĴ�������
	OnCreateTab();
	
	return 0;
}



