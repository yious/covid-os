#include "io.h"
#include "framebuffer.h"

static struct framebuffer * fb = 0;
static unsigned short cur_index = 0;

void fb_init()
{
	fb = (struct framebuffer *)FB_ADDRESS;
	cur_index = 0;
	fb_set_cursor_pos(cur_index);
}

void fb_write_cell(int offset, unsigned char ch, int fg, int bg)
{
	fb[offset].ascii = ch;
	fb[offset].attr = fg << 4 | bg; 
	/* int attr = fg << 4 | bg;
	char * addr = (char *)FB_ADDRESS + offset * 2;
	addr[0] = ch;
	addr[1] = attr;	
	*/
}

void fb_write_pos(unsigned short pos, unsigned char * st, int len)
{
	for (int i = 0; i < len; ++i)
	{
		fb_write_cell(pos + i, st[i], FB_WHITE, FB_BLACK);
	}
	fb_set_cursor_pos(pos + len); // 1 less bc of null byte
}

void fb_write(unsigned char * st, int len) 
{
	fb_write_pos(cur_index, st, len);
}

void fb_set_cursor_pos(unsigned short pos)
{
	cur_index = pos;
	outb(INDEX_REGISTER_PORT, FB_CURSOR_LOC_HIGH);
	outb(DATA_REGISTER_PORT, pos >> 8);
	outb(INDEX_REGISTER_PORT, FB_CURSOR_LOC_LOW);
	outb(DATA_REGISTER_PORT, 0xFF & pos);
}