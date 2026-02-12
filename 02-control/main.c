#include "stdio-task/stdio-task.h"
#include "pico/stdlib.h"

int main()
{
    stdio_init_all();

    stdio_task_init();

    while (1)
    {
        stdio_task_handle();
    }
}