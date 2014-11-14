#include <stdio.h>
#include "tscreen.h"
#include "tobject_count.h"
#include "master.h"
  
int main()
{
    screen_init();
    screen_check();
    screen_draw_board();
    screen_refresh();
  
    object_count_init();
  
    master_create();
}