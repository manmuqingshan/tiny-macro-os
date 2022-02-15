/*
 * Copyright (c) 2022, smartmx - smartmx@qq.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * This file is part of the tiny-macro-os Library.
 * 
 */
#ifndef _TINY_MACRO_OS_H_
#define _TINY_MACRO_OS_H_

/****TINY_MACRO_OS CONFIG*********************************************************************************/
#define TINY_MACRO_OS_TASKS_NUM           3U                //����ʹ�õ����������������255������
#define TINY_MACRO_OS_TIME_t              unsigned short    //����ʱ��������������ͣ�������ӳ��޸�
#define TINY_MACRO_OS_LINE_t              unsigned short    //���������л���¼���������ͣ����������ռ�������޸�
#define os_SEM_t                          signed short      //�ź�����������������Ϊsigned����
#define os_SEC_TICKS                      1000              //��ʱ��ʱ�Ӹ���Ƶ�ʣ�ÿ���Ӷ��ٸ�ticks

/****TINY_MACRO_OS ����ϵͳ��ʼ*********************************************************************************/
/*
 *  ������ͨ��ʹ��os_task_start��os_task_end���������в�����ʹ��switch case�����ǿ���ͨ�����ú���������һ������������ʹ��switch case��
 *  һ��ע�⣬����os�ĺ궨�岻�����κεĻ����޸ģ�������һ���У�����������
 */

extern volatile TINY_MACRO_OS_TIME_t os_task_timers[TINY_MACRO_OS_TASKS_NUM];      //���������ʱ�����ֵ��

//���������������ȷ�ķ������ֵ��Ӧ�ý��ú��ֶ��޸ģ�uint8_t->255��uint16_t->65535��uint32_t->4294967295
#define TINY_MACRO_OS_TIME_MAX            (TINY_MACRO_OS_TIME_t)(-1) //���ʱ�����ֵ

#define TINY_MACRO_OS_LINE_MAX            (TINY_MACRO_OS_LINE_t)(-1) //�������������ֵ

#define os_task         TINY_MACRO_OS_TIME_t                              //�����������䶨��

#define os_task_boot()      static TINY_MACRO_OS_LINE_t os_task_lc=0U; //��������ʼ������os_task_boot��os_task_start֮�����ͨ�����������к�������������λ�Ȳ���

#define os_task_start()        switch(os_task_lc){case 0U:       //��������ʼ֮ǰ

#define os_task_end()          break; default:break;}  os_task_lc=0U;  return TINY_MACRO_OS_TIME_MAX     //�����������֮ǰ

#define os_task_Reset()        do{os_task_lc=0;}while(0); //�����ͷ��ʼִ��

#define os_task_WaitX(ticks)   os_task_lc=(TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U;  if(os_task_lc){return (ticks);}  break; case ((TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX)+1U:  //�ȴ�ʱ�� ��λʱ��Ϊ�ж��ﶨ���ϵͳ��Сʱ��

#define os_task_CWaitX(ticks)    return ticks   //�ȴ�ʱ�䣬���ı���һ�κ�����תλ��

#define os_task_Yield()      os_task_WaitX(0)      //������ǰ����

#define os_task_Exit()        do{os_task_lc=0U;}while(0); return TINY_MACRO_OS_TIME_MAX     //ֹͣ�����ٲ��������е�ǰ����

#define os_task_Suspend()      os_task_WaitX(TINY_MACRO_OS_TIME_MAX)       //����ǰ����

#define os_task_Suspend_Another(TaskName,TaskID)        do{os_task_timers[(TaskID)]=TINY_MACRO_OS_TIME_MAX;}while(0);     //������һ��ָ�����񣬲�����������

#define os_task_Restart_Now(TaskName,TaskID)    do {if(os_task_timers[(TaskID)]==TINY_MACRO_OS_TIME_MAX){os_task_timers[(TaskID)]=(TaskName)();} } while(0)     //��������һ���Ѿ�ֹͣ���е�����������������

#define os_task_Restart_Later(TaskName,TaskID,ticks) do {if(os_task_timers[(TaskID)]==TINY_MACRO_OS_TIME_MAX){os_task_timers[(TaskID)]=ticks;}} while(0) //��ָ��ʱ���������������

#define os_task_Restart(TaskName,TaskID)    os_task_Restart_Later(TaskName,TaskID,0)    //����һ��������ѯʱ��������

#define os_task_Ticks_Update(TaskName,TaskID,ticks) do {os_task_timers[(TaskID)]=ticks;} while(0)   //��������ʱ�䣬�Ա�ͻ���¼���������

#define os_task_WaitUntil(C,ticks) do{ os_task_WaitX(ticks); if(!(C)) return ticks;} while(0) //�ȴ����� C �����ټ�������ִ�У���ѯƵ��Ϊÿticks��ʱ��һ��

#define os_RunTask(TaskName,TaskID)  do {if (os_task_timers[(TaskID)]==0U){os_task_timers[(TaskID)]=(TaskName)(); } } while(0)   //�����������������while��1�������

#define os_RunHpTask(TaskName,TaskID) { if (os_task_timers[(TaskID)]==0U) {os_task_timers[(TaskID)]=(TaskName)(); continue;} }    //High Priority�����ȼ���������ֻ��һ��������Ϊ�˼���51��Ƭ������ʹ�ÿɱ�����������񷵻ص�ʱ��ֵ����һֱΪ0�������еȴ�ʱ���ó�ʹ��Ȩ����Ȼ��������������޷��������С�

#define os_RunTaskWithParam(TaskName,TaskID,param)  do {if (os_task_timers[(TaskID)]==0U){os_task_timers[(TaskID)]=(TaskName)(param); } } while(0)   //�����������������������while��1�������

#define os_RunHpTaskWithParam(TaskName,TaskID,param) { if (os_task_timers[(TaskID)]==0U) {os_task_timers[(TaskID)]=(TaskName)(param); continue;} }    //High Priority�����ȼ��������񣬴�һ������

#define os_CallTask(TaskName,TaskID) do {os_task_timers[(TaskID)]=(TaskName)();} while(0) //�������񣬲�������ʱ��״̬�������С�

#define os_CallTaskWithParam(TaskName,TaskID,param)  do {os_task_timers[(TaskID)]=(TaskName)(param);} while(0)   //�������������񣬲�������ʱ��״̬�������С�

#define os_CallSub(SubTaskName) do { os_task_lc=(TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U; if(os_task_lc) {return 0U;} break; case (TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U:{ TINY_MACRO_OS_TIME_t subtasktime; subtasktime=(SubTaskName)(); if(subtasktime!=TINY_MACRO_OS_TIME_MAX) {return subtasktime;}}} while(0) //�������е��õ������������������֮ǰ������������񣬼�������ռ�����������ϵͳʱ�����ʹ��Ȩ

#define os_CallSubWithParam(SubTaskName, param) do { os_task_lc=(TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U; if(os_task_lc) {return 0U;} break; case (TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U: {TINY_MACRO_OS_TIME_t subtasktime; subtasktime=(SubTaskName)(param); if(subtasktime!=TINY_MACRO_OS_TIME_MAX) {return subtasktime;}}} while(0) //�������е��õ������񣨴�������

#define os_InitTasks() do {unsigned char i; for(i=0U;i<TINY_MACRO_OS_TASKS_NUM ;i++) {os_task_timers[i]=0U;} } while(0) //��ʼ������ÿ��������������ᱻִ��

#define os_UpdateTimers() do {unsigned char i; for(i=0U;i<TINY_MACRO_OS_TASKS_NUM ;i++){  if(os_task_timers[i]!=0U){  if(os_task_timers[i]!=TINY_MACRO_OS_TIME_MAX) { os_task_timers[i]--;}}}} while(0) //����ϵͳʱ�䣬�ú���Ӧ�÷��붨ʱ���ж��д���

#define os_TIMEOUT      (-1) //��ʱ

#define os_InitSem(sem) (sem)=0;  //��ʼ���ź���

#define os_WaitSem(sem,ticks) do{ (sem)=1; os_task_WaitX(ticks); if ((sem)>0) return ticks;} while(0) //�ȴ��ź�������ѯƵ��Ϊÿticks��ʱ��һ��

#define os_WaitSemX(sem,overtimes,ticks)  do { (sem)=(overtimes)+1; os_task_WaitX(ticks); if((sem)>1){ (sem)--;  return ticks;} else{(sem)=os_TIMEOUT;} } while(0)  //�ȴ��ź��������г�ʱ��������ѯƵ��Ϊÿticks��ʱ��һ�Σ���ʱʱ��Ϊticks*overtimes������ticks��ò�ҪΪ0

#define os_SendSem(sem)  do {(sem)=0;} while(0) //�����ź���

#endif /* _TINY_MACRO_OS_H_ */
