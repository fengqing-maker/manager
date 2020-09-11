#ifndef MSSAGE_DEF_ID83974
#define MSSAGE_DEF_ID83974

////////////////////////////////////////////////////////////////////////////////
//在提示框线程中显示提示信息
//
#define WM_SHOWINFO_BASE  WM_USER+10
#define WM_SHOW_DLG       WM_SHOWINFO_BASE+1   //显示对话框
#define WM_CLOSE_DLG      WM_SHOWINFO_BASE+2  //关闭对话款

//显示字符串
#define MSG_CONNECT_SEVER  1  //显示连接服务器
#define MSG_INITE_LOADING  2  //初始化加载
#define MSG_READ_TASK_DATA 3  //加载任务数据
#define MSG_READ_DATA      4  //正在加载数据

#define MAINFRAME_MSG_BASE              (WM_USER + 50)
#define MAINFRAME_MSG_TASKID            (MAINFRAME_MSG_BASE + 1)
//输出
#define WM_OUTPUT_BASE                        WM_USER+100
#define MSG_ADD_CNT_CHANGE_UPDATE            (WM_OUTPUT_BASE+1)    //数据量增减导致更新页面,lparam 任务id, Rparam, 类型
#define MSG_DECREASE_CNT_CHANGE_UPDATE       (WM_OUTPUT_BASE+2)    //数据量增减导致更新页面,lparam 任务id, Rparam, 类型
#define MSG_CONTENT_CHANGE_UPDAE             (WM_OUTPUT_BASE+3)   //数据内容更新导致更新页面,lparam 任务id, Rparam, 类型
#define MSG_SELECT_TASK_ITEM                 (WM_OUTPUT_BASE+4)   //选中任务
#define MSG_OUTPUT_SEL_SYS                   (WM_OUTPUT_BASE+5)   //选择显示列表信息框
#define MSG_WORKHOUR_CHECK                   (WM_OUTPUT_BASE+6)   //工时检查
#define MSG_UPDATE_TITLE_CNT                 (WM_OUTPUT_BASE+10)   //更新标题新个数                   
//任务监视
#define WM_CHECK_BASE                        WM_USER+150
#define WM_CHECK_TASK_START                 (WM_CHECK_BASE+0) //监视任务变化开始
#define WM_CHECK_TASK_EXCUTE                (WM_CHECK_BASE+1) //监视任务变化执行
#define WM_CHECK_TASK_FINISH                (WM_CHECK_BASE+2) //监视任务变化结束
#define MSG_CLOSE_APP                       (WM_CHECK_BASE+3) //关闭主程序  

#endif