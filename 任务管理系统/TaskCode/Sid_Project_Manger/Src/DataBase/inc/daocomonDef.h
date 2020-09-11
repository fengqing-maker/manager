#ifndef ERROR_INFO_TYPE_H3423
#define ERROR_INFO_TYPE_H3423

#include "../../Model/TaskModel.h"
#include "../../Model/TypeDefHead.h"
#include "../../Model/User.h"
#include <map>
#include <list>

typedef std::map<int,int>             TabIndxeMap;			//First =      second=索引值
typedef std::map<int,int>::iterator   TabIndxeMap_Itor;
typedef std::map<int, int>            IntToIntMap;
//任务
typedef std::list<CString>            ListString;
typedef std::list<CTaskObj>           ListTask;


typedef ListTask::iterator         ListTaskItor;
typedef ListTask::const_iterator   ListTaskConstItor;

typedef std::list<CUser>          ListUser;
typedef ListUser::iterator        ListUserItor;

enum UserDaoErrorInfo
{
	userExcOk = 0,
	insertNameRepeat=-8001,
	excSqlError=-8002, //执行sql失败
	validateUserNone=-8003, //用户名不存在
	userPwdError=-8004,  //用户名错误
	notfind=-8006,  //未查到记录
	mapError=-8007, //映射错误，比如一个任务不能有多个子节点
	paraError=-8008, //参数出错
	e_isOnline = -8009, //用户已登录
	not_connect = -8010, //未连接
	connect_error = -8011, //打开数据源失败 

};
//更新任务字段
enum TaskFildEnum
{
	taskName = 1,
	taskStatus,
	taskFactStartTime,
	taskFactWorkHour,
	taskDiffParam,
	taskDec,
	task_review_date,  //任务发布时间
	taskFactEndTime,
	fild_task_plan_starttime,
	fild_task_plan_closetime,
	fild_task_plan_period, //计划工期
	fild_task_ac_starttime,
	fild_task_ac_closetime,
	fild_task_ac_period,
	fild_forecast_task_score,
	fild_task_score,
	fild_task_score_date,
	fild_task_ownerid,
	fild_task_timeType_id,
	fild_task_auditorid,
	//
	fild_promanager_comment,
	fild_depmanager_comment,
	fild_task_update_watch,
	fild_task_check_begin,
	file_task_attri,
	file_task_type, //
	file_task_project_id, //
	task_score_des,
};
enum CSheduleFildEnum
{
	fild_task_id,
	fild_schedule_des,
	fild_schedule_tim,
	fild_schedule_percent,
	fild_update_time,
};
enum CChangeFildEnum
{
	fild_change_item,
	fild_change_content,
	fild_change_time,
	fild_change_reason,
	fild_auditor_id,
	fild_general_manager_id,
	fild_change_bakId,
	fild_status,
	fild_is_impact_schdule,
	fild_task_type,
	fild_project_review,
	fild_change_reason_option,
};
enum CProjectFildEnum
{
	fild_project_name,
	fild_project_managerid,
	fild_line_id,
	fild_project_shortName,
	fild_prj_product_type,
};
enum CProductLineFildEnum
{
	fild_name,
	fild_mangerId,
	fild_line_bes,
};
enum CDepartFildEnum
{
	fild_dep_manager,
    fild_dep_des,
	fild_dep_name,
	fild_dep_bak_manger,
	fild_dep_bak_validate,
	fild_dep_top,
};
enum CUserFildNum
{
	fild_user_name,
	fild_user_login_name,
	fild_user_depid,
	fild_user_roleid,
	fild_user_status,
};
#endif