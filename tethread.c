/**********************************************************************
  Module: ethread.c
  Author: Scott G Wang
  Date:   2006 Oct 28
  Purpose: create and destory a thread with error checking
  Modification History:
**********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "tethread.h"

void ethread_create(pthread_t *thread, const pthread_attr_t *attr, 
                    void (*start_routine)(void *), void *arg)
{
  if(pthread_create(thread,attr,(void *)start_routine,arg)){
    perror("pthread_create");
    exit(0);
  }
}

void ethread_exit(void *value_ptr)
{
  pthread_exit(value_ptr);
}
