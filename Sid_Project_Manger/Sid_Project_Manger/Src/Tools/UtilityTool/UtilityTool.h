#pragma once
#include "../SingletonTemplate/SingletonTemplate.h"


/** @brief ����CIPAddressCtrl�ؼ� */
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

	/** @brief ���ַ���ת��Ϊ���� */
	BOOL ConvertStringToInt(CString& str,int& integer);
	/** @brief ������ת��Ϊ�ַ��� */
	BOOL ConvertIntToString(int& integer,CString& str);

	/** @brief ���ַ���ת��Ϊ˫���ȸ����� */
	BOOL ConvertStringToDouble(CString& str,double& dValue);
	/** @brief ��˫���ȸ�����ת��Ϊ�ַ��� */
	BOOL ConvertDoubleToString(double& dValue,CString& str);

	/** @brief ���ַ���ת��ΪIPAddress */
	BOOL ConvertStringToIPAddress(CString& strAddress,IPAddress& ipAddress);
	/** @brief ��IPAddressת��Ϊ�ַ��� */
	BOOL ConvertIPAddressToString(IPAddress& ipAddress,CString& strAddress);

	/* �ļ�·������ */
	 BOOL			CreateDir(CString path);						/* �����ļ��� */

	/* ������Ϣ�Ի��� */
	void MessageDialog(CString msg,CWnd* pParent = NULL);			/* �Զ���Ϊģ̬�Ի��� */
	void ErrorDialog(CString err,CWnd* pParent = NULL);				/* �Զ���Ϊģ̬�Ի��� */
	void FailedInfoDialog(CString info,CWnd* pParent = NULL);		/* �Զ���Ϊģ̬�Ի��� */
	void SucceedInfoDialog(CString info,CWnd* pParent = NULL);		/* �Զ���Ϊģ̬�Ի��� */
	int OkCancelDialog(CString msg,CWnd* pParent = NULL);			/* �Զ���Ϊģ̬�Ի��� */
	int YesNoDialog(CString msg,CWnd* pParent = NULL);				/* �Զ���Ϊģ̬�Ի��� */


	CString		GetModuleDirPath();									/* ��ȡexe�ļ�����Ŀ¼ */

	/* �����ַ������� */
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

