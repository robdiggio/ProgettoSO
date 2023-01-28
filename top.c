#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "top_info.h"

int main() {
  //INFO GENERALI DI TOP (PRIMA PARTE)
  //system("uptime");

  //lettura del file meminfo
  FILE *fd;
  fd=fopen("/proc/meminfo", "r");
  if(fd==NULL){
    printf("errore nell'apertura del file");
    exit(1);
  }

  //RAM
  struct Mem{
    int total;
    int free;
    int disp;
    int used;
    int cache;
  } Mem;
  char x[30];

  //memoria totale
  //char tot[30],t[10];
  //fscanf(fd, "%s %s", tot,t);
  //printf("%s\n",t);
  fgets(x,30,fd);
  Mem.total=getMemOrSwap(x);

  //memoria libera
  fgets(x,30,fd);
  Mem.free=getMemOrSwap(x);

  //memoria disponibile
  fgets(x,30,fd);
  Mem.disp=getMemOrSwap(x);

  //memoria usata
  Mem.used=Mem.total-Mem.disp;

  //memoria cache
  fgets(x,30,fd);
  fgets(x,30,fd);
  Mem.cache=getMemOrSwap(x);

  //Swap
  struct Swap{
    int total;
    int free;
    int used;
  } Swap;

  //swap totale
  for(int i=0;i<10;i++)
    fgets(x,30,fd);
  Swap.total=getMemOrSwap(x);

  //swap libero
  fgets(x,30,fd);
  Swap.free=getMemOrSwap(x);

  //swap usato
  Swap.used=Swap.total-Swap.free;

  //lettura del file uptime
  fclose(fd);
  fd=fopen("/proc/uptime", "r");
  if(fd==NULL){
    printf("errore nell'apertura del file");
    exit(1);
  }
  int uptime=0;
  fscanf(fd, "%d", &uptime);
  fclose(fd);

  //directory proc
  DIR *dir;
  struct dirent *dirent;

  dir=opendir("/proc");
  if(dir==NULL){
    if(errno==EACCES) printf("permesso negato per la cartella /proc \n");
    else if(errno==ENAMETOOLONG) printf("nome della cartella troppo lungo \n");
    else printf("errore nell'apertura della directory proc");
    exit(1);
  }

  //acquisizione delle varie informazione sulla memoria e sui processi
  while((dirent=readdir(dir))!=NULL && strcmp(dirent->d_name,"thread-self")!=0) {}
  char *state;
  int tot=0,tot_sleep=0,tot_running=0,tot_zombie=0,tot_stopped=0;
  while((dirent=readdir(dir))!=NULL){
    char *pid=dirent->d_name;
    state=getState(pid);
    if(strcmp(state,"S")==0) tot_sleep++;
    else if(strcmp(state,"R")==0) tot_running++;
    else if(strcmp(state,"Z")==0) tot_zombie++;
    else if(strcmp(state,"T")==0) tot_stopped++;
    tot++;
    free(state);
  }

  //stampa a video delle informazioni generali
  system("uptime");

  printf("Task: %d total,  %d running,  %d sleeping,  %d stopped,  %d zombie\n",tot,tot_running,tot_sleep,tot_stopped,tot_zombie);

  printf("kB Mem :   %d total,   %d free,   %d used,   %d buff/cache\n",Mem.total,Mem.free,Mem.used,Mem.cache);

  printf("kB Swap:   %d total,   %d free,   %d used,   %d avail Mem\n\n",Swap.total,Swap.free,Swap.used,Mem.disp);

  //stampa a video delle informazioni sui processi
  rewinddir(dir);
  while((dirent=readdir(dir))!=NULL && strcmp(dirent->d_name,"thread-self")!=0) {}
  struct CPU_PR_NI cpn;
  struct COMMAND_S_USER_VIRT_RES csuvr;

  //printf("\033[1;31m");
  printf("\033[30m\033[47m");
  printf("  PID    USER          PR      NI       VIRT      RES   S      CPU     MEM      COMMAND\n");
  printf("\033[0m\033[K\n");
  
  while((dirent=readdir(dir))!=NULL){
    char *pid=dirent->d_name;
    cpn=getCPN(pid,uptime);
    csuvr=getCSUVR(pid);
    float mem=100*(csuvr.VmSize/Mem.total);
    printf("  %s\t%s\t\t%d\t%d\t%d\t%d\t%s\t%.1f\t%.1f\t%s\n",pid,csuvr.user,cpn.priority,cpn.nice,csuvr.VmSize,csuvr.VmHWM,csuvr.state,cpn.cpu_usage,mem,csuvr.name);
  }


  
  //printf("i valori del CPN sono: \n%f\n%d\n%d\n",cpn.cpu_usage,cpn.priority,cpn.nice);
  
  //printf("i valori del CSVR sono: \n%s\n%s\n%s\n%d\n%d\n",csuvr.name,csuvr.state,csuvr.user,csuvr.VmSize,csuvr.VmHWM);

  closedir(dir);
  /*
  while((dir_p=readdir(dr))!=NULL)
    printf("%s\n", dir_p->d_name);
  closedir(dr);
  */

  //time
  /*
  time_t now;
  struct tm *now_tm;

  now=time(NULL);
  now_tm=localtime(&now);
  printf("the hour is %d\n", now_tm->tm_hour);
  printf("the minutes is %d\n", now_tm->tm_min);
  */


  //lettura da file
  /*
  while(!feof(fd)){
    fscanf(fd, "%c\t", &x);
    printf("%c", x);
  }
  fclose(fd);
  */
  return 0;
}