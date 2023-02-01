#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include "top_info.h"

#define CLK_TCK   sysconf(_SC_CLK_TCK)

struct Mem_Swap getMemAndSwap(){
    struct Mem_Swap ms;
    FILE *fd=fopen("/proc/meminfo", "r");
    if(fd==NULL){
        printf("errore nell'apertura del file /proc/meminfo\n");
        exit(1);
    }
    char *word=(char *) malloc(sizeof(char)*20);
    while(fscanf(fd,"%s",word)!=EOF){
        if(strcmp(word,"MemTotal:")==0){
            fscanf(fd,"%s",word);
            ms.MemTotal=atoi(word);
        }
        else if(strcmp(word,"MemFree:")==0){
            fscanf(fd,"%s",word);
            ms.MemFree=atoi(word);
        }
        else if(strcmp(word,"MemAvailable:")==0){
            fscanf(fd,"%s",word);
            ms.MemDisp=atoi(word);
        }
        else if(strcmp(word,"Cached:")==0){
            fscanf(fd,"%s",word);
            ms.MemCache=atoi(word);
        }
        else if(strcmp(word,"SwapTotal:")==0){
            fscanf(fd,"%s",word);
            ms.SwapTotal=atoi(word);
        }
        else if(strcmp(word,"SwapFree:")==0){
            fscanf(fd,"%s",word);
            ms.SwapFree=atoi(word);
            break;
        }
    }
    ms.MemUsed=ms.MemTotal-ms.MemDisp;
    ms.SwapUsed=ms.SwapTotal-ms.SwapFree;
    
    free(word);
    fclose(fd);

    return ms;
}

char* getPath(char *pid, char *s){
    char *path;
    path=(char *) malloc(sizeof(char)*strlen("/proc")+strlen(pid)+strlen(s)+2);
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
        printf("errore nell'apertura del file /proc/%s/stat\n",pid);
        exit(1);
    }

    float utime,stime,cutime,cstime,starttime;
    int i=1,priority,nice;
    char *word=(char *) malloc(sizeof(char)*50);
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

struct COMMAND_S_USER_VIRT_RES getCSUVR(char *pid){
    struct COMMAND_S_USER_VIRT_RES csvr;
    char *path=getPath(pid,"/status");
    FILE *fd=fopen(path, "r");
    if(fd==NULL){
        printf("errore nell'apertura del file /proc/%s/status\n",pid);
        exit(1);
    }

    char *word=(char *) malloc(sizeof(char)*40);
    struct passwd* user_pwuid;
    while(fscanf(fd,"%s",word)!=EOF){
        if(strcmp(word,"Name:")==0){
            fscanf(fd,"%s",word);
            strcpy(csvr.name,word);
        }
        else if(strcmp(word,"State:")==0){
            fscanf(fd,"%s",word);
            strcpy(csvr.state,word);
        }
        else if(strcmp(word,"Uid:")==0){
            fscanf(fd,"%s",word);
            user_pwuid=getpwuid(atoi(word));
            strcpy(csvr.user,user_pwuid->pw_name);
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
        else if(strcmp(word,"Threads:")==0){
            csvr.VmSize=0;
            csvr.VmHWM=0;
            break;
        }
        //strcpy(word,"");
    }

    free(word);
    free(path);

    fclose(fd);

    return csvr;
}

char* getState(char *pid){
    char *path=getPath(pid,"/stat");
    FILE *fd=fopen(path, "r");
    if(fd==NULL){
        printf("errore nell'apertura del file /proc/%s/stat\n",pid);
        exit(1);
    }

    char *word=(char *) malloc(sizeof(char)*50);
    fscanf(fd,"%s",word);
    fscanf(fd,"%s",word);
    fscanf(fd,"%s",word);

    free(path);

    fclose(fd);

    return word;
}