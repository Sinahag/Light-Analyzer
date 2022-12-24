#include "includes/button.h"

void runCommand(char *command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}
void Button_initialize()
{
    runCommand(BUTTON_CONFIG);
    runCommand(USER_PORT_ACTIVE);
    runCommand(USER_PORT_DIRECTION);
}

int Button_readFromButton()
{
    FILE *pFile = fopen(USER_PORT, "r");
    if (pFile == NULL)
    {
        printf("ERROR: Unable to open file (%s) for read\n", USER_PORT);
        exit(-1);
    }

    // Read string (line)
    const int MAX_LENGTH = 5;
    char buff[MAX_LENGTH];
    fgets(buff, MAX_LENGTH, pFile);

    // Close
    fclose(pFile);
    if (buff[0] == '1')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}