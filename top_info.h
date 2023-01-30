struct CPU_PR_NI{
    float cpu_usage;
    int priority;
    int nice;
};
struct COMMAND_S_USER_VIRT_RES{
    char name[20];
    char state[20];
    char user[20];
    int VmSize;
    int VmHWM;
};
struct Mem_Swap{
    int MemTotal;
    int MemFree;
    int MemDisp;
    int MemUsed;
    int MemCache;
    int SwapTotal;
    int SwapFree;
    int SwapUsed;
};
struct Mem_Swap getMemAndSwap();
struct CPU_PR_NI getCPN(char *pid,int uptime);
struct COMMAND_S_USER_VIRT_RES getCSUVR(char *pid);
char* getState(char *pid);
void help();
void terminate(char *pid);
void kill_proc(char *pid);
void resume(char *pid);
void suspend(char *pid);