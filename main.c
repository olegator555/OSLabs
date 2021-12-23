#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

typedef void (*sighandler)(int);
void brake();
jmp_buf pos;
static char digit[3];
int num,num_s;
static char* who[] = { "Com:    ", "Man:    ", "You lose\n", "You win \n" };
static char cube[] = {'|','/','-','\\'};

int main(int argc, char** argv) {
    int timeout;
    int j;
    int ci = 0;
    if(argc < 2) {
        write(2, "Usage: dice timeout\n", 20);
        exit(0);
    } /* if */
    if((timeout = atoi(argv[1])) < 1)
        timeout = 1;
    digit[0] = cube[ci];
    digit[1] = '\b';
    digit[2] = '\n';
    num_s = 0;
    num = getpid() % 6;
    signal(SIGALRM, (sighandler) brake);
    signal(SIGINT, SIG_IGN);
    if((j = setjmp(pos)) == 0)
        alarm(timeout);
    else
        alarm(0);

    write(1, who[j], 9);
    while(j < 2) {
        write(1, digit, 2);
        digit[0] = cube[++ci % 4];
        num = (num+1)%6;
        usleep(50000);
    }
    signal(SIGINT, SIG_DFL);
    exit(0);
}

void brake() {
    static int j = 0;
    signal(SIGINT, (sighandler) brake);
    digit[0] = num+'1';
    num_s += num;
    if(j == 1) {
        char ctrlc[] = "\b \b\b";
        write(1, ctrlc, 5);
    }
    write(1, digit, 3);
    if(j == 1)
        longjmp(pos, num_s%2?3:2);
    longjmp(pos, ++j);
}
