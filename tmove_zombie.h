/**********************************************************************
  Module: move_zombie.h
  Author: Scott Wang
  Date:   2006 Sep 30

  Purpose: To make zombie move in the given field
**********************************************************************/

#ifndef MOVE_ZOMBIE_H
#define MOVE_ZOMBIE_H

#define ZOMBIE_PRE_MOVE 150000 
#define ZOMBIE_NUMBER 8 

extern void move_zombie_args_create();
extern void move_zombie();
extern void move_zombie_killed(void *data);

#endif
