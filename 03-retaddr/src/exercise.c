#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void print_instructions()
{
    puts("== [ Exercise 03: Retturn Address Overwrite ] ==");
    puts("In this exercise, you must not just corrupt the return address");
    puts("but actually overwrite it with something meaningful to dictate");
    puts("the control flow. There is a \"win-function\" provided for you.");
    puts("By overwriting the return address with the address of this function,");
    puts("the program will execute that function and give you a shell.");
}

#define NAME_SIZE 128

int setup_buffering()
{
    if (0 != setvbuf(stdin, NULL, _IONBF, 0))
    {
        perror("Failed to disable stdin buffering");
        return 0;
    }
    if (0 != setvbuf(stdout, NULL, _IONBF, 0))
    {
        perror("Failed to disable stdout buffering");
        return 0;
    }

    return 1;
}

int init()
{
    if (0 == setup_buffering())
    {
        return 0;
    }

    return 1;
}

__attribute__((used)) void win()
{
    printf("Congratulations! Have a shell.\n");
    char *argv[] = { "/bin/sh", NULL };
    execv("/bin/sh", argv);
    exit(EXIT_SUCCESS);
}

int exercise()
{
    char name[NAME_SIZE];
    printf("Hello! What is your name? ");

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstringop-overflow"
    char *res = fgets(name, 2 * NAME_SIZE, stdin);
#pragma GCC diagnostic pop

    if (res == NULL)
    {
        perror("Failed to read input");
        return EXIT_FAILURE;
    }

    size_t input_len = strlen(name);
    if (name[input_len - 1] == '\n')
    {
        name[input_len - 1] = 0;
    }

    printf("Hello %s! Nice to meet you!\n", name);

    return EXIT_SUCCESS;
}

int main()
{
    if (0 == init())
    {
        return EXIT_FAILURE;
    }

    print_instructions();

    return exercise();
}
