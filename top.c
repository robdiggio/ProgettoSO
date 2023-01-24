#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include "Mem_and_Swap.h"

int main() {
  //INFO GENERALI DI TOP (PRIMA PARTE)
  //system("uptime");
  //directory proc
  DIR *dr;
  struct dirent *dir_p;

  dr=opendir("/proc");
  if(dr==NULL){
    if(errno==EACCES) printf("permesso negato per la cartella /proc \n");
    else if(errno==ENAMETOOLONG) printf("nome della cartella troppo lungo \n");
    else printf("errore nell'apertura della directory proc");
    exit(1);
  }

  //lettura del file meminfo
  FILE *fd;
  fd=fopen("/proc/meminfo", "r");
  if(fd==NULL){
    printf("il file non può essere aperto");
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

  //stampa a video delle prime informazioni
  
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
  
  
  fclose(fd);

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
  char x;
  FILE *fd;
  fd=fopen("/proc/version", "r");
  if(fd==NULL){
    printf("errore");
    exit(1);
  }
  while(!feof(fd)){
    fscanf(fd, "%c\t", &x);
    printf("%c", x);
  }
  fclose(fd);
  */
  return 0;
}