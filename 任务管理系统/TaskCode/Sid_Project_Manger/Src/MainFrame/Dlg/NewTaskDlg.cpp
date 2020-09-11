// NewTaskDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewTaskDlg.h"
#include "afxdialogex.h"
#include "../MainFrm.h"
#include "Src/Tools/UtilityTool/UtilityTool.h"

// CNewTaskDlg �Ի���

IMPLEMENT_DYNAMIC(CNewTaskDlg, CDialogEx)

CNewTaskDlg::CNewTaskDlg( const CTaskObj *pObj, CWnd* pParent /*=NULL*/)
	: CDialogEx(CNewTaskDlg::IDD, pParent)
{
	m_pSever = ((CMainFrame *)AfxGetMainWnd())->GetSeverFactory();
	ASSERT( NULL != m_pSever );
	m_pTaskSever = m_pSever->GetTaskSever();
	m_pLoginSever = m_pSever->GetLoginServer();
	if ( pObj != NULL   )
	{
		if ( pObj->GetTaskStatus() == newTasking )
		{
			m_pOldTask = pObj; //�ϵ�����
			m_newTask = *pObj;
		}else
		{
			AfxMessageBox( _T("����״̬�������½��У�������Ϣ�����޸�"));
			return;
		}
	}
	m_lastPrjSel = -1;
}

CNewTaskDlg::~CNewTaskDlg()
{
}

void CNewTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TASK_TYPE, m_taskTypeBox);
	DDX_Control(pDX, IDC_COMBO4, m_publishUserBox);
	DDX_Control(pDX, IDC_COMBO_OWNER, m_owerUserBox);
	DDX_Control(pDX, IDC_COMBO_PROJECT_TYPE, m_prjTextBox);
	DDX_Control(pDX, IDC_COMBO5, m_workHourBox);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_startDateTimeCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_endTimeCtrl);
	DDX_Control(pDX, IDC_EDIT2, m_taskContentEdit);
	DDX_Control(pDX, IDC_COMBO6, m_fatherTaskBox);
	DDX_Control(pDX, IDC_EDIT_TASKDC, m_decText);
	DDX_Control(pDX, IDC_COMBO1, m_prjStepBox);
	DDX_Control(pDX, IDC_QUALITY_TASK, m_ctrlSelQualityTask);
}


BEGIN_MESSAGE_MAP(CNewTaskDlg, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SAVE, &CNewTaskDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(ID_BTN_SAVE_SEND, &CNewTaskDlg::OnBnClickedBtnSaveSend)
	ON_CBN_SELCHANGE(IDC_COMBO_OWNER, &CNewTaskDlg::OnCbnSelchangeComboOwner)
	ON_CBN_SELCHANGE(IDC_COMBO_TASK_TYPE, &CNewTaskDlg::OnCbnSelchangeComboTaskType)
	ON_CBN_SELCHANGE(IDC_COMBO_PROJECT_TYPE, &CNewTaskDlg::OnCbnSelchangeComboProjectType)
END_MESSAGE_MAP()


// CNewTaskDlg ��Ϣ�������


BOOL CNewTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	const CUser loginObj = m_pLoginSever->GetLoginUser();
	if ( FALSE == InitTaskTypeList() )
	{
		AfxMessageBox(_T("��½�û���Ȩ���½�����"));
		return FALSE;
	}
	OnInitPrjBox();
	if ( FALSE == OnInitOwernCtrl() )
	{
		return FALSE;
	}
	OnInitReviwerUser();
	OnInitWorkHourBox();
	OnInitFatherBox();
	OnInitPrjStepBox();
	//
	if ( m_newTask.GetId() >= 0 )
	{
		m_taskContentEdit.SetWindowText( m_newTask.GetName() );
		GetDlgItem( IDC_EDIT_TASKDC )->SetWindowText( m_newTask.GetDec() );
		CString temp;
		temp.Format( _T("%0.1f"), m_newTask.GetPlanWorkHour() );
		GetDlgItem( IDC_EDIT_WORKTIME )->SetWindowText( temp );

		m_startDateTimeCtrl.SetTime( m_newTask.GetPlanStartTime() );
		m_endTimeCtrl.SetTime( m_newTask.GetPlanEndTime() );
	}else
	{
		COleDateTime time;
		time = COleDateTime::GetCurrentTime();
		m_startDateTimeCtrl.SetTime( time );
		m_endTimeCtrl.SetTime( time );
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
BOOL CNewTaskDlg::InitTaskTypeList( )
{
	int count = 0;
	if ( m_pLoginSever->FindLoginPower( powerNewDepartTask ) )
	{
		m_taskTypeBox.InsertString( count,TASKTYPE_DEPARTSTR );
		count++;
	}
	if( m_pLoginSever->FindLoginPower(powerNewPrjTask) )
	{
		m_taskTypeBox.InsertString( count, TASKTYPE_PRJSTR );
		count++;
	}
	if ( 0 == count )
	{
		return FALSE;
	}
	if ( 1 == count)
	{
		m_taskTypeBox.SetCurSel( 0 );
		m_taskTypeBox.EnableWindow( FALSE );
		return TRUE;
	}
	if ( m_newTask.GetId() >= 0 )
	{
		m_taskTypeBox.SelectString( 0, m_newTask.GetType() );
	}else
	{
		m_taskTypeBox.SetCurSel( 0 );
	}
	int sel = m_taskTypeBox.GetCurSel();
	if ( sel >= 0 )
	{
		CString str;
		m_taskTypeBox.GetLBText( sel, str );
		if ( TASKTYPE_PRJSTR == str )
		{
			m_ctrlSelQualityTask.ShowWindow( SW_SHOW );
		}
	}
	return TRUE;
}
//��ʼ��������
void CNewTaskDlg::OnInitReviwerUser()
{
	m_publishUser.clear();
	const ListUser& publishUser = m_pLoginSever->GetPubishPowweUser();
	ListUser::const_iterator itor = publishUser.begin();
	int count = 0;
	CUser   leader = m_pLoginSever->GetLoginUserLeader();
	int index = m_owerUserBox.GetCurSel();
	if ( m_owerUser.size() > 0 && index >= 0 )
	{
		ListUserItor userObjItor = m_owerUser.begin();
		advance( userObjItor, index );
		if ( userObjItor != m_owerUser.end() && userObjItor->GetId() != m_pLoginSever->GetLoginUser().GetId() )
		{
			m_pLoginSever->GetUserLeader( *userObjItor, leader );
		}
	}
	
	int   leaderId = leader.GetId();
	int   leaderPos = 0;
	const CUser&  loginUser = m_pLoginSever->GetLoginUser();
	int   recordPos = -1;
	m_publishUserBox.ResetContent();
	while( itor != publishUser.end() )
	{
		//if ( itor->GetId() != loginUser.GetId() )
		//{
			m_publishUserBox.InsertString( count, itor->GetUserName_R() );
			if ( leaderId >= 0  && itor->GetId() == leader.GetId() )
			{
				leaderPos = count;
			}
			if ( m_newTask.GetId() >= 0 && itor->GetId() == m_newTask.GetMakeScoreUserId() )
			{
				recordPos = count;
			}
			m_publishUser[count] = &(*itor);
			count++;
		/*}*/
		itor++;
	}
	if ( recordPos >= 0 )
	{
		m_publishUserBox.SetCurSel( recordPos );
	}else
	{
		m_publishUserBox.SetCurSel( leaderPos );
	}
	
}
BOOL CNewTaskDlg::OnInitOwernCtrl()
{
	m_owerUserBox.ResetContent();
	CString strType;
	 m_taskTypeBox.GetLBText(m_taskTypeBox.GetCurSel(), strType );
	m_owerUser.clear();
	const CUser&  loginUser = m_pLoginSever->GetLoginUser();
	if ( strType == TASKTYPE_DEPARTSTR )
	{//���䲿������Ȩ��
		if ( m_pLoginSever->FindLoginPower( allocateDepartTask ) )
		{//
			m_pLoginSever->GetLoginOrginMember( m_owerUser );
		}else
		{
			m_owerUser.push_back( loginUser );
			m_owerUserBox.EnableWindow( FALSE );
		}
	}else
	{//������Ŀ����Ȩ��
		if ( m_pLoginSever->FindLoginPower( allocatePrjTask ) )
		{
			int index = m_prjTextBox.GetCurSel();
			ListProject::iterator itor = m_prjList.begin();
			if ( m_prjList.size() <= 0 || index < 0 )
			{
				AfxMessageBox(_T("��Ŀ�б�Ϊ�ջ�δѡ����Ŀ"));
				return FALSE;
			}
			advance( itor, index );
			if ( itor != m_prjList.end() )
			{
				m_pSever->GetProjectSever()->GetProjectMember( (*itor).GetId(), m_owerUser );
				m_owerUserBox.EnableWindow( TRUE );
			}else
			{
				m_owerUser.push_back( loginUser );
				m_owerUserBox.EnableWindow( FALSE );
			}
			
		}else
		{
			m_owerUser.push_back( loginUser );
			m_owerUserBox.EnableWindow( FALSE );
		}
	}
	ListUser::iterator itor = m_owerUser.begin();
	int count = 0;
	int userPos = 0;
	int   recordPos = -1;
	while( itor != m_owerUser.end() )
	{
		m_owerUserBox.InsertString( count, itor->GetUserName_R() );
		if ( itor->GetId() == loginUser.GetId() )
		{
			userPos = count;
		}
		if ( m_newTask.GetId() >= 0 && itor->GetId() == m_newTask.GetOwnerId() )
		{
			recordPos = count;
		}
		count++;
		itor++;
	}
	if ( m_newTask.GetId() > 0 && recordPos == -1 )
	{
		AfxMessageBox(_T("��Ŀ�����˲�����Ŀ��Ա����ҹ���Ա"));
		return FALSE;
	}
	if ( recordPos >= 0)
	{
		m_owerUserBox.SetCurSel( recordPos );
	}else
	{
		m_owerUserBox.SetCurSel(userPos);
	}
	return TRUE;
}
//��ʼ����Ŀ
void CNewTaskDlg::OnInitPrjBox()
{
	m_prjTextBox.ResetContent();
	CString strType;
	m_taskTypeBox.GetLBText( m_taskTypeBox.GetCurSel() ,strType );
	
	if ( strType == TASKTYPE_DEPARTSTR )
	{//��������
		m_prjTextBox.InsertString(0, _T("��") );
		m_prjTextBox.SetCurSel( 0 );
		m_prjTextBox.EnableWindow( FALSE );
	}else
	{//��Ŀ����
		const CUser&  loginUser = m_pLoginSever->GetLoginUser();
		if ( m_prjList.size() <= 0 )
		{
			m_prjList.clear();
			int loginUserId = loginUser.GetId();
			if ( _T("����Ա") == m_pLoginSever->GetLoginRole().GetRoleName()  
				|| loginUserId == m_pLoginSever->GetLoginDepart().GetMangerId()  )
			{
				m_pSever->GetProjectSever()->GetAllProject( m_prjList );
			}else
			{
				m_pSever->GetProjectSever()->GetProjectByMember( loginUserId, m_prjList );
			}
		}

		ListProject::iterator itor =  m_prjList.begin();
		int count = 0;
		int recordPrj = -1;
		while( itor != m_prjList.end() )
		{
			if ( m_newTask.GetId() >= 0 && m_newTask.GetProjctId() == itor->GetId() )
			{
				recordPrj = count;
			}
			m_prjTextBox.InsertString( count++, itor->GetShortName() );
			itor++;
		}
		if ( recordPrj >= 0 )
		{
			m_prjTextBox.SetCurSel( recordPrj );
			m_lastPrjSel = recordPrj;
		}else
		{
			m_prjTextBox.SetCurSel( 0 );
			m_lastPrjSel = 0;
		}
		if ( m_prjTextBox.GetCount() > 0 )
		{
			m_prjTextBox.EnableWindow( TRUE );
		}
		
	}
}
void CNewTaskDlg::OnInitPrjStepBox()
{
	CString strType;
	m_taskTypeBox.GetLBText( m_taskTypeBox.GetCurSel() ,strType );
	if ( strType == TASKTYPE_DEPARTSTR )
	{
		m_prjStepBox.EnableWindow( FALSE );
	}else
	{
		int selPrj = m_prjTextBox.GetCurSel();
		if ( selPrj >= 0 && m_prjList.size() > 0)
		{
			ListProject::iterator itor = m_prjList.begin();
			advance( itor, selPrj );
			ASSERT( itor != m_prjList.end() );
			m_prjStepBox.EnableWindow( TRUE );
			int selPos = m_prjStepBox.GetCurSel();
			m_prjStepBox.ResetContent();
			m_prjStepBox.InsertString( 0, _T("��") );
	        m_listStep.clear();
			m_pSever->GetSystemCfgSever()->GetProjectStepList( itor->GetPrjStepType(), m_listStep );
			int cnt = 1;
			int orginSel = 0;
			for ( ListProjectStepConstItor itorStep = m_listStep.begin(); itorStep != m_listStep.end(); itorStep++ )
			{
				if ( m_newTask.GetAttribute() == itorStep->GetId() )
				{
					orginSel = cnt;
				}
				m_prjStepBox.InsertString( cnt++, itorStep->GetName() );
			}
			if ( selPos < 0 || selPos >= cnt )
			{
				m_prjStepBox.SetCurSel( orginSel );
			}else
			{
				m_prjStepBox.SetCurSel( selPos );
			}
		}
	
	}

}
void CNewTaskDlg::OnInitWorkHourBox()
{
	const TaskIdStringMap list = m_pSever->GetTaskSever()->GetWorkHourString();
	TaskIdStringMap::const_iterator itor = list.begin();
	int recordPos = m_workHourBox.GetCurSel();
	int count = 0;
	while( itor != list.end() )
	{
		m_workHourBox.InsertString( count, itor->second );
		
		if ( recordPos < 0 && m_newTask.GetId() >= 0 && itor->first == m_newTask.GetWorkTypeId() )
		{
			recordPos = count;
		}
		count++;
		itor++;
	}
	if ( recordPos >= 0 )
	{
	   m_workHourBox.SetCurSel( recordPos );
	}
}
//��ȡ���������б�
void CNewTaskDlg::OnInitFatherBox()
{
	m_fatherTaskBox.ResetContent();
	CString strType;
	int curSel = m_prjTextBox.GetCurSel();
	m_taskTypeBox.GetLBText( m_taskTypeBox.GetCurSel() ,strType );

	m_fatherTaskBox.InsertString(0, _T("��") );
	m_fatherTaskBox.SetCurSel( 0 );
	m_fatherTaskBox.EnableWindow( FALSE );
	if ( strType == TASKTYPE_PRJSTR )
	{//��Ŀ����
		if ( !(m_lastPrjSel == curSel && m_fatherTaskList.size() > 0) )
		{
			int indexPrj = m_prjTextBox.GetCurSel();
			if ( indexPrj < 0 || m_prjList.size() <= 0 )
			{
				return;
			}
			ListProject::iterator itor = m_prjList.begin();
			advance( itor, indexPrj );
			if ( itor != m_prjList.end() )
			{
				m_fatherTaskList.clear();
				m_pTaskSever->GetOnGoingTaskByPrjId( itor->GetId(), m_fatherTaskList );
				ListTaskItor taskItor = m_fatherTaskList.begin();
				int count = 1;
				while( taskItor != m_fatherTaskList.end() )
				{
					m_fatherTaskBox.InsertString( count, taskItor->GetName() ); 
					count++;
					taskItor++;
				}
				m_fatherTaskBox.EnableWindow( TRUE );
			}
			m_lastPrjSel = curSel;
		}
	}
	
}
//��������
void CNewTaskDlg::OnBnClickedBtnSave()
{
	CTaskObj obj = m_newTask;
	if ( GetTaskFromView( obj ) )
	{
		obj.SetCreateTime( COleDateTime::GetCurrentTime() );
		obj.SetCreateId( m_pSever->GetLoginServer()->GetLoginUser().GetId() );
		BOOL ret = -1;
		if ( obj.GetId() >= 0 )
		{//����
			ret = m_pTaskSever->UpdateTask( obj );
			if ( userExcOk != ret  )
			{
				CString str = m_pSever->GetErrorString( ret );
				AfxMessageBox( str );
				return;
			}
			m_newTask = obj;
		}else
		{
			BOOL ret = m_pTaskSever->InsertTask( obj );
			if ( ret < 0 )
			{
				CString str = m_pSever->GetErrorString( ret );
				AfxMessageBox( str );
				return;
			}
			obj.SetId( ret );
			m_newTask = obj;
		}
		if ( UpdateFatherTask( obj ) )
			/*AfxMessageBox( _T("��������ɹ�") );*/
			CDialogEx::OnOK();
		else
			AfxMessageBox( _T("��������ɹ�,���������ӹ�ϵ����ʧ��") );
	}
}
BOOL CNewTaskDlg::UpdateFatherTask(const CTaskObj &obj )
{
	int selId = m_fatherTaskBox.GetCurSel();
	int fatherId = -1;
	if ( selId > 0 && m_fatherTaskList.size() > 0)
	{
		ListTaskItor itor = m_fatherTaskList.begin();
		advance( itor, selId - 1 );
		if ( itor != m_fatherTaskList.end() )
		{
			BOOL ret = m_pTaskSever->UpdateTaskFather( obj.GetId(), itor->GetId() );
			if ( userExcOk  != ret)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
//���沢��������
void CNewTaskDlg::OnBnClickedBtnSaveSend()
{
	CTaskObj obj = m_newTask;
	if ( GetTaskFromView( obj ) )
	{
		obj.SetCreateTime( COleDateTime::GetCurrentTime() );
		obj.SetCreateId( m_pSever->GetLoginServer()->GetLoginUser().GetId() );
		obj.SetTaskStatus( waitPublish );
		BOOL ret = -1;
		if ( obj.GetId() >= 0 )
		{//����
			ret = m_pTaskSever->UpdateTask( obj );
			if ( userExcOk != ret  )
			{
				
				CString str = m_pSever->GetErrorString( ret );
				AfxMessageBox( str );
				return;
			}
		}else
		{//����
			BOOL ret = m_pTaskSever->InsertTask( obj );
			if ( ret < 0 )
			{
				CString str = m_pSever->GetErrorString( ret );
				AfxMessageBox( str );
				return;
			}
			m_newTask = obj;
		}
		ret = UpdateFatherTask( obj );
		if (TRUE == ret )
			AfxMessageBox( _T("���漰�ύ��������ɹ�") );
		else
			AfxMessageBox( _T("���漰�ύ��������ɹ������������ӹ�ϵʧ��") );
		CDialogEx::OnOK();
	}
}
BOOL CNewTaskDlg::GetTaskFromView(CTaskObj &obj )
{
	//��������
	CString tempstr;
    m_taskContentEdit.GetWindowText( tempstr ) ;
	if ( tempstr.IsEmpty() )
	{
		AfxMessageBox(_T("�������Ʋ���Ϊ��"));
		return FALSE;
	}
	obj.SetName( tempstr);
	//������
	int publishId = -1;
	ListConstUserMap::iterator itor = m_publishUser.find( m_publishUserBox.GetCurSel()) ;
	if ( itor != m_publishUser.end() )
	{
		publishId = itor->second->GetId();
		obj.SetMakeScoreUserId( publishId );
	}
	//������
	int index = m_owerUserBox.GetCurSel();
	ListUserItor itoruser = m_owerUser.begin();
	if ( m_owerUser.size() <= 0 || index < 0)
	{
		AfxMessageBox(_T("������ѡ�����"));
		return FALSE;
	}
	advance( itoruser, index );
	if ( itoruser != m_owerUser.end() )
	{
		obj.SetOwnerId( itoruser->GetId() );
	}
	//��������
	CString type;
	m_taskTypeBox.GetLBText( m_taskTypeBox.GetCurSel(), type );
	obj.SetType( type );
	//������Ŀ
	if ( type == TASKTYPE_DEPARTSTR )
	{
		obj.SetProjctId( -1 );
		obj.SetAttribute( NORMAL_TASK );
	}else
	{//
		int selId = m_prjTextBox.GetCurSel();
		if ( m_prjList.size() <= 0 || selId < 0 )
		{
			AfxMessageBox(_T("������Ŀѡ�����"));
			return FALSE;
		}
		ListProject::iterator it = m_prjList.begin();
		advance( it, selId );
		if ( it != m_prjList.end() )
		{
			obj.SetProjctId( it->GetId() );
		}
	    selId = m_prjStepBox.GetCurSel();
		if ( selId > 0 && selId < ( m_listStep.size()+1) )
		{
			ListProjectStepConstItor itor = m_listStep.begin();
			advance( itor, selId - 1 );
			obj.SetAttribute( itor->GetId() );
		}else
		{
			obj.SetAttribute( NORMAL_TASK );
		}
	}
	//��ʱ����
	const TaskIdStringMap list = m_pSever->GetTaskSever()->GetWorkHourString();
	if ( list.size() <= 0 || m_workHourBox.GetCurSel() < 0 )
	{
		AfxMessageBox(_T("��ʱ����ѡ�����"));
		return FALSE;
	}
	TaskIdStringMap::const_iterator itorString = list.begin();
	advance( itorString, m_workHourBox.GetCurSel() );
	if ( itorString != list.end() )
	{
		obj.SetWorkTypeId( itorString->first);
	}
	//�ƻ���ʼ����
	COleDateTime time;
	m_startDateTimeCtrl.GetTime( time );
	obj.SetPlanStartTime( time );
	//�ƻ��������
	m_endTimeCtrl.GetTime( time );
	obj.SetPlanEndTime( time );
	if ( time < obj.GetPlanStartTime() )
	{
		AfxMessageBox(_T("�ƻ�������ڱȼƻ���ʼ������"));
		return FALSE;
	}
	//��ʱ��ʱ
	CString workHourStr;
	COleDateTimeSpan span = obj.GetPlanEndTime() - obj.GetPlanStartTime();
	float maxWorkHour = (span.GetDays()+1)*7.5f;

	GetDlgItem( IDC_EDIT_WORKTIME )->GetWindowText( workHourStr);
	if ( workHourStr.IsEmpty() )
	{
		AfxMessageBox( _T("�ƻ���ʱΪ��") );
		return FALSE;
	}
	double  value = 0.0;
	CUtilityTool::GetObj()->ConvertStringToDouble( workHourStr, value );
	if ( value < 1.0 || value > maxWorkHour)
	{
		CString str;
		str.Format( _T("�ƻ���ʱ���ڷ�Χ��[1.0~%0.1f]����%s��%s�ܹ���ʱΪ%0.1f"), maxWorkHour, 
			obj.GetPanStartDateStr(), obj.GetPanEndDateStr(), maxWorkHour );
		AfxMessageBox( str );
		return FALSE;
	}
	obj.SetPlanWorkHour( (float)value );

	//��������
	m_decText.GetWindowText( tempstr );
	obj.SetDec( tempstr );
	//
	if ( TASKTYPE_PRJSTR == type )
	{
		if (  BST_CHECKED == m_ctrlSelQualityTask.GetCheck() )
		{
			obj.SetTaskFlag( 1 );
		}else
		{
			obj.SetTaskFlag( 0 );
		}
	}

	return TRUE;
}

//�޸�������
void CNewTaskDlg::OnCbnSelchangeComboOwner()
{//Ӱ��������
	OnInitReviwerUser();
}

//�޸���������
void CNewTaskDlg::OnCbnSelchangeComboTaskType()
{
	OnInitPrjBox();
	OnInitOwernCtrl();
	OnInitReviwerUser();
	OnInitFatherBox();
	OnInitPrjStepBox();
	int sel = m_taskTypeBox.GetCurSel();
	if ( sel >= 0 )
	{
		CString str;
		m_taskTypeBox.GetLBText( sel, str );
		if ( TASKTYPE_PRJSTR == str )
		{
			m_ctrlSelQualityTask.ShowWindow( SW_SHOW );
		}
	}
}
//�޸���Ŀ����
void CNewTaskDlg::OnCbnSelchangeComboProjectType()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnInitOwernCtrl();
	OnInitReviwerUser();
	OnInitFatherBox();
	OnInitPrjStepBox();
}
