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

/****TINY_MACRO_OS TASKS DECLARE**************************************************************************/
/* 将tiny macro os任务函数的识别名字放到这里，后续使用都是从这里使用。自己每次创建任务都需要在这里添加 */
enum
{
    OS_TASK_TEST1 = 0,              /* 替换成自己的任务函数识别文字，如使用OS_TASK_DEFAULT，则任务函数名为OS_TASK_DEFAULT_task */
    OS_TASK_TEST2,
    TINY_MACRO_OS_TASKS_MAX_NUM,    /* 定义使用的主任务数量，最大255个任务 */
};

/****TINY_MACRO_OS CONFIG*********************************************************************************/

#define TINY_MACRO_OS_TIME_t                        unsigned short    /* 定义时间计数变量的类型，根据最长延迟修改 */
#define TINY_MACRO_OS_LINE_t                        unsigned short    /* 定义任务切换记录变量的类型，根据最大函数占用行数修改 */
#define OS_SEM_t                                    signed short      /* 信号量类型声明，必须为signed类型 */
#define OS_SEC_TICKS                                1000              /* 定时器时钟更新频率，每秒钟多少个ticks */

/* 最大支持32位MCU，可以支持64位以上，将下面修改即可uint8_t->255，uint16_t->65535，uint32_t->4294967295 */
#define TINY_MACRO_OS_TIME_MAX                      (TINY_MACRO_OS_TIME_t)(0xffffffff) /* 最大时间计数值 */

#define TINY_MACRO_OS_LINE_MAX                      (TINY_MACRO_OS_LINE_t)(0xffffffff) /* 最大函数行数计数值 */


/****TINY_MACRO_OS FUNCTIONS*********************************************************************************/
/*
 *  调度器通过使用os_task_start和os_task_end的任务函数中不可以使用switch case，但是可以通过调用函数，在另一个非任务函数中使用switch case。
 *  一定注意，下面os的宏定义不能有任何的换行修改，必须是一整行！！！！！！
 */

/* 所有任务的时间变量值。*/
extern volatile TINY_MACRO_OS_TIME_t                os_task_timers[TINY_MACRO_OS_TASKS_MAX_NUM];

/* 所有任务的函数运行标记值。*/
extern volatile TINY_MACRO_OS_LINE_t                os_task_linenums[TINY_MACRO_OS_TASKS_MAX_NUM];

/* 任务函数声明 */
#define OS_TASK(NAME, ...)                          TINY_MACRO_OS_TIME_t (NAME##_task)(__VA_ARGS__)

/* 运行调用任务函数，应当在主程序的while(1)里使用 */
#define OS_RUN_TASK(NAME, ...)                      do{if(os_task_timers[(NAME)]==0U){os_task_timers[NAME]=(NAME##_task)(__VA_ARGS__);}}while(0)

/* High Priority高优先级运行任务。该任务返回的时间值不能一直为0，必须有等待时间让出使用权，不然下面的其他任务将无法继续运行。*/
#define OS_RUN_HPTASK(NAME, ...)                    {if(os_task_timers[(NAME)]==0U){os_task_timers[(NAME)]=(NAME##_task)(__VA_ARGS__);continue;}}

/* 运行任务，不管任务时间状态立刻运行。 */
#define OS_CALL_TASK(NAME, ...)                     do{os_task_timers[(NAME)]=(NAME##_task)(__VA_ARGS__);} while(0)

/* 函数任务系统调度开始定义 */
#define OS_TASK_START(NAME)                         switch(os_task_linenums[(NAME)]){case 0U:

/* 函数任务系统调度结束定义 */
#define OS_TASK_END(NAME)                           break;default:break;}os_task_linenums[(NAME)]=0U;return TINY_MACRO_OS_TIME_MAX

/* 调度开始之前，初始化所有任务，每个任务接下来都会被执行 */
#define OS_INIT_TASKS()                             do{unsigned char i;for(i=0U;i<TINY_MACRO_OS_TASKS_MAX_NUM;i++){os_task_timers[i]=0U;os_task_linenums[i]=0U;}}while(0)

/* 更新系统时间，该函数应该放入定时器中断函数中处理，或者软件定时器中也可 */
#define OS_UPDATE_TIMERS()                          do {unsigned char i;for(i=0U;i<TINY_MACRO_OS_TASKS_MAX_NUM;i++){if(os_task_timers[i]!=0U){if(os_task_timers[i]!=TINY_MACRO_OS_TIME_MAX){os_task_timers[i]--;}}}} while(0)

/*********************************** 以下函数只可以在自身的任务函数中调用 *************************************/
/* 停止并且再不运行运行当前任务，复位任务状态，下一次运行从头开始，只可以在本任务中使用。 */
#define OS_TASK_EXIT(NAME)                          do{os_task_linenums[(NAME)]=0U;}while(0);return TINY_MACRO_OS_TIME_MAX

/* 退出当前任务，并等待对应时间，保存当前运行位置，时间单位为中断里定义的系统最小时间。 */
#define OS_TASK_WAITX(NAME, TICKS)                  os_task_linenums[(NAME)]=(TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U;if(os_task_linenums[(NAME)]){return(TICKS);}break;case ((TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX)+1U:

/* 跳出当前任务，保存当前运行位置 */
#define OS_TASK_YIELD(NAME)                         OS_TASK_WAITX(NAME, 0)

/* 等待时间，不改变上一次函数跳转位置 */
#define OS_TASK_CWAITX(TICKS)                       return (TICKS)

/* 挂起当前任务 */
#define OS_TASK_SUSPEND(NAME)                       OS_TASK_WAITX(NAME, TINY_MACRO_OS_TIME_MAX)

/* 复位当前任务，在指定时间后开始下一次运行，并从头开始 */
#define OS_TASK_RESET(NAME, TICKS)                  do{os_task_linenums[(NAME)]=0;}while(0);return (TICKS)

/* 等待条件 C 满足再继续向下执行，查询频率为每ticks个时钟一次 */
#define OS_TASK_WAIT_UNTIL(NAME, C, TICKS)          do{OS_TASK_WAITX(NAME, TICKS); if(!(C)) return (TICKS);} while(0)

/* 等待条件 C 满足再继续向下执行，查询频率为每ticks个时钟一次，带有超时次数，默认超时次数最大255，可以通过更改下方_overtimes的定义改为unsigned short等增大次数 */
#define OS_TASK_WAIT_UNTILX(NAME, C, TICKS, TIMES)  do{static unsigned char _overtimes;_overtimes=(TIMES);OS_TASK_WAITX(NAME, TICKS);if(!(C)&&(_overtimes>0)){_overtimes--;return (TICKS);}} while(0)

/*************************************以下函数只可以在其他任务函数中调用，不可以在自身函数中调用*******************************/
/* 挂起另一个指定任务，不可挂起自身 */
#define OS_TASK_SUSPEND_ANOTHER(NAME)               do{os_task_timers[(NAME)]=TINY_MACRO_OS_TIME_MAX;}while(0)

/* 复位另一个任务，在下次运行时从头开始，可以和OS_CALL_TASK配合使用来立刻重新开始运行一个任务 */
#define OS_TASK_RESET_ANOTHER(NAME)                 do{os_task_linenums[(NAME)]=0;}while(0)

/* 在指定时间后重新启动另一个已经停止运行的任务*/
#define OS_TASK_RESTART_ANOTHER(NAME, TICKS)        do{if(os_task_timers[(NAME)]==TINY_MACRO_OS_TIME_MAX){os_task_timers[(NAME)]=(TICKS);}}while(0)

/* 更新另一个任务的时间，以便突发事件运行任务 */
#define OS_TASK_UPDATE_ANOTHER(NAME, TICKS)         do {os_task_timers[(NAME)]=(TICKS);}while(0)

/*************************************信号量*******************************************/
/* 等待信号量超时 */
#define OS_SEM_TIMEOUT                              (-1)

/* 初始化信号量 */
#define OS_INIT_SEM(SEM)                            (SEM)=0;

/* 等待信号量，查询频率为每ticks个时钟一次 */
#define OS_WAIT_SEM(NAME, SEM, TICKS)               do{(SEM)=1;OS_TASK_WAITX(NAME, TICKS);if((SEM)>0)return (TICKS);}while(0)

/* 等待信号量，并有超时次数，查询频率为每ticks个时钟一次，超时时间为ticks*TIMES，所以ticks最好不要为0 */
#define OS_WAIT_SEMX(NAME, SEM, TICKS, TIMES)       do{(SEM)=(TIMES)+1;OS_TASK_WAITX(NAME, TICKS); if((SEM)>1){(SEM)--;return (TICKS);}else if((SEM)!=0){(SEM)=OS_SEM_TIMEOUT;}}while(0)

/* 发送信号量 */
#define OS_SEND_SEM(SEM)                            do {(SEM)=0;} while(0)

/*************************************子任务*******************************************/
/* 在任务中调用的子任务，会先退出主任务，在下一次执行主任务时，直接执行子任务。在子任务结束之前不会继续主任务，即子任务占用了主任务的系统时间变量使用权。 */
#define OS_CALL_SUB(NAME, SUBNAME, ...)             do {os_task_linenums[(NAME)]=(TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U; if(os_task_linenums[(NAME)]) {return 0U;} break; case (TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U:{os_task_timers[(NAME)]=(SUBNAME##_task)(__VA_ARGS__); if(os_task_timers[(NAME)]!=TINY_MACRO_OS_TIME_MAX) {return os_task_timers[(NAME)];}}} while(0)

/* 在任务中调用的子任务并带有超时时间，在子任务结束之前不会继续主任务，即子任务占用了主任务的系统时间变量使用权，子任务的时间变量为子任务最大运行时间，即使子任务没有结束，达到超时时间也会退出，但是为最大时间值时不会退出，和OS_CALL_SUB效果一致。 */
#define OS_CALL_SUBX(NAME, TICKS, SUBNAME, ...)     do {os_task_timers[(SUBNAME)]=TICKS;os_task_linenums[(NAME)]=(TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U;if(os_task_linenums[(NAME)]){return 0U;} break; case (TINY_MACRO_OS_LINE_t)(__LINE__)%TINY_MACRO_OS_LINE_MAX+1U:if(os_task_timers[(SUBNAME)]>0){os_task_timers[(NAME)]=(SUBNAME##_task)(__VA_ARGS__);if(os_task_timers[(NAME)]!=TINY_MACRO_OS_TIME_MAX) {return os_task_timers[(NAME)];}}} while(0)

#endif /* _TINY_MACRO_OS_H_ */
