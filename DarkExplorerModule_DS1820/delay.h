#define SYS_CLK 48 //arm core clock (MHz)
#define delay(arg) wait(arg*SYS_CLK/22)

void wait(volatile unsigned int d);
