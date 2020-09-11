#pragma once
#include "resource.h"
#include "CComonDlg.h"
#include <map>
// CDlgUserChose �Ի���

enum TREEVALUETYPE{
	treeTypeDepart,
	treeTypeProject,
	treeTypeUser,
};
struct TreeValue
{
   int       type; //��������
   HTREEITEM fatherTree; //��Դ���ĸ��ڵ�,��Ϊ��������
   const void *pData; //����ֵ
   TreeValue()
   {
	   type = 0;
	   fatherTree = 0;
	   pData = 0;
   }
};
typedef std::map< HTREEITEM, TreeValue> TreeValueMap;
typedef std::map< int, int> DepartMap; //�ڶ���������

class CDlgUserChose : public CComonDlg
{
	DECLARE_DYNAMIC(CDlgUserChose)
	
public:
	CDlgUserChose(BOOL isMutilSel, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgUserChose();

	void  SetDelSelUser( const ListUser &defUser );
	void  SetDelSelUser(  int id );

	BOOL SetDefTreeRoot( const ListDepart *defTreeRoot ); //������ʾ�Ĳ���
	BOOL SetDefTreeRoot( const ListProject *defTreeRoot ); //������ʾ����Ŀ

	const CPtrArray* GetSelArrary();
// �Ի�������
	enum { IDD = IDD_DLG_CHOSE_USER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
protected:
	void InitSrcTree();
	void InitSelectTree();

	void InsertDeparMember( const CDepartment &depart, HTREEITEM &pDepartTree, BOOL isRecurrence ); //isRecurrence�Ƿ�Ϊ�ݹ�
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

	CPtrArray            m_selUserArray;//��������Ĭ��ֵ
	BOOL                 m_isMutilSel;
	ListDepart	         m_listDepart;//���в���
	CDepartSeverInc      *m_pDepartSeverInc;
	TreeValueMap         m_treeDataMap;
	TreeValueMap         m_selTreeDataMap; //ѡ������ӳ��
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
