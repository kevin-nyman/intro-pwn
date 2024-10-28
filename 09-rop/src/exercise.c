#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/personality.h>

void print_instructions()
{
    puts("== [ Exercise 09: ROP Chain Creation ] ==");
    puts("This exercise also uses return-oriented programming but this");
    puts("time it is a little bit more realistic. In the program, there is");
    puts("a call to system() present. There is also a gadget to set up");
    puts("the register correctly. You can also figure out the location of");
    puts("the stack which is useful for placing strings you want to use.");
    puts("Use these pieces together to get a shell.");
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

__attribute__((used)) void gadget1()
{
    system("echo \"You failed!\"");
    exit(EXIT_SUCCESS);
}

__attribute__((used)) void gadget2()
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
