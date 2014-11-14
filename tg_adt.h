/**********************************************************************
  Module: g_adt.h
  Author: Scott Wang
  Date:   2006 Oct 5
  Purpose: A data struct for Omega game
**********************************************************************/

#ifndef G_ADT_H
#define G_ADT_H

/* define the possible things that appear in the game structure */
typedef enum game_type {
  EMPTY, /* indicate an empty slot */
  MAN,
  ZOMBIE,
  ZOMBIE_BULLET,
  MAN_BULLET,
  GAME_TYPE_LAST,
}game_type_t;

extern void g_adt_init();

/* return the type of the given spot */
extern game_type_t g_adt_get_type(int row,int col);

/* add object to the game struct
   deal with collision
   if zombie moves to man or man moves to zombie, man die,zombie reset
   if bullet shot a zombie, zombie die
   bullet has no effect on man, and continue on the screen*/
extern void g_adt_add(int row,int col,void *d, game_type_t t);

extern void g_adt_remove(int row, int col);
extern void g_adt_lose(void);
extern void g_adt_win(void);

/* lock & unlock the given cell */
extern void g_adt_lock(int row,int col);
extern void g_adt_unlock(int row,int col);

/* get number of objects remains on screen */
//extern int g_adt_get_objects_num();

#endif
