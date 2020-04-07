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

struct cursor_location {
    unsigned short x;
    unsigned short y;
} __attribute__((packed));
typedef struct cursor_location cursor_location_t;

struct framebuffer {
    unsigned char ascii;
    unsigned char attr; 
} __attribute__((packed));

void fb_init();

void fb_puts(char * s);
void fb_putch(char c);
void fb_put_dec(unsigned int dec);
void fb_put_hex(unsigned int hex);
void fb_clear();

void fb_update_cursor();
void fb_set_cursor_pos(unsigned short pos);

void fb_scroll();

#endif // FRAMEBUFFER_H