/**********************************************************************
  Module: globals.h
  Author: Scott Wang 
  Date:   2006 Oct 28

  Purpose: some global variables for threaded omega
**********************************************************************/
#ifndef GLOBALS_H
#define GLOBALS_H

#include <pthread.h>
#include <semaphore.h>

/* defined for zombie status */
#define ALIVE 1
#define DEAD 0

/* the mutex to protect curses */
pthread_mutex_t curses_mutex;

/* the mutex to protect malloc */
pthread_mutex_t malloc_mutex;

/* the mutex to lock all the fields on the screen */
pthread_mutex_t g_adt_mutex;

/* if man die, set reset_flag */
volatile int reset_flag;

/* semaphore that used for master thread to wait
   need_to_reset is default 0. */
sem_t need_to_reset;

/* semaphore that waits all the objects on the screen 
   kill themself if man die */
sem_t num_of_objects;

/* if all the threads are killed in reset mode, the wakeup
   man */
sem_t reset_done;



#endif
