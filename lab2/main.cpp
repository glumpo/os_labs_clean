#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define READ 0
#define WRITE 1

const int FAILURE_CREATE_PID = -1;
const int FILE_EXIST = 0;
const int FAILED_TO_OPEN_FILE = -1;
const int DEFAULT_COUNT_FILES = 2;
const int MAX_SIZE_COMMAND_LINE = 100;
const char COMAND_EXIT[] = "DIE\n";

void execute(char *cmd, char *filename);

int fd[2];

int main(int argc, char *argv[]) {
    char cmd[MAX_SIZE_COMMAND_LINE];
    while (true) {
        printf("> ");
        fgets(cmd, MAX_SIZE_COMMAND_LINE, stdin);
        if (strcmp(cmd, COMAND_EXIT) == 0) {
            printf("Dieing...\n");
            break;
        }
        execute(cmd, "output");
    }
    return 0;
}

void execute(char *command, char *fileName) {
    int outFileID;
    pid_t pidChild;
    pipe(fd);
    int status;
    pidChild = fork();
    if (pidChild == FAILURE_CREATE_PID) {
        perror("Child process not created");
        return;
    }

    //------fd[0] - read
    //------fd[1] - write

    int sizeStr;
    if (pidChild == 0) {
        close(STDOUT_FILENO);
        dup2(fd[WRITE], STDOUT_FILENO);

        close(fd[WRITE]);
        close(fd[READ]);

        execl("/bin/bash", "/bin/bash", "-c", command, (char *) 0);
        perror("Command not perfom\n\n");
    } else {
        wait(&status);
        if (access(fileName, F_OK) == FILE_EXIST) {
            outFileID = open(fileName, O_APPEND | O_RDWR | S_IRUSR | S_IWUSR);
        } else {
            outFileID = open(fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
        }
        if (outFileID == FAILED_TO_OPEN_FILE) {
            perror("WTF with that file");
            exit(EXIT_FAILURE);
        }

        write(fd[WRITE], "\0", sizeof(char));
        close(fd[WRITE]);
        char answer[MAX_SIZE_COMMAND_LINE];

        sizeStr = read(fd[READ], answer, MAX_SIZE_COMMAND_LINE*sizeof(char));
        write(outFileID, answer, sizeStr);
        close(outFileID);
    }
    return;
}
