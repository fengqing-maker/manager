#include "stdafx.h"
#include "ProgressDialog.h"


#ifdef _DEBUG                      /* 判断是否定义_DEBUG */
#undef THIS_FILE                   /* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;   /* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              /* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */

IMPLEMENT_DYNCREATE(CProgressDialog,CDialog)

CProgressDialog::CProgressDialog(const PD_CONTENT* content /* NULL */,
								BOOL bMarqueeMod /*= FALSE*/,
								CWnd* pParent /*= NULL*/)
:CDialog(CProgressDialog::IDD,pParent)
{
	if (content != 0)
	{
		m_content = *content;
	}
	m_bCreated = FALSE;
	m_bMarqueeMode = bMarqueeMod;
	m_percent = 0;	/* 初始进度 */
}
CProgressDialog::~CProgressDialog()
{
	this->DestroyWindow();
}
void CProgressDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_PROGRESS_CTRL,m_progressCtrl);
	DDX_Control(pDX,IDC_TEXT,m_textCtrl);
}
BEGIN_MESSAGE_MAP(CProgressDialog,CDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CProgressDialog::show(bool isModel /* = false */)
{
	m_bModel = isModel;
	if (isModel == true)
	{
		m_bCreated = TRUE;
		int ret = this->DoModal();
		if ( ret < 0 )
		{
			TRACE("进度条对话框创建失败.\n");
		}
	} 
	else
	{
		if (m_bCreated == FALSE)
			if (!this->Create(IDD_PROGRESS_DIALOG))
				TRACE("进度条对话框创建失败.\n");
			else
				m_bCreated = TRUE;
		this->ShowWindow(SW_SHOWNORMAL);
	}
}

void CProgressDialog::close()
{
	if (m_bCanceled == FALSE)
	{
		OnOK();
	}
}

void CProgressDialog::setText(CString text)
{
	m_textCtrl.SetWindowText(text);
}

void CProgressDialog::setPos(int pos)
{
	m_progressCtrl.SetPos(pos);
}

int CProgressDialog::getPos()
{
	return m_progressCtrl.GetPos();
}

BOOL CProgressDialog::isCanceled()
{
	return m_bCanceled;
}
/************************消息响应*************************************/
BOOL CProgressDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitControls();	//初始化界面控件

	UpdateData(FALSE);
	//By default,returns TRUE to indicate that the focus should be set to the first control in the dialog box. 
	return TRUE;
}

void CProgressDialog::OnOK()
{
	if (m_bModel == TRUE)
	{
		/* 模态对话框，什么也不做 */
		CDialog::OnOK();
	} 
	else
	{
		/* 非模态对话框，手动销毁窗口 */
		DestroyWindow();
	}
}
void CProgressDialog::OnCancel()
{
	m_bCanceled = TRUE;
	if (m_bModel == TRUE)
	{
		/* 模态对话框，什么也不做 */
		CDialog::OnOK();
	} 
	else
	{
		/* 非模态对话框，手动销毁窗口 */
		DestroyWindow();
	}
}

void CProgressDialog::OnDestroy()
{
	m_bCreated = FALSE;
	CDialog::OnDestroy();
}
void CProgressDialog::OnClose()
{
	//CDialog::OnClose();
}
/******************************private function*****************************/
BOOL CProgressDialog::InitControls()
{
	m_bCanceled = FALSE;
	/* 设置图标 */
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->SetIcon(m_hIcon,TRUE);
	this->SetIcon(m_hIcon,FALSE);
	/* 控件初始化 */
	m_textCtrl.SetWindowText(m_content.text);
	if (m_bMarqueeMode == TRUE)
	{
		m_progressCtrl.SetMarquee(true,30);
		m_progressCtrl.SetShowPercent(false);
		m_progressCtrl.SetWindowText(_T(""));	/* 进度条中不显示内容 */
	}
	else
	{
		m_progressCtrl.SetMarquee(false,0);
		m_progressCtrl.SetShowPercent(true);
		m_progressCtrl.SetWindowText(_T(""));	/* 进度条中不显示内容 */
	}
	m_progressCtrl.SetBarColor(m_content.barColor);
	m_progressCtrl.SetBarBkColor(m_content.barBkColor);
	m_progressCtrl.SetTextColor(m_content.textColor);
	m_progressCtrl.SetTextBkColor(m_content.textBkColor);

	if (m_content.bCancelButtonVisible == FALSE)
	{
		/* 隐藏取消按钮 */
		CWnd* cancelButton = GetDlgItem(IDCANCEL);
		if (cancelButton != 0 && cancelButton->IsKindOf(RUNTIME_CLASS(CWnd)))
		{
			cancelButton->ShowWindow(SW_HIDE);
		}
		/* 关闭按钮失效 */
		//this->ModifyStyle(WS_SYSMENU,0);
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		int i = pSysMenu->GetMenuItemID(1) ;
		pSysMenu->EnableMenuItem(i,MF_DISABLED );
	}

	m_progressCtrl.Invalidate();
	m_progressCtrl.UpdateWindow();
	return TRUE;
}