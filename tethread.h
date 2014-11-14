/**********************************************************************
  Module: ethread.h
  Author: Scott G Wang
  Date:   2006 Oct 28
  Purpose: create and destory a thread with error checking
  Modification History:
**********************************************************************/

#ifndef ETHREAD_H
#define ETHREAD_H

#include <pthread.h>

extern void ethread_create(pthread_t *thread, const pthread_attr_t *attr, 
                           void (*start_routine)(void *), void *arg);

extern void ethread_exit(void *value_ptr);
 
#endif
