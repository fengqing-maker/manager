// ChangeReview.cpp : 实现文件
//

#include "stdafx.h"
#include "ChangeReviewDlg.h"
#include "afxdialogex.h"
#include "../MainFrm.h"
#include "DlgTaskPictrueView.h"

// CChangeReview 对话框

IMPLEMENT_DYNAMIC(CChangeReviewDlg, CDialogEx)

CChangeReviewDlg::CChangeReviewDlg(const CTaskObj &taskObj, const CTaskChange &cheObj, BOOL onlyRead, CWnd* pParent /*=NULL*/)
	: CDialogEx(CChangeReviewDlg::IDD, pParent)
{
	m_taskObj = taskObj;
	m_changeObj = cheObj;

	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != m_pSever );
	m_pTaskSever = m_pSever->GetTaskSever();
    m_pChangeSever = m_pSever->GetChangeSever();
	m_readOnly = onlyRead;
	m_changeObj.GetAllResonOptionMap( m_optionStrings );

	VERIFY(m_font.CreateFont(
		35,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Cambria")));        
	VERIFY(m_font1.CreateFont(
		25,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Cambria")));  

}

CChangeReviewDlg::~CChangeReviewDlg()
{
}

void CChangeReviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_firstReviewEdit);
	DDX_Control(pDX, IDC_EDIT_GERN_REVIEW, m_secondReviewEdit);
	DDX_Control(pDX, IDC_EDIT_PRJREVIEW, m_eidtPrjReview);
	DDX_Control(pDX, IDC_RADIO_IMPACT1, m_ridoYes);
	DDX_Control(pDX, IDC_RADIO_IMPACT2, m_ridoNo);
}


BEGIN_MESSAGE_MAP(CChangeReviewDlg, CDialogEx)
	ON_BN_CLICKED(ID_BTN_AGRRE, &CChangeReviewDlg::OnBnClickedBtnAgrre)
	ON_BN_CLICKED(ID_BTN_UNAGREE, &CChangeReviewDlg::OnBnClickedBtnUnagree)
	ON_BN_CLICKED(IDC_BTN_SHOW_PICTRUE, &CChangeReviewDlg::OnBnClickedBtnShowPictrue)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CChangeReview 消息处理程序



BOOL CChangeReviewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	OnInitStaticText();
	OnInitHistoryChange();
	int type = m_changeObj.GetChangeTaskType();
	 CRect rect1, rect2, rect3;
	 CString strProject;
	 CProject prj;
     if ( m_taskObj.GetProjctId() > 0 )
     {
		 m_pSever->GetProjectSever()->GetProjectById( m_taskObj.GetProjctId(), prj );
		 strProject = prj.GetShortName(); 
     }
	 m_isPrjReview = FALSE;
	 if ( prj_remark_on == m_changeObj.GetStatus()/*changeWait0 == m_taskObj.GetTaskStatus() */
		 && prj.GetMangerId() == m_pSever->GetLoginServer()->GetLoginUser().GetId())
	 {//
		 GetDlgItem( ID_BTN_AGRRE )->SetWindowText( _T("确定") );
		 GetDlgItem( ID_BTN_UNAGREE )->SetWindowText( _T("取消") );
		 m_isPrjReview = TRUE;
	 }
	 OnInitChangeResonOption();
	if( changeNormalDepartTask == type )
	{//隐藏二级评审与项目点评
         OnInitDepartReview();
		 OnHideGeneralReviewWnd();
		 OnHideProjectView();
		 SetWindowText( _T("部门任务变更审批"));
		 //移动对话框
		 GetDlgItem( IDC_STATIC_PRJ )->GetWindowRect( &rect1 );
		 m_eidtPrjReview.GetWindowRect( &rect2 );
		 ScreenToClient( &rect1 );
		 GetDlgItem( IDC_STATIC_REVIEW )->MoveWindow( rect1 );
		 m_firstReviewEdit.GetWindowRect( rect3 );
		 rect3.bottom = rect2.bottom;
		 rect3.top = rect2.top;
		 ScreenToClient( &rect3 );
		 m_firstReviewEdit.MoveWindow( rect3 );
	}else if ( changeNormalPrjTask == type || (changePrjPhase == type && changeStop== m_changeObj.GetType() ) )
	{
		OnInitProjectReview();
		OnInitDepartReview();
		if ( m_changeObj.GetImpactSchudle() > 0 || changeNormalPrjTask != type )
		{
			OnInitGeneralReview();
			if ( changeNormalPrjTask != type )
			{
				strProject += _T("项目阶段变更审批");
				if ( up_apper == m_changeObj.GetStatus() )
				{
					GetDlgItem( ID_BTN_UNAGREE )->EnableWindow( FALSE );
				}
			}else
			{
				strProject += _T("项目任务变更审批");
			}
			SetWindowText( strProject );
		}else
		{//隐藏二级评审
           OnHideGeneralReviewWnd();
		   strProject += _T("项目任务变更审批");
		   SetWindowText( strProject );
		}
	
	}else
	{//隐藏一级评审
	    strProject += _T("项目阶段变更审批");
		SetWindowText( strProject );
		OnInitProjectReview();
		OnInitGeneralReview();
		OnHideReviewWnd();
		GetDlgItem( IDC_STATIC_REVIEW )->GetWindowRect( &rect1 );
		m_firstReviewEdit.GetWindowRect( &rect2 );
		ScreenToClient( &rect1 );
		GetDlgItem( IDC_STATIC_GENERAL )->MoveWindow( rect1 );
		ScreenToClient( &rect2 );
		m_secondReviewEdit.MoveWindow( rect2 );
	}

	OnInitChangId();
	if ( m_readOnly )
	{
		GetDlgItem( ID_BTN_AGRRE )->ShowWindow( SW_HIDE );
		GetDlgItem( ID_BTN_UNAGREE )->ShowWindow( SW_HIDE );
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CChangeReviewDlg::OnInitChangId()
{
	CString strInfo;
	CString status;

	const CTaskStaus& stMap = m_pTaskSever->GetTaskStausMap();
	stMap.GetStrStausById( m_taskObj.GetId(), status );

	strInfo.Format(_T("变更编号:%05d  %s"), m_changeObj.GetId(),status);
	GetDlgItem( IDC_STATIC_ID )->SetWindowText( strInfo ) ;

}
void CChangeReviewDlg::OnInitChangeResonOption()
{
	if ( m_optionStrings.size() > 0 )
	{
		CRect startRect;
		GetDlgItem(IDC_STATIC_OPTION)->GetWindowRect(&startRect);
		ScreenToClient(startRect);
		CRect rtFirst;
		int  wide = 150;
		int  hight = 20;
		rtFirst.top = startRect.top;
		rtFirst.left = startRect.right + 5;
		rtFirst.bottom = rtFirst.top + hight;
		rtFirst.right = rtFirst.left + wide;
		int  horizDistance = 5;
		int  verticalDistance = 4;
		CRect temp = rtFirst;
		int  cnt = 0;
		for(IntStringMap::iterator itor = m_optionStrings.begin(); itor != m_optionStrings.end();
			itor++)
		{
			m_resonOptionBtns[cnt].Create( itor->second, BS_CHECKBOX , temp,
				this, SET_DLG_BTN_FIRST_ID + cnt);
			temp.left = temp.right + horizDistance; //横间隔
			temp.right = temp.left + wide;
			if ((cnt % 4) == 3) //换行
			{
				temp.top = temp.bottom + verticalDistance;
				temp.bottom = temp.top + hight;
				temp.left = rtFirst.left;
				temp.right = rtFirst.right;
			}
			m_resonOptionBtns[cnt].SetCheck( m_changeObj.GetResonOptionSelectSt(cnt) );
			m_resonOptionBtns[cnt].ShowWindow(SW_SHOW);
			m_resonOptionBtns[cnt].EnableWindow( FALSE );
			cnt++;
		}
	}
}
void     CChangeReviewDlg::OnInitHistoryChange()
{
	CString str;
	ListChange historyChanges;
	m_pChangeSever->GetChangeByTaskId( m_changeObj.GetTaskId(), historyChanges );
	int cnt = historyChanges.size();
	if ( 1 == cnt || 0 == cnt )
	{
		str = _T("无历史变更");
	}else
	{
		for ( ListChangeConstItor itor = historyChanges.begin(); itor != historyChanges.end(); itor++ )
		{
			CString tmpStr;
			if ( itor->GetId() != m_changeObj.GetId() )
			{
				int st = itor->GetStatus();
				tmpStr.Format(_T("%s 内容:%s 原因:%s%s 结果:%s \r\n "), itor->GetChangeDateTimeStr(), 
					itor->GetChangeContent(), itor->GetResonSelsOptionString(),itor->GetChangeReason(), itor->GetChangeStatusString( st, true ) );
				tmpStr += _T("...............................................................\r\n");
				str += tmpStr;
			}
		}
	}
	GetDlgItem( IDC_STATIC_HISTORY )->SetWindowText( str );
}

void CChangeReviewDlg::OnInitStaticText()
{
	CDepartSeverInc *pDepartSever = m_pSever->GetDepartSever();
	CLoginActionInc *pLoginSever = m_pSever->GetLoginServer();
	CString orginName;
	CUser ownerUser;
	CDepartment departObj;
	pLoginSever->GetUserById( m_taskObj.GetOwnerId(), ownerUser );
	pDepartSever->GetDepartById( ownerUser.GetDepartId(), departObj);
	orginName = departObj.GetName()/*pLoginSever->GetLoginDepart().GetName()*/;
    //orginName =  pLoginSever->GetOrganizationString();
	GetDlgItem( IDC_STATIC_DEPART )->SetWindowText( orginName ) ;
   //日期
	GetDlgItem( IDC_STATIC_DATE )->SetWindowText( m_changeObj.GetChangeDateTimeStr() );
	//任务
	CString str;
	CString strNewSch;
	CSchedule schObj;
	if ( userExcOk == m_pSever->GetScheduleSever()->GetMaxNewScheduleByTaskId( m_taskObj.GetId(), schObj ) )
	{
		strNewSch.Format(_T("\r\n最新进度:%03d%%,%03.1fh T:%s\r\nDC:%s"), schObj.GetPercent(), schObj.GetWorkHours(),
			schObj.GetUpdateTimeStr(), schObj.GetUpdateContent() );
	}
	CString strTemp = m_taskObj.GetPanEndDateStr();
	str.Format( _T("名称:%s\r\n计划时间:%s--%s %0.1fh 实际开始时间：%s--%s %0.1f"),
		       m_taskObj.GetName(), m_taskObj.GetPanStartDateShowStr(), m_taskObj.GetPanEndDateShowStr(),
			   m_taskObj.GetPlanWorkHour(), m_taskObj.GetFactStartDateShowStr(), m_taskObj.GetFactEndDateShowStr(), m_taskObj.GetFactWorkHour(),
			   strNewSch );
	CTaskObj bakTaskObj;
	if ( userExcOk ==  m_pTaskSever->GetTaskByTaskId( m_changeObj.GetBakTaskId(), bakTaskObj ))
	{
		CString bakStr = bakTaskObj.GetDec();
		if ( bakStr.GetLength() > 0 )
		{
			str += _T("\r\n任务追加描述：");
			str += bakStr;
		}
		
	}
	//申请者

	GetDlgItem( IDC_STATIC_OWNER )->SetWindowText( ownerUser.GetUserName_R() );

	GetDlgItem( IDC_STATIC_TASK )->SetWindowText( str );
	//变更原因
	GetDlgItem( IDC_STATIC_RESON )->SetWindowText( m_changeObj.GetChangeReason() );
	//变更内容
	GetDlgItem( IDC_STATIC_CONTENT )->SetWindowText( m_changeObj.GetChangeContent() );

}
void CChangeReviewDlg::OnInitDepartReview()
{
	BOOL enableEdit = FALSE;
	if ( up_apper == m_changeObj.GetStatus() && FALSE == m_readOnly )
	{//2015-11-24 moidy
		enableEdit = TRUE;
	}
	CString str = m_changeObj.GetReviewContent();
	GetDlgItem( IDC_EDIT2 )->SetWindowText( str );
	m_firstReviewEdit.EnableWindow( enableEdit );
	
}
void CChangeReviewDlg::OnInitGeneralReview()
{
	BOOL enableEdit = FALSE;
	if ( departAgree == m_changeObj.GetStatus()/*changeWait2 == m_taskObj.GetTaskStatus()*/ && FALSE == m_readOnly )
	{
		enableEdit = TRUE;
	}
	m_secondReviewEdit.SetWindowText( m_changeObj.GetGeneralReviewContent() );
	m_secondReviewEdit.EnableWindow( enableEdit );
}
void  CChangeReviewDlg::OnInitProjectReview()
{
	BOOL enableEdit = FALSE;
	if ( m_isPrjReview && FALSE == m_readOnly )
	{
		enableEdit = TRUE;
	}
	m_eidtPrjReview.SetWindowText( m_changeObj.GetProjectReview() );
	int chSt = BST_UNCHECKED;
	//if ( m_changeObj.GetImpactSchudle() )
	//{
	//	chSt = BST_CHECKED;
	//}
	BOOL imp = m_changeObj.GetImpactSchudle();
	if ( imp >= 0 )
	{
		if (  imp == 0  )
		{//
			m_ridoNo.SetCheck( BST_CHECKED );
		}else
		{
			//开启定时器
			SetTimer( 1, 1000, NULL );
			//m_ridoYes.SetShowColor( true );
			CDC *pDc = m_ridoYes.GetDC();
			pDc->SetTextColor( RGB(0, 162, 232) );
			m_ridoYes.SetFont( &m_font1 );
			m_ridoYes.SetCheck( BST_CHECKED );
			m_ridoYes.UpdateWindow();
		}
	}else
	{
		m_ridoNo.SetCheck( BST_UNCHECKED );
		m_ridoYes.SetCheck( BST_UNCHECKED );
	}
	
	//m_eidtPrjReview.EnableWindow( enableEdit );  //11111111
	m_eidtPrjReview.SetEnableEdit( enableEdit );
	m_ridoNo.EnableWindow( enableEdit );
	m_ridoYes.EnableWindow( enableEdit );
}

void CChangeReviewDlg::OnBnClickedBtnAgrre()
{//
	CString temp;
	m_firstReviewEdit.GetWindowText( temp );
	m_changeObj.SetReviewContent( temp );
	m_secondReviewEdit.GetWindowText( temp );
	m_changeObj.SetGeneralReviewContent( temp );
	if ( m_isPrjReview && FALSE == m_readOnly )
	{
		if ( BST_CHECKED == m_ridoYes.GetCheck() && BST_CHECKED  == m_ridoNo.GetCheck() )
		{
			AfxMessageBox(_T("项目进度影响勾选不正确,请重新选择"));
			return;
		}
		if ( BST_UNCHECKED == m_ridoYes.GetCheck() && BST_UNCHECKED  == m_ridoNo.GetCheck() )
		{
			AfxMessageBox(_T("项目进度影响未进行点评"));
			return;
		}
		if ( BST_CHECKED == m_ridoYes.GetCheck() )
		{
			m_changeObj.SetImpactSchudle( 1 );
		}
		if (  BST_CHECKED  == m_ridoNo.GetCheck() )
		{
			m_changeObj.SetImpactSchudle( 0 );
		}
	}
	
	m_eidtPrjReview.GetWindowText( temp );
	m_changeObj.SetProjectReview( temp );
	const CUser userObj = m_pSever->GetLoginServer()->GetLoginUser();
	if ( up_apper == m_changeObj.GetStatus()  )
	{//2015-11-24 moidy
		BOOL ret = m_pChangeSever->ChangeDepartReview( TRUE, userObj.GetUserName_R(), m_changeObj );
		int loginId = m_pSever->GetLoginServer()->GetLoginUser().GetId();
		if ( userExcOk == ret )
		{
			if ( loginId == m_changeObj.GetGeneralAuditorId() && 
				( changeNormalDepartTask != m_changeObj.GetType() 
				    || (changeNormalPrjTask == m_changeObj.GetType() && 1 != m_changeObj.GetImpactSchudle() )) )
			{
				if( userExcOk == m_pTaskSever->GetTaskByTaskId( m_taskObj.GetId(), m_taskObj )
					&& userExcOk == m_pChangeSever->GetChangeById( m_changeObj.GetId(), m_changeObj) )
				{
					OnInitGeneralReview();
					OnInitDepartReview();
					OnInitChangId();
					return;
				}
			}
			CChangeReviewDlg::OnOK();
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSever->GetErrorString( ret) );
		}

	}else if ( departAgree == m_changeObj.GetStatus()/*changeWait2 == m_taskObj.GetTaskStatus()*/ )
	{
		BOOL ret = m_pChangeSever->ChangeGernalReview( TRUE, userObj.GetUserName_R(), m_changeObj );
		if ( userExcOk == ret )
		{
			CChangeReviewDlg::OnOK();
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSever->GetErrorString( ret) );
		}
	}else if ( m_isPrjReview )
	{//
		BOOL ret = m_pChangeSever->ChangePrjectReview( userObj.GetUserName_R(), m_changeObj );
		if ( userExcOk == ret )
		{
			CChangeReviewDlg::OnOK();
		}else if ( paraError != ret )
		{
			AfxMessageBox( m_pSever->GetErrorString( ret) );
		}
	}
}


void CChangeReviewDlg::OnBnClickedBtnUnagree()
{
	int type = -1;
	if ( up_apper == m_changeObj.GetStatus()/*changeWait1 == m_taskObj.GetTaskStatus()*/ )
	{//2015-11-24 moidy
		type = 0;
		CString temp;
		m_firstReviewEdit.GetWindowText( temp );
		if ( temp.IsEmpty() )
		{
			AfxMessageBox( _T("不同意任务变更，评审意见不能为空"));
			return;
		}
		m_changeObj.SetReviewContent( temp );

	}else if ( departAgree == m_changeObj.GetStatus()/*changeWait2 == m_taskObj.GetTaskStatus()*/ )
	{
		type = 1;
		CString temp;
		m_secondReviewEdit.GetWindowText( temp );
		if ( temp.IsEmpty() )
		{
			AfxMessageBox( _T("不同意任务变更，评审意见不能为空"));
			return;
		}
		m_changeObj.SetGeneralReviewContent( temp );
	}else if ( m_isPrjReview )
	{
		OnCancel();
		return;
	}
	//
	BOOL ret = paraError;
	const CUser userObj = m_pSever->GetLoginServer()->GetLoginUser();
	if ( 0 == type)
	{
		 ret = m_pChangeSever->ChangeDepartReview( FALSE, userObj.GetUserName_R(), m_changeObj );
	}else if ( 1 == type )
	{
		ret = m_pChangeSever->ChangeGernalReview( FALSE, userObj.GetUserName_R(), m_changeObj );
	}
	if ( userExcOk == ret )
	{
		CChangeReviewDlg::OnOK();
	}
}

void CChangeReviewDlg::OnHideReviewWnd()
{
	GetDlgItem( IDC_EDIT2 )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_REVIEW )->ShowWindow( SW_HIDE );
}
void CChangeReviewDlg::OnHideGeneralReviewWnd()
{
	GetDlgItem( IDC_STATIC_GENERAL )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_EDIT_GERN_REVIEW )->ShowWindow( SW_HIDE );
}
void CChangeReviewDlg::OnHideProjectView()
{
	GetDlgItem( IDC_STATIC_PRJ )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_EDIT_PRJREVIEW )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_STATIC_IMPACT )->ShowWindow( SW_HIDE );
	m_ridoYes.ShowWindow( SW_HIDE );
	m_ridoYes.EnableWindow( FALSE );
	m_ridoNo.ShowWindow( SW_HIDE );
	m_ridoNo.EnableWindow( FALSE );
	GetDlgItem( IDC_BTN_SHOW_PICTRUE )->ShowWindow( SW_HIDE );
	GetDlgItem( IDC_BTN_SHOW_PICTRUE )->EnableWindow( FALSE );
}

void CChangeReviewDlg::OnBnClickedBtnShowPictrue()
{
	//添加甘特图对比
	CTaskObj bakTask;
	m_pTaskSever->GetTaskByTaskId( m_changeObj.GetBakTaskId(), bakTask );
	ListTask lstTask;
	//CProject prj;
	//m_pSever->GetProjectSever()->GetProjectById( m_taskObj.GetProjctId(), prj; );
	if ( changePrjPhase == m_changeObj.GetChangeTaskType() )
	{
		//获取项目所有父级任务
		int hasChildCnt = 0;
		m_pSever->GetProjectSever()->GetProjectTaskNoFather( m_taskObj.GetProjctId(), -1, -1, lstTask, hasChildCnt, 0 );
		if ( hasChildCnt > 0 )
		{
			ListTaskItor itor = lstTask.begin();
			advance( itor, hasChildCnt );
			lstTask.erase( itor, lstTask.end() );
		}
	}else if ( changeNormalPrjTask == m_changeObj.GetChangeTaskType() )
	{
		CTaskObj fatherTask;
		m_pSever->GetProjectSever()->GetFatherTask( m_taskObj.GetId(), fatherTask );
		
		if( fatherTask.GetId() > 0 )
		{
			int hasCildCount= 0;
			m_pSever->GetProjectSever()->GetProjectTaskNoFather( m_taskObj.GetProjctId(), -1, -1, lstTask, hasCildCount, 0 );
			
			if ( hasCildCount > 0 )
			{
				if ( hasCildCount <= lstTask.size() )
				{
					ListTaskItor itor = lstTask.begin();
					advance( itor, hasCildCount );
					lstTask.erase( itor, lstTask.end() );
					ListTaskItor itorLast = lstTask.begin();
					itor = lstTask.begin();
					ListTaskItor itorNext = itor;
					bool isInsertChild = false;
					int cnt = 0;
					for ( ; itor != lstTask.end(); itor++ )
					{
						if ( itorNext != lstTask.end() )
						{
							itorNext++;
						}
						if ( task_close == itor->GetTaskStatus() && itor->GetId() != fatherTask.GetId() )
						{
							lstTask.erase( itor );
							if (  cnt > 0 )
							{
								itor = itorLast;
							}else
							{
                                itor = lstTask.begin();
							}
							itorNext = itor;
							itorNext++;
						}else
						{
							cnt++;
							if ( false == isInsertChild && itor->GetId() == fatherTask.GetId() )
							{//进行插入其孩子节点
								ListTask childList;
								m_pSever->GetProjectSever()->GetChildTask( fatherTask.GetId(), -1, -1, childList );
								if ( childList.size() > 0 )
								{
									for ( ListTaskItor itorChild = childList.begin(); itorChild != childList.end(); itorChild++ )
									{
										if ( task_close != itorChild->GetTaskStatus() || m_taskObj.GetId() == itorChild->GetId() )
										{
											lstTask.insert( itorNext, *itorChild );
										}
									}
								}else
								{
									lstTask.insert( itorNext, m_taskObj );
								}
								isInsertChild = true;
							}
							itorLast = itor;
						}
						
						
					}//for end
				}		
			}else
			{
				lstTask.clear();
				if( fatherTask.GetId() > 0 )
				{
					lstTask.push_back( fatherTask );
					lstTask.push_back( m_taskObj );
				}
			}
		}else
		{
			m_pSever->GetProjectSever()->GetProjectTask( m_taskObj.GetProjctId(), -1, -1, lstTask );
			ListTaskItor itor = lstTask.begin();
			for ( ; itor != lstTask.end(); itor++ )
			{
				if ( itor->GetId() != m_taskObj.GetId() )
				{
					lstTask.erase( itor );
					itor = lstTask.begin();
				}else
				{
					itor++;
					itor++;
					lstTask.erase( itor, lstTask.end() );
					break;
				}
			}
		}
	}else
	{
		AfxMessageBox(_T("非项目任务"));
		return;
	}
	CDlgTaskPictrueView dlg( &lstTask, m_changeObj, bakTask, AfxGetMainWnd() );
	dlg.DoModal();
}
void CChangeReviewDlg::OnTimer(UINT nIDEvent)
{
	static int cnt = 0;
	cnt = (cnt+1)%1000;
	if ( 1 == nIDEvent )
	{
		if ( cnt%2 == 0 )
		{
			m_ridoYes.SetFont( &m_font1 );
		}else
		{
			m_ridoYes.SetFont( &m_font );
		}
		//this->RedrawWindow();
	}
}

HBRUSH CChangeReviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	int id = pWnd->GetDlgCtrlID() ;
	if ( pWnd->GetDlgCtrlID() == IDC_RADIO_IMPACT1  )
	{
		pDC->SetTextColor( RGB(255, 0, 0) );
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
