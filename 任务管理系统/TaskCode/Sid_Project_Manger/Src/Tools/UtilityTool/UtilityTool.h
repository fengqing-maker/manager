#pragma once
#include "../SingletonTemplate/SingletonTemplate.h"


/** @brief 用于CIPAddressCtrl控件 */
typedef struct ipaddress		
{
	BYTE field0;		/** @brief bit24-bit31 */
	BYTE field1;		/** @brief bit16-bit23 */
	BYTE field2;		/** @brief bit8-bit15 */
	BYTE field3;		/** @brief bit0-bit7 */
	ipaddress()
	{
		field0 = 0;field1 = 0; field2 = 0;field3 = 0;
	}
}IPAddress;
#define Precision_1 (1e-6)
#define Precision_2 (1e-8)

class CUtilityTool : public SingletonS<CUtilityTool>
{
	friend class SingletonS<CUtilityTool>;
public:

	/** @brief 从字符串转换为整型 */
	BOOL ConvertStringToInt(CString& str,int& integer);
	/** @brief 从整型转换为字符串 */
	BOOL ConvertIntToString(int& integer,CString& str);

	/** @brief 从字符串转换为双精度浮点型 */
	BOOL ConvertStringToDouble(CString& str,double& dValue);
	/** @brief 从双精度浮点型转换为字符串 */
	BOOL ConvertDoubleToString(double& dValue,CString& str);

	/** @brief 从字符串转换为IPAddress */
	BOOL ConvertStringToIPAddress(CString& strAddress,IPAddress& ipAddress);
	/** @brief 从IPAddress转换为字符串 */
	BOOL ConvertIPAddressToString(IPAddress& ipAddress,CString& strAddress);

	/* 文件路径操作 */
	 BOOL			CreateDir(CString path);						/* 创建文件夹 */

	/* 弹出消息对话框 */
	void MessageDialog(CString msg,CWnd* pParent = NULL);			/* 自定义为模态对话框 */
	void ErrorDialog(CString err,CWnd* pParent = NULL);				/* 自定义为模态对话框 */
	void FailedInfoDialog(CString info,CWnd* pParent = NULL);		/* 自定义为模态对话框 */
	void SucceedInfoDialog(CString info,CWnd* pParent = NULL);		/* 自定义为模态对话框 */
	int OkCancelDialog(CString msg,CWnd* pParent = NULL);			/* 自定义为模态对话框 */
	int YesNoDialog(CString msg,CWnd* pParent = NULL);				/* 自定义为模态对话框 */


	CString		GetModuleDirPath();									/* 获取exe文件所在目录 */

	/* 基本字符串操作 */
	void	SplitString(CString str,CString separator,CStringList& resultList);
	CString	JoinString(const CStringList& strList,CString separator);
	CString	JoinString(CString foreStr,CString backStr,CString separator);
	void	SplitDirPath(CString dirPath,CStringList& pathList);
	CString	JoinDirPath(CStringList& pathList);
	CString	JoinDirPath(CString dirPath,CString filePath);

	CString GetMd5String( const CString str );
	int     GetDayOfWeek( const COleDateTime &date ) const;
	long    DoubleCmp(double d) const;

	BOOL GetQuarterDateRange( int year, int quarter, COleDateTime &beginDate, COleDateTime &endDate ) const;
protected:
	CUtilityTool(void);
	~CUtilityTool(void);


private:



};

