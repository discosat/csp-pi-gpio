#pragma once
#include <vmem/vmem.h>
#include <param/param.h>

// 16 bits (2 bytes) each for csp_addr and csp_mask
#define VMEM_CONF_CSP_ADDR_ADDR 0x00
#define VMEM_CONF_CSP_MASK_ADDR 0x02
// Arrays of 32x 8-bit values, thus 32 bytes total
#define VMEM_CONF_GPIO_MODES_ADDR 0x10
#define VMEM_CONF_GPIO_INPUTS_ADDR 0x30
#define VMEM_CONF_GPIO_OUTPUTS_ADDR 0x50

extern vmem_t vmem_config;
