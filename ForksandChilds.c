
#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int *num = calloc(sizeof(int), 6);

    // Parsing array from terminal
    for (int i = 0; i < argc - 2; i++)
    {
        num[i] = atoi(argv[i + 2]);
    }

    // How many child processes there are
    int childProcesses = atoi(argv[1]);

    // Declaring pipes and file descriptor
    int fileDescriptor[childProcesses][2];
    pipe(fileDescriptor[0]);

    pid_t parentPID, childPID, child2PID, child3PID;
    parentPID = getpid();

    printf("I am parent with PID: %i sending array: ", parentPID);
    for (int i = 0; i < argc - 2; ++i)
    {
        printf("%i ", num[i]);
    }
    printf("\n");

    int arraySize = 6 / childProcesses;

    int fork1, fork2, fork3;
    fork1 = fork();

    if (fork1 > 0)
    // Parent Process
    {
        if (childProcesses > 1)
        {
            pipe(fileDescriptor[1]);
            fork2 = fork();

            if (fork2 != 0)
            {
                // Still parent

                if (childProcesses > 2)
                {
                    pipe(fileDescriptor[2]);
                    fork3 = fork();
                    if (fork3 == 0)
                    {
                        // Child Process
                        child3PID = getpid();
                        printf("I am child with PID: %i adding array ", child3PID);

                        // Close read (not needed)
                        close(fileDescriptor[2][0]);

                        int startIndex = arraySize * 2;
                        int endIndex = arraySize * 3;
                        int sum = 0;

                        for (int k = startIndex; k < endIndex; ++k)
                        {
                            printf("%i ", num[k]);
                        }

                        for (int k = startIndex; k < endIndex; ++k)
                        {
                            sum += num[k];
                        }
                        printf("and sending partial sum of: %d\n", sum);

                        // Write value
                        write(fileDescriptor[2][1], &sum, sizeof(int));

                        // Close write from second pipe
                        close(fileDescriptor[2][1]);

                        exit(0);
                    }
                }
            }
            else
            {
                // Fork (child) 2 process
                child2PID = getpid();

                // Close read (not needed)
                close(fileDescriptor[1][0]);

                // Child Process
                printf("I am child with PID: %i, adding array ", child2PID);

                int startIndex = arraySize;
                int endIndex = arraySize * 2;
                int sum = 0;

                for (int k = startIndex; k < endIndex; ++k)
                {
                    printf("%i ", num[k]);
                }

                for (int k = startIndex; k < endIndex; ++k)
                {
                    sum += num[k];
                }
                printf("and sending partial sum of: %d\n", sum);

                // Write value
                write(fileDescriptor[1][1], &sum, sizeof(int));

                // Close write from second pipe
                close(fileDescriptor[1][1]);
                exit(0);
            }
        }
    }
    else if (fork1 == 0)
    {
        // Child Process
        childPID = getpid();

        // Close read from first pipe, (not needed)
        close(fileDescriptor[0][0]);

        printf("I am child with PID: %i adding array ", childPID);

        int startIndex = 0;
        int endIndex = arraySize;
        int sum = 0;

        for (int k = startIndex; k < endIndex; ++k)
        {
            printf("%i ", num[k]);
        }

        for (int k = startIndex; k < endIndex; ++k)
        {
            sum += num[k];
        }
        printf("and sending partial sum of: %d\n", sum);

        // Write value
        write(fileDescriptor[0][1], &sum, sizeof(int));

        // Close write from second pipe
        close(fileDescriptor[0][1]);
        exit(0);
    }

    // Computing the sum for parent
    // Wait for children using waitpid()
    waitpid(fork1, NULL, 0);
    if (childProcesses > 1)
    {
        waitpid(fork2, NULL, 0);
        if (childProcesses == 3)
        {
            waitpid(fork3, NULL, 0);
        }
    }
    printf("I am the parent and have the pid of %i and got partial reults ", parentPID);

    int finalSum = 0;

    for (int i = 0; i < childProcesses; ++i)
    {
        int partialSum;
        close(fileDescriptor[i][1]);
        read(fileDescriptor[i][0], &partialSum, sizeof(int));
        printf("%i ", partialSum);
        close(fileDescriptor[i][0]);
        finalSum += partialSum;
    }

    printf("final result is %i. \n", finalSum);

    return 0;
}
