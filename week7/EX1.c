/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                          (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #2
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                              CONSTANTS
*********************************************************************************************************
*/

#define          TASK_STK_SIZE     512                /* Size of each task's stacks (# of WORDs)       */
#define N_TASK 5

/*
*********************************************************************************************************
*                                              VARIABLES
*********************************************************************************************************
*/

OS_STK TaskStartStk[TASK_STK_SIZE];
OS_STK TaskStk[N_TASK][TASK_STK_SIZE];
char TaskData[N_TASK];



OS_EVENT* colorMbox[1];
OS_EVENT* numberMbox[3];
OS_FLAG_GRP *grp;

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void Task(void * data);
static  void  TaskStart(void *data);                  /* Function prototypes of tasks                  */
static  void  TaskStart(void *data);                  /* Function prototypes of tasks                  */
static  void  TaskStartCreateTasks(void);
static  void  TaskStartDispInit(void);
static  void  TaskStartDisp(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                  MAIN
*********************************************************************************************************
*/

void main(void)
{
	OS_STK *ptos;
	OS_STK *pbos;
	INT32U  size;

	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);                        /* Clear the screen                         */

	OSInit();                                              /* Initialize uC/OS-II                      */

	PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
	PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

	OSTaskCreate(TaskStart, (void*)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

	OSStart();                                             /* Start multitasking                       */
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                               STARTUP TASK
*********************************************************************************************************
*/

static void  TaskStart(void *pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
	OS_CPU_SR  cpu_sr;
#endif
	INT16S key;
	INT8U i;
	INT8U err;

	pdata = pdata;                                         /* Prevent compiler warning                 */

	TaskStartDispInit();                                   /* Setup the display                        */

	OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
	PC_VectSet(0x08, OSTickISR);
	PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
	OS_EXIT_CRITICAL();

	OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

	
	
	
	// initialization for Mbox, eventflag ... (your code)
	for(i = 0; i < 3; i++) {
		numberMbox[i] = OSMboxCreate((void *)0); 
	}
	colorMbox[0] = OSMboxCreate((void *)0);

	grp = OSFlagCreate(0x00, &err);
	





	TaskStartCreateTasks();                                /* Create all other tasks                   */

	for (;;) {
		TaskStartDisp();                                   /* Update the display                       */

		if (PC_GetKey(&key)) {                             /* See if key has been pressed              */
			if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
				PC_DOSReturn();                            /* Yes, return to DOS                       */
			}
		}

		OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
		OSTimeDly(OS_TICKS_PER_SEC);                       /* Wait one second                          */
	}
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDispInit(void)
{
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
	PC_DispStr(0, 0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	PC_DispStr(0, 1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 3, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 9, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 22, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 23, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDisp(void)
{
	char   s[80];

	sprintf(s, "%5d", OSTaskCtr);                                  /* Display #tasks running               */
	PC_DispStr(18, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "%3d", OSCPUUsage);                                 /* Display CPU usage in %               */
	PC_DispStr(36, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "%5d", OSCtxSwCtr);                                 /* Display #context switches per second */
	PC_DispStr(18, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "V%4.2f", (float)OSVersion() * 0.01);               /* Display uC/OS-II's version number    */
	PC_DispStr(75, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	switch (_8087) {                                               /* Display whether FPU present          */
	case 0:
		PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 1:
		PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 2:
		PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 3:
		PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static  void  TaskStartCreateTasks(void)
{
	INT8U i;

	for (i = 0; i < N_TASK; i++) {
		TaskData[i] = '0' + i;
		OSTaskCreate(Task, (void*)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
	}
}

void Task(void * pdata) {
	int rand_num, num, maxnum, bgnd_color, fgnd_color;
	int task_num = (int)(*(char*)pdata - 48);
	INT8U err, i, j;
	OS_FLAGS flags[3] = {0x01, 0x02, 0x04}; //0000 0001, 0000 0010, 0000 0100, 0000 1000
	char color;
	char colors[3] = { 'R','B','G' };
	char s[10];

	if (*(char*)pdata == '4') { //s display task
		for (;;) {
			// pending for R B G (your code)
			color = *(char *)OSMboxPend(colorMbox[0], 0, &err);
			if (color == 'R')
				bgnd_color = DISP_BGND_RED;
			else if (color == 'B')
				bgnd_color = DISP_BGND_BLUE;
			else if (color == 'G')
				bgnd_color = DISP_BGND_GREEN;

			for (j = 5; j < 20; j++) {
				for (i = 0; i < 80; i++)
					PC_DispChar(i, j, ' ', bgnd_color);
			}
			OSTimeDlyHMSM(0, 0, 3, 0); 
			// post to all task (your code)
			OSFlagPost(grp, 0x0F, OS_FLAG_SET, &err);
		}
	}
	else if (*(char*)pdata == '3') { // calculate task
		for (;;) {
			maxnum = 0; 
			for (i = 0; i < 3; i++) {
				// pending for random number (your code)
				num = *(int *)OSMboxPend(numberMbox[i], 0, &err);
				if (num > maxnum) {
					maxnum = num;
					color = colors[i];
				}
				else if (num == maxnum) { 
					maxnum = 64;
					color = 'R';
				}
			}
			// post the R,B,G to display task and pending(your code)
			OSMboxPost(colorMbox[0], (void *)&color);
			OSFlagPend(grp, 0x08, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		}
	}
	else { //three random tasks
		for (;;) {
			rand_num = random(64);
			sprintf(s, "%2d",rand_num);
			PC_DispStr(0 + 18 * task_num, 4, "task", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(4 + 18 * task_num, 4, *(char*)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			

			PC_DispStr(5 + 18 * task_num, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(8 + 18 * task_num, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(6 + 18 * task_num, 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

			// post random number to caculate task and pending (your code)
			OSMboxPost(numberMbox[task_num], (void *)&rand_num);
			OSFlagPend(grp, flags[task_num], OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
		}
	}
}