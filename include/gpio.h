#ifndef GPIO_H
#define GPIO_H

#include <param/param.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "param_config.h"

#define GPIO_MODE_INPUT (0)
#define GPIO_MODE_OUTPUT (1)

#define GP_FSEL_INPUT (0)
#define GP_FSEL_OUTPUT (1)

// The GPIO registers base address - the same for both Raspberry Pi 3 and 4
#define GP_FSEL (0)
#define GP_SET (7)
#define GP_CLEAR (10)
#define GP_LEVEL (13)

// How pull-up/down works is different between RPi3 and RPi4

// BCM2837 (https://www.raspberrypi.org/app/uploads/2012/02/BCM2835-ARM-Peripherals.pdf)
#ifdef RPI3 
    #define GP_PUD_OFF (0)
    #define GP_PUD_DOWN (1)
    #define GP_PUD_UP (2)
    #define GP_PULL_UP_DOWN (37)
    #define GP_PUDCLK (38)
#endif

// BCM2711 (https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf)
#ifdef RPI4 
    #define GP_PUD_OFF (0)
    #define GP_PUD_UP (1)
    #define GP_PUD_DOWN (2)
    #define GP_PULL_UP_DOWN (57)
#endif

#if !defined(RPI3) && !defined(RPI4)
    #define VIRTUAL_GPIO
#endif

void gpio_init();
void gpio_terminate();

void gpio_param_set_value_callback(struct param_s *param, int offset);
void gpio_param_set_mode_callback(struct param_s *param, int offset);
void gpio_write(int pin, int value);
int gpio_read(int pin);
void gpio_set_mode(int pin, int mode);

void gpio_input_polling_hook(void);

#endif