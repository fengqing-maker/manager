#ifndef  COMMON_DEF_H_3I47
#define  COMMON_DEF_H_3I47

#include <map>

/**@brief defination for grid control  */
typedef struct gridcolumninfo
{
	int		type;			/* ������ */
	CString title;			/* �ı� */
	int		width;			/* ��� */
	UINT    alignment;		/* ���뷽ʽ */
	int     dataType;       /*������*/
	gridcolumninfo()
	{
		type=0;title.Empty();width=0;alignment=0;
		dataType = 0;
	}
}GridColumnInfo;
typedef std::tr1::shared_ptr<GridColumnInfo>			GridColumnPtr;
typedef std::map<int,GridColumnPtr>						GridColumnMap;	/* (����ţ�����Ϣ) */
typedef GridColumnMap::iterator							GridColumnMapItor;
typedef GridColumnMap::const_iterator					GridColumnMapConstItor;

/**@brief defination for Rich Edit Control */
typedef struct textformat 
{
	CString  font;		/* ʹ���������� */
	COLORREF color;		/* ʹ��������ɫ */
	BOOL	 bUnderLine;/* �Ƿ�ʹ���»��� */
	BOOL	 bBold;		/* �Ƿ�ʹ�ô��� */
	textformat()
	{
		font = _T("����");
		color = RGB(0,0,0);
		bUnderLine = FALSE;
		bBold = FALSE;
	}
}RichEditTextFormat;

#endif
