#include "hardware/adc.h"

#include "stdio.h"
#include "pico/stdlib.h"

#include "adc-task.h"

static int GPIO = 26;
static int ADC = 0;

void adc_task_init()
{
    adc_init();

    adc_gpio_init(GPIO);
}

float adc_task_measurement()
{
    adc_select_input(ADC);
    uint16_t voltage_counts = adc_read();
    float voltage_V = voltage_counts * 3.3f / 4096.0f;
    return voltage_V;
}