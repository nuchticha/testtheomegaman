#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <curses.h>
#include "tscreen.h"
  
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
  
static char *screen[] = {
  "     +---------------------------------------------+",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |    Attack Force",
  "     |                                             |",
  "     |                                             |    Time: ",
  "     |                                             |",
  "     |                                             |    Lives: n",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     |                                             |",
  "     +---------------------------------------------+",
};

void screen_init(void) {
    initscr();
    crmode();
    noecho();

    screen_draw_board();

    screen_refresh();
}

void screen_draw_board(void) {
    clear();
    screen_draw_image(0, 0, screen, ARRAY_SIZE(screen));
}

void screen_draw_image(int row, int col, char *image[], int height) {
    int i, length;
    int new_left, new_right, new_offset, new_length;

    assert(image != NULL);

    for (i = 0; i < height; i++) {
	if (row+i < 0 || row+i >= SCR_HEIGHT || image[i] == NULL)
	    continue;
	length = strlen(image[i]);
	new_left  = col < 0 ? 0 : col;
	new_offset = col < 0 ? -col : 0;
	new_right = col+length > SCR_WIDTH ? SCR_WIDTH-1 : col+length-1;
	new_length = new_right - new_left + 1;

	mvaddnstr(row+i, new_left, image[i]+new_offset, new_length);
    }
}

void screen_refresh(void) {
    move(LINES-1, COLS-1);
    refresh();
}

void screen_fini(void) {
    endwin();
}

void screen_check(void)
{
  if(LINES < SCR_HEIGHT || COLS < SCR_WIDTH){
    screen_fini(); 
    fprintf(stderr,"Screen size too small, minimum should be %d * %d\n",
	    SCR_WIDTH,SCR_HEIGHT);
  exit(1);
  }
}