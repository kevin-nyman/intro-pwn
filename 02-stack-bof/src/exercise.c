#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

void print_instructions()
{
    puts("== [ Exercise 02: Stack Buffer Overflow ] ==");
    puts("In this exercise you simply need to cause a buffer overflow");
    puts("and corrupt the stack which will lead to a segmentation fault");
    puts("and give you a shell.");
}

#define NAME_SIZE 128

#ifdef UNUSED
#elif defined(__GNUC__)
#define UNUSED(x) UNUSED_##x __attribute__((unused))
#elif defined(__LCLINT__)
#define UNUSED(x) /*@unused@*/ x
#else
#define UNUSED(x) x
#endif

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

void sigsegv_handler(int UNUSED(sig), siginfo_t *si, void *UNUSED(unused))
{
    printf("Got SIGSEGV at address: %p\n", si->si_addr);
    printf("Congratulations! You triggered a buffer overflow. Have a shell.\n");
    system("/bin/sh");
    exit(EXIT_SUCCESS);
}

int setup_sig_handler()
{
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    action.sa_sigaction = sigsegv_handler;
    if (sigaction(SIGSEGV, &action, NULL) == -1)
    {
        perror("Failed to install signal handler");
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

    if (0 == setup_sig_handler())
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

int main()
{
    if (0 == init())
    {
        return EXIT_FAILURE;
    }

    print_instructions();

    return exercise();
}
