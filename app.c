#include <libbip.h>
#include "app.h"
#include "biptask.h"

#define SCREEN_SWITCH 0
#define SCREEN_BUTTONS 1
#define SCREEN_RANDOM 2

// Уникальный идентификатор приложения для BipTask
char app_id[3] = { 0x00, 0x00, 0x10 };

struct regmenu_ screen_data = {
	60,
	1,
	0,
	dispatch_screen,
	key_press_screen,
	screen_job,
	0,
	show_screen,
	0,
	0
};

int main(int param0, char** argv){
	show_screen((void*)param0);
}

void show_screen (void *param0){
	struct app_data_** app_data_p = get_ptr_temp_buf_2();
	struct app_data_* app_data;

	Elf_proc_* proc;

	if ((param0 == *app_data_p) && get_var_menu_overlay()){
		app_data = *app_data_p;
		*app_data_p = NULL;

		reg_menu(&screen_data, 0);
		
		*app_data_p = app_data;
	} else {
		reg_menu(&screen_data, 0);

		*app_data_p = (struct app_data_*)pvPortMalloc(sizeof(struct app_data_));
		app_data = *app_data_p;
		
		_memclr(app_data, sizeof(struct app_data_));
		
		proc = param0;
		
		if (param0 && proc->ret_f)
			app_data->ret_f = proc->elf_finish;
		else
			app_data->ret_f = show_watchface;

		app_data->current_screen = SCREEN_SWITCH;

		app_data->switch_1 = false;
		app_data->switch_2 = false;

		app_data->randomValue1 = 0;
		app_data->randomValue2 = 0;
		app_data->randomValue3 = 0;

		_srand(get_tick_count());
	}

	set_display_state_value(8, 1);
	set_display_state_value(4, 1);
	set_display_state_value(2, 1);

	draw_switch_screen();

	set_update_period(1, 200);
}

void key_press_screen(){
	struct app_data_** app_data_p = get_ptr_temp_buf_2();
	struct app_data_ * app_data = *app_data_p;

	set_display_state_value(8, 0);
	set_display_state_value(4, 0);
	set_display_state_value(2, 0);

	show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);	
};

void screen_job(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *	app_data = *app_data_p;

	set_update_period(1, 200);
}

int dispatch_screen (void *param){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *	app_data = *app_data_p;

	struct gesture_ *gest = param;

	switch (gest->gesture){
		case GESTURE_CLICK:	
			if (app_data->current_screen == SCREEN_SWITCH){
				if (
					(gest->touch_pos_x > 94) && (gest->touch_pos_x < 171) &&
					(gest->touch_pos_y > 16) && (gest->touch_pos_y < 82)
				){
					app_data->switch_1 = !app_data->switch_1;
					
					// Отправка одного значения используя biptask_send_byte
					// Рекомендуется использовать эту функцию вместо send_host_app_msg для избежания конфликтов

					// Значения могут быть абсолютно любыми, такие значения указаны лишь для того, чтобы совпадать
					// с демонстрационным профилем для Tasker

					// В качестве аргументов функции biptask_send_byte указывается идентификатор приложения и
					// передаваемый байт

					if (app_data->switch_1)
						biptask_send_byte(app_id, 0x11); // Первый переключатель включен
					else
						biptask_send_byte(app_id, 0x10); // Первый переключатель выключен
				}

				if (
					(gest->touch_pos_x > 94) && (gest->touch_pos_x < 171) &&
					(gest->touch_pos_y > 92) && (gest->touch_pos_y < 138)
				){
					app_data->switch_2 = !app_data->switch_2;
					
					if (app_data->switch_2)
						biptask_send_byte(app_id, 0x13); // Второй переключатель включен
					else
						biptask_send_byte(app_id, 0x12); // Второй переключатель выключен
				}

				draw_switch_screen();
				repaint_screen_lines(0, 176);
			} else if (app_data->current_screen == SCREEN_BUTTONS){
				if (
					(gest->touch_pos_x > 16) && (gest->touch_pos_x < 160) &&
					(gest->touch_pos_y > 16) && (gest->touch_pos_y < 76)
				){
					biptask_send_byte(app_id, 0x20); // Первая кнопка нажата
				}

				if (
					(gest->touch_pos_x > 16) && (gest->touch_pos_x < 160) &&
					(gest->touch_pos_y > 100) && (gest->touch_pos_y < 160)
				){
					biptask_send_byte(app_id, 0x21); // Вторая кнопка нажата
				}
				
				draw_buttons_screen();
				repaint_screen_lines(0, 176);
			} else if (app_data->current_screen == SCREEN_RANDOM){
				if (
					(gest->touch_pos_x > 16) && (gest->touch_pos_x < 160) &&
					(gest->touch_pos_y > 100) && (gest->touch_pos_y < 160)
				){					
					// Отправка нескольких значений используя biptask_send_bytes

					// Значения могут быть абсолютно любыми, как и размер массива
					// В данном примере отправляется массив с тремя случайными числами от 0 до 255

					app_data->randomValue1 = (1 + _rand() % 255);
					app_data->randomValue2 = (1 + _rand() % 255);
					app_data->randomValue3 = (1 + _rand() % 255);

					char bytes[3] = {
						app_data->randomValue1,
						app_data->randomValue2,
						app_data->randomValue3
					};

					// В качестве аргументов функции указывается идентификатор приложения, массив с данными и его размер
					biptask_send_bytes(app_id, bytes, 3);
				}

				draw_random_screen();
				repaint_screen_lines(0, 176);
			}
		break;

		case GESTURE_SWIPE_RIGHT:
			set_display_state_value(8, 0);
			set_display_state_value(4, 0);
			set_display_state_value(2, 0);

			show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);	
		break;

		case GESTURE_SWIPE_LEFT:

		break;

		case GESTURE_SWIPE_UP:
			switch (app_data->current_screen){
				case SCREEN_SWITCH: 
					show_menu_animate(draw_buttons_screen, 0, ANIMATE_DOWN);
					app_data->current_screen = SCREEN_BUTTONS;
				break;
				case SCREEN_BUTTONS: 
					show_menu_animate(draw_random_screen, 0, ANIMATE_DOWN);
					app_data->current_screen = SCREEN_RANDOM; 
				break;
				case SCREEN_RANDOM: 
					show_menu_animate(draw_switch_screen, 0, ANIMATE_DOWN);
					app_data->current_screen = SCREEN_SWITCH; 
				break;
			}
		break;

		case GESTURE_SWIPE_DOWN:
			switch (app_data->current_screen){
				case SCREEN_SWITCH: 
					show_menu_animate(draw_random_screen, 0, ANIMATE_UP);
					app_data->current_screen = SCREEN_RANDOM; 
				break;
				case SCREEN_RANDOM: 
					show_menu_animate(draw_buttons_screen, 0, ANIMATE_UP);
					app_data->current_screen = SCREEN_BUTTONS;
				break;
				case SCREEN_BUTTONS: 
					show_menu_animate(draw_switch_screen, 0, ANIMATE_UP);
					app_data->current_screen = SCREEN_SWITCH; 
				break;
			}
		break;

	}
	
	return 0;
};

void draw_switch_screen(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *	app_data = *app_data_p;

	load_font();
	set_bg_color(COLOR_BLACK);
	fill_screen_bg();
	set_fg_color(COLOR_WHITE);

	text_out("Switch 1", 18, 40);
	if (app_data->switch_1)
		show_res_by_id(ICON_TOGGLE_ON, 106, 36);
	else
		show_res_by_id(ICON_TOGGLE_OFF, 106, 36);

	text_out("Switch 2", 18, 116);
	if (app_data->switch_2)
		show_res_by_id(ICON_TOGGLE_ON, 106, 112);
	else
		show_res_by_id(ICON_TOGGLE_OFF, 106, 112);
}

void draw_buttons_screen(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *	app_data = *app_data_p;

	load_font();
	set_bg_color(COLOR_BLACK);
	fill_screen_bg();
	set_fg_color(COLOR_WHITE);

	draw_rect(16, 16, 160, 76);
	draw_vertical_line(156, 20, 72);
	draw_horizontal_line(72, 20, 156);
	text_out_center("Button 1", 88, 36);

	draw_rect(16, 100, 160, 160);
	draw_vertical_line(156, 104, 156);
	draw_horizontal_line(156, 20, 156);
	text_out_center("Button 2", 88, 120);
}

void draw_random_screen(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
	struct app_data_ *	app_data = *app_data_p;

	load_font();
	set_bg_color(COLOR_BLACK);
	fill_screen_bg();
	set_fg_color(COLOR_WHITE);

	char text[16];

	text_out_center("Random array send", 88, 30);
	_sprintf(text, "[%d, %d, %d]", app_data->randomValue1, app_data->randomValue2, app_data->randomValue3);
	text_out_center(text, 88, 48);

	draw_rect(16, 100, 160, 160);
	draw_vertical_line(156, 104, 156);
	draw_horizontal_line(156, 20, 156);
	text_out_center("Send", 88, 120);
}