#include "framebuffer.h"
#include "serial.h"
#include "gdt.h"

int kmain()
{
	unsigned char msg[] = "Framebuffer initialized!";
	unsigned char msg2[] = "Initializing serial";
	unsigned char msg3[] = "Initializing gdt";
	unsigned char msg4[] = "Serial done.";
	unsigned char msg_done[] = "Done.";

	fb_init();
	fb_write(msg, sizeof(msg));
	fb_set_cursor_pos(FB_COLS * 1);

	
	fb_write(msg2, sizeof(msg2));
	fb_set_cursor_pos(FB_COLS * 2);
	serial_init_port(SERIAL_COM1);
	serial_write(SERIAL_COM1, msg4 , sizeof(msg4));


	fb_write(msg3, sizeof(msg3));
	fb_set_cursor_pos(FB_COLS * 3);
	gdt_init();

	fb_write(msg_done, sizeof(msg_done));
	fb_set_cursor_pos(FB_COLS * 4);

	return 0xDEADBEEF;
}