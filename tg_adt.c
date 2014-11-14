/**********************************************************************
  Module: g_adt.c
  Author: Scott Wang
  Date:   2006 Oct 5
  Purpose: A data struct for Omega game
**********************************************************************/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <pthread.h>
#include "g_adt.h"
#include "eheap.h"
#include "field.h"
#include "screen.h"
#include "move_man.h"
#include "move_zombie.h"
#include "move_bullet.h"
#include "image.h"
#include "collision.h"
#include "globals.h"

/* calculate the array index from given 
   row and col */
#define ROW_INDEX(row) (row - FIELD_MIN_ROW)
#define COL_INDEX(col) (col - FIELD_MIN_COL)

/* return true iff the calculating result from given
   row and col are correct indexes.. */
#define IS_CORRECT_INDEX(row,col) \
         ( ROW_INDEX(row) >= 0 && ROW_INDEX(row) < FIELD_HEIGHT \
           && COL_INDEX(col) >= 0 && COL_INDEX(col) < FIELD_WIDTH )

/* define winning or losing message */
#define WON "You won"
#define LOST "You need to practice steering!"

/* an array holds every spots in the field where
   t indicate what characters in the game 
   d is an void pointer points to the character
   lock is a mutex lock for the cell
 */
static struct g_array {
  game_type_t t;
  void *d;
  pthread_mutex_t lock;
}g[FIELD_HEIGHT][FIELD_WIDTH];

/* finish the game, clean up whatever left on the screen
   free memeory locations */
static void g_adt_fini(char *message);


void g_adt_init()
{
  int row,col;
  for(row=0; row < FIELD_HEIGHT; row++){
    for(col=0; col<FIELD_WIDTH; col++){
      g[row][col].t = EMPTY;
      g[row][col].d = NULL;
    }
  }
}

game_type_t g_adt_get_type(int row, int col)
{
  assert(IS_CORRECT_INDEX(row,col));
  return g[ROW_INDEX(row)][COL_INDEX(col)].t;
}

void g_adt_add(int row,int col,void *d, game_type_t t)
{
  assert(IS_CORRECT_INDEX(row,col));

  int r_index = ROW_INDEX(row);
  int c_index = COL_INDEX(col);
 
  if(!reset_flag &&
      !collision_is_occur(t,d,g[r_index][c_index].t,g[r_index][c_index].d)){
    g[r_index][c_index].d = d;
    g[r_index][c_index].t = t;
  }
}

void g_adt_remove(int row, int col)
{
  assert(IS_CORRECT_INDEX(row,col));

  g[ROW_INDEX(row)][COL_INDEX(col)].d = NULL;
  g[ROW_INDEX(row)][COL_INDEX(col)].t = EMPTY;
}

void g_adt_lose()
{
  g_adt_fini(LOST);
}

void g_adt_win()
{
  g_adt_fini(WON);
}

void g_adt_fini(char *message)
{
  screen_fini();
  printf("%s\n",message);

  /* clean up */
  int row,col;
  for(row=0; row < FIELD_HEIGHT; row++){
    for(col=0; col<FIELD_WIDTH; col++){
      efree(g[row][col].d);
    }
  }
  exit(1);
}

/* lock the cell */
void g_adt_lock(int row, int col)
{
  if(IS_CORRECT_INDEX(row,col))
  pthread_mutex_lock(&g[ROW_INDEX(row)][COL_INDEX(col)].lock);
}

/* unlock the cell */
void g_adt_unlock(int row,int col)
{
  if(IS_CORRECT_INDEX(row,col))
  pthread_mutex_unlock(&g[ROW_INDEX(row)][COL_INDEX(col)].lock);
}
