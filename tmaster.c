#include "tglobals.h"
#include "tobject_count.h"
#include "tg_adt.h"
#include "tethread.h"

#define PSHARED 0
#define RESET_FLAG 0
#define RESET_DONE 0
#define OBJECT_NUM 0

static pthread_t master_thread;

void master_reset()
{
  while(1){
    /* wait until man die */
    sem_wait(&need_to_reset);

    /* get object number, and wait each of 
       object on screen exits its thread */
    int i = object_count_get();
    while(i > 0){
      sem_wait(&num_of_objects);
      i--;
    }

    /* initilize the game */
    g_adt_init();
    object_count_init();
    move_zombie_args_create();

    /* signal man, reset is done */
    sem_post(&reset_done);
    reset_flag = 0;
  }
}  
  
void master_create()
{
  reset_flag = 0;
  sem_init(&need_to_reset,PSHARED,RESET_FLAG);
  sem_init(&reset_done,PSHARED,RESET_DONE);
  sem_init(&num_of_objects,PSHARED,OBJECT_NUM);
  ethread_create(&master_thread,NULL,master_reset,NULL);
}

