#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "top_info.h"

#define CLK_TCK   sysconf(_SC_CLK_TCK)

int getMemOrSwap(char line[]){
    char *mem=(char *) malloc(sizeof(char)*30);
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
    char *path;
    if(strcmp(s,"/status")==0) path=(char *) malloc(sizeof(char)*strlen("proc")+strlen(pid)+strlen("/status"));
    else if(strcmp(s,"/stat")==0) path=(char *) malloc(sizeof(char)*strlen("proc")+strlen(pid)+strlen("/stat"));
    strcpy(path,"");
    strcat(path, "/proc/");
    strcat(path, pid);
    strcat(path, s);
    return path;
}

struct CPU_PR_NI getCPN(char *pid,int uptime){
    struct CPU_PR_NI cpn;
    char *path=getPath(pid,"/stat");
    FILE *fd=fopen(path, "r");
    if(fd==NULL){
        printf("errore nell'apertura del file /proc/%s/stat",pid);
        exit(1);
    }

    float utime,stime,cutime,cstime,starttime;
    int i=1,priority,nice;
    char *word=(char *) malloc(sizeof(char));
    while(fscanf(fd,"%s",word)!=EOF){
        if(i==14) utime=atoi(word);
        else if(i==15) stime=atoi(word);
        else if(i==16) cutime=atoi(word);
        else if(i==17) cstime=atoi(word);
        else if(i==18) priority=atoi(word);
        else if(i==19) nice=atoi(word);
        else if(i==22) starttime=atoi(word);
        i++;
    }
    free(word);
    free(path);

    float total_time=utime+stime+cutime+cstime;
    total_time=total_time/CLK_TCK;
    float elapsed=uptime-(starttime/CLK_TCK);
    float cpu_usage=100*(total_time/elapsed);
    fclose(fd);

    cpn.cpu_usage=cpu_usage;
    cpn.priority=priority;
    cpn.nice=nice;
    return cpn;
}

struct COMMAND_S_VIRT_RES getCSVR(char *pid){
    struct COMMAND_S_VIRT_RES csvr;
    char *path=getPath(pid,"/status");
    FILE *fd=fopen(path, "r");
    if(fd==NULL){
        printf("errore nell'apertura del file /proc/%s/status",pid);
        exit(1);
    }

    char *word=(char *) malloc(sizeof(char)*20);
    strcpy(csvr.name,"");
    strcpy(csvr.state,"");
    while(fscanf(fd,"%s",word)!=EOF){
        if(strcmp(word,"Name:")==0){
            fscanf(fd,"%s",word);
            strcat(csvr.name,word);
        }
        else if(strcmp(word,"State:")==0){
            fscanf(fd,"%s",word);
            strcat(csvr.state,word);
        }
        else if(strcmp(word,"VmSize:")==0){
            fscanf(fd,"%s",word);
            csvr.VmSize=atoi(word);
        }
        else if(strcmp(word,"VmHWM:")==0){
            fscanf(fd,"%s",word);
            csvr.VmHWM=atoi(word);
            break;
        }
    }

    free(word);
    free(path);

    fclose(fd);

    return csvr;
}