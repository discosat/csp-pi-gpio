#include <stdio.h>
#include <pthread.h>
#include "gpio.h"
#include <csp_proc/proc_server.h>


void hook_onehz(void) {
}

void hook_tenhz(void) {
	gpio_input_polling_hook();
}


/* This is at the beginning of the main routine, before usart and peripherals are inited */
void hook_init_early(void) {
    // Proc init
    proc_server_init();
    csp_bind_callback(proc_serve, PROC_PORT_SERVER);
}

/* This is at the end of the main routine */
void hook_init(void) {
    gpio_init();
}
