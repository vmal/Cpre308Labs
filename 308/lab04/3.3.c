#include <signal.h> 
#include <stdio.h> 
void my_routine(); 
int main()
{
    signal(SIGFPE, my_routine); 
    int a = 4;
    a = a/0;
    while(1)
    {
        sleep(10); 
    }
}
void my_routine()
{
    printf("Caught a SIGFPE.\n"); 
}
