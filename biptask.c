#include <libbip.h>
#include "biptask.h"

void biptask_send_byte(char* app_id, char byte){
	send_host_app_msg(0xDE);
	send_host_app_msg(0xE0);

	for (unsigned int i = 0; i < 3; i++) {
		send_host_app_msg(0xE0 + ((app_id[i] == 0 ? 1 : app_id[i] & 0xF0) >> 4));
		send_host_app_msg(0xE0 + (app_id[i] == 0 ? 1 : app_id[i] & 0x0F));
	}

	send_host_app_msg(0xE0 + ((byte & 0xF0) >> 4));
	send_host_app_msg(0xE0 + (byte & 0x0F));

	send_host_app_msg(0xDF);
}

void biptask_send_bytes(char* app_id, char* bytes, unsigned int length)
{
	send_host_app_msg(0xDE);
	send_host_app_msg(0xE0);

	for (unsigned int i = 0; i < 3; i++) {
		send_host_app_msg(0xE0 + ((app_id[i] == 0 ? 1 : app_id[i] & 0xF0) >> 4));
		send_host_app_msg(0xE0 + (app_id[i] == 0 ? 1 : app_id[i] & 0x0F));
	}

	for (unsigned int i = 0; i < length; i++) {
		send_host_app_msg(0xE0 + ((bytes[i] & 0xF0) >> 4));
		send_host_app_msg(0xE0 + (bytes[i] & 0x0F));
	}

	send_host_app_msg(0xDF);
}