#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/personality.h>

void print_instructions()
{
    puts("== [ Exercise 07: Stack Cookie Bypass ] ==");
    puts("In this exercise, stack cookies have been enabled.");
    puts("Fortunately, the cookie has been leaked thanks to a bug.");
    puts("Additionally, the \"win-function\" is back.");
    puts("Use the leaked cookie and the function to get a shell.");
}

#ifdef __x86_64__
#define canary_t uint64_t
#define INSN_READ "movq %%fs:0x28, %0;"

#elif __i386__
#define canary_t uint32_t
#define INSN_READ "movl %%gs:0x14, %0;"

#elif __aarch64__
#define canary_t uint64_t
#define INSN_READ "ldr x1, =__stack_chk_guard; ldr x1, [x1]; mov %0, x1;"

#elif __arm__
#define canary_t uint32_t
#define INSN_READ "ldr r0, =__stack_chk_guard; ldr r0, [r0]; mov %0, r0;"
#endif

canary_t read_canary()
{
    canary_t val = 0;

    __asm__(INSN_READ
            : "=r"(val)
            :
            :);

    return val;
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

__attribute__((used)) void win()
{
    printf("Congratulations! Have a shell.\n");
    char *argv[] = { "/bin/sh", NULL };
    execv("/bin/sh", argv);
    exit(EXIT_SUCCESS);
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
    printf("Through a bug in the program you happen to know that the stack cookie is %#lx\n", read_canary());
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
