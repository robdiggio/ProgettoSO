#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
//#include <delay.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

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
  char x[30];
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
  char *mem_tot=(char *) malloc(sizeof(char));
  char *mem_free=(char *) malloc(sizeof(char));
  char *mem_disp=(char *) malloc(sizeof(char));
  char *mem_cache=(char *) malloc(sizeof(char));

  //memoria totale
  fgets(x,30,fd);
  int j=0;
  for(int i=0;i<30;i++){
    if(isdigit(x[i])) {
      mem_tot[j]=x[i];
      j++;
    }
  }
  Mem.total=atoi(mem_tot);
  j=0;

  //memoria libera
  fgets(x,30,fd);
  for(int i=0;i<30;i++){
    if(isdigit(x[i])) {
      mem_free[j]=x[i];
      j++;
    }
  }
  Mem.free=atoi(mem_free);
  j=0;

  //memoria usata
  fgets(x,30,fd);
  for(int i=0;i<30;i++){
    if(isdigit(x[i])) {
      mem_disp[j]=x[i];
      j++;
    }
  }
  Mem.disp=atoi(mem_disp);
  Mem.used=Mem.total-Mem.disp;
  j=0;
  //itoa(used,Mem.used,10);

  //memoria cache
  fgets(x,30,fd);
  fgets(x,30,fd);
  for(int i=0;i<30;i++){
    if(isdigit(x[i])) {
      mem_cache[j]=x[i];
      j++;
    }
  }
  Mem.cache=atoi(mem_cache);
  j=0;
  free(mem_tot);
  free(mem_free);
  free(mem_disp);
  free(mem_cache);

  //Swap
  struct Swap{
    int total;
    int free;
    int used;
  } Swap;
  char *swap_tot=(char *) malloc(sizeof(char));
  char *swap_free=(char *) malloc(sizeof(char));

  //swap totale
  for(int i=0;i<10;i++)
    fgets(x,30,fd);
  for(int i=0;i<30;i++){
    if(isdigit(x[i])) {
      swap_tot[j]=x[i];
      j++;
    }
  }
  Swap.total=atoi(swap_tot);
  j=0;

  //swap libero
  fgets(x,30,fd);
  for(int i=0;i<30;i++){
    if(isdigit(x[i])) {
      swap_free[j]=x[i];
      j++;
    }
  }
  Swap.free=atoi(swap_free);
  j=0;

  //swap usato
  Swap.used=Swap.total-Swap.free;

  free(swap_tot);
  free(swap_free);

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