// ChangeApplyFor.cpp : 实现文件
//

#include "stdafx.h"
#include "ChangeApplyFor.h"
#include "afxdialogex.h"
#include "../MainFrm.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"
// CChangeApplyFor 对话框
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CChangeApplyFor, CComonDlg)

CChangeApplyFor::CChangeApplyFor(const CTaskObj &taskObj, const CTaskChange &cheObj, CWnd* pParent /*=NULL*/)
	: CComonDlg(CChangeApplyFor::IDD, pParent)
{
	m_taskObj = taskObj;
	m_changeObj = cheObj;
	m_pChangeSever = m_pSever->GetChangeSever();


	bool setBak = true;
	if ( cheObj.GetBakTaskId() > 0)
	{
		BOOL ret = m_pTaskSever->GetTaskByTaskId( cheObj.GetBakTaskId(), m_bakTaskObj );
		if ( ret == userExcOk )
		{
		   setBak = false;
		}
	}
	if ( task_suspend == m_taskObj.GetTaskStatus() )
	{//设置备用任务状态
		CTaskChange tempObj;
		if ( userExcOk == m_pChangeSever->GetMaxNewSupentChangeByTaskId(m_changeObj.GetTaskId(), tempObj) )
		{
			CTaskObj bakTemp;
			if ( userExcOk == m_pTaskSever->GetTaskByTaskId( tempObj.GetBakTaskId(), bakTemp) )
			{
				m_bakTaskObj.SetTaskStatus( bakTemp.GetTaskStatus() );
			}
			//将备用任务的创建时间改成申请挂起的时间，用于计算需不填进度的次数 lhz 
			bakTemp.SetFactStartTime( tempObj.GetChangeDateTime() );
			m_pTaskSever->UpdateTask( bakTemp );
		}else
		{
			if ( m_bakTaskObj.GetFactStartTime().invalid )
			{
				m_bakTaskObj.SetTaskStatus( ongoing );
			}else
			{
				m_bakTaskObj.SetTaskStatus( waitStart );
			}
		}
		m_taskObj.SetTaskStatus( m_bakTaskObj.GetTaskStatus() );
	}
	if ( setBak )
	{
		m_bakTaskObj = m_taskObj;
		m_bakTaskObj.SetId( - 1);
		m_bakTaskObj.SetDec(_T(""));
	}
	m_bakTaskObj.SetAttribute( -1 );

	m_changeObj.GetAllResonOptionMap( m_optionStrings );
}

CChangeApplyFor::~CChangeApplyFor()
{
}

void CChangeApplyFor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CHANGE_TYPE, m_changeTypeBox);
	DDX_Control(pDX, IDC_DATE_PLAN_TIME, m_panEndTimeCtrl);
	DDX_Control(pDX, IDC_EDIT_PAN_WORK_HOUR, m_addWorkHourTask);
}


BEGIN_MESSAGE_MAP(CChangeApplyFor, CComonDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANGE_TYPE, &CChangeApplyFor::OnCbnSelchangeComboChangeType)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATE_PLAN_TIME, &CChangeApplyFor::OnDtnDatetimechangeDatePlanTime)
	ON_EN_CHANGE(IDC_EDIT_PAN_WORK_HOUR, &CChangeApplyFor::OnEnChangeEditPanWorkHour)
	ON_BN_CLICKED(ID_BTN_SAVE , &CChangeApplyFor::OnBnClickedBtnSave)
	ON_BN_CLICKED(ID_BTN_APPLY, &CChangeApplyFor::OnBnClickedBtnApply)
	ON_BN_CLICKED(IDC_BTN_DELETE, &CChangeApplyFor::OnBnClickedBtnDelete)
END_MESSAGE_MAP()


// CChangeApplyFor 消息处理程序
BOOL CChangeApplyFor::OnInitDialog()
{
	//2015-11-24 add验证是否有多次未完成的变更
	int  cnt = 0;
	ListChange lstCh;
	m_pChangeSever->GetChangeByTaskId( m_taskObj.GetId(), lstCh );
	if ( lstCh.size() > 0 )
	{
		for ( ListChangeConstItor itor = lstCh.begin(); itor != lstCh.end(); itor++ )
		{
			if ( false == itor->IsFinishChange()  )
			{
				cnt++;
				if ( m_changeObj.GetId() > 0 && m_changeObj.GetId() == itor->GetId() )
				{
					cnt--;
				}
			}
		}
	}
	if ( cnt > 0 )
	{
		AfxMessageBox(_T("此任务还有未处理完的变更单,请完成变更流程后再进行申请") );
		OnCancel();
		return FALSE;
	}
	CDialogEx::OnInitDialog();
	OnInitChangeType();
	OnInitStaticText();
	OnInitChangId();
	OnInitOtherEdit();
	//2015-10-26 修改变更流程
	int  type = 0;
	if ( m_taskObj.GetProjctId() > 0 )
	{//项目任务
		BOOL hasChild = FALSE;
		m_pSever->GetProjectSever()->IsHasChildTask( m_taskObj.GetId(), hasChild );
		if ( hasChild )
		{
			type = changePrjPhase;
			SetWindowText( _T("项目任务阶段变更申请") );
		}else
		{
			type = changeNormalPrjTask;
		}
	}else
	{//部门任务
		type = changeNormalDepartTask;
	}
	m_changeObj.SetChangeTaskType( type );
	//
	if (FALSE == OnInitReviewUser() )
	{
		OnCancel();
		return FALSE;
	}
	if ( FALSE == OnInitSecondReviewUser() )
	{
		OnCancel();
		return FALSE;
	}
	OnInitSysChangeContent();
	
	BOOL isEnableDel = m_changeObj.GetId();
	if ( isEnableDel )
	{
		GetDlgItem( IDC_BTN_DELETE )->ShowWindow( SW_SHOW );
		GetDlgItem( IDC_BTN_DELETE )->EnableWindow( isEnableDel );
	}
	OnInitChangeResonOption();
	return TRUE;
}

void CChangeApplyFor::OnInitChangeType()
{
    m_changeTypeBox.InsertString( changeTime, _T("变更时间"));
	m_changeTypeBox.InsertString( changeSupsend, _T("任务挂起") );
	m_changeTypeBox.InsertString( changeReStart, _T("恢复挂起任务") );
	m_changeTypeBox.InsertString( changeStop, _T("中止任务") );
	if ( m_changeObj.GetId() < 0 )
	{
		m_changeTypeBox.SetCurSel( changeTime );
	}else
	{
		m_changeTypeBox.SetCurSel( m_changeObj.GetType() );
	}
	if ( task_suspend == m_taskObj.GetTaskStatus() )
	{
		m_changeTypeBox.SetCurSel( changeReStart );
		m_changeTypeBox.EnableWindow( FALSE );
	}
	OnSetEnable();
}
void CChangeApplyFor::OnInitOtherEdit()
{
	m_panEndTimeCtrl.SetTime( m_bakTaskObj.GetPlanEndTime() );
	double value = m_bakTaskObj.GetPlanWorkHour() - m_taskObj.GetPlanWorkHour();
    CString str;
	str.Format(_T("%0.1f"), value );
	m_addWorkHourTask.SetWindowText( str );
	GetDlgItem(IDC_EDIT_TASCKDC)->SetWindowText( m_bakTaskObj.GetDec() ); 
	GetDlgItem(IDC_EDIT_CONTENT)->SetWindowText( m_changeObj.GetChangeContent() );
	GetDlgItem(IDC_EDIT_RESON)->SetWindowText( m_changeObj.GetChangeReason() );
}
BOOL  CChangeApplyFor::GetBakMangerUser( const CUser  &userObj, CUser &bakUser, BOOL isSecond/* = FALSE*/ )
{
	CDepartment depart;
	BOOL ret;
	ret = m_pSever->GetDepartSever()->GetDepartById( userObj.GetDepartId(), depart );
	if( isSecond && depart.GetTopDepartId() > 0 )
	{//
		ListDepart lst;
		m_pSever->GetDepartSever()->GetTopDepartByMangerId( userObj.GetId(), lst );
		if (lst.size() > 0)
		{
			depart = *(lst.begin());
		}
	}
	if ( depart.GetBakValidate() && m_pLoginAction->FindUserPower(depart.GetBakManger(),changeLevelAgree1))
	{
		ret = m_pLoginAction->GetUserById( depart.GetBakManger(), bakUser );
	}
	
	return ret;
}
BOOL  CChangeApplyFor::OnInitReviewUser()
{
	//IDC_EDIT_REVIEW , IDC_EDIT_SECOND_REVIEW
	int userId = m_taskObj.GetMakeScoreUserId();
	CUser reviewUser;
	CString str;
	if ( changePrjPhase != m_changeObj.GetTaskId() )
	{//需走部门经理审核
		BOOL ret =  m_pLoginAction->GetUserById( userId, reviewUser );
		if ( ret != userExcOk )
		{
			str.Format( _T("初始化评审人失败，%s"), m_pSever->GetErrorString(ret));
			AfxMessageBox( str );
			return FALSE;
		}
		if ( m_pLoginAction->GetLoginUser().GetId() == reviewUser.GetId() )
		{//获取上级部门
			reviewUser = m_pLoginAction->GetLoginUserLeader();
		}
		CDepartment depart;
		CUser    bakUser;
		ret = GetBakMangerUser( reviewUser, bakUser );
		if ( userExcOk == ret && bakUser.GetId() > 0 )
		{
			m_changeObj.SetAuditorId( bakUser.GetId() );
			str = bakUser.GetUserName_R();
		}else if ( m_pLoginAction->FindUserPower(reviewUser.GetId(), changeLevelAgree1) )
		{
			m_changeObj.SetAuditorId( reviewUser.GetId() );
			str = reviewUser.GetUserName_R();
		}else 
		{
			AfxMessageBox(_T("初始化一级评审人失败"));
			return FALSE;
		}
		GetDlgItem( IDC_EDIT_REVIEW )->SetWindowText( str );
	}else
	{
		GetDlgItem( IDC_EDIT_REVIEW )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_REVIEWER )->ShowWindow( SW_HIDE );
	}
	return TRUE;
}
BOOL CChangeApplyFor::OnInitSecondReviewUser()
{
	const CUser& gernalUser = m_pLoginAction->GetGernalMangerUser();
	CString str;
	if ( gernalUser.GetId() <= 0 )
	{  
	    AfxMessageBox( _T("副总角色无，请系统管理员添加") );
		return FALSE;
	}
	CDepartment depart;
	CUser    bakUser;
	BOOL ret = GetBakMangerUser( gernalUser, bakUser, TRUE );
	if ( userExcOk == ret && bakUser.GetId() > 0 )
	{
		m_changeObj.SetGeneralAuditorId( bakUser.GetId() );
		str = bakUser.GetUserName_R();
	}else if ( m_pLoginAction->FindUserPower(gernalUser.GetId(), changeLevelAgree1) )
	{
		m_changeObj.SetGeneralAuditorId( gernalUser.GetId() );
		str = gernalUser.GetUserName_R();
	}else 
	{
		AfxMessageBox(_T("初始化二级评审人失败"));
		return FALSE;
	}
	if ( changeNormalDepartTask == m_changeObj.GetChangeTaskType() )
	{
		GetDlgItem( IDC_EDIT_SECOND_REVIEW )->SetWindowText( str );
		GetDlgItem( IDC_EDIT_SECOND_REVIEW )->ShowWindow( SW_HIDE );
		GetDlgItem( IDC_STATIC_GENERAL )->ShowWindow( SW_HIDE );
	}
	
	return TRUE;
}
void CChangeApplyFor::OnInitChangeResonOption()
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
			m_resonOptionBtns[cnt].Create( itor->second, WS_TABSTOP|BS_CHECKBOX|WS_CHILD|BS_AUTOCHECKBOX, temp,
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
			m_resonOptionBtns[cnt].ShowWindow( SW_SHOW );
			m_resonOptionBtns[cnt].EnableWindow( TRUE );
			cnt++;
		}
	}
}
void CChangeApplyFor::OnSetEnable()
{
	int index = m_changeTypeBox.GetCurSel();
	BOOL  isEditDate = TRUE, isEditAddWorkHour = TRUE; 
	if (changeSupsend == index || changeStop == index )
	{
		isEditDate = FALSE;
		isEditAddWorkHour = FALSE;
	}

	m_panEndTimeCtrl.EnableWindow( isEditDate );
	m_addWorkHourTask.EnableWindow( isEditAddWorkHour );
}
CString CChangeApplyFor::OnInitSysChangeContent()
{
	CString sysChangeContent;
	int index = m_changeTypeBox.GetCurSel();
	BOOL setTimeChange = FALSE;
	switch ( index )
	{
	case changeSupsend:
		sysChangeContent = _T("申请任务挂起");
		break;
	case changeStop:
		sysChangeContent = _T("申请任务中止");
		break;
	case  changeTime:
		setTimeChange = TRUE;
		sysChangeContent = _T("申请变更时间:");
		break;
	case  changeReStart:
		setTimeChange = TRUE;
		sysChangeContent = _T("恢复挂起任务:");
		break;
	}
	CString panTimeStr;
 	m_panEndTimeCtrl.GetWindowText( panTimeStr );
	COleDateTime panTime;
	m_panEndTimeCtrl.GetTime( panTime );
	CString str;
	if ( setTimeChange && panTime > m_taskObj.GetPlanEndTime() )
	{
		str.Format( _T("从原计划完成日期%s更改为%s; "), 
			m_taskObj.GetPanEndDateStr(), panTimeStr );
		sysChangeContent += str;
	}
	m_addWorkHourTask.GetWindowText( str );
	double planValue = 0.0;
    CUtilityTool::GetObj()->ConvertStringToDouble( str, planValue);
	double value = m_bakTaskObj.GetPlanWorkHour() + (float)planValue/*- m_taskObj.GetPlanWorkHour()*/;
	if ( setTimeChange && planValue > 0.1 )
	{
		str.Format( _T("从原计划工时%0.1fh更改为%0.1fh "), 
			m_bakTaskObj.GetPlanWorkHour(),  value );
		sysChangeContent += str;
	}
	if ( str.IsEmpty() )
	{
		sysChangeContent.Remove( sysChangeContent.GetLength() - 1 );
	}
	GetDlgItem( IDC_STATIC_SYS_CONTENT )->SetWindowText( sysChangeContent );
	return sysChangeContent;
}
void CChangeApplyFor::OnInitStaticText()
{
	//任务
	CString str;
	CString strNewSch;
	CSchedule schObj;
	if ( userExcOk == m_pSever->GetScheduleSever()->GetMaxNewScheduleByTaskId( m_taskObj.GetId(), schObj ) )
	{
		strNewSch.Format(_T("\r\n最新进度:%03d%%,%03.1fh T:%s\r\nDC:%s"), schObj.GetPercent(), schObj.GetWorkHours(),
			schObj.GetUpdateTimeStr(), schObj.GetUpdateContent() );
	}

	str.Format( _T("名称：%s\r\n 计划时间：%s--%s %0.1fh 实际开始时间：%s--%s %0.1f"),
		m_taskObj.GetName(), m_taskObj.GetPanStartDateShowStr(), m_taskObj.GetPanEndDateShowStr(),
		m_taskObj.GetPlanWorkHour(), m_taskObj.GetFactStartDateShowStr(), m_taskObj.GetFactEndDateShowStr(), m_taskObj.GetFactWorkHour(),
		strNewSch );
	GetDlgItem( IDC_STATIC_TASK )->SetWindowText( str );
}
void CChangeApplyFor::OnInitChangId()
{
	CString strInfo;
	CString status = m_changeObj.GetChangeStatusString( m_changeObj.GetStatus() );

	//const CTaskStaus& stMap = m_pTaskSever->GetTaskStausMap();
	//stMap.GetStrStausById( m_taskObj.GetId(), status );
	if ( m_changeObj.GetId() > 0 )
	{
		strInfo.Format(_T("变更编号:%05d %s"), m_changeObj.GetId(),status);
		GetDlgItem( IDC_STATIC_ID )->SetWindowText( strInfo ) ;
		GetDlgItem( IDC_STATIC_DATE )->SetWindowText( m_changeObj.GetChangeDateTimeStr() );
	}else
	{
		GetDlgItem( IDC_STATIC_DATE )->SetWindowText( COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d")) );
	}
}

void CChangeApplyFor::OnCbnSelchangeComboChangeType()
{
	OnSetEnable();
	OnInitSysChangeContent();
}


void CChangeApplyFor::OnDtnDatetimechangeDatePlanTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnInitSysChangeContent();
	*pResult = 0;
}


void CChangeApplyFor::OnEnChangeEditPanWorkHour()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	OnInitSysChangeContent();
	// TODO:  在此添加控件通知处理程序代码
}

BOOL CChangeApplyFor::GetChangeAndTask( CTaskObj &bakObj, CTaskChange &chObj )
{
	int type = m_changeTypeBox.GetCurSel();
	assert( type >= 0);
	chObj.SetType( type );
	//任务相关的
	CString str;
	if ( changeTime == type || changeReStart == type )
	{
		 COleDateTime time;
		 m_panEndTimeCtrl.GetTime( time );
		 if ( time < m_taskObj.GetPlanEndTime() )
		 {
			 AfxMessageBox(_T("计划延时时间不能比计划完成时间早"));
			 return FALSE;
		 }
		 bakObj.SetPlanEndTime( time );
         m_addWorkHourTask.GetWindowText( str );
		 double workHourEidt;
		 if ( FALSE == CUtilityTool::GetObj()->ConvertStringToDouble( str , workHourEidt ))
		 {
			 AfxMessageBox(_T("追加工时有非法字符"));
			 return FALSE;
		 }
		 if ( changeTime == type && time == m_taskObj.GetPlanEndTime()
			     && 0 == CUtilityTool::GetObj()->DoubleCmp( workHourEidt ) )
		 {
			 AfxMessageBox( _T("变更计划时间，计划完成时间工时未改变或未进行追加工时!") );
			 return FALSE;
		 }
		 COleDateTimeSpan span = bakObj.GetPlanEndTime() - m_taskObj.GetPlanStartTime();
		 double workHour = (span.GetDays()+1)*7.5f;
		 double setWorkHour = (float)workHourEidt + m_taskObj.GetPlanWorkHour();
		 if ( setWorkHour > workHour )
		 {
			 AfxMessageBox(_T("追加的工时与计划工时之和大于了计划开始日期和变更完成日期直接的总合"));
			 return FALSE;
		 }
		 bakObj.SetPlanWorkHour( setWorkHour );
		 GetDlgItem(IDC_EDIT_TASCKDC)->GetWindowText(str);
		 bakObj.SetDec( str );
	}
	//变更表相关的
	GetDlgItem(IDC_EDIT_RESON)->GetWindowText( str );
	if ( str.IsEmpty() )
	{
		AfxMessageBox(_T("变更原因不能为空") );
		return FALSE;
	}
	if ( str.GetLength() < 11 )
	{
		AfxMessageBox(_T("变更原因过短") );
		return FALSE;
	}
	chObj.SetChangeReason( str );
	//变更原因选项
	int cnt = m_optionStrings.size();
	if ( cnt > 0 )
	{
		bool hasCheck = false;
		for ( int i = 0; i < cnt; i++ )
		{
			bool bCheck= m_resonOptionBtns[i].GetCheck() == BST_CHECKED;
			if ( bCheck )
			{
				hasCheck = true;
			}
			chObj.SetResonOptionSelectSt( i, bCheck );
		}
		if ( false == hasCheck && changeReStart != m_changeObj.GetType() )
		{
			AfxMessageBox(_T("请选择变更原因选项") );
			return FALSE;
		}
	}
	GetDlgItem(IDC_EDIT_CONTENT)->GetWindowText( str );
	if ( str.GetLength() > 0 )
	{
		str += _T("\r\n");
	}
	str += OnInitSysChangeContent();
	chObj.SetChangeContent( str );
	chObj.SetChangeDateTime( COleDateTime::GetCurrentTime());
	
	bakObj.SetAttribute( -1 );
	return TRUE;
}
//isAppFor 是否申请
BOOL CChangeApplyFor::SaveChangeAndTask( BOOL isAppFor )
{
	CTaskObj bakObj = m_bakTaskObj;
	CTaskChange chObj = m_changeObj;
	if ( FALSE == GetChangeAndTask( bakObj, chObj ) )
	{
		return paraError;
	}
	BOOL ret = m_pChangeSever->SaveChange(  chObj, bakObj, isAppFor);
	if ( userExcOk == ret )
	{
		if ( isAppFor )
		{
			CDialogEx::OnOK();
		}
		m_changeObj = chObj;
		m_bakTaskObj = bakObj;
		OnInitChangId();
		GetDlgItem( IDC_BTN_DELETE )->EnableWindow( m_changeObj.GetId() > 0 );
	}else
	{
		AfxMessageBox( m_pSever->GetErrorString( ret) );
	}
	return ret;
}

void CChangeApplyFor::OnBnClickedBtnSave()
{
	SaveChangeAndTask( FALSE );
}


void CChangeApplyFor::OnBnClickedBtnApply()
{
	SaveChangeAndTask( TRUE );
}


void CChangeApplyFor::OnBnClickedBtnDelete()
{// TODO: 在此添加控件通知处理程序代码
	if ( m_changeObj.GetBakTaskId() > 0 )
	{//删除备用任务
		BOOL ret = m_pChangeSever->DeleteChange( m_changeObj );
		if ( userExcOk == ret )
		{
			CDialogEx::OnCancel();
		}else
		{
			CString str;
			str.Format(_T("删除变更失败，%s"), m_pSever->GetErrorString(ret));
			AfxMessageBox( str );
		}
	}
}
