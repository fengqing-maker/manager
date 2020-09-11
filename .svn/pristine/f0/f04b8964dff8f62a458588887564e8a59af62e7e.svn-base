#ifndef POWER_MODLE_2345_H
#define POWER_MODLE_2345_H
#include <list>
/*
 此表必须与数据库的role表一一对应
*/

enum Power
{
	powerNewDepartTask = 1, //新建部门任务 
	powerNewPrjTask = 2, //新建项目任务
	taskAgree = 3,  //任务下发及进度点评
	taskScore = 4,  //任务评分
	changeLevelAgree1 =6, //任务变更一级评审
	changeLevelAgree2 = 7,   //任务变更二级评审
	projectReview = 8, //项目任务点评          
	projctTaskManger = 13, //项目任务管理，主要为索引功能
	departTaskManger = 14, //部门任务管理，主要为索引功能
	backgroundManger = 15,//后台管理
	allocatePrjTask = 16, //创建不是自己的任务
	allocateDepartTask = 17, //创建不是自己的任务
	allocateWorkHour = 18, //任意分配他人非任务工时
	LookUpAllPrj = 19, //查看所有项目任务
	ReviewProjectTaskFinish = 20, //项目任务完成审核
};

//typedef std::map<int, int> RolePowerMap;  //角色与权力映射
typedef std::list<int>  RolePowerList;

#endif