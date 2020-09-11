#ifndef PROGRESS_DIALOG_H
#define PROGRESS_DIALOG_H

#include "TextProgressCtrl.h"
#include "../../Sid_Project_Manger.h"
//pre-definition
typedef struct PD_Content
{
	CString			text;
	COLORREF		barColor;
	COLORREF		barBkColor;
	COLORREF		textColor;
	COLORREF		textBkColor;
	BOOL			bCancelButtonVisible;	/* 是否有取消按钮 */
	PD_Content()
	{
		text.Empty();
		barColor = RGB(119,240,47);
		barBkColor = RGB(205,205,205);
		textColor = RGB(43,43,249);
		textBkColor = RGB(255,255,255);
		bCancelButtonVisible = TRUE;
	}
	explicit PD_Content(const PD_Content& other)
	{
		this->text = other.text;
		this->barColor = other.barColor;
		this->barBkColor = other.barBkColor;
		this->textColor = other.textColor;
		this->textBkColor = other.textBkColor;
		this->textBkColor = other.bCancelButtonVisible;
	}
	PD_Content& operator =(const PD_Content& other)
	{
		this->text = other.text;
		this->barColor = other.barColor;
		this->barBkColor = other.barBkColor;
		this->textColor = other.textColor;
		this->textBkColor = other.textBkColor;
		this->bCancelButtonVisible = other.bCancelButtonVisible;
		return *this;
	}
}PD_CONTENT;

class CProgressDialog : public CDialog
{
	DECLARE_DYNCREATE(CProgressDialog)
public:
	CProgressDialog(const PD_CONTENT* content = NULL,
					BOOL bMarqueeMod = FALSE,
					CWnd* pParent = NULL);
	virtual ~CProgressDialog();

	enum { IDD = IDD_PROGRESS_DIALOG };

	void show(bool isModel = false);	/* 显示进度对话框，isModel模态/非模态 */
	void close();						/* 关闭进度对话框 */
	void setText(CString text);			/* 更新对话框显示文字 */
	void setPos(int pos);				/* 更新进度条位置 */
	int  getPos();						/* 获取进度条位置 */

	BOOL isCanceled();

protected: 
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();		/* 正常结束 */
	virtual void OnCancel();	/* 取消结束 */

	afx_msg void OnDestroy();
	afx_msg void OnClose();	

	DECLARE_MESSAGE_MAP()
private:
	CTextProgressCtrl	m_progressCtrl;
	CStatic				m_textCtrl;

	BOOL				m_bCreated;				/* 非模态时是否已经创建 */
	BOOL				m_bCanceled;			/* 对话框是否被取消 */
	BOOL				m_bModel;				/* 模态对话框/非模态对话框 */
	BOOL				m_bMarqueeMode;			/* 是否滚动模式 */

	PD_CONTENT			m_content;				/* 用户定义显示内容 */
	int					m_pos;					/* 滚动条位置 */
	int					m_percent;				/* 滚动条百分值 */

	//function
	BOOL InitControls();
};

#endif