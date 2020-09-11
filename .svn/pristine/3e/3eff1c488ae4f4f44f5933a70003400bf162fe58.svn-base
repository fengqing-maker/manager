// DlgUserChose.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgUserChose.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// CDlgUserChose 对话框

IMPLEMENT_DYNAMIC(CDlgUserChose, CComonDlg)

CDlgUserChose::CDlgUserChose(BOOL isMutilSel,CWnd* pParent /*=NULL*/)
	: CComonDlg(CDlgUserChose::IDD, pParent)
{
	m_isMutilSel = isMutilSel;
	m_pDepartSeverInc = m_pSever->GetDepartSever();
	m_pDepartSeverInc->GetAllDepart( m_listDepart );
	ListDepartConstItor itor = m_listDepart.begin();
	m_pLoginAction->GetAllUser( m_allUser );
	m_defPrjTreeRoot = NULL;
	m_defDepartTreeRoot = NULL;
}

CDlgUserChose::~CDlgUserChose()
{
}

void CDlgUserChose::DoDataExchange(CDataExchange* pDX)
{
	CComonDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_ORGANIZATION, m_srcTree);
	DDX_Control(pDX, IDC_TREE_USER, m_selUserTree);
}

BEGIN_MESSAGE_MAP(CDlgUserChose, CComonDlg)

	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ORGANIZATION, &CDlgUserChose::OnNMDblclkTreeOrganization)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_USER, &CDlgUserChose::OnNMDblclkTreeUser)
	ON_NOTIFY(NM_CLICK, IDC_TREE_ORGANIZATION, &CDlgUserChose::OnNMClickTreeOrganization)
	ON_NOTIFY(NM_CLICK, IDC_TREE_USER, &CDlgUserChose::OnNMClickTreeUser)
	ON_BN_CLICKED(IDC_BTN_MOVETO_SELECT, &CDlgUserChose::OnBnClickedBtnMovetoSelect)
	ON_BN_CLICKED(IDC_BTN_MOVETO_UNSELECT, &CDlgUserChose::OnBnClickedBtnMovetoUnselect)
END_MESSAGE_MAP()
// CDlgUserChose 消息处理程序
void  CDlgUserChose::SetDelSelUser( const ListUser &defUser )
{
	if ( m_isMutilSel )
	{
	   for ( ListUser::const_iterator itor = defUser.begin(); itor != defUser.end(); itor++)
	   {
		   const CUser *pUser = FindUserById( itor->GetId() );
		   if ( pUser )
		   {
			   m_selUserArray.Add( (void *)pUser );
		   }
	   }
	}else
	{
		m_selUserArray.RemoveAll();
		if ( defUser.size() > 0)
		{
			ListUser::const_iterator itor = defUser.begin();
			SetDelSelUser( itor->GetId() );
		}
	}
}
void  CDlgUserChose::SetDelSelUser( int id )
{
	const CUser *pUser = FindUserById( id );
	if ( pUser )
	{
		m_selUserArray.Add( (void *)pUser );
		SetDefExpentByUserId( pUser->GetDepartId() );
	}
}
BOOL CDlgUserChose::OnInitDialog()
{
	CComonDlg::OnInitDialog();
	InitSelectTree();
	InitSrcTree();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CDlgUserChose::InitProjectTree( const CProject &project, HTREEITEM &brother )
{
	TreeValue valueTmp;
	valueTmp.pData = (void *)&project;
	valueTmp.type = treeTypeProject;
	valueTmp.fatherTree = NULL;
	brother = m_srcTree.InsertItem( project.GetShortName(), TVI_ROOT, brother);
	m_treeDataMap.insert( std::make_pair( brother, valueTmp ) );
	ListUser userLst;
	BOOL ret = m_pSever->GetProjectSever()->GetProjectMember( project.GetId(), userLst );
	if ( userExcOk == ret )
	{//
		CPtrArray userArray;
		for(ListUserItor itor = userLst.begin(); itor != userLst.end(); itor++ )
		{
			const CUser *pObj = FindUserById( itor->GetId() );
			if ( pObj )
			{
				userArray.Add( (void *)pObj );
			}
		}
		InsertUserList( userArray, brother );
	}
}
void CDlgUserChose::InitSrcTree()
{
	HTREEITEM lastTopDepart = TVI_LAST;
	if ( m_defPrjTreeRoot && m_defPrjTreeRoot->size() > 0 )
	{
		for( ListProject::const_iterator itor = m_defPrjTreeRoot->begin(); itor != m_defPrjTreeRoot->end(); itor++ )
		{
			InitProjectTree( *itor, lastTopDepart );
			m_srcTree.Expand( lastTopDepart,TVE_EXPAND );
		}
		return;
	}
	CPtrArray   topDepartArray;
	CDepartment *pDepartment = NULL;
	if ( NULL != m_defDepartTreeRoot && m_defDepartTreeRoot->size() > 0 )
	{
		for( ListDepartConstItor itor = m_defDepartTreeRoot->begin(); itor != m_defDepartTreeRoot->end(); itor++ )
		{
			topDepartArray.Add( (void *)&(*itor) );
		}
	}else
	{
		FindDepartByFatherId( 0, topDepartArray );
	}
	int count = topDepartArray.GetCount(); //插入顶级部门
	for ( int i= 0; i < count; i++ )
	{
		pDepartment = (CDepartment *)topDepartArray.GetAt( i );
		if ( pDepartment )
		{//插入部门员工
			InsertDeparMember( *pDepartment, lastTopDepart, FALSE );
			
		}
	}
	m_selUserTree.Expand( m_srcTree.GetRootItem(), TVE_EXPAND );
	return ;
}
void CDlgUserChose::ExpandSelDepart( HTREEITEM selDepart )
{
	m_srcTree.Expand( selDepart, TVE_EXPAND );
	HTREEITEM father = m_srcTree.GetParentItem( selDepart );
	if (NULL == father )
	{
		return;
	}
	ExpandSelDepart( father );
}
void CDlgUserChose::InsertDeparMember( const CDepartment &depart, HTREEITEM &pDepartTree, BOOL isRecurrence /*= FALSE*/)
{
	TreeValue valueTmp;
	valueTmp.pData = (void *)&depart;
	valueTmp.type = treeTypeDepart;
	valueTmp.fatherTree = NULL;

	pDepartTree = m_srcTree.InsertItem( depart.GetName(), isRecurrence?pDepartTree:TVI_ROOT, pDepartTree);
	m_treeDataMap.insert( std::make_pair( pDepartTree, valueTmp) );

	CPtrArray userArray;
	FindUserByDepart( depart.GetId(), FALSE, userArray);
	InsertUserList( userArray,  pDepartTree );
	CPtrArray   departArray;
	FindDepartByFatherId( depart.GetId(), departArray );

	if ( m_defExpDepart.end() != m_defExpDepart.find( depart.GetId())  )
	{
		ExpandSelDepart( pDepartTree );
	}
	if ( departArray.GetCount() < 0)
	{
		return;
	}else
	{ 
		int count = departArray.GetCount();
		for ( int i = 0; i < count; i++ )
		{
			HTREEITEM father = pDepartTree;
			InsertDeparMember( *( (CDepartment*)( departArray.GetAt(i))), father, TRUE );
		}
	}
}
void CDlgUserChose::FindDepartByFatherId( int topId, CPtrArray &departeAarry )
{
	ListDepart::const_iterator itor = m_listDepart.begin();
	TreeValue value;
	while( itor != m_listDepart.end() )
	{
		if ( topId == itor->GetTopDepartId() && FALSE == IsInsertDepart( itor->GetId(), value ) )
		{
			departeAarry.Add( (void *)&(*itor) );
		}
		itor++;
	}
}
void CDlgUserChose::SetDefExpentByUserId( int departId )
{
	if ( m_defExpDepart.end() ==m_defExpDepart.find( departId) )
	{
		m_defExpDepart.insert( std::make_pair( departId, NULL ) );
	}
}
HTREEITEM CDlgUserChose::IsInsertDepart( int departId, TreeValue &value  )
{
	if ( m_treeDataMap.size() <= 0 )
	{
		return NULL;
	}
	TreeValueMap::const_iterator itor = m_treeDataMap.begin();
	while( itor != m_treeDataMap.end() )
	{
		if ( treeTypeDepart ==  itor->second.type && departId == ( (const CDepartment *)(itor->second.pData))->GetId() )
		{
			value = itor->second;
			return itor->first;
		}
		itor++;
	}
	return NULL;
}
HTREEITEM CDlgUserChose::IsInSelTree( int userId, TreeValue &value  )
{
	if ( m_selTreeDataMap.size() <= 0 )
	{
		return NULL;
	}
	TreeValueMap::const_iterator itor = m_selTreeDataMap.begin();
	while( itor != m_selTreeDataMap.end() )
	{
		if ( treeTypeUser ==  itor->second.type && 
			userId == ((const CUser *)itor->second.pData)->GetId() )
		{
			value = itor->second;
			return itor->first;
		}
		itor++;
	}
	return NULL;
}
void CDlgUserChose::InsertUserList( const CPtrArray &userArray, HTREEITEM pFatherTree )
{
	int count = userArray.GetSize();
	CUser *pData;
	TreeValue tmpValue;
	tmpValue.fatherTree = pFatherTree;
	tmpValue.type = treeTypeUser;
	TreeValue tmpValueFind;
	for ( int i = 0; i < count; i++)
	{
		pData = (CUser *)userArray.GetAt( i );
		tmpValue.pData = pData;
		if ( pData && FALSE == IsInSelTree(pData->GetId(), tmpValueFind) )
		{
		  HTREEITEM userTreeItem = m_srcTree.InsertItem( pData->GetUserName_R(), pFatherTree);
		  m_treeDataMap.insert( std::make_pair(userTreeItem, tmpValue) );
		}
	}
}
void CDlgUserChose::FindUserByDepart( int departId, BOOL isIncludeGroup,CPtrArray &userArray)
{
	ListUser::const_iterator itor = m_allUser.begin();
	while( itor != m_allUser.end() )
	{
		if ( itor->GetDepartId() == departId )
		{
			userArray.Add( (void *)&(*itor) );
		}
		itor++;
	}
}
const CUser* CDlgUserChose::FindUserById( int useId )
{
	ListUser::const_iterator itor = m_allUser.begin();
	while( itor != m_allUser.end() )
	{
		if ( itor->GetId() == useId )
		{
			return &(*itor);
		}
		itor++;
	}
	return NULL;
}
BOOL  CDlgUserChose::SetDefTreeRoot( const ListDepart *defTreeRoot )
{
	if ( NULL == m_defPrjTreeRoot || NULL == defTreeRoot  )
	{
		m_defDepartTreeRoot = defTreeRoot;
		return TRUE;
	}
	return FALSE;
}
BOOL  CDlgUserChose::SetDefTreeRoot( const ListProject *defTreeRoot )
{
	if ( NULL == m_defDepartTreeRoot || NULL == defTreeRoot )
	{
		m_defPrjTreeRoot  = defTreeRoot;
		return TRUE;
	}
	return FALSE;
}
void CDlgUserChose::InitSelectTree()
{
	HTREEITEM hRoot = m_selUserTree.InsertItem( _T("选择人员"), TVI_ROOT );
	int count = m_selUserArray.GetCount();
	HTREEITEM treeItem = NULL;
	TreeValue valueTmp;
	valueTmp.type = treeTypeUser;
	valueTmp.fatherTree = NULL;
	for ( int i = 0; i < count; i++ )
	{
	  const CUser *pData = (const CUser *)m_selUserArray.GetAt( i );
	  valueTmp.pData = pData;
	  treeItem = m_selUserTree.InsertItem( pData->GetUserName_R(), hRoot );
	  m_selTreeDataMap.insert( std::make_pair( treeItem,valueTmp ) ); 
	}
     m_selUserTree.Expand( m_selUserTree.GetRootItem(), TVE_EXPAND );
}


void CDlgUserChose::OnNMDblclkTreeOrganization(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgUserChose::OnNMDblclkTreeUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CDlgUserChose::OnNMClickTreeOrganization(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	UINT uFlag;   //接收有关点击测试的信息的整数
	HTREEITEM hTree;
	BOOL bCheck;
	GetCursorPos(&point);    //获取屏幕鼠标坐标
	m_srcTree.ScreenToClient(&point);           //转化成客户坐标
	hTree = m_srcTree.HitTest(point,&uFlag);    //返回与CtreeCtrl关联的光标的当前位置和句柄

	if (hTree && (TVHT_ONITEMSTATEICON & uFlag))  //点中复选框
	{

		bCheck = m_srcTree.GetCheck(hTree);      //获取当前复选状态
		SetChildCheck(m_srcTree, hTree,!bCheck);                  //设置子项复选状态
	}

	*pResult = 0;
}


void CDlgUserChose::OnNMClickTreeUser(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint point;
	UINT uFlag;   //接收有关点击测试的信息的整数
	HTREEITEM hTree;
	BOOL bCheck;
	GetCursorPos(&point);    //获取屏幕鼠标坐标
	m_selUserTree.ScreenToClient(&point);           //转化成客户坐标
	hTree = m_selUserTree.HitTest(point,&uFlag);    //返回与CtreeCtrl关联的光标的当前位置和句柄

	if (hTree && (TVHT_ONITEMSTATEICON & uFlag))  //点中复选框
	{
		bCheck = m_selUserTree.GetCheck(hTree);      //获取当前复选状态
		SetChildCheck(m_selUserTree, hTree,!bCheck);                  //设置子项复选状态
	}

	*pResult = 0;
}
void CDlgUserChose::SetChildCheck(CTreeCtrl &tree,HTREEITEM hTree,BOOL bCheck)
{
	hTree = tree.GetChildItem(hTree);  //获取子项句柄
	while (hTree)
	{
		tree.SetCheck(hTree, bCheck);
		SetChildCheck(tree, hTree, bCheck);     //递归调用
		hTree = tree.GetNextSiblingItem(hTree);    //获取兄弟的句柄
	}
}
void CDlgUserChose::MoveSrcToSelect( HTREEITEM hTree  )
{
	hTree = m_srcTree.GetChildItem(hTree);  //获取子项句柄
	while (hTree)
	{
		BOOL selTree = m_srcTree.GetCheck( hTree );
		if ( selTree )
		{
			if ( FALSE ==  m_isMutilSel )
			{//删除之前选择的
				HTREEITEM hSelRoot = m_selUserTree.GetRootItem();
				HTREEITEM hChild = m_selUserTree.GetChildItem( hSelRoot );
				while( hChild )
				{
					DeleteSelTreeItem( hChild );
					hChild = m_srcTree.GetNextSiblingItem(hChild);
				}
			}
			DeleteSrcTreeItem( hTree );		
		}
		MoveSrcToSelect( hTree );     //递归调用
		hTree = m_srcTree.GetNextSiblingItem(hTree);    //获取兄弟的句柄
	}
}
void CDlgUserChose::OnBnClickedBtnMovetoSelect()
{
	HTREEITEM hRoot = m_srcTree.GetRootItem();
	while( NULL != hRoot )
	{
	    MoveSrcToSelect( hRoot );
		hRoot = m_srcTree.GetNextItem(hRoot,TVGN_NEXT);
	}
	
}
void  CDlgUserChose::DeleteSrcTreeItem( HTREEITEM hTree )
{
	TreeValueMap::iterator dataItor =  m_treeDataMap.find( hTree );
	if ( dataItor != m_treeDataMap.end() && treeTypeUser == dataItor->second.type  )
	{
		CString name = ((const CUser *)dataItor->second.pData)->GetUserName_R();
		HTREEITEM item = m_selUserTree.InsertItem( name, m_selUserTree.GetRootItem() );
		m_selTreeDataMap.insert( std::make_pair(item, dataItor->second ) );
		m_selUserTree.Expand( m_selUserTree.GetRootItem(), TVE_EXPAND );
		m_treeDataMap.erase( dataItor );
		m_srcTree.DeleteItem( hTree );
	}
}
void  CDlgUserChose::DeleteSelTreeItem( HTREEITEM hTree )
{
	TreeValueMap::iterator dataItor =  m_selTreeDataMap.find( hTree );
	if ( dataItor != m_selTreeDataMap.end() && treeTypeUser == dataItor->second.type  )
	{
		m_selUserTree.DeleteItem( hTree );
		HTREEITEM InsertFather = dataItor->second.fatherTree;
		if ( NULL == InsertFather )
		{//查找父节点
			int departId = ((const CUser *)dataItor->second.pData)->GetDepartId();
			TreeValue temp;
			InsertFather = IsInsertDepart( departId, temp );
			if ( NULL == InsertFather )
			{
				InsertFather = m_srcTree.GetRootItem();
			}
			dataItor->second.fatherTree = InsertFather;
		}
		HTREEITEM item = m_srcTree.InsertItem( ((const CUser *)dataItor->second.pData)->GetUserName_R(),InsertFather );
		m_srcTree.Expand( InsertFather, TVE_EXPAND );
		m_treeDataMap.insert( std::make_pair(item, dataItor->second ) );
		m_selTreeDataMap.erase( dataItor );
	}
}
const CPtrArray* CDlgUserChose::GetSelArrary() 
{
	m_selUserArray.RemoveAll();
	TreeValueMap::iterator itor = m_selTreeDataMap.begin();
	while( itor != m_selTreeDataMap.end() )
	{
		m_selUserArray.Add( (void *)itor->second.pData );
		itor++;
	}
	return &m_selUserArray;
}
void CDlgUserChose::OnBnClickedBtnMovetoUnselect()
{
	//遍历树
	HTREEITEM hRoot = m_selUserTree.GetRootItem();
	//while( NULL != hRoot )
	//{
		HTREEITEM hTree = m_selUserTree.GetChildItem(hRoot);  //获取子项句柄
		while (hTree)
		{
			BOOL selTree = m_selUserTree.GetCheck( hTree );
			if ( selTree )
			{
				DeleteSelTreeItem( hTree );
			}
			hTree = m_srcTree.GetNextSiblingItem(hTree);    //获取兄弟的句柄
		}
	//	hRoot = m_srcTree.GetNextItem(hRoot,TVGN_NEXT);
	//}
}
