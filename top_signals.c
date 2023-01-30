#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include "top_signals.h"

void help(){
    printf("\033[0;32m");
    printf("  I comandi che puoi eseguire all'interno del programma sono:\n\n");
    printf("  s:    sospende il processo\n");
    printf("  r:    riprende il processo sospeso\n");
    printf("  t:    termina il processo\n");
    printf("  k:    esegue la kill del processo\n");
    printf("  u:    ricarica il programma\n");
    printf("  q:    esce dal programma\n");
    printf("\033[0m");
}

void terminate(char *pid){
    int p=atoi(pid);
    int res=kill(p,SIGTERM);
    if(res==-1){
        if (errno==EPERM) printf("  Non hai i permessi necessari per terminare il processo con PID %d\n",p);
        else if (errno == ESRCH) printf("  Il processo con PID %d non esiste\n",p);
    }
    else printf("  il processo con PID %d è terminato con successo\n",p);
}

void kill_proc(char *pid){
    int p=atoi(pid);
    int res=kill(p,SIGKILL);
    if(res==-1){
        if (errno==EPERM) printf("  Non hai i permessi necessari per eseguire la kill del processo con PID %d\n",p);
        else if (errno == ESRCH) printf("  Il processo con PID %d non esiste\n",p);
    }
    else printf("  il processo con PID %d è terminato con successo\n",p);
}

void resume(char *pid){
    int p=atoi(pid);
    int res=kill(p,SIGCONT);
    if(res==-1){
        if (errno==EPERM) printf("  Non hai i permessi necessari per riprendere il processo con PID %d\n",p);
        else if (errno == ESRCH) printf("  Il processo con PID %d non esiste\n",p);
    }
    else printf("  il processo con PID %d è ripartito con successo\n",p);
}

void suspend(char *pid){
    int p=atoi(pid);
    int res=kill(p,SIGSTOP);
    if(res==-1){
        if (errno==EPERM) printf("  Non hai i permessi necessari per sospendere il processo con PID %d\n",p);
        else if (errno == ESRCH) printf("  Il processo con PID %d non esiste\n",p);
    }
    else printf("  il processo con PID %d è stato sospeso con successo\n",p);
}