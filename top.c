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
  DIR *dr;
  struct dirent *dir;

  dr=opendir("/proc");
  if(dr==NULL){
    if(errno==EACCES) printf("permesso negato per la cartella /proc \n");
    else if(errno==ENAMETOOLONG) printf("nome della cartella troppo lungo \n");
    else printf("errore nell'apertura della directory proc");
    exit(1);
  }

  //stampa a video delle informazioni
  /*
  while((dir=readdir(dr))!=NULL){
    char *pid=dir->d_name;
    float cpu=getCpu(pid,uptime);
    printf("%s\n",pid);

  }
  */
  char *pid="1";
  struct CPU_PR_NI cpn;
  struct COMMAND_S_VIRT_RES csvr;

  cpn=getCPN(pid,uptime);
  printf("i valori del CPN sono: \n%f\n%d\n%d\n",cpn.cpu_usage,cpn.priority,cpn.nice);
  csvr=getCSVR(pid);
  printf("i valori del CSVR sono: \n%s\n%s\n%d\n%d\n",csvr.name,csvr.state,csvr.VmSize,csvr.VmHWM);

  printf("top \n");

  printf("kB Mem :   ");
  printf("%d",Mem.total);
  printf(" total,   ");
  printf("%d",Mem.free);
  printf(" free,   ");
  printf("%d",Mem.used);
  printf(" used,   ");
  printf("%d",Mem.cache);
  printf(" buff/cache \n");

  printf("kB Swap:   ");
  printf("%d",Swap.total);
  printf(" total,   ");
  printf("%d",Swap.free);
  printf(" free,   ");
  printf("%d",Swap.used);
  printf(" used,   ");
  printf("%d",Mem.disp);
  printf(" avail Mem \n");

  closedir(dr);
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