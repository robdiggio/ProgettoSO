struct CPU_PR_NI{
    float cpu_usage;
    int priority;
    int nice;
};
struct COMMAND_S_VIRT_RES{
    char name[20];
    char state[20];
    int VmSize;
    int VmHWM;
};
int getMemOrSwap(char line[]);
struct CPU_PR_NI getCPN(char *pid,int uptime);
struct COMMAND_S_VIRT_RES getCSVR(char *pid);