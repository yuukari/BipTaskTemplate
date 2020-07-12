#ifndef __APP_TEMPLATE_H__

#define __APP_TEMPLATE_H__

struct app_data_ {
	void* ret_f;

	unsigned int current_screen;

	unsigned int switch_1;
	unsigned int switch_2;
};

void 	show_screen (void *return_screen);
void 	key_press_screen();
int 	dispatch_screen (void *param);
void 	screen_job();
void	draw_switch_screen();
void	draw_buttons_screen();

#endif