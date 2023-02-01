#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <ucontext.h>
#include "top_info.h"
#include "top_signals.h"

int main(){
  ucontext_t main_context;
  getcontext(&main_context);
  // info sulla RAM e sullo Swap
  struct Mem_Swap ms;

  ms = getMemAndSwap();

  // lettura del file uptime
  FILE *fd = fopen("/proc/uptime", "r");
  if (fd == NULL)
  {
    printf("errore nell'apertura del file /proc/uptime");
    exit(1);
  }
  int uptime = 0;
  fscanf(fd,"%d",&uptime);
  fclose(fd);

  // directory proc
  DIR *dir;
  struct dirent *dirent;

  dir = opendir("/proc");
  if (dir == NULL){
    if (errno == EACCES) printf("permesso negato per la cartella /proc \n");
    else printf("errore nell'apertura della cartella proc");
    exit(1);
  }

  // acquisizione delle varie informazione sulla memoria e sui processi
  while ((dirent = readdir(dir)) != NULL && strcmp(dirent->d_name, "thread-self") != 0){}
  char *state;
  char *pid;
  int tot = 0, tot_sleep = 0, tot_running = 0, tot_zombie = 0, tot_stopped = 0;
  while ((dirent = readdir(dir)) != NULL){
    pid = dirent->d_name;
    state = getState(pid);
    if (strcmp(state, "S") == 0)
      tot_sleep++;
    else if (strcmp(state, "R") == 0)
      tot_running++;
    else if (strcmp(state, "Z") == 0)
      tot_zombie++;
    else if (strcmp(state, "T") == 0)
      tot_stopped++;
    tot++;
    free(state);
  }

  // stampa a video delle informazioni generali
  system("uptime");

  printf(" Task: %d total,  %d running,  %d sleeping,  %d stopped,  %d zombie\n", tot, tot_running, tot_sleep, tot_stopped, tot_zombie);

  printf(" kB Mem :   %d total,   %d free,   %d used,   %d buff/cache\n", ms.MemTotal, ms.MemFree, ms.MemUsed, ms.MemCache);

  printf(" kB Swap:   %d total,   %d free,   %d used,   %d avail Mem\n\n", ms.SwapTotal, ms.SwapFree, ms.SwapUsed, ms.MemDisp);

  // stampa a video delle informazioni sui processi
  rewinddir(dir);
  while ((dirent = readdir(dir)) != NULL && strcmp(dirent->d_name, "thread-self") != 0){}
  struct CPU_PR_NI cpn;
  struct COMMAND_S_USER_VIRT_RES csuvr;

  // printf("\033[1;31m");
  printf("\033[30m\033[47m");
  printf("  PID    USER          PR      NI       VIRT      RES   S      CPU     MEM      COMMAND  \n");
  printf("\033[0m\033[K\n");

  while ((dirent = readdir(dir)) != NULL){
    pid = dirent->d_name;
    cpn = getCPN(pid, uptime);
    csuvr = getCSUVR(pid);
    float mem = 100 * (csuvr.VmSize / ms.MemTotal);
    printf("  %s\t%s\t\t%d\t%d\t%d\t%d\t%s\t%.1f\t%.1f\t%s\n", pid, csuvr.user, cpn.priority, cpn.nice, csuvr.VmSize, csuvr.VmHWM, csuvr.state, cpn.cpu_usage, mem, csuvr.name);
  }

  while (1){
    char *com = (char *)malloc(sizeof(char) * 10);
    printf("\033[0;34m");
    printf("\n  inserisci il comando che vuoi eseguire:   ");
    printf("\033[0m");
    scanf("%s", com);
    printf("\n");
    if(strcmp(com,"q")==0){
      free(com);
      break;
    }
    else if(strcmp(com,"h")==0){
      help();
    }
    else if(strcmp(com,"t")==0){
      printf("\033[0;31m");
      printf("  inserisci il PID del processo:   ");
      printf("\033[0m");
      scanf("%s", pid);
      printf("\033[0;31m");
      terminate(pid);
      printf("\033[0m");
    }
    else if(strcmp(com,"k")==0){
      printf("\033[0;31m");
      printf("  inserisci il PID del processo:   ");
      printf("\033[0m");
      scanf("%s", pid);
      printf("\033[0;31m");
      kill_proc(pid);
      printf("\033[0m");
    }
    else if(strcmp(com,"r")==0){
      printf("\033[0;31m");
      printf("  inserisci il PID del processo:   ");
      printf("\033[0m");
      scanf("%s", pid);
      printf("\033[0;31m");
      resume(pid);
      printf("\033[0m");
    }
    else if(strcmp(com,"s")==0){
      printf("\033[0;31m");
      printf("  inserisci il PID del processo:   ");
      printf("\033[0m");
      scanf("%s", pid);
      printf("\033[0;31m");
      suspend(pid);
      printf("\033[0m");
    }
    else if(strcmp(com,"u")==0){
      setcontext(&main_context);
    }
    else{
      printf("\033[0;31m");
      printf("  comando sconosciuto, esegui il comando 'h' per la lista dei comandi\n");
      printf("\033[0m");
    }
    free(com);
  }
 
  closedir(dir);

  return 0;
}