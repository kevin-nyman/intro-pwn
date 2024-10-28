#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/personality.h>

void print_instructions()
{
    puts("== [ Exercise 05: ASLR Bypass with Leak ] ==");
    puts("This exercise is similar to the previous one, but ASLR is enabled.");
    puts("Fortunately, there is a memory leak which allows you to deduce the");
    puts("location of a stack variable. With this you can compute where your");
    puts("shellcode will be located and jump to it. Use this to execute shellcode,");
    puts("just like in the previous exercise, and get a shell.");
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

int exercise()
{
    char name[NAME_SIZE];
    printf("Through a bug in the program you happen to know that &name == %p\n", (void *)&name);
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
    if (0 == init())
    {
        return EXIT_FAILURE;
    }

    print_instructions();

    return exercise();
}
