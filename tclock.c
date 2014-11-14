/**********************************************************************
  Module: clock.c
  Author: Scott Wang
  Date:   2006 Oct 10
  Purpose: A count down clock for the game 
**********************************************************************/
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <curses.h>
#include <unistd.h>

#include "tglobals.h"
#include "tclock.h"
#include "eheap.h"
#include "tscreen.h"
#include "tethread.h"
#include "tg_adt.h"

/* used to convert integer to string */
#define BUF_SIZE 10

/* define max minutes for count down */
#define MAX_MINUTES 3

/* define for default starting second */
#define DEFAULT_SECONDS 0

/* Clock size is used for draw and clear on the screen */
#define CLOCK_WIDTH 5
#define CLOCK_HEIGHT 1

#define CLOCK_CYCLE 60
#define CLOCK_PRE_MOVE 500000

/* data type for the clock 
   minutes and seconds are used for count down.
   curr_sec is used to keep track the system time */
typedef struct gametime{
  int minutes;
  int seconds;
  time_t curr_sec;
  pthread_t thread;
}* Gametime;

/* draw clock on the given field on screen */
static void clock_draw_image(Gametime t);

/* clear clock on the screen */
static void clock_clear_image(Gametime t);

/* a system call to get current time second */
static time_t clock_get_current_time();

/* decrement time counter */
static void clock_countdown(Gametime t, time_t passed_seconds);

void clock_init()
{
  pthread_mutex_lock(&malloc_mutex);
  Gametime t = (Gametime) emalloc(sizeof(struct gametime));
  pthread_mutex_unlock(&malloc_mutex);

  t->minutes = MAX_MINUTES;
  t->seconds = DEFAULT_SECONDS;
  t->curr_sec = clock_get_current_time();

  pthread_mutex_lock(&curses_mutex);
  clock_draw_image(t);
  pthread_mutex_unlock(&curses_mutex);

  ethread_create(&t->thread,NULL,clock_update,t);
}

void clock_draw_image(Gametime t)
{
  assert(t != NULL);

  char buf[BUF_SIZE];
  snprintf(buf,(sizeof(buf)+1),"%d:%d",t->minutes,t->seconds);
  mvaddstr(SCR_TIMER_ROW,SCR_TIMER_COL,buf);
  screen_refresh();
}

void clock_clear_image(Gametime t)
{
  assert(t != NULL);
  screen_clear_image(SCR_TIMER_ROW,SCR_TIMER_COL,CLOCK_WIDTH,CLOCK_HEIGHT);
}

void clock_update(void *data)
{
  Gametime t = (Gametime) data;
  while(1){
    usleep(CLOCK_PRE_MOVE);

    pthread_mutex_lock(&curses_mutex);
    clock_clear_image(t);
    pthread_mutex_unlock(&curses_mutex);
    
    time_t s = clock_get_current_time();
    time_t passed_seconds;
    /* if the time has been passed more than one
     seconds, decrement time counter */
    if((passed_seconds = s - t->curr_sec) > 0){
      clock_countdown(t,passed_seconds);
      t->curr_sec = s;
    }

    pthread_mutex_lock(&curses_mutex);
    clock_draw_image(t);
    pthread_mutex_unlock(&curses_mutex);
  }
}

time_t clock_get_current_time()
{
  struct timeval t;
  if(gettimeofday(&t,NULL) == -1){
    perror("gettimeofday");
    exit(1);
  }else{
    return t.tv_sec;
  }
}

void clock_countdown(Gametime t, time_t passed_seconds)
{
  if(t->seconds <= 0){
    if(t->minutes -- <= 0) g_adt_lose();
    t->seconds += CLOCK_CYCLE ;
  }
  t->seconds -= passed_seconds;
}
