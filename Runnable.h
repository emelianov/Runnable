/////////////////////////////////////////////////////////
// Runable 0.1 (Run 2.0)
// Arduino simple cooperative multitask library
// (c)2017, Alexander Emelianov (a.m.emelianov@gmail.com)
//

#pragma once

#define RUN_DELETE	0
#define RUN_NEVER	0xFFFFFFFF
#define RUN_NOW		1
#ifndef RUN_TASKS
 #define RUN_TASKS  16
#endif

class Runnable;

uint8_t runnableCount = 0;
Runnable* runnableTasks[RUN_TASKS];

class Runnable {
friend void runnableLoop();
private:
 uint32_t lastRun;
 uint32_t delay;
 uint16_t* signal;
 virtual uint32_t run();
public:
 bool runWithDelay(uint32_t d, uint16_t* s = NULL) {
   if (runnableCount < RUN_TASKS) {
     lastRun  = 0;
     delay    = d;
     signal   = s;
     runnableTasks[runnableCount] = this;
     runnableCount++;
     return true;
   }
   return false;
 }
 bool runWithSemaphore(uint16_t* signal) {
   return runWithDelay(RUN_NEVER, signal);
 }
 bool runNow() {
   return runWithDelay(RUN_NOW);
 }
 bool stopNow() {
  for (uint8_t i = 0; i < runnableCount; i++) {
    if (runnableTasks[i] == this) {
      runnableTasks[i]->delay = RUN_DELETE;
      return true;
    }
  }
  return false;
 }
 bool isRunning() {
  for (uint8_t i = 0; i < runnableCount; i++) {
    if (runnableTasks[i] == this && runnableTasks[i]->delay != RUN_DELETE) {
      return true;
    }
  }
  return false;
 }
 uint32_t remainder() {
  for (uint8_t i = 0; i < runnableCount; i++) {
    if (runnableTasks[i] == this) {
    	if (runnableTasks[i]->delay == RUN_DELETE) return RUN_DELETE;
    	if (runnableTasks[i]->delay == RUN_NEVER) return RUN_NEVER;
    	uint32_t t = millis() - runnableTasks[i]->lastRun;
    	if (t > runnableTasks[i]->delay) return RUN_NOW;
    	return runnableTasks[i]->delay - t;
    }
  }
  return RUN_DELETE;
 }
};

uint16_t runnableSignal(uint16_t* signal) {
  return *signal++;
}

void runnableLoop() {
  uint8_t i, j;
  for(i = 0; i < runnableCount; i++) {
    if (runnableTasks[i]->delay != 0) {
      if (runnableTasks[i]->signal != NULL && *runnableTasks[i]->signal > 0) {
        for (j = 0; j < runnableCount; j++) {
          if (runnableTasks[j]->signal == runnableTasks[i]->signal) {
            runnableTasks[j]->lastRun = millis();
            runnableTasks[j]->delay = runnableTasks[j]->run();
          }
        }
        if (*runnableTasks[i]->signal > 0) {
        	*runnableTasks[i]->signal = *runnableTasks[i]->signal - 1;
        }
      }
      if (millis() - runnableTasks[i]->lastRun > runnableTasks[i]->delay) {
        runnableTasks[i]->lastRun = millis();
        runnableTasks[i]->delay = runnableTasks[i]->run();
      }
    }
  }
  for(i = 0; i < runnableCount; i++) {
   if (runnableTasks[i]->delay == RUN_DELETE) {
     memcpy(&runnableTasks[i], &runnableTasks[i + 1], (runnableCount - i - 1) * sizeof(Runnable*));
     runnableCount--;
     i--;
   }
  }
}