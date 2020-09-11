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
	BOOL			bCancelButtonVisible;	/* �Ƿ���ȡ����ť */
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

	void show(bool isModel = false);	/* ��ʾ���ȶԻ���isModelģ̬/��ģ̬ */
	void close();						/* �رս��ȶԻ��� */
	void setText(CString text);			/* ���¶Ի�����ʾ���� */
	void setPos(int pos);				/* ���½�����λ�� */
	int  getPos();						/* ��ȡ������λ�� */

	BOOL isCanceled();

protected: 
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void OnOK();		/* �������� */
	virtual void OnCancel();	/* ȡ������ */

	afx_msg void OnDestroy();
	afx_msg void OnClose();	

	DECLARE_MESSAGE_MAP()
private:
	CTextProgressCtrl	m_progressCtrl;
	CStatic				m_textCtrl;

	BOOL				m_bCreated;				/* ��ģ̬ʱ�Ƿ��Ѿ����� */
	BOOL				m_bCanceled;			/* �Ի����Ƿ�ȡ�� */
	BOOL				m_bModel;				/* ģ̬�Ի���/��ģ̬�Ի��� */
	BOOL				m_bMarqueeMode;			/* �Ƿ����ģʽ */

	PD_CONTENT			m_content;				/* �û�������ʾ���� */
	int					m_pos;					/* ������λ�� */
	int					m_percent;				/* �������ٷ�ֵ */

	//function
	BOOL InitControls();
};

#endif