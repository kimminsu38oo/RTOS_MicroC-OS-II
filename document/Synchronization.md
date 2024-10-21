# Synchronization

# Mailbox

Mailboxes are typically used for sending single pieces of data.
To create a mailbox using OSMboxCreate, you pass a NULL pointer.
When you send data with OSMboxPost(), you pass the address of the data. This updates the mailbox to point to the new data. Any task that was waiting for the mailbox then wakes up and can access this new data.

```c
OS_EVENT *mbox;

mbox = OSMboxCreate((void *)0)
INT8U err;
// task1 is waiting "hello" from task2
task 1{
	int *r;
	r = (int *)OSMboxPend(mbox, 0, &err);
	printf("%d", *r);
}
task 2{
	int r;
	r = *(int *)OSMboxPend(mbox, 0, &err);
	printf("%d", r);
}

task 3{
	int b = 3;
	//OSMboxPost(mbox, (void *)&b);
	OSMboxPostOpt(mbox, (void *)&b, OS_POST_OPT_BROADCAST);
}
```

# Message Queue

Message queues are used for transmitting multiple items.

```c
void* messageQ[20];
OS_EVENT *msgQueue;
msgQueue = OSQCreate(&messageQ[0], 20);

task 1 {
	INT8U err;
	char *str;
	str = (char *)OSQPend(msgQueue, 0, &err);
	printf("%s", str);
}

task 2 {
	char msg[20];
	strcpy(&msg, "hello");
	OSQPost(msgQueue, (void *)&msg[0]);
	//OSQPostOpt(msgQueue, (void *)&msg[0], OS_POST_OPT_BROADCAST);
	
}
```

# Semaphore

Semaphores can be used in two ways:

1. Synchronization of shared memory
2. Synchronization between tasks

```c
OS_EVENT *sem;

sem = OSSemCreate(1);
int a;

task 1 {
	INT8U err;
	OSSemPend(sem, 0, &err);
	a++; //critical section
	OSSemPost(sem);
}
```

```c
// priority :  task 1 > task 2 but,
// order : task 2 -> task 1
OS_EVENT *sem;
sem = OSSemCreate(0);
INT8U err;
task 1 {
	OSSemPend(sem, 0, &err);
	
	//routine ...
}

task 2 {

	//routine ...
	
	OSSemPost(sem);
}

```

# Event Flag

Event flags can effectively synchronize multiple tasks.
Unlike others that use ECB, this uses OS_FLAG_GRP.

Event flags have two data structures. 

1. Event flag group structure
2. Event flag Node structure.

The Event flag group is a bitmap data structure that contains information about multiple event flags.
The Event flag Node structure manages each task waiting for an Event flag.
When specific conditions are met, tasks that called OSFlagPend become ready.

```c
// priority : task 1 > task 2 > task 3
// execution : task 2 -> task 3 -> task 1

EVENT_FLAG_GRP *e_grp;
INT8U err;

e_grp = OSFlagCreate(0x00, &err);

task 1 {
	OSFlagPend(e_grp, 0x03, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
}

task 2 {
	OSFlagPost(e_grp, 0x02, OS_FLAG_SET, &err); 
}

task 3 {
	OSFlagPost(e_grp, 0x01, OS_FLAG_SET, &err);
}
```

# Mutex

```c
OS_EVENT *mutex
INT8U err;
mutex = OSMutexCreate(3, &err); //3 is pip num
int a;

task 4 {
	OSMutexPend(mutex, 0, &err);
	a++; //critical section
	OSMutexPost(mutex);
}
```

# Priority inversion

Priority inversion occurs when a high-priority task is blocked by a lock on shared memory held by a low-priority task. While the low-priority task is executing, if a medium-priority task preempts it, this causes a delay in the execution of the high-priority task.

## There is two ways to solve Priority inversion.

### PIP(priority inheritence protocol)

When a high priority task is blocked by a semaphore of a low-priority task, the low-priority task inherits the priority of the high-priority task. 
When a low-priority task releases the lock on a semaphore, it returns to its original priority.

However, PIP (Priority Inheritance Protocol) potentially has two dangerous situations.
One is Deadlock, and the other is Chained blocking.

Deadlock can occur in nested critical sections, and Chained blocking can occur when a high-priority task is blocked multiple times in succession. Both of these situations can result in missing deadlines.

To address these issues, here’s PCP 

### PCP(priority ceiling protocol)

PCP has two concepts:

1. Priority ceiling
2. System ceiling

Priority ceiling is per semaphore and is calculated before the program starts. Priority ceiling is defined as the priority of the highest priority task among those accessing that semaphore.

System ceiling exists only one per system and is calculated at the moment of accessing a semaphore. System ceiling is defined as the maximum value of priority ceilings of all semaphores currently locked by other tasks.

Here's the pseudocode for PCP.
Situation where a specific task is trying to access a semaphore:

```nasm
if (priority > S.C | S.C X)
	Enter C.S
else
	block
```

µC/OS-IIs uses PIP. 
However, the programmer must specify the pip number.