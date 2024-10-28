#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/personality.h>

void print_instructions()
{
    puts("== [ Exercise 04: Shellcode Injection ] ==");
    puts("In this exercise there is no conventient function to ");
    puts("divert the control flow to. Instead you must place shellcode");
    puts("on the stack and jump to it so that it is executed.");
    puts("The goal is to do this to get a shell.");
}

#define NAME_SIZE 128

int setup_stack(char **argv)
{
    const int old_personality = personality(ADDR_NO_RANDOMIZE);
    if (!(old_personality & ADDR_NO_RANDOMIZE))
    {
        const int new_personality = personality(ADDR_NO_RANDOMIZE);
        if (new_personality & ADDR_NO_RANDOMIZE)
        {
            execv(argv[0], argv);
        }
        else
        {
            fprintf(stderr, "Failed to disable ASLR. Exiting.\n");
            return 0;
        }
    }

    return 1;
}

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

int init(char **argv)
{
    if (0 == setup_stack(argv))
    {
        return 0;
    }

    if (0 == setup_buffering())
    {
        return 0;
    }

    return 1;
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

int main(int argc, char **argv)
{
    if (0 == init(argv))
    {
        return EXIT_FAILURE;
    }

    print_instructions();

    return exercise();
}
