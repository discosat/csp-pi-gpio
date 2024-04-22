// Standard libraries
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
// CSP and other project libs
#include <csp/csp.h>
#include <csp/interfaces/csp_if_zmqhub.h>
#include <param/param_server.h>
#include <vmem/vmem_file.h>
#include <vmem/vmem_server.h>
// Project headers
#include "gpio.h"
#include "hooks.h"
#include "vmem_config.h"
#include "param_config.h"


// For catching ctrl-c
static volatile int keepRunning = 1;
void intHandler(int dummy) {
    keepRunning = 0;
}


// Define file to store persistent params
// binds to the vmem_t vmem_config defined in vmem_config.h
VMEM_DEFINE_FILE(config, "config", "config.vmem", 5000);

void* vmem_server_task(void* param) {
    vmem_server_loop(param);
    return NULL;
}

void* router_task(void* param) {
    while (1) {
        csp_route_work();
    }
    return NULL;
}

static void* onehz_task(void* param) {
    while (1) {
        hook_onehz();
        sleep(1);
    }
    return NULL;
}

static void* tenhz_task(void* param) {
    while (1) {
        hook_tenhz();
        usleep(100 * 1000);
    }
    return NULL;
}

static void csp_init_fun(void) {
    csp_conf.hostname = "CSP-GPIO";
    csp_conf.model = "RPi3";
    csp_conf.revision = "1";
	csp_conf.version = 2;
	csp_conf.dedup = 3;

    csp_init();

    hook_init_early();

    csp_bind_callback(csp_service_handler, CSP_ANY);
    csp_bind_callback(param_serve, PARAM_PORT_SERVER);

    static pthread_t vmem_server_handle;
    pthread_create(&vmem_server_handle, NULL, &vmem_server_task, NULL);

    static pthread_t router_handle;
    pthread_create(&router_handle, NULL, &router_task, NULL);

    static pthread_t onehz_handle;
    pthread_create(&onehz_handle, NULL, &onehz_task, NULL);

    static pthread_t tenhz_handle;
    pthread_create(&tenhz_handle, NULL, &tenhz_task, NULL);
}

static void iface_zmq_init(void) {
    uint16_t addr = param_get_uint16(&csp_addr);
    uint16_t mask = param_get_uint16(&csp_mask);

    if (addr == 0) {
        const uint16_t default_addr = 12;
        param_set_uint16(&csp_addr, default_addr);
        printf("Using default address %d\r\n", default_addr);
        addr = default_addr;
    }
    if (mask == 0) {
        const uint16_t default_mask = 16;
        param_set_uint16(&csp_mask, default_mask);
        printf("Using default netmask %d\r\n", default_mask);
        mask = default_mask;
    }


    csp_iface_t* iface;
    csp_zmqhub_init_filter2("ZMQ", "localhost", addr, mask, true, &iface, NULL, CSP_ZMQPROXY_SUBSCRIBE_PORT, CSP_ZMQPROXY_PUBLISH_PORT);
    
    iface->is_default = true;
    iface->addr = addr;
    iface->netmask = mask;
    iface->name = "ZMQ";
}

int main(void) {
    printf("\nbootmsg\n");

    srand(time(NULL));

    void serial_init(void);
    serial_init();
    

    /* Interfaces */
    vmem_file_init(&vmem_config);
    iface_zmq_init();

    /* Init CSP */
    csp_init_fun();

    csp_print("Connection table\r\n");
    csp_conn_print_table();

    csp_print("Interfaces\r\n");
    csp_iflist_print();

    csp_print("Route table\r\n");
    csp_rtable_print();

    // server_start();
    // kiss_init();
    // csp_rt_init(); // Routing table ???

    hook_init();

    signal(SIGINT, intHandler);
    while (keepRunning) {
        sleep(1);
    }

    printf("Exiting...\n");
    gpio_terminate();

    return 0;
}
