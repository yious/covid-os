#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#define FB_ROWS 25
#define FB_COLS  80

#define FB_WHITE 0
#define FB_BLACK 15

#define FB_CURSOR_LOC_HIGH 0x0E
#define FB_CURSOR_LOC_LOW 0X0F

#define INDEX_REGISTER_PORT 0x3D4
#define DATA_REGISTER_PORT 0x3D5

#define FB_ADDRESS 0x000b8000


struct framebuffer {
    unsigned char ascii;
    unsigned char attr; 
} __attribute__((packed));

void fb_init();

void fb_write_cell(int offset, unsigned char ch, int fg, int bg);

void fb_write_pos(unsigned short pos, unsigned char * st, int len);

void fb_write(unsigned char * st, int len);

void fb_set_cursor_pos(unsigned short pos);

#endif // FRAMEBUFFER_H