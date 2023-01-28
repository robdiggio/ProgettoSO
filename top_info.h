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
int getMemOrSwap(char line[]);
struct CPU_PR_NI getCPN(char *pid,int uptime);
struct COMMAND_S_USER_VIRT_RES getCSUVR(char *pid);
char* getState(char *pid);