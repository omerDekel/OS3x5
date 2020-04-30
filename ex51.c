// Created by omer on 19/06/18.
//
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <signal.h>

#define STDERR_FD 2
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define MOVE_DOWN 's'
#define MOVE_UP 'w'
#define QUIT 'q'


char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0 , &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0 , TCSANOW , &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0 , &buf , 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0 , TCSADRAIN , &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

/**
 * prints error in sys call to stderr.
 */
void printErrorInSysCallToSTDERR() {
    char error_msg[] = "Error in stsyem call\n";
    write(STDERR_FD , error_msg , sizeof(error_msg));
}

int main() {
    char ch = 0;
    int fd[2];
    pipe(fd);
    pid_t  pid;
    pid = fork();
    char* args[] = {"./draw.out", NULL};
    //child
    if( pid == 0){
        dup2(fd[0], 0);
        execvp(args[0], args);
        printf("gere\n");
        printErrorInSysCallToSTDERR();
        //parent
    } else {

        while (ch!= 'q') {
            ch = getch();
            if (ch== QUIT || ch==MOVE_DOWN || ch == MOVE_LEFT || ch == MOVE_RIGHT || ch == MOVE_UP ) {
                write(fd[1],&ch,1);
                kill(pid,SIGUSR2);
            }

        }
        return 0;
    }
    return 0;
}
