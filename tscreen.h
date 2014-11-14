#ifndef SCREEN_H
#define SCREEN_H

#define SCR_WIDTH 80
#define SCR_HEIGHT 24

extern void screen_init(void);
extern void screen_draw_board(void);
extern void screen_draw_image(int row, int col, char *image[], int height);
extern void screen_refresh(void);
extern void screen_fini(void);
extern void screen_check(void);

#endif /* SCREEN_H */