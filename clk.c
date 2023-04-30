/*
 * This file is done for you.
 * Probably you will not need to change anything.
 * This file represents an emulated clock for simulation purpose only.
 * It is not a real part of operating system!
 */

#include "headers.h"
#include <time.h>

int shmid;

/* Clear the resources before exit */
void cleanup(int signum)
{
    shmctl(shmid, IPC_RMID, NULL);
    printf("[CLK] terminating!\n");
    exit(0);
}

/* This file represents the system clock for ease of calculations */
int main(int argc, char * argv[])
{

#ifdef OUT_TO_FILE
    SYNC_IO;
#endif

    printf("[CLK] starting\n");
    signal(SIGINT, cleanup);
    int clk = 0;
    //Create shared memory for one integer variable 4 bytes
    shmid = shmget(SHKEY, 4, IPC_CREAT | 0644);
    if ((long)shmid == -1)
    {
        perror("Error in creating shm!");
        exit(-1);
    }
    int * shmaddr = (int *) shmat(shmid, (void *)0, 0);
    if ((long)shmaddr == -1)
    {
        perror("Error in attaching the shm in clock!");
        exit(-1);
    }
    *shmaddr = clk; /* initialize shared memory */

    usleep(CLK_START_DELAY_MS * 1000);
    
    while (1)
    {
        //sleep(1);
        usleep(CLK_MS * 1000);
        (*shmaddr)++;
    }
}
