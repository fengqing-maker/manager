#ifndef TRACE_D_H
#define TRACE_D_H

/************************************************************************
该文件中的宏定义用于调试程序时在“输出”窗口打印调试信息，
************************************************************************/
#ifdef DEBUG_TRACE		/* DEBUG版本才有效 */

#define TRACE_D0(str)\
	TRACE(str)
#define TRACE_D1(str,parm1)\
	TRACE(str,parm1)
#define TRACE_D2(str,parm1,parm2)\
	TRACE(str,parm1,parm2)

#else

#define TRACE_D0(str)
#define TRACE_D1(str,parm1)
#define TRACE_D2(str,parm1,parm2)

#endif

#endif
