#include "gpio.h"

int fdgpio = -1;
unsigned int* gpio;

void gpio_init() {
    if (fdgpio >= 0) {
        printf("gpio already initialized\n");
    }

    fdgpio = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (fdgpio < 0) {
        printf("error opening /dev/gpiomem\n");
    }
    printf("fdgpio: %d\n", fdgpio);

    gpio = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fdgpio, 0);
    if (gpio == MAP_FAILED) {
        printf("error mapping /dev/gpiomem\n");
    }
    printf("gpio: %p\n", gpio);


    // Read mode from vmem and set it
    for (int i = 0; i < 32; i++) {
        int mode = param_get_uint8_array(&gpio_modes, i);
        gpio_set_mode(i, mode);

        if (mode == GPIO_MODE_OUTPUT) {
            printf("Initializing output %d\n", i);
            gpio_write(i, param_get_uint8_array(&gpio_outputs, i));
            param_set_uint8_array(&gpio_inputs, i, 0);
        } else if (mode == GPIO_MODE_INPUT) {
            printf("Initializing input %d\n", i);
            param_set_uint8_array(&gpio_inputs, i, gpio_read(i));
            param_set_uint8_array(&gpio_outputs, i, 0);
        }
    }
}

void gpio_terminate() {
    if (fdgpio < 0) {
        printf("gpio not initialized\n");
    }

    munmap(gpio, 4096);
    close(fdgpio);
    fdgpio = -1;
}

void gpio_param_set_value_callback(struct param_s *param, int offset) {
    printf("gpio_param_set_value_callback\n");
    int pin = offset;
    int value = param_get_uint8_array(param, offset);

    gpio_write(pin, value);
}

void gpio_param_set_mode_callback(struct param_s *param, int offset) {
    printf("gpio_param_set_mode_callback\n");
    
    int pin = offset;
    int mode = param_get_uint8_array(param, offset);

    gpio_set_mode(pin, mode);
}

void gpio_set_mode(int pin, int mode) {
    printf("gpio_set_mode(%d, %d)\n", pin, mode);
    if (fdgpio < 0) {
        printf("gpio not initialized\n");
        return;
    }
    if (pin < 0 || pin > 31) {
        printf("invalid pin\n");
        return;
    }
    if (mode < 0 || mode > 7) {
        printf("invalid mode\n");
        return;
    }

    /* FSEL (mode selection) is split over 6 different registers, as the value
        is 3 bits wide. Each register controls 10 pins. 
        The correct register is found by dividing the pin number by 10, such that
        pins 0-9 are controlled by register 0, pins 10-19 are controlled by register 1, etc.

        Then the remainder is used to get the offset for the pin in the register.

        This offset is multiplied by 3 to get the bit offset in the register.
     */

    int fseln = pin / 10;
    int fselb = pin % 10;
    int fsel = gpio[GP_FSEL + fseln];

    // Clear the 3 bits for the pin
    fsel &= ~(0b111 << (fselb * 3));
    // Set the 3 bits for the pin
    fsel |= mode << (fselb * 3);
    // Push to register
    gpio[GP_FSEL + fseln] = fsel;


    // Set Pull-up on input pins

    if (mode == GPIO_MODE_INPUT) {
#ifdef RPI3
        printf("RPI3 pull-up\n");
        gpio[GP_PULL_UP_DOWN] = GP_PUD_UP;
        usleep(5);
        gpio[GP_PUDCLK] = 1 << pin;
        usleep(5);
        gpio[GP_PULL_UP_DOWN] = GP_PUD_OFF;
        gpio[GP_PUDCLK] = 0;
#else
#ifdef RPI4
        printf("RPI4 pull-up\n");
        int PUDn = pin/16;
        int PUDb = pin%16;
        int PUD = gpio[GP_PULL_UP_DOWN + PUDn];

        PUD &= ~(0b11 << (PUDb * 2));
        PUD |= GP_PUD_UP << (PUDb * 2);

        gpio[GP_PULL_UP_DOWN + PUDn] = PUD;
#else
        printf("Not RPI3 or RPI4. Won't set pull-up\n");
#endif // RPI4
#endif // RPI3
    }
}

void gpio_write(int pin, int value) {
    printf("gpio_set(%d, %d)\n", pin, value);
    if (fdgpio < 0) {
        printf("gpio not initialized\n");
        return;
    }
    if (pin < 0 || pin > 31) {
        printf("invalid pin\n");
        return;
    }

    gpio[GP_CLEAR] = 1 << pin;
    uint8_t level = value ? 1 : 0;
    gpio[GP_SET] = gpio[GP_LEVEL] | (level << pin);
}

int gpio_read(int pin) {
    if (fdgpio < 0) {
        printf("gpio not initialized\n");
        return -1;
    }
    if (pin < 0 || pin > 31) {
        printf("invalid pin\n");
        return -1;
    }
    return (gpio[GP_LEVEL]&(1<<pin)) ? 1 : 0;
}

void gpio_input_polling_hook(void) {
    for (int i = 0; i < 32; i++) {
        if (param_get_uint8_array(&gpio_modes, i) == GPIO_MODE_INPUT) {
            param_set_uint8_array(&gpio_inputs, i, gpio_read(i));
        } else {
            param_set_uint8_array(&gpio_inputs, i, 0);
        }
    }
}