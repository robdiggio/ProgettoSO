#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "top_info.h"

#define CLK_TCK   sysconf(_SC_CLK_TCK)

int getMemOrSwap(char line[]){
    char *mem=(char *) malloc(sizeof(char));
    int j=0;
    for(int i=0;i<30;i++){
        if(isdigit(line[i])) {
        mem[j]=line[i];
        j++;
        }
    }
    int x=atoi(mem);
    free(mem);
    return x;
}

char* getPath(char *pid, char *s){
    char *path=(char *) malloc(sizeof(char));
    strcat(path, "/proc/");
    strcat(path, pid);
    strcat(path, s);
    return path;
}

void getCpuUsage(char *pid,int uptime){
    char *s="/stat";
    char *path=getPath(pid,s);
    FILE *fd=fopen(path, "r");
    if(fd==NULL){
        printf("errore nell'apertura del file");
        exit(1);
    }

    float utime,stime,cutime,cstime,starttime;
    int i=1;
    char *x=(char *) malloc(sizeof(char));
    while(fscanf(fd,"%s",x)!=EOF){
        if(i==14) utime=atoi(x);
        else if(i==15) stime=atoi(x);
        else if(i==16) cutime=atoi(x);
        else if(i==17) cstime=atoi(x);
        else if(i==22) starttime=atoi(x);
        i++;
    }
    free(x);
    free(path);

    float total_time=utime+stime+cutime+cstime;
    total_time=total_time/CLK_TCK;
    float elapsed=uptime-(starttime/CLK_TCK);
    float cpu_usage=100*(total_time/elapsed);
    printf("%f\n",cpu_usage);
}