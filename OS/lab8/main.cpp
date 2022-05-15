#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

struct sembuf sem[2];
char str[100];

void Send(){
    if (sem[0].sem_op == 0) { // произвести данные
        printf("Input string: ");
        fgets(str, 16, stdin);
        sem[0].sem_op = 1;
    }
}

void Receive(){
    if (sem[0].sem_op == 1) { // потербить данные
        if (str[0] == '\0' || str[0] == '\n') {
            printf("Empty string received. Stopping...\n");
            sem[1].sem_op = 1;
        } else {
            printf("Received string: %s\n", str);
            sem[0].sem_op = 0;
        }
    }

}

int main (int argc, char **argv) {
	int semid; // идентификатор семафора

	semid = semget(IPC_CREAT, 2, IPC_CREAT);
	if (semid < 0) {
		printf("Error - %s\n", strerror(errno));
		_exit(1);
	}

	sem[0].sem_num = 0;
	sem[1].sem_num = 1;
	sem[0].sem_flg = SEM_UNDO;
	sem[1].sem_flg = SEM_UNDO;

	sem[0].sem_op = 0;

	while(sem[0].sem_op == 0) {
		Send();
        Receive();
	}
	printf("Stopped \n");
	semctl(semid,0,IPC_RMID); // semctl - выполняет операции, определенные в cmd IPC_RMID - Помечает сегмент для удаления.
}