#include <unistd.h> // подключаем, чтобы использовать - fork, execl
#include <stdio.h>
#include <sys/wait.h>	// подключаем, чтобы использовать - WEXITSTATUS, wait

int status_info;
int pid_child;

void create_new_procc() {
    pid_child = fork(); // создаем новый процесс
    if (pid_child > 0) { // если parrent процесс
        wait(&status_info);   // ждем завершение child процесса
        if (WEXITSTATUS(status_info)==0)  //
            printf("parrent: child процесс успешно завершен.\n");
        else {
            printf("parrent: child процесс завис, перезагрузка..\n");
            create_new_procc();
        }
    }
    else if(pid_child==0) {  // если child процесс
        printf("parrent: запускаю child процесс. Ожидание...\n");
        execlp("./child.out","child",NULL);
    }
    else
        printf("parrent: Ошибка: не удалось запустить процесс.\n");
}

int main() {
    printf("parrent: запущен parrent процесс.\n");
    create_new_procc();
    return 0;
}