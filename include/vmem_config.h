#pragma once
#include <vmem/vmem.h>
#include <param/param.h>

// 16 bits (2 bytes) each for csp_addr and csp_mask
#define VMEM_CONF_CSP_ADDR_ADDR 0x00
#define VMEM_CONF_CSP_MASK_ADDR 0x02
// Arrays of 28x 8-bit values, 28(0x1C) bytes each
#define VMEM_CONF_GPIO_MODES_ADDR 0x10
#define VMEM_CONF_GPIO_INPUTS_ADDR 0x2C
#define VMEM_CONF_GPIO_OUTPUTS_ADDR 0x48

extern vmem_t vmem_config;
