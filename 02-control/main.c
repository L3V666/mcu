#include "stdio-task/stdio-task.h"
#include "pico/stdlib.h"
#include "protocol-task/protocol-task.h"
#include "led-task/led-task.h"
#include "stdio.h"

#define DEVICE_NAME "my-pico-device"
#define DEVICE_VRSN "v0.0.1"

void version_callback(const char *args)
{
    printf("device name: '%s', firmware version: %s\n", DEVICE_NAME, DEVICE_VRSN);
}

void led_on_callback(const char *args)
{
    led_state_t state = LED_STATE_ON;
    led_task_state_set(state);
    printf("LED On\n");
}

void led_off_callback(const char *args)
{
    led_state_t state = LED_STATE_OFF;
    led_task_state_set(state);
    printf("LED Off\n");
}

void led_blink_callback(const char *args)
{
    led_state_t state = LED_STATE_BLINK;
    led_task_state_set(state);
    printf("LED Blink\n");
}

api_t device_api[] =
    {
        {"version", version_callback, "get device name and firmware version"},
        {"on", led_on_callback, "led on"},
        {"off", led_off_callback, "led off"},
        {"blink", led_blink_callback, "led blink"},
        {NULL, NULL, NULL},
};

int main()
{
    stdio_init_all();

    stdio_task_init();

    protocol_task_init(device_api);

    led_task_init();

    while (1)
    {
        // stdio_task_handle();

        protocol_task_handle(stdio_task_handle());

        led_task_handle();
    }
}