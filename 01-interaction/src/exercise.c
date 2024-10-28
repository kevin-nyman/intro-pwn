#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TIMELIMIT 3

void print_instructions()
{
    puts("== [ Exercise 01: Interaction ] ==");
    puts("In this exercise you will practice how to automate");
    puts("interaction with a program using pwntools");
    puts("The program will print a number and you need");
    puts("to send parse the number, double it and send it back");
    printf("within %d seconds to solve the exercise.\n", TIMELIMIT);
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

int seed_rand()
{
    FILE *hdl_rand = fopen("/dev/urandom", "r");
    if (hdl_rand == NULL)
    {
        perror("Failed to open /dev/urandom");
        return 0;
    }

    unsigned int seed;
    if (1 != fread((void *)&seed, sizeof(seed), 1, hdl_rand))
    {
        perror("Failed to read /dev/urandom");
        if (0 != fclose(hdl_rand))
        {
            perror("Failed to close file handle");
        }
        return 0;
    }

    srand(seed);

    if (0 != fclose(hdl_rand))
    {
        perror("Failed to close file handle");
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

    if (0 == seed_rand())
    {
        return 0;
    }

    return 1;
}

int get_int(int *value)
{
    char *lineptr = NULL;
    size_t linesize = 0;
    if (-1 == getline(&lineptr, &linesize, stdin))
    {
        free(lineptr);
        perror("Failed to read line");
        return 0;
    }

    if (1 != sscanf(lineptr, "%d", value))
    {
        free(lineptr);
        perror("Failed to get integer input");
        return 0;
    }

    free(lineptr);
    return 1;
}

int exercise()
{
    time_t t0 = time(NULL);
    int rand_val1 = rand();
    printf("The starting value is: %d\n", rand_val1);
    printf("Please give me twice the value back: ");

    int input1;
    if (0 == get_int(&input1))
    {
        return EXIT_FAILURE;
    }
    time_t t1 = time(NULL);

    if ((t1 - t0) > TIMELIMIT)
    {
        printf("Too slow. Goodbye!\n");
        return EXIT_SUCCESS;
    }

    if (input1 != 2 * rand_val1)
    {
        printf("Incorrect value. Goodbye!\n");
        return EXIT_SUCCESS;
    }

    printf("Correct value! Congratulations\n");
    system("/bin/sh");

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
