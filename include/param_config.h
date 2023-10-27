#pragma once
#include <param/param.h>

#define PARAMID_CSP_ADDR     1
#define PARAMID_CSP_MASK     2
#define PARAMID_GPIO_MODES   3
#define PARAMID_GPIO_INPUTS  4
#define PARAMID_GPIO_OUTPUTS 5

extern param_t csp_addr;
extern param_t csp_mask;
extern param_t gpio_modes;
extern param_t gpio_inputs;
extern param_t gpio_outputs;