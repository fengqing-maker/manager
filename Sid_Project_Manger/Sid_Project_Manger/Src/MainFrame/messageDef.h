#ifndef MSSAGE_DEF_ID83974
#define MSSAGE_DEF_ID83974

////////////////////////////////////////////////////////////////////////////////
//����ʾ���߳�����ʾ��ʾ��Ϣ
//
#define WM_SHOWINFO_BASE  WM_USER+10
#define WM_SHOW_DLG       WM_SHOWINFO_BASE+1   //��ʾ�Ի���
#define WM_CLOSE_DLG      WM_SHOWINFO_BASE+2  //�رնԻ���

//��ʾ�ַ���
#define MSG_CONNECT_SEVER  1  //��ʾ���ӷ�����
#define MSG_INITE_LOADING  2  //��ʼ������
#define MSG_READ_TASK_DATA 3  //������������
#define MSG_READ_DATA      4  //���ڼ�������

#define MAINFRAME_MSG_BASE              (WM_USER + 50)
#define MAINFRAME_MSG_TASKID            (MAINFRAME_MSG_BASE + 1)
//���
#define WM_OUTPUT_BASE                        WM_USER+100
#define MSG_ADD_CNT_CHANGE_UPDATE            (WM_OUTPUT_BASE+1)    //�������������¸���ҳ��,lparam ����id, Rparam, ����
#define MSG_DECREASE_CNT_CHANGE_UPDATE       (WM_OUTPUT_BASE+2)    //�������������¸���ҳ��,lparam ����id, Rparam, ����
#define MSG_CONTENT_CHANGE_UPDAE             (WM_OUTPUT_BASE+3)   //�������ݸ��µ��¸���ҳ��,lparam ����id, Rparam, ����
#define MSG_SELECT_TASK_ITEM                 (WM_OUTPUT_BASE+4)   //ѡ������
#define MSG_OUTPUT_SEL_SYS                   (WM_OUTPUT_BASE+5)   //ѡ����ʾ�б���Ϣ��
#define MSG_WORKHOUR_CHECK                   (WM_OUTPUT_BASE+6)   //��ʱ���
#define MSG_UPDATE_TITLE_CNT                 (WM_OUTPUT_BASE+10)   //���±����¸���                   
//�������
#define WM_CHECK_BASE                        WM_USER+150
#define WM_CHECK_TASK_START                 (WM_CHECK_BASE+0) //��������仯��ʼ
#define WM_CHECK_TASK_EXCUTE                (WM_CHECK_BASE+1) //��������仯ִ��
#define WM_CHECK_TASK_FINISH                (WM_CHECK_BASE+2) //��������仯����
#define MSG_CLOSE_APP                       (WM_CHECK_BASE+3) //�ر�������  

#endif