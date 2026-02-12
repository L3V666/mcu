#include "protocol-task.h"
#include "stdio.h"
#include "string.h"

static api_t *api = {0};
static int commands_count = 0;

void protocol_task_init(api_t *device_api);

void protocol_task_handle(char *command_string);

void protocol_task_init(api_t *device_api)
{
    api = device_api;
    commands_count = sizeof(device_api);
}

void protocol_task_handle(char *command_string)
{
    if (command_string == NULL)
    {
        return;
    }
    const char *command_name = command_string;
    const char *command_args = NULL;

    char *space_symbol = strchr(command_string, ' ');
    if (space_symbol)
    {
        *space_symbol = '\0';
        command_args = space_symbol + 1;
    }
    else
    {
        command_args = "";
    }

    printf("command name: '%s', command args: %s\n", command_name, command_args);
}