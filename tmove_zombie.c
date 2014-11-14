/**********************************************************************
  Module: move_zombie.c
  Author: Scott Wang
  Date:   2006 Sep 30

  Purpose: To make zombie move in the given field
**********************************************************************/

#include <curses.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#include "ethread.h"
#include "globals.h"
#include "move_zombie.h"
#include "image.h"
#include "screen.h"
#include "eheap.h"
#include "field.h"
#include "g_adt.h"
#include "dir.h"
#include "move_bullet.h"
#include "object_count.h"

/* define the status for zombies */
#define NOT_INIT -1

/* define the random rate for randomly
   changing zombies direction */
#define RANDOM_RATE 15

/* define value for zombie to shoot */
#define SHOOT_ENABLE 1 
#define SHOT_RATE 10

typedef struct zombie_args {
  int row;
  int col;
  dir_t dir;
  int status;
  pthread_t thrds;
}*Zombie_args;

/* array of zombies */
volatile Zombie_args zombies[ZOMBIE_NUMBER];

/* The number counts alive zombies */
volatile int zombie_count = ZOMBIE_NUMBER;

pthread_mutex_t zombie_count_mutex = PTHREAD_MUTEX_INITIALIZER;

/* Initialize zombies' arguments*/
static void move_zombie_init_args(Zombie_args z);

/* draw and clear zombies' image */
static void move_zombie_draw_image(Zombie_args z);
static void move_zombie_clear_image(Zombie_args z);

/* once zombie is showed up on the screen once, then 
   put the zombie in regularly move */
static void move_zombie_regularly(Zombie_args z);

/* deal with zombies when they first show on the screen */
static void move_zombie_first_move(Zombie_args z);

/* update zombies' status to either ALIVE or DEAD */
static void move_zombie_update_status(Zombie_args z,int s);

/* create a single zombie */
static void move_zombie_create_a_zombie(int i);

/* zombie creates bullet */
static void move_zombie_create_bullet(Zombie_args z);

/* zombie exit when the game needs to reset */
static void move_zombie_exit(Zombie_args z);

void move_zombie(void *data)
{
  while(1){
    usleep(ZOMBIE_PRE_MOVE);
    Zombie_args z = (Zombie_args) data;

    if(reset_flag){
      move_zombie_clear_image(z);
      sem_post(&num_of_objects);
      move_zombie_exit(z);
    }

    switch (z->status){
    case ALIVE:
	    move_zombie_regularly(z);
	    break;
    case NOT_INIT:
	    move_zombie_first_move(z);
	    break;
    case DEAD:
            object_count_dec();
            move_zombie_exit(z);
	    break;
    }
  }
}

void move_zombie_create_a_zombie(int i)
{
  pthread_mutex_lock(&malloc_mutex);
  zombies[i] = (Zombie_args)emalloc(sizeof(struct zombie_args));
  pthread_mutex_unlock(&malloc_mutex);
  move_zombie_init_args(zombies[i]);

  object_count_inc();

  ethread_create(&zombies[i]->thrds,NULL,move_zombie,zombies[i]);
}

void move_zombie_args_create()
{
  int i;
  for(i=0; i < zombie_count; i++){
    move_zombie_create_a_zombie(i);
  }
}

void move_zombie_init_args(Zombie_args z)
{
  z->row = FIELD_MIN_ROW;
  z->col = FIELD_MIN_COL-1;
  z->dir = DIR_EAST;
  z->status = NOT_INIT;
}

void move_zombie_clear_image(Zombie_args z)
{
  assert(z != NULL);
  pthread_mutex_lock(&curses_mutex);
  screen_clear_image(z->row,z->col,ZOMBIE_WIDTH,ZOMBIE_HEIGHT);
  screen_refresh();
  pthread_mutex_unlock(&curses_mutex);
}

void move_zombie_draw_image(Zombie_args z)
{
    assert(z != NULL);
    
    char *z_img = zombie(z->dir);
    pthread_mutex_lock(&curses_mutex);
    screen_draw_image(z->row,z->col,&z_img,ZOMBIE_HEIGHT);
    screen_refresh();
    pthread_mutex_unlock(&curses_mutex);
}

void move_zombie_regularly(Zombie_args z)
{
  move_zombie_clear_image(z);
  
  
  int tmp_row = z->row;
  int tmp_col = z->col;

  /* lock global mutex for whole field
       then lock current cell
       get the next row, col
       lock where it goes    
       unlock the global mutex */
  pthread_mutex_lock(&g_adt_mutex);
    g_adt_lock(tmp_row,tmp_col);
    dir_move(z->dir,&z->row,&z->col);
    g_adt_lock(z->row,z->col);
  pthread_mutex_unlock(&g_adt_mutex);

  if(FIELD_IS_IN_PLAY(z->row,z->col) 
          && (g_adt_get_type(z->row,z->col) == EMPTY ||
	      g_adt_get_type(z->row,z->col) == MAN)
          && random()%RANDOM_RATE != 0){
    g_adt_remove(tmp_row,tmp_col);
    g_adt_add(z->row,z->col,z,ZOMBIE);
    
    g_adt_unlock(z->row,z->col);
    g_adt_unlock(tmp_row,tmp_col);
  }else{
    g_adt_unlock(z->row,z->col);
    g_adt_unlock(tmp_row,tmp_col);
    z->row = tmp_row;
    z->col = tmp_col;
    z->dir = dir_change_dir(z->row,z->col,z->dir);
  }

  if (SHOOT_ENABLE && random()%SHOT_RATE == 0) 
    move_zombie_create_bullet(z);

  move_zombie_draw_image(z);
}

void move_zombie_first_move(Zombie_args z)
{
  int tmp_row = z->row;
  int tmp_col = z->col;  
  
  /* lock global mutex for whole field
     then lock current cell
     get the next row, col
     lock where it goes    
     unlock the global mutex */
  pthread_mutex_lock(&g_adt_mutex);
    g_adt_lock(tmp_row,tmp_col);
    dir_move(z->dir,&z->row,&z->col);
    g_adt_lock(z->row,z->col);
  pthread_mutex_unlock(&g_adt_mutex);
  
  if(g_adt_get_type(z->row,z->col) == EMPTY){
    move_zombie_update_status(z,ALIVE);
    move_zombie_draw_image(z);
    g_adt_add(z->row,z->col,z,ZOMBIE);
    g_adt_unlock(z->row,z->col);
    g_adt_unlock(tmp_row,tmp_col);
  }else{
    g_adt_unlock(z->row,z->col);
    g_adt_unlock(tmp_row,tmp_col);
    z->row = tmp_row;
    z->col = tmp_col;
  }
}

void move_zombie_update_status(Zombie_args z,int s)
{
  z->status = s;
}

void move_zombie_killed(void *data)
{
  Zombie_args z = (Zombie_args) data;
  move_zombie_update_status(z,DEAD);
  g_adt_remove(z->row,z->col);

  pthread_mutex_lock(&zombie_count_mutex);
  if(--zombie_count <= 0){
    g_adt_win();
  }
  pthread_mutex_unlock(&zombie_count_mutex);
}

void move_zombie_create_bullet(Zombie_args z)
{
  move_bullet_create(z->row,z->col,z->dir,ZOMBIE_BULLET);
}

void move_zombie_exit(Zombie_args z)
{
  pthread_t tmp = z->thrds;
  efree(z);
  ethread_exit(&tmp);
}
