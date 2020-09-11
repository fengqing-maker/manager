#include "StdAfx.h"
#include "UtilityTool.h"
#include "../../MainFrame/Sid_Project_Manger.h"
#include "../MD5/Md5Class.h"

#include <stdlib.h>
#include <math.h>

#ifdef _DEBUG                      /* 判断是否定义_DEBUG */
#undef THIS_FILE                   /* 取消THIS_FILE的定义 */
static char THIS_FILE[]=__FILE__;   /* 定义THIS_FILE指向文件名 */
#define new DEBUG_NEW              /* 定义调试new宏，取代new关键字 */
#endif								/* 结束 */


CUtilityTool::CUtilityTool(void)
{
}
CUtilityTool::~CUtilityTool(void)
{
}

BOOL CUtilityTool::ConvertStringToInt(CString& str,int& integer)
{
	if (str.IsEmpty())
		return FALSE;
	CStringA strA;
	strA = str;
	integer = atoi(strA);
	return TRUE;
}
BOOL CUtilityTool::ConvertIntToString(int& integer,CString& str)
{
	str.Format(_T("%d"),integer);
	return TRUE;
}

BOOL CUtilityTool::ConvertDoubleToString(double& dValue,CString& str)
{
	str.Format(_T("%lf"),dValue);
	return TRUE;
}
BOOL CUtilityTool::ConvertStringToDouble(CString& str,double& dValue)
{
	CStringA strA;
	strA = str;
	dValue = atof(strA);
	double tempValue = dValue + Precision_2;
	int    value1 = (int)(tempValue*10);
	int    value2 = (int)(dValue*10);
	if ( value1 != value2 )
	{
		dValue = tempValue;
	}
	return TRUE;
}

BOOL CUtilityTool::ConvertStringToIPAddress(CString& strAddress,IPAddress& ipAddress)
{
	int beginPos = 0;
	int endPos = 0;
	CString strField;
	int field = 0;
	/* 取第一位地址域 */
	if (-1 != (endPos = strAddress.Find(_T('.'),beginPos)))
	{
		strField = strAddress.Mid(beginPos,(endPos-beginPos));
		if(!ConvertStringToInt(strField,field))
			return FALSE;
		ipAddress.field0 = (BYTE)field;
		endPos++;
		beginPos = endPos;
	}
	else
	{
		return FALSE;
	}
	/* 取第二位地址域 */
	if (-1 != (endPos = strAddress.Find(_T('.'),beginPos)))
	{
		strField = strAddress.Mid(beginPos,(endPos-beginPos));
		if(!ConvertStringToInt(strField,field))
			return FALSE;
		ipAddress.field1 = (BYTE)field;
		endPos++;
		beginPos = endPos;
	}
	else
	{
		return FALSE;
	}
	/* 取第三位地址域 */
	if (-1 != (endPos = strAddress.Find(_T('.'),beginPos)))
	{
		strField = strAddress.Mid(beginPos,(endPos-beginPos));
		if(!ConvertStringToInt(strField,field))
			return FALSE;
		ipAddress.field2 = (BYTE)field;
		endPos++;
		beginPos = endPos;
	}
	else
	{
		return FALSE;
	}
	/* 取第四位地址域 */
	endPos = strAddress.GetLength()+1;
	strField = strAddress.Mid(beginPos,(endPos-beginPos));
	if(!ConvertStringToInt(strField,field))
		return FALSE;
	ipAddress.field3 = (BYTE)field;
	beginPos = endPos;
	return TRUE;
}
BOOL CUtilityTool::ConvertIPAddressToString(IPAddress& ipAddress,CString& strAddress)
{
	CString strField;
	int field = 0;
	strAddress.Empty();
	/* 填写第一位地址域 */
	field = (int)ipAddress.field0;
	if (!ConvertIntToString(field,strField))
		return FALSE;
	strAddress += strField + _T(".");
	/* 填写第二位地址域 */
	field = (int)ipAddress.field1;
	if (!ConvertIntToString(field,strField))
		return FALSE;
	strAddress += strField + _T(".");
	/* 填写第三位地址域 */
	field = (int)ipAddress.field2;
	if (!ConvertIntToString(field,strField))
		return FALSE;
	strAddress += strField + _T(".");
	/* 填写第四位地址域 */
	field = (int)ipAddress.field3;
	if (!ConvertIntToString(field,strField))
		return FALSE;
	strAddress += strField;
	return TRUE;
}


void CUtilityTool::MessageDialog(CString msg,CWnd* pParent /*= NULL*/)
{
	HWND hParent = NULL;
	if (pParent == NULL)
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		hParent = pMainFrm->GetSafeHwnd();
	}
	else
	{
		hParent = pParent->GetSafeHwnd();
	}
	::MessageBox(hParent,msg,_T("消息"),
		MB_OK|MB_ICONINFORMATION);
}
void CUtilityTool::ErrorDialog(CString err,CWnd* pParent /*= NULL*/)
{
	HWND hParent = NULL;
	if (pParent == NULL)
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		hParent = pMainFrm->GetSafeHwnd();
	}
	else
	{
		hParent = pParent->GetSafeHwnd();
	}
	::MessageBox(hParent,err,_T("错误"),
		MB_OK|MB_ICONERROR);
}
void CUtilityTool::FailedInfoDialog(CString info,CWnd* pParent /*= NULL*/)
{
	HWND hParent = NULL;
	if (pParent == NULL)
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		hParent = pMainFrm->GetSafeHwnd();
	}
	else
	{
		hParent = pParent->GetSafeHwnd();
	}
	::MessageBox(hParent,info,_T("失败"),
		MB_OK|MB_ICONSTOP);
}
void CUtilityTool::SucceedInfoDialog(CString info,CWnd* pParent /*= NULL*/)
{
	HWND hParent = NULL;
	if (pParent == NULL)
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		hParent = pMainFrm->GetSafeHwnd();
	}
	else
	{
		hParent = pParent->GetSafeHwnd();
	}
	::MessageBox(hParent,info,_T("成功"),
		MB_OK|MB_ICONINFORMATION);
}
int CUtilityTool::OkCancelDialog(CString msg,CWnd* pParent /*= NULL*/)
{
	HWND hParent = NULL;
	if (pParent == NULL)
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		hParent = pMainFrm->GetSafeHwnd();
	}
	else
	{
		hParent = pParent->GetSafeHwnd();
	}
	return ::MessageBox(hParent,msg,_T("注意"),
		MB_OKCANCEL|MB_ICONQUESTION);
}
int CUtilityTool::YesNoDialog(CString msg,CWnd* pParent /*= NULL*/)
{
	HWND hParent = NULL;
	if (pParent == NULL)
	{
		CWnd* pMainFrm = theApp.GetMainWnd();
		hParent = pMainFrm->GetSafeHwnd();
	}
	else
	{
		hParent = pParent->GetSafeHwnd();
	}
	return ::MessageBox(hParent,msg,_T("注意"),
		MB_YESNO|MB_ICONQUESTION);
}

CString CUtilityTool::GetModuleDirPath()
{
	CString targetFilePath;
	::GetModuleFileName(NULL,targetFilePath.GetBuffer(),MAX_PATH);

	int pos = targetFilePath.ReverseFind('\\');
	targetFilePath = targetFilePath.Left(pos+1);
	return targetFilePath;
}

/* 基本字符串操作 */
void CUtilityTool::SplitString(CString str,CString separator,CStringList& resultList)
{
	resultList.RemoveAll();
	int pos = 0;
	int begin = 0;
	CString seg;
	while (-1 != (pos = str.Find(separator,begin)))
	{
		seg = str.Mid(begin,pos-begin);
		if (seg.IsEmpty() == FALSE)
		{
			resultList.AddTail(seg);
		}
		begin = pos + 1;
	}
	pos = str.GetLength();
	seg = str.Mid(begin,pos-begin);
	if (seg.IsEmpty() == FALSE)
	{
		resultList.AddTail(seg);
	}
}
CString CUtilityTool::JoinString(const CStringList& strList,CString separator)
{
	POSITION pos = strList.GetHeadPosition();
	CString result;
	if (pos == NULL)
		return result;
	CString firstSeg = strList.GetAt(pos);
	result = firstSeg;
	strList.GetNext(pos);
	while(pos != NULL)
	{	
		CString secondSeg = strList.GetAt(pos);
		result = JoinString(result,secondSeg,separator);
		strList.GetNext(pos);
	}
	return result;
}
CString CUtilityTool::JoinString(CString foreStr,CString backStr,CString separator)
{
	while (foreStr.Find(separator,foreStr.GetLength() - separator.GetLength()) 
		== foreStr.GetLength() - separator.GetLength())
	{
		foreStr.Delete(foreStr.GetLength() - separator.GetLength(),
			separator.GetLength());
	}
	while(backStr.Find(separator , 0) == 0)
	{
		backStr.Delete(separator.GetLength(),0);
	}
	return foreStr + separator + backStr;
}
void	CUtilityTool::SplitDirPath(CString dirPath,CStringList& pathList)
{
	CString separator;
	if (dirPath.Find('\\') != -1)
	{
		separator = _T("\\");
	}
	else if(dirPath.Find('/') != -1)
	{
		separator = _T("/");
	}
	else
	{
		separator = _T("\\");
	}
	SplitString(dirPath,separator,pathList);
}
CString CUtilityTool::JoinDirPath(CStringList& pathList)
{
	CString firstPath = pathList.GetHead();
	POSITION pos = pathList.GetHeadPosition();
	do
	{
		pathList.GetNext(pos);
		CString secondPath = pathList.GetAt(pos);
		firstPath = JoinDirPath(firstPath,secondPath);

	}while(pos != pathList.GetTailPosition());
	return firstPath;
}
CString CUtilityTool::JoinDirPath(CString dirPath,CString filePath)
{
	CString separator = _T("\\");
	if (dirPath.ReverseFind(_T('\\')) == dirPath.GetLength()-1)
	{
		dirPath.Delete(dirPath.GetLength()-1);
		separator = _T("\\");
	} 
	else if(dirPath.ReverseFind(_T('/')) == dirPath.GetLength()-1)
	{
		dirPath.Delete(dirPath.GetLength()-1);
		separator = _T("/");
	}
	if (filePath.Find(_T('\\')) == 0)
	{
		filePath.Delete(0);
		separator = _T("\\");
	} 
	else if(filePath.Find(_T('/')) == 0)
	{
		filePath.Delete(0);
		separator = _T("\\");
	}

	CString resultPath = JoinString(dirPath,filePath,separator);
	return resultPath;
}
CString CUtilityTool::GetMd5String( const CString str )
{
	char lstrPsw[512];
	memset( lstrPsw, 0, sizeof(lstrPsw));
	VERIFY(::WideCharToMultiByte(CP_ACP,0,str,-1,lstrPsw,sizeof(lstrPsw),NULL,NULL));
	char *pStr = MD5String(lstrPsw);
	CString ret ;
	ret = pStr;
	free(pStr);
	return ret;
}
BOOL	CUtilityTool::CreateDir(CString path)
{
	CStringList pathList;
	SplitDirPath(path,pathList);
	CString dirPath = pathList.GetHead();
	POSITION pos = pathList.GetHeadPosition();
	while(pos != pathList.GetTailPosition() && pos != NULL)
	{
		pathList.GetNext(pos);
		ASSERT(pos != NULL);
		CString newPath = pathList.GetAt(pos);
		dirPath = JoinDirPath(dirPath,newPath);
		if (!PathIsDirectory(dirPath))
		{
			if(!CreateDirectory(dirPath,NULL))
				return FALSE;
		}
	}
	return TRUE;
}
int CUtilityTool::GetDayOfWeek( const COleDateTime &date ) const
{
	CString temp;
	temp = date.Format( _T("%Y-%m-%d") );
	temp.Format(_T("%d-%d-%d"), date.GetYear(), date.GetMonth(), date.GetDay() );
	int weekDay = date.GetDayOfWeek() - 1;
	ASSERT( weekDay >= 0);
	if ( weekDay == 0 )
	{
		weekDay = 7;
	}
	return weekDay;
}
//判断浮点数是是否为0
long CUtilityTool::DoubleCmp(double d) const
{
	if(fabs(d) < Precision_1) return 0;
	return (d>0)?1:-1;
}
BOOL CUtilityTool::GetQuarterDateRange( int year, int quarter, COleDateTime &beginDate, COleDateTime &endDate ) const
{
	int beginMonth = 1, beginDay = 1, endMonth = 1, endDay = 1;
	if ( year <= 2000 )
	{
		return FALSE;
	}
	switch( quarter )
	{
	case 1:
		beginMonth = 1;
		beginDay = 1;
		endMonth = 3;
		endDay = 31;
		break;
	case 2:
		beginMonth = 4;
		beginDay = 1;
		endMonth = 6;
		endDay = 30;
		break;
	case 3:
		beginMonth = 7;
		beginDay = 1;
		endMonth = 9;
		endDay = 30;
		break;
	case 4:
		beginMonth = 10;
		beginDay = 1;
		endMonth = 12;
		endDay = 31;
		break;
	default:
		return FALSE;
	}
	beginDate.SetDateTime(year, beginMonth, beginDay, 0, 0, 0 );
	endDate.SetDateTime( year, endMonth, endDay, 0, 0, 0 );
	return TRUE;
}