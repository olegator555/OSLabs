#include <unistd.h>
#include <stdio.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <sys/wait.h>

void clrscr() {
    unsigned char esc[11];
    esc[0] = 27; esc[1] = '[';
    esc[2] = 'H';
    write(1,esc,3);
    esc[2] = '2'; esc[3] = 'J';
    write(1,esc,4);
}
void gotoxy(int tx, int ty, char c) {
    unsigned char esc[16];
    static unsigned char ystr[3];
    static unsigned char xstr[3];
    int i;
    int j;

    if((tx > 99) || (ty > 99))
        tx = ty = 99;
    if((tx < 1) || (ty < 1))
        tx = ty = 1;
    xstr[0] = xstr[1] = xstr[2] = '\0';
    ystr[0] = ystr[1] = ystr[2] = '\0';

    sprintf((char *) xstr,"%d",tx);
    sprintf((char *) ystr,"%d",ty);

    esc[0] = 27; esc[1] = '[';
    i=2; j=0;
    while(ystr[j])
        esc[i++]=ystr[j++];
    j=0;
    esc[i++]=';';
    while(xstr[j])
        esc[i++]=xstr[j++];
    esc[i++]='H';
    esc[i++]='\b';
    esc[i++]=' ';
    esc[i++] = c;
    esc[i] = '\0';
    write(1,esc,i);
}

int main(int argc, char* argv[]) {

    if(argc < 2) {
        puts("Using: ./[distance] [offset] [number_of_process]");
        return 1;
    }

    unsigned long PROCNUM = 4;
    if(argc >= 3) PROCNUM = atoi(argv[2]);

    int x = 1;
    int status;
    int i;
    int j = 0;
    int dist = atoi(argv[0]+2);
    int offset = atoi(argv[1]);
    int* pid = (int*)sbrk((PROCNUM+1)*sizeof(int));
    char* lead = (char*)sbrk((PROCNUM+1)*sizeof(char));
    int p;
    char bell = '\007';
    struct timeb tp[1];
    clrscr();
    while(j < PROCNUM) {
        if((pid[j] = fork()) == 0) {
            usleep(PROCNUM - j);
            while(x <= dist) {
                if(x<=2) gotoxy(offset, j + 1, '|');
                gotoxy(x+offset, j + 1, 'A' + j);
                ftime(tp);
                if((tp[0].millitm % (j + 'A')) != j)
                    continue;
                x++;
                for(i=0; i<1000000; i++);
            }
            exit('A' + j);
        }
        j++;
    }

    j = 0;
    while((p = wait(&status)) != (-1)) {
        for(i = 0; i < PROCNUM; i++)
            if(pid[i] == p)
                lead[j++] = (char) ((status >> 8) & '\377');
        write(1,&bell,1);
    }
    lead[j] = '\n';
    sleep(1);
    gotoxy(1, PROCNUM + 3, '\n');
    write(1, lead, PROCNUM + 1);
    exit(0);
}
