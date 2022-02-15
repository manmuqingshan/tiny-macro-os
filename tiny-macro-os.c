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
#include "tiny-macro-os.h"

volatile TINY_MACRO_OS_TIME_t os_task_timers[TINY_MACRO_OS_TASKS_NUM];      //���������ʱ�����ֵ��

/*
 * �õ�����˼������protothread��ͨ������������γɸ�os����ʵ�������������״̬����
 * �ο��˰�Ī��̳https://www.amobbs.com/thread-5508720-2-1.html�е�smset�����СС��������
 * Ŀ��ֻΪ���Ⱦ��򣬲��ҿ�Ϊ51���ã����Բ����������������ȫ���ֶ���
 */
/*
 * ��ͨ�����д��ʽ
os_task os_task_test1(void){
  os_task_boot();
  os_task_start();
  static uint8_t i = 0;//�����ж���Ļ��������л�ǰ��ʹ�õľֲ�������Ҫʹ��static���壬��Ȼ�����ᶪʧ
  //��ֹ��os_task_start��os_task_end��ʹ��switch��return;
  while(1){
    printf("os test1\n");
    os_task_WaitX(1000);
  }
  os_task_end();
}
*/

/*
 * �����������д��ʽ
os_task os_task_test2(uint8_t params){
  os_task_boot();
  //os_task_boot��os_task_start֮��Ĵ��룬ÿ��ִ�����񶼻����У������ڴ���������λ�ȹ����˳��������ڵȴ��е�С����
  if(params){
      os_task_Reset();
  }
  os_task_start();
  //��ֹʹ��switch
  while(1){
    printf("os test2:%d\n",params);
    os_task_WaitX(600);
  }
  os_task_end();
}
*/


/*
 * ���е���������Ҫ�ֶ���main������while(1)�е���
void main()
{
  os_InitTasks();
  while(1){
    os_RunTask(os_task_test1,0);
    os_RunTaskWithParam(os_task_test2,1,1);
//    os_RunHpTask(os_task_test1,0);
//    os_RunHpTaskWithParam(os_task_test2,1,1);
  }
}
*/

/*
 * �ڶ�ʱ���ж��и���ϵͳ����ʱ��timer
void SysTick_Handler(void)
{
  os_UpdateTimers();
}
*/

/*
 * �ź�������
os_SEM_t test;

os_task os_sem_test(void){
  os_task_boot();
  os_task_start();
  os_InitSem(test);
  //��ֹʹ��switch
  while(1){
    os_WaitSem(test,0);
    printf("uart rec\n");
    os_WaitSemX(test,10,1);//������������ò�ҪΪ0����Ȼ�޷�����ʱ�䣬��ɿ�����������м����ˡ�
    if(test == os_TIMEOUT){
      printf("uart timeout\n");
    } else {
      printf("uart rec\n");
    }
  }
  os_task_end();
}

void UART0_IRQHandler()
{
  if(RX){
    os_SendSem(test);
  }
}
*/

/*
 * os_CallSub��������������
os_task os_child_task(void){
  os_task_boot();
  os_task_start();
  //��ֹʹ��switch
  os_task_WaitX(500);
  printf("os_child_task 1\n");
  os_task_WaitX(500);
  printf("os_child_task 2\n");
  os_task_WaitX(500);
  printf("os_child_task 3\n");
  os_task_WaitX(500);
  os_task_end();
}

os_task os_father_task(void){
  os_task_boot();
  os_task_start();
  //��ֹʹ��switch
  while(1){
    printf("os_father_task\n");
    os_CallSub(os_child_task);
  }
  os_task_end();
}
*/



