# Runnable

Arduino simple cooperative multitask library

*&copy;2017, Alexander Emelianov (a.m.emelianov@gmail.com)*

### 1. Usage

Simple task:

```c
#define RUN_TASKS 8
#include <Runnable.h>
class MyClass : public Runnable {
 uint32_t run() {
  //DO SOMETHING
  return delayToNextRun_OR_0_ToRemoveTask
 }
}
MyClass myRun();
void setup() {
  myRun.runNow();
//or
  myRun.runWithDelay(firstRunIn_value_miliseconds);
}
void loop() {
  runnableLoop();
}
```

Semaphore task:

```c
#define RUN_TASKS 8
#include <Runnable.h>
class MyClass : public Runnable {
  uint32_t run() {
    //DO SOMETHING
   return RUN_NEVER;	//Delay To NextRun, RUN_DELETE To Remove Task or RUN_NEVER to run on semaphore
  }
}
uint16_t semaphore = 0;
MyRun myRun();
void setup() {
  myRun.runWithSemaphore(&semaphore);	//first runs as semaphore become non-zerro
}
void loop() {
  runnableLoop();
}
```

### 2. Reference
`#define RUN_TASKS 16`

Defines maximum task count.

`void runnableLoop()`

Execute all tasks assigned to schedule. Should be run in loop().

Maximum count of tasks to schedulle.

#### 2.1 Methods

`bool runWithDelay(uint32_t delay, uint16_t* signal = NULL)`

Add task with callback function thread and first run in delay milliseconds or when signal become true.

Returns true on success.

`bool runNow()`

Add task to run ASAP.

Returns true on success.

`bool runWithSemaphore(uint16_t* signal)`

Add task to run when signal become not 0.

Returns true on success.

`bool stopNow()`

Remove task from schedule.

Returns false if task not found.

`bool isRunning()`

Return true if task exists in schedule.

Returns false if task not found.

`uint32_t remaider()`

Return time to next run of task.

Returns RUN_NOW  if timeout is expired.

Returns RUN_DELETE if task not exists.

Returns RUN_NEVER for task that will never run by schedule run. But can run by semaphore.

### 3. Changes history
* 0.1

  * Initial release
