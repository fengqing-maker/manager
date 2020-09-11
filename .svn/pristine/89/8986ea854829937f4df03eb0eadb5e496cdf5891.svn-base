#pragma once
#include "resource.h"
#include "CComonDlg.h"
#include <map>
// CDlgUserChose 对话框

enum TREEVALUETYPE{
	treeTypeDepart,
	treeTypeProject,
	treeTypeUser,
};
struct TreeValue
{
   int       type; //数据类型
   HTREEITEM fatherTree; //在源树的父节点,若为部门则存的
   const void *pData; //数据值
   TreeValue()
   {
	   type = 0;
	   fatherTree = 0;
	   pData = 0;
   }
};
typedef std::map< HTREEITEM, TreeValue> TreeValueMap;
typedef std::map< int, int> DepartMap; //第二参无意义

class CDlgUserChose : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgUserChose)
	
public:
	CDlgUserChose(BOOL isMutilSel, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserChose();

	void  SetDelSelUser( const ListUser &defUser );
	void  SetDelSelUser(  int id );

	BOOL SetDefTreeRoot( const ListDepart *defTreeRoot ); //设置显示的部门
	BOOL SetDefTreeRoot( const ListProject *defTreeRoot ); //设置显示的项目

	const CPtrArray* GetSelArrary();
// 对话框数据
	enum { IDD = IDD_DLG_CHOSE_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
protected:
	void InitSrcTree();
	void InitSelectTree();

	void InsertDeparMember( const CDepartment &depart, HTREEITEM &pDepartTree, BOOL isRecurrence ); //isRecurrence是否为递归
	void InsertUserList( const CPtrArray &userArray, HTREEITEM pFatherTree );
	void InitProjectTree( const CProject &project, HTREEITEM &brother );
	
	void ExpandSelDepart( HTREEITEM selDepart );
	void SetChildCheck(CTreeCtrl &tree,HTREEITEM hTree,BOOL bCheck);
	void DeleteSrcTreeItem( HTREEITEM hTree );
	void DeleteSelTreeItem( HTREEITEM hTree );
	void MoveSrcToSelect( HTREEITEM hTree  );

	HTREEITEM IsInsertDepart( int departId, TreeValue &value  );
	HTREEITEM IsInSelTree( int userId, TreeValue &value  );

	void FindDepartByFatherId( int topId, CPtrArray &departeAarry );
	void SetDefExpentByUserId( int userId );
	void FindUserByDepart( int departId, BOOL isIncludeGroup,CPtrArray &pUser);
	const CUser* FindUserById( int useId );

protected:
	CTreeCtrl	     m_srcTree;
	CTreeCtrl	     m_selUserTree;

	CPtrArray            m_selUserArray;//最初存的是默认值
	BOOL                 m_isMutilSel;
	ListDepart	         m_listDepart;//所有部门
	CDepartSeverInc      *m_pDepartSeverInc;
	TreeValueMap         m_treeDataMap;
	TreeValueMap         m_selTreeDataMap; //选择树的映射
	ListUser             m_allUser;
	const ListProject *  m_defPrjTreeRoot;
	const ListDepart *   m_defDepartTreeRoot;

	DepartMap            m_defExpDepart;
public:
	afx_msg void OnNMDblclkTreeOrganization(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkTreeUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeOrganization(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTreeUser(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnMovetoSelect();
	afx_msg void OnBnClickedBtnMovetoUnselect();
};
