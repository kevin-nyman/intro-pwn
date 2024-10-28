#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/personality.h>

void print_instructions()
{
    puts("== [ Exercise 08: Return-Oriented-Programming Introdcution ] ==");
    puts("In this exercise there is no \"win-function\" and no way to inject shellcode.");
    puts("However, we can piece together existing code and perform return-oriented programming.");
    puts("You need to use the provided gadgets to set up function arguments and jump to the final");
    puts("function which will give you a shell if all the values are correct.");
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

__attribute__((used)) void win(int x, int y, int z)
{
    int ok1 = 0;
    int ok2 = 0;
    int ok3 = 0;
    char command[16];
    if (x == 1337)
    {
        ok1 = 1;
        command[0] = '/';
        command[1] = 'b';
    }
    if (y == 24601)
    {
        ok2 = 1;

        command[2] = 'i';
        command[3] = 'n';
        command[4] = '/';
    }
    if (z == 555)
    {
        ok3 = 1;
        command[5] = 's';
        command[6] = 'h';
        command[7] = 0;
    }
    if (ok1 == 1 && ok2 == 1 && ok3 == 1)
    {
        char *argv[] = { command, NULL };
        execv(command, argv);
    }
}

__attribute__((used)) void gadget1()
{
#ifdef __x86_64__
    asm("pop %rdi; ret");
#elif __i386__
    asm("ret");
#elif __aarch64__
    asm("ldr x0, [sp], #8; ret");
#elif __arm__
    asm("ldr r0, [sp], #4; ret");
#else
#error "Unsupported architecture"
#endif
}

__attribute__((used)) void gadget2()
{
#ifdef __x86_64__
    asm("pop %rsi; ret");
#elif __i386__
    asm("ret");
#elif __aarch64__
    asm("ldr x1, [sp], #8; ret");
#elif __arm__
    asm("ldr r1, [sp], #4; ret");
#else
#error "Unsupported architecture"
#endif
}

__attribute__((used)) void gadget3()
{
#ifdef __x86_64__
    asm("pop %rdx; ret");
#elif __i386__
    asm("ret");
#elif __aarch64__
    asm("ldr x2, [sp], #8; ret");
#elif __arm__
    asm("ldr r2, [sp], #4; ret");
#else
#error "Unsupported architecture"
#endif
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
    if (0 == init())
    {
        return EXIT_FAILURE;
    }

    print_instructions();

    return exercise();
}
