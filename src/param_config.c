#include "param_config.h"
#include "vmem_config.h"
#include <stdio.h>
#include "gpio.h"

/*
PARAM_DEFINE_STATIC_VMEM(
    id,           // unique parameter id, from param_config.h
    name,         // param_t variable name, from param_config.h
    type,         // Available types in enum param_type_e in param.h
    array_count,  // Number of elements/(bytes?) in the array. -1 for single values
    array_step,   // ??maybe element size in bytes. 0 for single values, 1 for arrays
    flags,        // See param.h; PM_SYSCONF for system/network config, PM_CONF for user config, PM_READONLY for read-only
    callback,     // Callback function for when the value is set
    unit,         // ?? Possibly the unit of the value, e.g. "m/s"
    vmem_name,    // Name of the vmem, without "vmem_" prefix Defined in vmem_config.h. Initialized in main.c
    vmem_address, // Offset in the vmem, from vmem_config.h
    docstr        // Documentation string (for param info)
)
*/

PARAM_DEFINE_STATIC_VMEM(
    PARAMID_CSP_ADDR,
    csp_addr,
    PARAM_TYPE_UINT16,
    -1,
    0,
    PM_SYSCONF,
    NULL,
    NULL,
    config,
    VMEM_CONF_CSP_ADDR_ADDR,
    NULL);

PARAM_DEFINE_STATIC_VMEM(
    PARAMID_CSP_MASK,
    csp_mask,
    PARAM_TYPE_UINT16,
    -1,
    0,
    PM_SYSCONF,
    NULL,
    NULL,
    config,
    VMEM_CONF_CSP_MASK_ADDR,
    NULL);

PARAM_DEFINE_STATIC_VMEM(
    PARAMID_GPIO_MODES,
    gpio_modes,
    PARAM_TYPE_UINT8,
    28,
    1,
    PM_CONF,
    gpio_param_set_mode_callback,
    NULL,
    config,
    VMEM_CONF_GPIO_MODES_ADDR,
    "Pin modes (0 = input, 1 = output)");

PARAM_DEFINE_STATIC_VMEM(
    PARAMID_GPIO_OUTPUTS,
    gpio_outputs,
    PARAM_TYPE_UINT8,
    28,
    1,
    PM_CONF,
    gpio_param_set_value_callback,
    NULL,
    config,
    VMEM_CONF_GPIO_OUTPUTS_ADDR,
    "Values set for output GPIO pins (0 = low, 1 = high)");

PARAM_DEFINE_STATIC_VMEM(
    PARAMID_GPIO_INPUTS,
    gpio_inputs,
    PARAM_TYPE_UINT8,
    28,
    1,
    PM_READONLY,
    NULL,
    NULL,
    config,
    VMEM_CONF_GPIO_INPUTS_ADDR,
    "Values for input GPIOs (0 = low, 1 = high)");

const uint8_t zero = 0;
const uint8_t one = 1;

PARAM_DEFINE_STATIC_RAM(
    PARAMID_ZERO,           // unique parameter id, from param_config.h
    _zero,                  // param_t variable name, from param_config.h
    PARAM_TYPE_UINT8,       // Available types in enum param_type_e in param.h
    -1,                     // Number of elements/(bytes?) in the array. -1 for single values
    0,                      // ??maybe element size in bytes. 0 for single values, 1 for arrays
    PM_READONLY,            // See param.h; PM_SYSCONF for system/network config, PM_CONF for user config, PM_READONLY for read-only
    NULL,                   // Callback function for when the value is set
    NULL,                   // ?? Possibly the unit of the value, e.g. "m/s"
    &zero,                  // Physical address of the value
    "Constant value 0 (U8)" // Documentation string (for param info)
);

PARAM_DEFINE_STATIC_RAM(
    PARAMID_ONE,            // unique parameter id, from param_config.h
    _one,                   // param_t variable name, from param_config.h
    PARAM_TYPE_UINT8,       // Available types in enum param_type_e in param.h
    -1,                     // Number of elements/(bytes?) in the array. -1 for single values
    0,                      // ??maybe element size in bytes. 0 for single values, 1 for arrays
    PM_READONLY,            // See param.h; PM_SYSCONF for system/network config, PM_CONF for user config, PM_READONLY for read-only
    NULL,                   // Callback function for when the value is set
    NULL,                   // ?? Possibly the unit of the value, e.g. "m/s"
    &one,                   // Physical address of the value
    "Constant value 1 (U8)" // Documentation string (for param info)
);