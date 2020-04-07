#include "io.h"
#include "framebuffer.h"

static struct framebuffer * fb = 0;
static unsigned short cur_index = 0;
static cursor_location_t cur_loc = {0};

void fb_init()
{
	fb = (struct framebuffer *)FB_ADDRESS;
	cur_loc.x = 0;
	cur_loc.y = 0;
	fb_update_cursor();
	fb_clear();
}

void fb_clear()
{
	char blank = 0x20;
	unsigned char attr = FB_WHITE << 4 | FB_BLACK;
	for (unsigned int i = 0; i < FB_COLS * FB_ROWS; ++i)
	{
		fb[i].ascii = blank;
		fb[i].attr = attr;
	}
}

void fb_scroll()
{
	char blank = 0x20;
	unsigned char attr = FB_WHITE << 4 | FB_BLACK;
	if (cur_loc.y >= FB_ROWS)
	{
		for(unsigned int i = 0; i < FB_ROWS - 1; ++i)
		{
			for(unsigned int j = 0; j < FB_COLS; ++j)
			{
				fb[i * FB_COLS + j] = fb[(i + 1) * FB_COLS + j];
			}
		}

		// clear last line
		for(unsigned int j = 0; j < FB_COLS; ++j)
		{
			fb[(FB_ROWS - 1) * FB_COLS + j].ascii = blank;
			fb[(FB_ROWS - 1) * FB_COLS + j].attr = attr;
		}
		cur_loc.y = 24;
	}
}

void fb_update_cursor()
{
	fb_set_cursor_pos(cur_loc.y * FB_COLS + cur_loc.x);
}

void fb_puts(char * s)
{
	unsigned int i = 0; 
	while (s[i] != 0)
	{
		fb_putch(s[i]);
		++i;
	}
}

void fb_put_dec(unsigned int num)
{
	unsigned char digit = 0; 
	int i = 0; 
	char buff[32] = { 0 };
	if (num == 0)
	{
		fb_puts("0");
		return;
	}
	while(num != 0)
	{
		digit = num % 10;
		buff[32 - i - 1] = digit + '0';
		num /= 10;
		++i;
	}
	fb_puts(buff + 32 - i);
}

void fb_put_hex(unsigned int hex)
{
	char digits[] = "0123456789abcdef";
	unsigned char digit = 0; 
	char buff[34] = { 0 };
	int i = 32; // last offset is \0
	
	if (hex == 0)
	{
		fb_puts("0x0");
		return;
	}

	while(hex != 0)
	{
		digit = hex % 16;
		buff[i - 1] = digits[digit];
		hex /= 16;
		--i;
	}
	buff[--i] = 'x';
	buff[--i] = '0';
	fb_puts(buff + i);
}

void fb_putch(char c)
{
	/* put the character on the screen */
	unsigned int offset = 0; 
	switch (c)
	{
	case '\b':
		if (cur_loc.x > 0)
		{
			cur_loc.x -= 1;
		}
		break;
	
	case '\r':
		cur_loc.x = 0;
		break;
	case '\n':
		cur_loc.y += 1;
		cur_loc.x = 0;
		break;
	case '\t':
		cur_loc.x += 8 - (cur_loc.x % 8);
		break;
	default:
		if (c >= ' ')
		{
			offset = cur_loc.y * FB_COLS + cur_loc.x;
			fb[offset].ascii = c;
			fb[offset].attr = FB_WHITE << 4 | FB_BLACK;
			cur_loc.x += 1;
		}
		break;
	}
	fb_scroll();

	fb_update_cursor();
}

void fb_set_cursor_pos(unsigned short pos)
{
	cur_loc.x = pos % FB_COLS;
	cur_loc.y = pos / FB_COLS;
	cur_index = pos;
	outb(INDEX_REGISTER_PORT, FB_CURSOR_LOC_HIGH);
	outb(DATA_REGISTER_PORT, pos >> 8);
	outb(INDEX_REGISTER_PORT, FB_CURSOR_LOC_LOW);
	outb(DATA_REGISTER_PORT, 0xFF & pos);
}