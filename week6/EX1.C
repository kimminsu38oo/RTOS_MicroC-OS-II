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

	OS_EVENT *r_sem;
	OS_EVENT *s_sem;

	int send_array[4]; 
	char receive_array[4];

	OS_FLAG_GRP *r_grp;
	OS_FLAG_GRP *s_grp;

	INT8U select=1;  

	/*
	*********************************************************************************************************
	*                                         FUNCTION PROTOTYPES
	*********************************************************************************************************
	*/

	void Task(void * data);
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

	void main (void)
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

	static void  TaskStart (void *pdata)
	{
	#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
		OS_CPU_SR  cpu_sr;
	#endif
		INT16S     key;
		INT8U i;
		INT8U err;

		pdata = pdata;                                         /* Prevent compiler warning                 */

		TaskStartDispInit();                                   /* Setup the display                        */

		OS_ENTER_CRITICAL();                                   /* Install uC/OS-II's clock tick ISR        */
		PC_VectSet(0x08, OSTickISR);
		PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
		OS_EXIT_CRITICAL();

		OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

		if (select == 1) {
			r_sem = OSSemCreate(1);
			s_sem = OSSemCreate(1);

			s_grp = OSFlagCreate(0x00, &err);
			r_grp = OSFlagCreate(0x00, &err);
		}
		else if (select == 2) {
			
			
		}
	
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

	static  void  TaskStartDispInit (void)
	{
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
		PC_DispStr( 0,  0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
		PC_DispStr( 0,  1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  3, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0,  9, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 22, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 23, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
		PC_DispStr( 0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
	}

	/*$PAGE*/
	/*
	*********************************************************************************************************
	*                                           UPDATE THE DISPLAY
	*********************************************************************************************************
	*/

	static  void  TaskStartDisp (void)
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

	static  void  TaskStartCreateTasks (void)
	{
		INT8U i;

		for (i = 0; i < N_TASK; i++) {
			TaskData[i] = '0' + i;
			OSTaskCreate(Task, (void*)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
		}
	}

	void Task(void * pdata) {
		INT8U err;

		INT8U push_number;
		int get_number[4];
		
		INT8U i, j;

		INT8U min;		// Variable to store the minimum value
		INT8U min_task;	
		int task_number = (int)(*(char*)pdata-48); // index of each task (Since pdata is of char type, subtracting 48 (ASCII '0') converts it to an int)

		char push_letter; 
		char get_letter; 

		int fgnd_color, bgnd_color;

		char s[10];//

		// If pdata is 0-3, it's a random task, if it's 4, decision task.
		if (*(char*)pdata == '4') { // decision task
			for (;;) {
				if (select == 1) {
					OSFlagPend(s_grp, 0x0F, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
				}
				else if (select == 2) {
					
				} 
				//CS start
				OSSemPend(s_sem, 0, &err);
				min = send_array[0];
				min_task = 0;
				for (i = 1; i < N_TASK - 1; i++) {
					// Find the smallest number among the 4 random numbers in global variable(send_array[])
					if (send_array[i] < min) {
						min = send_array[i];
						min_task = i;
					}	
				}
				OSSemPost(s_sem);
				for (i = 0; i < N_TASK - 1; i++) {
					if (i == min_task) {
						push_letter = 'W';
					}
					else {
						push_letter = 'L';
					}

					if (select == 1) {
						//CS start
						OSSemPend(r_sem, 0, &err);
						receive_array[i] = push_letter;
						OSSemPost(r_sem);
						//CS END
						OSFlagPost(r_grp, 0x01 << i, OS_FLAG_SET, &err);
					}
					else if (select == 2) {
						
					}
				}
				OSTimeDlyHMSM(0, 0, 5, 0);
			}
		}
		else {
			for (;;) {
				push_number = random(64);
				sprintf(s, "%2d", push_number);

				PC_DispStr(0 + 18 * task_number, 4, "task", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				PC_DispChar(4 + 18 * task_number, 4, *(char*)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				PC_DispStr(6 + 18 * task_number, 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

				if (select == 1) {
					//CS start
					OSSemPend(s_sem, 0, &err);
					send_array[task_number] = push_number;
					OSSemPost(s_sem);
					//CS END

					/** flags
					 * task 0 - 0x01
					 * task 1 - 0x02
					 * task 2 - 0x04
					 * task 3 - 0x08
					 */
					OSFlagPost(s_grp, 0x01 << task_number, OS_FLAG_SET, &err);

					//wait task until decision task write the word 'W' or 'L' on receive_array and set eventFlag r_grp.
					OSFlagPend(r_grp, 0x01 << task_number, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

					//CS start
					OSSemPend(r_sem, 0, &err);
					get_letter = receive_array[task_number];
					OSSemPost(r_sem);
					//CS END

				}
				else if (select == 2) {
					
				}
				
				if (*(char*)pdata == '0') {
					bgnd_color = DISP_BGND_RED;
					fgnd_color = DISP_FGND_RED;
				}
				else if (*(char*)pdata == '1') {
					bgnd_color = DISP_BGND_CYAN;
					fgnd_color = DISP_FGND_CYAN;
				}
				else if (*(char*)pdata == '2') {
					bgnd_color = DISP_BGND_BLUE;
					fgnd_color = DISP_FGND_BLUE;
				}
				else if (*(char*)pdata == '3') {
					bgnd_color = DISP_BGND_GREEN;
					fgnd_color = DISP_FGND_GREEN;
				}
				PC_DispStr(8 + 18 * task_number, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				PC_DispStr(10 + 18 * task_number, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
				PC_DispChar(9 + 18 * task_number, 4, get_letter, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

				if (get_letter == 'W') {
					for (j = 5; j < 20; j++) {
						for (i = 0; i < 80; i++) {
							PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
						}
					}
				}
				OSTimeDlyHMSM(0, 0, 5, 0);
			}
		}
	}

