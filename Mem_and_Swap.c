#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Mem_and_Swap.h"

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