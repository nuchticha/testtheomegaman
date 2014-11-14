/**********************************************************************
  Module: clock.h
  Author: Scott Wang
  Date:   2006 Oct 10
  Purpose: A count down clock for the game 
**********************************************************************/

#ifndef CLOCK_H
#define CLOCK_H

/* Each time clock_update runs, it gets a new time
   from system and check the old timer to see if
   it has been passed more than one second. If it
   does, then decrement the timer on the screen */ 
extern void clock_update(void *data); 

/* initialize the clock */
extern void clock_init();

#endif
